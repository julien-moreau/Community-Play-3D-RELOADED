/*****************************************************************************

        VideoThreadImpl.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62309

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/VideoThreadImpl.h"
#include "ohm/opal/directx/Direct3d.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/opal/View.h"
#include "ohm/math/fnc.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/util/trace.h"

#undef OHM_OPAL_COARSE_INVALIDATE
#undef OHM_OPAL_NO_INVALIDATION

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Create the video thread, which happens to be a display link on MacOS.
==============================================================================
*/

VideoThreadImpl::VideoThreadImpl (VideoThread & parent)
:  _parent (parent)
,  _thread_id (sys::ThreadMgr::get_invalid_thread_id ())

,  _state (State_STOPPED)
,  _run_sync ()
,  _stop_sync ()
,  _run_loop (true)
,  _observer ("ohm.opal.VideoThreadImpl.display_link")
,  _nbr_skip_frame (0)
,  _nbr_req_vbr (0)
{
   _observer.bind_process <
      VideoThreadImpl,
      &VideoThreadImpl::process
   > (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

VideoThreadImpl::~VideoThreadImpl ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  VideoThreadImpl::run ()
{
   // run thread
   
   ThreadBase::run ();

   _run_sync.wait ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  VideoThreadImpl::stop ()
{
   _run_loop.stop ();
   
   _stop_sync.wait ();
   
   ThreadBase::wait_for_death ();
   
   _state = State_STOPPED;
}



/*
==============================================================================
Name : set_nbr_skip_frame
Description :
   Set the nbr of skipped frame to reduce power GPU consumption
Note :
   Called from main thread, but we don't use the observer to change the
   internal value used by the video thread, since it is not critical.
==============================================================================
*/

void  VideoThreadImpl::set_nbr_skip_frame (int nbr)
{
   assert (nbr >= 0);
   
   _nbr_skip_frame = nbr;
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  VideoThreadImpl::add (View & view)
{
   assert (&view != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_ADD_VIEW);
   event.push (&view);
   
   _observer.send (event);
   
   // also update
   update (view);
}



/*
==============================================================================
Name : deactivate
==============================================================================
*/

void  VideoThreadImpl::deactivate (View & view)
{
   assert (&view != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_DEACTIVATE_VIEW);
   event.push (&view);
   
   _observer.send (event);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  VideoThreadImpl::remove (View & view)
{
   assert (&view != 0);
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_REMOVE_VIEW);
   event.push (&view);
   
   _observer.send (event);
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  VideoThreadImpl::update (View & view)
{
   assert (&view != 0);
   
   opa::Size bounds = view.get_bounds ();
   
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_UPDATE_VIEW);
   event.push (&view);
   event.push (bounds);
   
   _observer.send (event);
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  VideoThreadImpl::invalidate (View & view, const opa::Rect & rect)
{
   assert (&view != 0);

   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_INVALIDATE_VIEW);
   event.push (&view);
   event.push (rect);
   
   _observer.send (event);
}



/*
==============================================================================
Name : request_vbr
==============================================================================
*/

void  VideoThreadImpl::request_vbr ()
{
   task::Event & event = ohm_task_EVENT_GET;
   event.push (EventType_REQ_VBR);
   
   _observer.send (event);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_run
==============================================================================
*/

void  VideoThreadImpl::do_run ()
{
   // init system
   
   Direct3d & direct_3d_inst = Direct3d::use_instance ();
   
   direct_3d_inst.create ();
   
   ::IDirect3D9 * direct_3d_ptr = direct_3d_inst.get ();
   assert (direct_3d_ptr != 0);
   
   // increase scheduler resolution for ::Sleep/sys::TimerReliable precision
   
   ::timeBeginPeriod (1);
   
   // get the primary display adapter height and refresh for vbr
   
   ::D3DDISPLAYMODE mode;
   
   ::HRESULT err = direct_3d_ptr->GetAdapterDisplayMode (
      D3DADAPTER_DEFAULT,  // Only link refresh rate with the main display
      &mode
   );
   assert (err == D3D_OK);
   
   _height = mode.Height;
   assert (mode.RefreshRate != 0);
   _refresh_period = 1.0 / double (mode.RefreshRate);
   
   // run loop
   
   while (_state != State_STOPPING)
   {
      // for now, but should be a good approximation
      double time_s = sys::TimerReliable::use_instance ().get () + _refresh_period;
      
      handle_display_link (time_s);
      
      // for now
      time_s = sys::TimerReliable::use_instance ().get () + _refresh_period;
   }
   
   // release scheduler resolution
   
   ::timeEndPeriod (1);
   
   // restore system
   
   direct_3d_inst.release ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_display_link
==============================================================================
*/

void  VideoThreadImpl::handle_display_link (double time_s)
{
   _run_loop.set_time (time_s);
   
   if (_state == State_STOPPED)
   {
      task::RunLoopManager::use_instance ().push_current (_run_loop);
      
      _thread_id = sys::ThreadMgr::get_current_thread_id ();
      
      _parent.init (time_s);

      _run_loop.add (_observer);
      
      _state = State_RUNNING;
      
      _run_sync.signal ();
   }
   
   if (_state == State_RUNNING)
   {
      // will trigger invalidations
      _parent.tick (time_s);
      
      // will trigger context update on resize
      bool run_flag = _run_loop.run ();
      
      // draw all view context
      
      if (!_view_ptr_set.empty ())
      {
         ViewPtrSet::iterator it = _view_ptr_set.begin ();
         const ViewPtrSet::iterator it_end = _view_ptr_set.end ();
         
         ViewPtrSet::iterator it_last = it_end;
         --it_last;
         
         for (; it != it_end ; ++it)
         {
            View * view_ptr = *it;
            assert (view_ptr != 0);
            
            // the last drawn view will wait for vertical retrace
            bool last_flag = (it == it_last) && (_nbr_req_vbr > 0);
            
            view_ptr->vt_draw (last_flag);
         }
      }

      ::Sleep (5);
      
      if (_nbr_req_vbr > 0)
      {
         _parent.notify_vbr ();
         --_nbr_req_vbr;
      }
      
      skip_frames ();
      
      if (!run_flag)
      {
         _state = State_STOPPING;
         
         _run_loop.remove (_observer);
         
         _parent.restore ();

         _thread_id = sys::ThreadMgr::get_invalid_thread_id ();
         
         task::RunLoopManager::use_instance ().pop_current (_run_loop);
         
         _stop_sync.signal ();
      }
   }
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  VideoThreadImpl::process (task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   switch (type)
   {
   case EventType_ADD_VIEW:
      process_add_view (event);
      break;

   case EventType_DEACTIVATE_VIEW:
      process_deactivate_view (event);
      break;
      
   case EventType_REMOVE_VIEW:
      process_remove_view (event);
      break;

   case EventType_UPDATE_VIEW:
      process_update_view (event);
      break;

   case EventType_INVALIDATE_VIEW:
      process_invalidate_view (event);
      break;

   case EventType_REQ_VBR:
      process_request_vbr (event);
      break;
   }
}



/*
==============================================================================
Name : process_add_view
==============================================================================
*/

void  VideoThreadImpl::process_add_view (task::Event & event)
{
   View * view_ptr = event.pop <View *> ();
   assert (view_ptr != 0);
   
   assert (_view_ptr_set.find (view_ptr) == _view_ptr_set.end ());
   
   view_ptr->vt_init ();

   _view_ptr_set.insert (view_ptr);
}



/*
==============================================================================
Name : process_deactivate_view
==============================================================================
*/

void  VideoThreadImpl::process_deactivate_view (task::Event & event)
{
   View * view_ptr = event.pop <View *> ();
   assert (view_ptr != 0);
   
   assert (_view_ptr_set.find (view_ptr) != _view_ptr_set.end ());
   
   _view_ptr_set.erase (view_ptr);

   // unblock main thread
   _parent.notify_deactivate ();
}



/*
==============================================================================
Name : process_remove_view
==============================================================================
*/

void  VideoThreadImpl::process_remove_view (task::Event & event)
{
   View * view_ptr = event.pop <View *> ();
   assert (view_ptr != 0);
   
   assert (_view_ptr_set.find (view_ptr) == _view_ptr_set.end ());

   view_ptr->vt_restore ();
}



/*
==============================================================================
Name : process_update_view
==============================================================================
*/

void  VideoThreadImpl::process_update_view (task::Event & event)
{
   View * view_ptr = event.pop <View *> ();
   assert (view_ptr != 0);
   
   opa::Size bounds = event.pop <opa::Size> ();
   
   assert (_view_ptr_set.find (view_ptr) != _view_ptr_set.end ());

   view_ptr->vt_update (bounds);
}



/*
==============================================================================
Name : process_invalidate_view
==============================================================================
*/

void  VideoThreadImpl::process_invalidate_view (task::Event & event)
{
   View * view_ptr = event.pop <View *> ();
   assert (view_ptr != 0);
   
   opa::Rect rect = event.pop <opa::Rect> ();
   
   assert (_view_ptr_set.find (view_ptr) != _view_ptr_set.end ());

   view_ptr->invalidate (rect);
}



/*
==============================================================================
Name : process_request_vbr
==============================================================================
*/

void  VideoThreadImpl::process_request_vbr (task::Event & event)
{
   ++_nbr_req_vbr;
}



/*
==============================================================================
Name : skip_frames
==============================================================================
*/

void  VideoThreadImpl::skip_frames ()
{
   if (_nbr_skip_frame > 0)
   {
      double millis = double (_nbr_skip_frame) * _refresh_period * 1.0e3;
      ::DWORD millis_d = ::DWORD (math::round_int (millis));
      
      // we may achieve a 1 ms resolution
      ::Sleep (millis_d);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
