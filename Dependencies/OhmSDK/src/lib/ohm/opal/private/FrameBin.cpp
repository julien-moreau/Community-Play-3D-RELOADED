/*****************************************************************************

        FrameBin.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68519

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

#include "ohm/opal/private/FrameBin.h"
#include "ohm/opal/Frame.h"

#include <algorithm>

#include <cassert>

#undef OHM_OPAL_FB_DEBUG_FLAG

#if defined (OHM_OPAL_FB_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_OPAL_FB_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define fb_TRACE(msg_0) ohm_util_TRACE_INFO(msg_0)
   #define fb_TRACE1(fmt_0, arg1)   ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define fb_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define fb_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define fb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define fb_TRACE(msg_0) (void (0))
   #define fb_TRACE1(fmt_0, arg1)   (void (0))
   #define fb_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define fb_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define fb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameBin::~FrameBin ()
{
}



/*
==============================================================================
Name : set_destroyable
Description :
   add a frame to the frame bin
==============================================================================
*/

void  FrameBin::set_destroyable (Frame & frame)
{
   assert (&frame != 0);
   assert (!_destroyable_frame_guard_flag);
   
   fb_TRACE1 ("%1% set_destroyable", frame.get_debug_name ());
   
   assert (
      std::find (
         _destroyable_frame_ptr_list.begin (),
         _destroyable_frame_ptr_list.end (),
         &frame
      ) == _destroyable_frame_ptr_list.end ()
   );
   
   _destroyable_frame_ptr_list.push_back (&frame);
}



/*
==============================================================================
Name : require_post_synchronize
Description :
   add a frame for post sync of its children iterators
==============================================================================
*/

void  FrameBin::require_post_synchronize (Frame & frame)
{
   assert (&frame != 0);
   assert (!_post_sync_frame_guard_flag);
   
   fb_TRACE1 ("%1% require_post_synchronize", frame.get_debug_name ());
   
   _post_sync_frame_ptr_set.insert (&frame);
}



/*
==============================================================================
Name : recycle
Description :
   delete all frame in the frame bin
Note :
   called as a post action
==============================================================================
*/

void  FrameBin::recycle ()
{
   {
      _post_sync_frame_guard_flag = true;
      
      FramePtrSet::iterator it = _post_sync_frame_ptr_set.begin ();
      const FramePtrSet::iterator it_end = _post_sync_frame_ptr_set.end ();
      
      for (; it != it_end ;)
      {
         FramePtrSet::iterator it_next = it;
         ++it_next;
         
         Frame * frame_ptr = *it;
         assert (frame_ptr != 0);

         fb_TRACE1 ("%1% post_synchronize", frame_ptr->get_debug_name ());
         
         frame_ptr->post_synchronize ();
         
         it = it_next;
      }
      
      _post_sync_frame_ptr_set.clear ();
      
      _post_sync_frame_guard_flag = false;
   }
   
#if 1
   // Bug correction
   // to reproduce the old crash : right click an object then press backspace
   // the idea is that the frame might contains an object that contains frames
   // in that case the list might get corrupted
   // Here we make a copy to avoid this situation, at the copy expense
   
   {
      FramePtrList destroyable_frame_ptr_list = _destroyable_frame_ptr_list;
      _destroyable_frame_ptr_list.clear ();
      
      FramePtrList::iterator it = destroyable_frame_ptr_list.begin ();
      const FramePtrList::iterator it_end = destroyable_frame_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         Frame * frame_ptr = *it;
         assert (frame_ptr != 0);
         
         assert (
            _post_sync_frame_ptr_set.find (frame_ptr)
            == _post_sync_frame_ptr_set.end ()
         );
         
         fb_TRACE1 ("%1% delete", frame_ptr->get_debug_name ());
         
         delete frame_ptr;
      }
   }

#elif 0  
   {
      _destroyable_frame_guard_flag = true;
      
      FramePtrList::iterator it = _destroyable_frame_ptr_list.begin ();
      const FramePtrList::iterator it_end = _destroyable_frame_ptr_list.end ();
      
      for (; it != it_end ;)
      {
         FramePtrList::iterator it_next = it;
         ++it_next;
         
         Frame * frame_ptr = *it;
         assert (frame_ptr != 0);
         
         assert (
            _post_sync_frame_ptr_set.find (frame_ptr)
            == _post_sync_frame_ptr_set.end ()
         );
         
         fb_TRACE1 ("%1% delete", frame_ptr->get_debug_name ());
         
         delete frame_ptr;
         
         it = it_next;
      }
      
      _destroyable_frame_ptr_list.clear ();
      
      _destroyable_frame_guard_flag = false;
   }
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FrameBin::FrameBin ()
:  _destroyable_frame_guard_flag (false)
,  _destroyable_frame_ptr_list ()

,  _post_sync_frame_guard_flag (false)
,  _post_sync_frame_ptr_set ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
