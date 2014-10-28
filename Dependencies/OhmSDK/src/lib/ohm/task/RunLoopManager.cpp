/*****************************************************************************

        RunLoopManager.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49378

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

#include "ohm/task/RunLoopManager.h"
#include "ohm/task/RunLoopBase.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*
==============================================================================
Name : dtor
==============================================================================
*/

RunLoopManager::~RunLoopManager ()
{
}



/*
==============================================================================
Name : get_current
==============================================================================
*/

RunLoopBase &  RunLoopManager::get_current ()
{
   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   _run_loop_maps_rwlock.lock_read ();
   
   ThreadRunLoopMap::iterator it = _thread_run_loop_map.find (cur_thread_id);
   assert (it != _thread_run_loop_map.end ());
   
   RunLoopPtrList & run_loop_ptr_list = it->second;
   assert (!run_loop_ptr_list.empty ());
   
   RunLoopBase * run_loop_ptr = run_loop_ptr_list.back ();
   assert (run_loop_ptr != 0);
   
   _run_loop_maps_rwlock.unlock_read ();
   
   return *run_loop_ptr;
}



/*
==============================================================================
Name : push_current
==============================================================================
*/

void  RunLoopManager::push_current (RunLoopBase & run_loop)
{
   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   _run_loop_maps_rwlock.lock_write ();
   
   RunLoopPtrList & run_loop_ptr_list = _thread_run_loop_map [cur_thread_id];

   assert (
      std::find (run_loop_ptr_list.begin (), run_loop_ptr_list.end (), &run_loop)
      == run_loop_ptr_list.end ()
   );
   
   run_loop_ptr_list.push_back (&run_loop);
   
   assert (_run_loop_set.find (&run_loop) == _run_loop_set.end ());
   _run_loop_set.insert (&run_loop);
   
   _run_loop_maps_rwlock.unlock_write ();
}



/*
==============================================================================
Name : pop_current
==============================================================================
*/

void  RunLoopManager::pop_current (RunLoopBase & run_loop)
{
   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   _run_loop_maps_rwlock.lock_write ();
   
   RunLoopPtrList & run_loop_ptr_list = _thread_run_loop_map [cur_thread_id];

   assert (!run_loop_ptr_list.empty ());
   
   run_loop_ptr_list.pop_back ();
   
   assert (_run_loop_set.find (&run_loop) != _run_loop_set.end ());
   _run_loop_set.erase (&run_loop);

   _run_loop_maps_rwlock.unlock_write ();
}



/*
==============================================================================
Name : timer_fired
Description :
   Propagate event to RunLoopBase if and only if it exists.
   The timer existence itself will be handled by the run loop.
==============================================================================
*/

void  RunLoopManager::timer_fired (const std::pair <RunLoopBase *, Timer *> & run_loop_timer)
{
   RunLoopBase * run_loop_ptr = run_loop_timer.first;
   
   _run_loop_maps_rwlock.lock_read ();
   
   if (_run_loop_set.find (run_loop_ptr) != _run_loop_set.end ())
   {
      Timer * timer_ptr = run_loop_timer.second;
      
      run_loop_ptr->timer_fired (timer_ptr);
   }
   
   _run_loop_maps_rwlock.unlock_read ();
}



/*
==============================================================================
Name : send
Description :
   Propagate event to RunLoopBase if and only if it exists.
==============================================================================
*/

void  RunLoopManager::send (RunLoopBase * run_loop_ptr, Event & event)
{
   _run_loop_maps_rwlock.lock_read ();
   
   if (_run_loop_set.find (run_loop_ptr) != _run_loop_set.end ())
   {
      run_loop_ptr->send (event);
   }
   
   _run_loop_maps_rwlock.unlock_read ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

RunLoopManager::RunLoopManager ()
:  _run_loop_maps_rwlock ()
,  _thread_run_loop_map ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
