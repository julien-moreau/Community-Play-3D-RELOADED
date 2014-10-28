/*****************************************************************************

        RunLoopGui.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59159

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

#include "ohm/task/RunLoopGui.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/task/win32/RunLoopGuiImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/task/carbon/RunLoopGuiImpl.h"

#else
   #error Operation System not supported

#endif

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

RunLoopGui::~RunLoopGui ()
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
Name : get_time
==============================================================================
*/

double   RunLoopGui::get_time ()
{
   return _time_s;
}



/*
==============================================================================
Name : quit
Thread Safety :
   NOT thread safe
==============================================================================
*/

void  RunLoopGui::quit ()
{
   do_stop ();
}



/*
==============================================================================
Name : notify_pre_gui_event
==============================================================================
*/

void  RunLoopGui::notify_pre_gui_event ()
{
   assert (!_processing_gui_event_flag);
   
   bool ok_flag = process_pre ();
   
   assert (ok_flag);
   
   _processing_gui_event_flag = true;
}



/*
==============================================================================
Name : notify_post_gui_event
==============================================================================
*/

void  RunLoopGui::notify_post_gui_event ()
{
   assert (_processing_gui_event_flag);
   
   process_post ();
   
   _processing_gui_event_flag = false;
}



/*
==============================================================================
Name : notify_post_external_modality
Description :
   When the OS start a modal run loop, it might "eat" some of the signals
   we've been sending. The events datas are safe, but the run loop might be
   blocked because it thinks its signal went to its recipient.
   This function is a fix, which force a signal to deblock any pending events. 
==============================================================================
*/

void  RunLoopGui::notify_post_external_modality ()
{
   do_signal ();
}



/*
==============================================================================
Name : use_impl
==============================================================================
*/

RunLoopGuiImpl &  RunLoopGui::use_impl ()
{
   assert (_impl_aptr.get () != 0);
   
   return *_impl_aptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

RunLoopGui::RunLoopGui ()
:  _impl_aptr ()
,  _time_s ()
,  _processing_gui_event_flag (false)
{
   _impl_aptr = std::auto_ptr <RunLoopGuiImpl> (new RunLoopGuiImpl (*this));
}



/*
==============================================================================
Name : do_run
==============================================================================
*/

bool  RunLoopGui::do_run ()
{
   _impl_aptr->run ();
   
   return false;
}



/*
==============================================================================
Name : do_stop
==============================================================================
*/

void  RunLoopGui::do_stop ()
{
   _impl_aptr->stop ();
}



/*
==============================================================================
Name : do_signal
==============================================================================
*/

void  RunLoopGui::do_signal ()
{
   _impl_aptr->signal ();
}



/*
==============================================================================
Name : do_update_time
==============================================================================
*/

void  RunLoopGui::do_update_time ()
{
   _time_s = _impl_aptr->get_time ();
}



/*
==============================================================================
Name : req_process
==============================================================================
*/

void  RunLoopGui::req_process ()
{
   bool ok_flag = process_pre ();
   
   if (ok_flag)
   {
      process ();
      process_post ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
