/*****************************************************************************

        TypeMessage.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67886

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



#if defined (ohm_opak_TypeMessage_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeMessage code header.
#endif
#define  ohm_opak_TypeMessage_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeMessage_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeMessage_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/TransactionMaster.h"



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Target, class T>
TypeMessage <Target, T>::TypeMessage (Target & target, Proc proc)
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

template <class Target, class T>
TypeMessage <Target, T>::~TypeMessage ()
{
}



/*
==============================================================================
Name : send
Description :
   Push blob to video thread
==============================================================================
*/

template <class Target, class T>
void  TypeMessage <Target, T>::send (const T & obj)
{
   archi::UByte buf [sizeof (T)];
   
   new (buf) T (obj);
   
   TransactionMaster::use ().push (
      do_send_proc, this, sizeof (T), buf
   );
}



/*
==============================================================================
Name : do_signal_proc
==============================================================================
*/

template <class Target, class T>
void  TypeMessage <Target, T>::do_send_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_ptr != 0);
   assert (data_size == sizeof (T));
   
   TypeMessage * this_ptr = reinterpret_cast <TypeMessage *> (target_ptr);
   
   const T & obj
      = reinterpret_cast <const T &> (*data_ptr);
   
   this_ptr->do_send (obj);
   
   // dtor of obj is missing here, we rely on the fact that dtor
   // is not needed for class T
}



/*
==============================================================================
Name : do_send
==============================================================================
*/

template <class Target, class T>
void  TypeMessage <Target, T>::do_send (const T & obj)
{
   call_proc (obj);
}



/*
==============================================================================
Name : notify_parent_destroyable
==============================================================================
*/

#if defined (OHM_OPAK_TRANSACTION_DEBUG_FLAG)
template <class Target, class T>
void  TypeMessage <Target, T>::notify_parent_destroyable ()
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
void  TypeMessage <Target, T>::call_proc (const T & obj)
{
   assert (&_target != 0);
   (_target.*_proc) (obj);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeMessage_CODEHEADER_INCLUDED

#undef ohm_opak_TypeMessage_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
