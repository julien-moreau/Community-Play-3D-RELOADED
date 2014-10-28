/*****************************************************************************

        Observer.cpp
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

#include "ohm/task/Observer.h"
#include "ohm/task/RunLoopBase.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/sys/TimerReliable.h"

#include <algorithm>



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

Observer::Observer (const char * debug_name_0)
:  _debug_name_0 (debug_name_0)
,  _run_loop_ptr (0)
,  _order (0.f)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Observer::~Observer ()
{
   try
   {
      assert (_run_loop_ptr == 0);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_order
==============================================================================
*/

void  Observer::set_order (float order)
{
   _order = order;
}



/*
==============================================================================
Name : get_order
==============================================================================
*/

float Observer::get_order () const
{
   return _order;
}



/*
==============================================================================
Name : send
Note :
   There is no strict checking on '_run_loop_ptr' for thread safety, but
   observers are expected to only remove themselves from the run loop.
==============================================================================
*/

void  Observer::send (Event & event)
{
   RunLoopBase * run_loop_ptr = _run_loop_ptr;
   
   if (run_loop_ptr != 0)
   {
      event.push_front (this);
      
      RunLoopManager::use_instance ().send (run_loop_ptr, event);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : attach
==============================================================================
*/

void  Observer::attach (RunLoopBase & run_loop)
{
   assert (&run_loop != 0);
   assert (_run_loop_ptr == 0);
   
   _run_loop_ptr = &run_loop;
}



/*
==============================================================================
Name : detach
==============================================================================
*/

void  Observer::detach (RunLoopBase & run_loop)
{
   assert (&run_loop != 0);
   assert (_run_loop_ptr == &run_loop);
   
   _run_loop_ptr = 0;
}



/*
==============================================================================
Name : receive_enter
==============================================================================
*/

void  Observer::receive_enter ()
{
   util::Delegate & delegate = _delegate_arr [Phase_ENTER];
      
   delegate.signal ();
}



/*
==============================================================================
Name : receive_exit
==============================================================================
*/

void  Observer::receive_exit ()
{
   util::Delegate & delegate = _delegate_arr [Phase_EXIT];
      
   delegate.signal ();
}



/*
==============================================================================
Name : receive_pre
==============================================================================
*/

void  Observer::receive_pre ()
{
   util::Delegate & delegate = _delegate_arr [Phase_PRE];
      
   delegate.signal ();
}



/*
==============================================================================
Name : receive_post
==============================================================================
*/

void  Observer::receive_post ()
{
   util::Delegate & delegate = _delegate_arr [Phase_POST];
      
   delegate.signal ();
}



/*
==============================================================================
Name : receive_event
==============================================================================
*/

void  Observer::receive_event (Event & event)
{
   util::Delegate & delegate = _delegate_arr [Phase_EVENT];
      
   delegate.param_ref_signal (event);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

