/*****************************************************************************

        RunLoopGuiImpl.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 56376

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

#include "ohm/task/carbon/RunLoopGuiImpl.h"
#include "ohm/task/RunLoopGui.h"

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

RunLoopGuiImpl::RunLoopGuiImpl (RunLoopGui & parent)
:  _parent (parent)
,  _run_loop_source_ref (0)
{
   // we need to set up the '_run_loop_source_ref' early
   // and to keep it betweens runs because we can be signalled
   // before the run loop is runned
   
   CFRunLoopSourceContext context;
   bzero (&context, sizeof (CFRunLoopSourceContext));
   
   context.info = this;
   context.perform = perform_proc;
   
   _run_loop_source_ref = CFRunLoopSourceCreate (
      kCFAllocatorDefault,
      0,
      &context
   );
   
   CFRunLoopAddSource (
      get_main_run_loop (),
      _run_loop_source_ref,
      kCFRunLoopCommonModes
   );
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

RunLoopGuiImpl::~RunLoopGuiImpl ()
{
   try
   {
      if (_run_loop_source_ref != 0)
      {
         CFRunLoopRemoveSource (
            get_main_run_loop (),
            _run_loop_source_ref,
            kCFRunLoopCommonModes
         );
         
         CFRelease (_run_loop_source_ref);
         _run_loop_source_ref = 0;
      }
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

void  RunLoopGuiImpl::run ()
{
   RunApplicationEventLoop ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  RunLoopGuiImpl::stop ()
{
   // QuitApplicationEventLoop is not thread safe
   
   QuitApplicationEventLoop ();
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  RunLoopGuiImpl::signal ()
{
   assert (_run_loop_source_ref != 0);
   
   CFRunLoopSourceSignal (_run_loop_source_ref);
   CFRunLoopWakeUp (get_main_run_loop ());
}



/*
==============================================================================
Name : get_time
==============================================================================
*/

double   RunLoopGuiImpl::get_time ()
{
   return GetCurrentEventTime ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_main_run_loop
==============================================================================
*/

CFRunLoopRef   RunLoopGuiImpl::get_main_run_loop ()
{
   /*
   From Apple documentation  :
   "There isn't necessarily a one-to-one correspondence between Carbon event
   loops and Core Foundation event loops, so you should use this function
   instead of simply calling the Core Foundation function CFRunLoopGetCurrent."
   */

   EventLoopRef event_loop_ref = GetMainEventLoop ();
   assert (event_loop_ref != 0);
   
   CFRunLoopRef run_loop_ref = (CFRunLoopRef) GetCFRunLoopFromEventLoop (event_loop_ref);
   assert (run_loop_ref != 0);
   
   return run_loop_ref;
}



/*
==============================================================================
Name : perform_proc
==============================================================================
*/

void  RunLoopGuiImpl::perform_proc (void * info)
{
   try
   {
      RunLoopGuiImpl * this_ptr = reinterpret_cast <RunLoopGuiImpl *> (info);
      assert (this_ptr != 0);
      
      this_ptr->perform ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : perform
==============================================================================
*/

void  RunLoopGuiImpl::perform ()
{
   _parent.req_process ();
}




/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
