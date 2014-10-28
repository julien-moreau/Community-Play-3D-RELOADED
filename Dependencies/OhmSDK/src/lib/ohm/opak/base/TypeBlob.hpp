/*****************************************************************************

        TypeBlob.hpp
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



#if defined (ohm_opak_TypeBlob_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeBlob code header.
#endif
#define  ohm_opak_TypeBlob_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeBlob_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeBlob_CODEHEADER_INCLUDED



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

template <class Target>
TypeBlob <Target>::TypeBlob (Target & target, Proc proc)
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

template <class Target>
TypeBlob <Target>::~TypeBlob ()
{
}



/*
==============================================================================
Name : set
Description :
   Push blob to video thread
==============================================================================
*/

template <class Target>
void  TypeBlob <Target>::set (const std::vector <archi::UByte> & data)
{
   size_t data_size = data.size ();
   
   if (data_size > 0)
   {
      TransactionMaster::use ().push (
         do_set_proc, this, data.size (), &data [0]
      );
   }
   else
   {
      TransactionMaster::use ().push (
         do_set_proc, this, 0, 0
      );
   }
}



/*
==============================================================================
Name : do_set_proc
==============================================================================
*/

template <class Target>
void  TypeBlob <Target>::do_set_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert ( 
      (data_ptr != 0 && data_size > 0) || 
      (data_ptr == 0 && data_size == 0)
   );
   
   TypeBlob * this_ptr = reinterpret_cast <TypeBlob *> (target_ptr);
   
   this_ptr->do_set (data_ptr, data_size);
}



/*
==============================================================================
Name : do_set
==============================================================================
*/

template <class Target>
void  TypeBlob <Target>::do_set (const archi::UByte * data_ptr, size_t data_size)
{
   _cur_val.assign (data_ptr, data_ptr + data_size);
   
   call_proc ();
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target>
const std::vector <archi::UByte> &  TypeBlob <Target>::get_cur_value ()
{
   return _cur_val;
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target>
void  TypeBlob <Target>::notify_parent_destroyable ()
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

template <class Target>
void  TypeBlob <Target>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeBlob_CODEHEADER_INCLUDED

#undef ohm_opak_TypeBlob_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
