/*****************************************************************************

        ObjectRef.hpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48808

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



#if defined (ohm_flip_ObjectRef_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectRef code header.
#endif
#define  ohm_flip_ObjectRef_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ObjectRef_CODEHEADER_INCLUDED)
#define  ohm_flip_ObjectRef_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/MachineCode.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

template <class T>
void  ObjectRef <T>::declare ()
{
   ClassDescription <ObjectRef <T> >::use ().set_name (BasicTypesName::_object_ref_0);
   
   ClassDescManager::declare (ClassDescription <ObjectRef <T> >::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectRef <T>::ObjectRef (DocumentBase & document)
:  ObjectRefMaster (document)
,  _ref (0)
,  _old_ref (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
ObjectRef <T>::~ObjectRef ()
{
   // destruction (undefine) is handled in base class Object

   assert (_ref == 0);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
void  ObjectRef <T>::ctor (ohm::flip::Args & args)
{
   if (!args.empty ())
   {
      args.pop (*this);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
void  ObjectRef <T>::ctor (Ref ref)
{
   make_transaction_and_set (ref);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (const T * ptr)
{
   // ptr can be null
   
   if (ptr != 0)
   {
      make_transaction_and_set (ptr->get_ref ());
   }
   else
   {
      make_transaction_and_set (0);
   }
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (Ref ref)
{
   make_transaction_and_set (ref);
   
   return *this;
}



/*
==============================================================================
Name : operator T *
==============================================================================
*/

template <class T>
ObjectRef <T>::operator T * () const
{
   T * ptr = 0;
   
   if (_ref != 0)
   {
      // compile error : did you include the 'T' class ?
      ptr = dynamic_cast <T *> (use_document ().get_object (_ref));

      // ptr can be null
   }
   
   return ptr;
}



/*
==============================================================================
Name : operator T *
==============================================================================
*/

template <class T>
void  ObjectRef <T>::reset (const T * ptr)
{
   this->operator = (ptr);
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
Ref   ObjectRef <T>::get () const
{
   return _ref;
}



/*
==============================================================================
Name : set
==============================================================================
*/

template <class T>
void  ObjectRef <T>::set (Ref ref)
{
   make_transaction_and_set (ref);
}



/*
==============================================================================
Name : did_ref_change
==============================================================================
*/

template <class T>
bool  ObjectRef <T>::did_ref_change () const
{
   return _ref != _old_ref;
}



/*
==============================================================================
Name : get_old_ref
==============================================================================
*/

template <class T>
Ref   ObjectRef <T>::get_old_ref () const
{
   return _old_ref;
}



/*
==============================================================================
Name : get_old_ref
==============================================================================
*/

template <class T>
T *   ObjectRef <T>::get_old_ptr () const
{
   T * ptr = 0;
   
   if (_old_ref != 0)
   {
      ptr = dynamic_cast <T *> (use_document ().get_object (_old_ref));
      
      // ptr can be null
   }

   return ptr;
}



/*
==============================================================================
Name : backup
==============================================================================
*/

template <class T>
void  ObjectRef <T>::backup ()
{
   make_transaction_and_set (0);
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

template <class T>
void  ObjectRef <T>::synchronize ()
{
   _old_ref = _ref;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

template <class T>
void  ObjectRef <T>::check_synchronized () const
{
   assert (_old_ref == _ref);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

template <class T>
void  ObjectRef <T>::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_ref != _ref)
   {
      ohm_util_TRACE_DEBUG ("  REF");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_ref
==============================================================================
*/

template <class T>
void  ObjectRef <T>::set_ref (Ref ref)
{
   assert (_ref != ref);
   
   if (_old_ref == _ref)
   {
      _ref = ref;
      
      invalidate ();
   }
   else if (_old_ref == ref)
   {
      _ref = ref;
      
      validate ();
   }
   else
   {
      _ref = ref;
   }
}



/*
==============================================================================
Name : make_transaction_and_set
==============================================================================
*/

template <class T>
void  ObjectRef <T>::make_transaction_and_set (Ref ref)
{
   if (_ref == ref)
   {
      return;
   }
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   MachineCode::build_object_ref_set (transaction, this_ref, _ref, ref);
   
   set_ref (ref);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ObjectRef_CODEHEADER_INCLUDED

#undef ohm_flip_ObjectRef_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
