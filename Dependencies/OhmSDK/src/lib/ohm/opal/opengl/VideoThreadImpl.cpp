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

#include "ohm/opal/opengl/VideoThreadImpl.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/opal/View.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/util/trace.h"

#undef OHM_OPAL_COARSE_INVALIDATE
#undef OHM_OPAL_NO_INVALIDATION

#include <mach/mach.h>

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
,  _display_link_ref (0)
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

   // Only link refresh rate with the main display
   CGDirectDisplayID display_id = CGMainDisplayID ();
   
   CVReturn err = CVDisplayLinkCreateWithCGDisplay (display_id, &_display_link_ref);
   assert (err == kCVReturnSuccess);
   
   CVTime refresh_period
      = CVDisplayLinkGetNominalOutputVideoRefreshPeriod (_display_link_ref);
   
   assert (refresh_period.timeScale > 0);
   assert ((refresh_period.flags & kCVTimeIsIndefinite) == 0);
   _refresh_period = double (refresh_period.timeValue) / double (refresh_period.timeScale);
   
   err = CVDisplayLinkSetOutputCallback (_display_link_ref, handle_display_link_proc, this);
   assert (err == kCVReturnSuccess);
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
      CVDisplayLinkRelease (_display_link_ref);
      _display_link_ref = 0;
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
   // setup thread
   
   assert (_display_link_ref != 0);
   CVReturn err = CVDisplayLinkStart (_display_link_ref);
   assert (err == kCVReturnSuccess);

   _run_sync.wait ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  VideoThreadImpl::stop ()
{
   assert (_display_link_ref != 0);
   
   _run_loop.stop ();
   
   _stop_sync.wait ();

   CVReturn err = CVDisplayLinkStop (_display_link_ref);
   assert (err == kCVReturnSuccess);
   
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
   assert (false);
   
   // nothing
   // only meaningful for non composited window managers.
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



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

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
      double nanos = double (_nbr_skip_frame) * _refresh_period * 1.0e9;
      
      mach_timespec_t sleep_time;
      sleep_time.tv_sec = 0;
      sleep_time.tv_nsec = nanos;
      mach_timespec_t wake_time;
      
      kern_return_t err = clock_sleep (
         REALTIME_CLOCK,
         TIME_RELATIVE,
         sleep_time,
         &wake_time
      );
      assert (err == KERN_SUCCESS);
   }
}



/*
==============================================================================
Name : handle_display_link_proc
==============================================================================
*/

CVReturn VideoThreadImpl::handle_display_link_proc (CVDisplayLinkRef display_link_ref, const CVTimeStamp * now_ptr, const CVTimeStamp * due_time_ptr, CVOptionFlags in_flags, CVOptionFlags * out_flags_ptr, void * user_info)
{
   CVReturn ret_val = kCVReturnSuccess;
   
   try
   {
      assert (user_info != 0);
      
      VideoThreadImpl * this_ptr = reinterpret_cast <VideoThreadImpl *> (user_info);
      
      ret_val = this_ptr->handle_display_link (
         display_link_ref, *now_ptr, *due_time_ptr, in_flags, *out_flags_ptr
      );
   }
   catch (...)
   {
      assert (false);
      
      ret_val = kCVReturnError;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_display_link
==============================================================================
*/

CVReturn VideoThreadImpl::handle_display_link (CVDisplayLinkRef display_link_ref, const CVTimeStamp & now, const CVTimeStamp & due_time, CVOptionFlags in_flags, CVOptionFlags & out_flags)
{
   double time_s = double (due_time.videoTime) / double (due_time.videoTimeScale);
   
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
   
   return kCVReturnSuccess;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
