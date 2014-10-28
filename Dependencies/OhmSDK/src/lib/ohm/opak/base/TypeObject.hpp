/*****************************************************************************

        TypeObject.hpp
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



#if defined (ohm_opak_TypeObject_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeObject code header.
#endif
#define  ohm_opak_TypeObject_CURRENT_CODEHEADER

#if ! defined (ohm_opak_TypeObject_CODEHEADER_INCLUDED)
#define  ohm_opak_TypeObject_CODEHEADER_INCLUDED



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
TypeObject <Target, T>::TypeObject (Target & target, Proc proc)
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
TypeObject <Target, T>::~TypeObject ()
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
TypeObject <Target, T> &   TypeObject <Target, T>::operator = (const T & obj)
{
   set (obj);
   
   return *this;
}



/*
==============================================================================
Name : set
Description :
   Push blob to video thread
==============================================================================
*/

template <class Target, class T>
void  TypeObject <Target, T>::set (const T & obj)
{
   archi::UByte buf [sizeof (T)];
   
   new (buf) T (obj);
   
   TransactionMaster::use ().push (
      do_set_proc, this, sizeof (T), buf
   );
}



/*
==============================================================================
Name : do_set_proc
==============================================================================
*/

template <class Target, class T>
void  TypeObject <Target, T>::do_set_proc (void * target_ptr, size_t data_size, const archi::UByte * data_ptr)
{
   assert (target_ptr != 0);
   assert (data_ptr != 0);
   assert (data_size == sizeof (T));
   
   TypeObject * this_ptr = reinterpret_cast <TypeObject *> (target_ptr);
   
   const T & obj
      = reinterpret_cast <const T &> (*data_ptr);
   
   this_ptr->do_set (obj);
}



/*
==============================================================================
Name : do_set
==============================================================================
*/

template <class Target, class T>
void  TypeObject <Target, T>::do_set (const T & obj)
{
   _cur_val = obj;
   
   call_proc ();
}



/*
==============================================================================
Name : get_cur_value
==============================================================================
*/

template <class Target, class T>
T  TypeObject <Target, T>::get_cur_value ()
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
void  TypeObject <Target, T>::notify_parent_destroyable ()
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
void  TypeObject <Target, T>::call_proc ()
{
   assert (&_target != 0);
   (_target.*_proc) ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_TypeObject_CODEHEADER_INCLUDED

#undef ohm_opak_TypeObject_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
