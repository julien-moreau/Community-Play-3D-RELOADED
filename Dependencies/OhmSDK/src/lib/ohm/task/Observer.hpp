/*****************************************************************************

        Observer.hpp
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



#if defined (ohm_task_Observer_CURRENT_CODEHEADER)
   #error Recursive inclusion of Observer code header.
#endif
#define  ohm_task_Observer_CURRENT_CODEHEADER

#if ! defined (ohm_task_Observer_CODEHEADER_INCLUDED)
#define  ohm_task_Observer_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/TimerReliable.h"

#include <algorithm>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind_enter_process
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*enter_process_method) ()>
void  Observer::bind_enter_process (ReceiverType & receiver)
{
   util::Delegate & delegate = _delegate_arr [Phase_ENTER];
   
   delegate.set <ReceiverType, enter_process_method> (receiver);
}



/*
==============================================================================
Name : bind_exit_process
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*exit_process_method) ()>
void  Observer::bind_exit_process (ReceiverType & receiver)
{
   util::Delegate & delegate = _delegate_arr [Phase_EXIT];
   
   delegate.set <ReceiverType, exit_process_method> (receiver);
}



/*
==============================================================================
Name : bind_pre_process
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*pre_process_method) ()>
void  Observer::bind_pre_process (ReceiverType & receiver)
{
   util::Delegate & delegate = _delegate_arr [Phase_PRE];
   
   delegate.set <ReceiverType, pre_process_method> (receiver);
}



/*
==============================================================================
Name : bind_post_process
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*post_process_method) ()>
void  Observer::bind_post_process (ReceiverType & receiver)
{
   util::Delegate & delegate = _delegate_arr [Phase_POST];
   
   delegate.set <ReceiverType, post_process_method> (receiver);
}



/*
==============================================================================
Name : bind_process
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*process_method) (Event &)>
void  Observer::bind_process (ReceiverType & receiver)
{
   util::Delegate & delegate = _delegate_arr [Phase_EVENT];
   
   delegate.param_ref_set <ReceiverType, Event, process_method> (receiver);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace task
}  // namespace ohm



#endif   // ohm_task_Observer_CODEHEADER_INCLUDED

#undef ohm_task_Observer_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

