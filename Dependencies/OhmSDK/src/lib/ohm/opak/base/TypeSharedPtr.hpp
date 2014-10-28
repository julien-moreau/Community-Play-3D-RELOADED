/*****************************************************************************

        TypeSharedPtr.hpp
        Copyright (c) 2010 Ohm Force

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



#if defined (ohm_opak_TypeSharedPtr_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeSharedPtr code header.
#endif
#define  ohm_opak_TypeSharedPtr_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeSharedPtr_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeSharedPtr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"
#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/TransactionMaster.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/lang/fnc.h"

#if (ohm_opal_API == ohm_opal_API_OPENGL)
   #include "ohm/opal/opengl/OpenGLEngine.h"

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)
   #include "ohm/opal/directx/DirectXEngine.h"
   
#else
   #error Unsupported Architecture
   
#endif

#include <cstring>



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

template <class Target, class T>
TypeSharedPtr <Target, T>::TypeSharedPtr (Target & target, Proc proc)
:  _target (target)
,  _proc (proc)

,  _cur_val ()
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

template <class Target, class T>
TypeSharedPtr <Target, T>::~TypeSharedPtr ()
{
}



/*
==============================================================================
Name : set
Description :
   Push blob to video thread
==============================================================================
*/

template <class Target, class T>
void  TypeSharedPtr <Target, T>::set (const mem::SharedPtr <T> & data_sptr)
{
   archi::UByte buf [sizeof (mem::SharedPtr <T>)];
   
   new (buf) mem::SharedPtr <T> (data_sptr);
   
   TransactionMaster::use ().push (
      do_set_proc, this, sizeof (mem::SharedPtr <T>), buf
   );
}



/*
==============================================================================
Name : do_set_proc
==============================================================================
*/

template <class Target, class T>
void  TypeSharedPtr <Target, T>::do_set_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_ptr != 0);
   assert (data_size == sizeof (mem::SharedPtr <T>));
   
   TypeSharedPtr * this_ptr = reinterpret_cast <TypeSharedPtr *> (target_ptr);
   
   const mem::SharedPtr <T> & data_sptr
      = reinterpret_cast <const mem::SharedPtr <T> &> (*data_ptr);
   
   this_ptr->do_set (data_sptr);
}



/*
==============================================================================
Name : do_set
==============================================================================
*/

template <class Target, class T>
void  TypeSharedPtr <Target, T>::do_set (const mem::SharedPtr <T> & data_sptr)
{
   _cur_val = data_sptr;
   
   call_proc ();
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target, class T>
mem::SharedPtr <const T>   TypeSharedPtr <Target, T>::get_cur_value ()
{
   return _cur_val;
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target, class T>
void  TypeSharedPtr <Target, T>::notify_parent_destroyable ()
{
   _target.remove_type_debug (*this);
   TransactionMaster::use ().debug_remove (this);
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : animate
==============================================================================
*/

template <class Target, class T>
void  TypeSharedPtr <Target, T>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeSharedPtr_CODEHEADER_INCLUDED

#undef ohm_opak_TypeSharedPtr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
