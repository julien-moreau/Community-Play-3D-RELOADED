/*****************************************************************************

        TypeSignal.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53796

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



#if defined (ohm_opak_TypeSignal_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeSignal code header.
#endif
#define  ohm_opak_TypeSignal_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeSignal_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeSignal_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/lang/fnc.h"



namespace ohm
{
namespace opak
{



/*
==============================================================================
                                 MODEL
   
   Internally, the proxy maintains a queue of animation with each element
   (target_val, transition_time, transition_function)
   
   Cumulative mode : a new element is pushed into the queue
   Immediate mode : the queue is flushed and the element is pushed into the
   queue.
   
   prev_proc : some primitive requires to know if the cur_val is going to
   change. Therefore if we are to change the value of cur_val, we need
   to call the callback before.
   
   stabilization : some primitive aspect depends on both cur_val & prev_val
   (that is not only cur_val). When stabilized, the callback will fire until
   prev_val == cur_val (that is we don't stop when cur_val reaches target_val)
   
   State properties : delay, function are state properties of this proxy.
   This means that they are implicitly accumulated in immediate mode, and
   changing them have no effect on the current animation.
   
   operator type () : the user except the following :
   a = b => a == b. So in command thread, the return value is the last one
   previously set. (the latest element pushed in cumulative mode)
   
   Loop : the animation can be looped. In that case, when a queue element
   has been fully executed, it is removed from front and pushed to back.
   Normally before starting a loop animation, the client would flush the
   current queue.
   
==============================================================================
*/



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target>
TypeSignal <Target>::TypeSignal (Target & target, Proc proc)
:  _target (target)
,  _proc (proc)
{
   assert (&_target != 0);

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
   TransactionMaster::use ().debug_add (this);
   _target.add_type_debug (*this);
#endif   
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Target>
TypeSignal <Target>::~TypeSignal ()
{
}



/*
==============================================================================
Name : signal
==============================================================================
*/

template <class Target>
void  TypeSignal <Target>::signal ()
{
   TransactionMaster::use ().push (
      do_signal_proc, this, 0, 0
   );
}



/*
==============================================================================
Name : do_signal_proc
==============================================================================
*/

template <class Target>
void  TypeSignal <Target>::do_signal_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_size == 0);
   assert (data_ptr == 0);
   
   TypeSignal * this_ptr = reinterpret_cast <TypeSignal *> (target_ptr);
   
   this_ptr->do_signal ();
}



/*
==============================================================================
Name : do_signal
==============================================================================
*/

template <class Target>
void  TypeSignal <Target>::do_signal ()
{
   call_proc ();
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target>
void  TypeSignal <Target>::notify_parent_destroyable ()
{
   _target.remove_type_debug (*this);
   TransactionMaster::use ().debug_remove (this);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : call_proc
==============================================================================
*/

template <class Target>
void  TypeSignal <Target>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeSignal_CODEHEADER_INCLUDED

#undef ohm_opak_TypeSignal_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
