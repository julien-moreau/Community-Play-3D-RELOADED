/*****************************************************************************

        Timer.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52263

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

#include "ohm/task/Clock.h"
#include "ohm/task/Timer.h"
#include "ohm/task/RunLoopBase.h"
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

Timer::Timer (const char * debug_name_0)
:  _debug_name_0 (debug_name_0)
,  _run_loop_ptr (0)
,  _kind (Kind_NONE)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Timer::~Timer ()
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
Name : set_periodic
==============================================================================
*/

void  Timer::set_periodic (double period_s)
{
   assert (_kind == Kind_NONE);
   
   _kind = Kind_PERIODIC;
   _time_s = period_s;
}



/*
==============================================================================
Name : is_periodic
==============================================================================
*/

bool  Timer::is_periodic () const
{
   assert (_kind != Kind_NONE);

   return (_kind == Kind_PERIODIC);
}



/*
==============================================================================
Name : get_period
==============================================================================
*/

double   Timer::get_period () const
{
   assert (_kind == Kind_PERIODIC);
   
   return _time_s;
}



/*
==============================================================================
Name : set_one_shot
==============================================================================
*/

void  Timer::set_one_shot (double delay_s)
{
   assert (_run_loop_ptr == 0);
   
   _kind = Kind_ONE_SHOT;
   _time_s = delay_s;
}



/*
==============================================================================
Name : is_one_shot
==============================================================================
*/

bool  Timer::is_one_shot () const
{
   assert (_kind != Kind_NONE);

   return (_kind == Kind_ONE_SHOT);
}



/*
==============================================================================
Name : get_delay
==============================================================================
*/

double   Timer::get_delay () const
{
   assert (_kind == Kind_ONE_SHOT);
   
   return _time_s;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : attach
==============================================================================
*/

void  Timer::attach (RunLoopBase & run_loop)
{
   assert (&run_loop != 0);
   assert (_run_loop_ptr == 0);
   
   _run_loop_ptr = &run_loop;
   
   task::Clock::use_instance ().bind_alarm (_run_loop_ptr, this, _time_s);
}



/*
==============================================================================
Name : detach
==============================================================================
*/

void  Timer::detach (RunLoopBase & run_loop)
{
   assert (&run_loop != 0);
   assert (_run_loop_ptr == &run_loop);
   
   task::Clock::use_instance ().unbind_alarm (_run_loop_ptr, this);

   _run_loop_ptr = 0;
}



/*
==============================================================================
Name : receive_fire
==============================================================================
*/

void  Timer::receive_fire ()
{
   assert (_run_loop_ptr != 0);
   
   _delegate.signal ();
   
   if (_kind == Kind_PERIODIC)
   {
      task::Clock::use_instance ().bind_alarm (_run_loop_ptr, this, _time_s);
   }  
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

