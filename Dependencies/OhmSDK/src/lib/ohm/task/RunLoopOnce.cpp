/*****************************************************************************

        RunLoopOnce.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44889

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

#include "ohm/task/RunLoopOnce.h"
#include "ohm/sys/TimerReliable.h"

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

RunLoopOnce::RunLoopOnce (bool external_clock_flag)
:  _external_clock_flag (external_clock_flag)
,  _run_flag (true)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

RunLoopOnce::~RunLoopOnce ()
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
Name : set_time
==============================================================================
*/

void  RunLoopOnce::set_time (double time_s)
{
   _time_s = time_s;
}



/*
==============================================================================
Name : get_time
==============================================================================
*/

double   RunLoopOnce::get_time ()
{
   return _time_s;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_run
==============================================================================
*/

bool  RunLoopOnce::do_run ()
{
   bool run_flag = _run_flag;
   _run_flag = true;

   if (run_flag)
   {
      process_pre ();
      process ();
      process_post ();
   }
   
   return run_flag;
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

void  RunLoopOnce::do_stop ()
{
   _run_flag = false;
}



/*
==============================================================================
Name : do_signal
==============================================================================
*/

void  RunLoopOnce::do_signal ()
{
   // nothing
}



/*
==============================================================================
Name : do_update_time
==============================================================================
*/

void  RunLoopOnce::do_update_time ()
{
   if (!_external_clock_flag)
   {
      _time_s = sys::TimerReliable::use_instance ().get ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
