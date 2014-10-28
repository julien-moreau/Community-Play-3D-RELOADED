/*****************************************************************************

        Enum.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if defined (ohm_flip_Enum_CURRENT_CODEHEADER)
   #error Recursive inclusion of Enum code header.
#endif
#define  ohm_flip_Enum_CURRENT_CODEHEADER

#if ! defined (ohm_flip_Enum_CODEHEADER_INCLUDED)
#define  ohm_flip_Enum_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/EnumDescription.h"
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

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::declare ()
{
   ClassDescription <Enum <EnumType, MIN, NBR> >::use ().set_name (BasicTypesName::_enum_0);
   
   ClassDescManager::declare (ClassDescription <Enum <EnumType, MIN, NBR> >::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
Enum <EnumType, MIN, NBR>::Enum (DocumentBase & document)
:  EnumMaster (document)
,  _val (EnumType (MIN))
,  _old_val (EnumType (MIN))
{
   assert (&document != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
Enum <EnumType, MIN, NBR>::~Enum ()
{
   // destruction (undefine) is handled in base class Object
   
   assert (_val == EnumType (MIN));
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::ctor (ohm::flip::Args & args)
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

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::ctor (EnumType val)
{
   make_transaction_and_set (val);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::ctor (archi::Int64 val)
{
   assert (val >= MIN);
   assert (val < MIN + NBR);

   make_transaction_and_set (static_cast <EnumType> (val));
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
archi::Int64   Enum <EnumType, MIN, NBR>::get () const
{
   return archi::Int64 (_val);
}



/*
==============================================================================
Name : get_min
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
archi::Int64   Enum <EnumType, MIN, NBR>::get_min_val () const
{
   return archi::Int64 (MIN);
}



/*
==============================================================================
Name : get_max
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
archi::Int64   Enum <EnumType, MIN, NBR>::get_nbr_val () const
{
   return archi::Int64 (NBR);
}



/*
==============================================================================
Name : get_name_for_val
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
int   Enum <EnumType, MIN, NBR>::get_name_for_val (const char * & name_0, archi::Int64 val) const
{
   return EnumDescription <EnumType>::use ().get_name_for_value (name_0, val);
}



/*
==============================================================================
Name : get_val_for_name
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
int   Enum <EnumType, MIN, NBR>::get_val_for_name (archi::Int64 & val, const std::string & name) const
{
   return EnumDescription <EnumType>::use ().get_value_for_name (val, name);
}



/*
==============================================================================
Name : get_val_for_name
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
const char *   Enum <EnumType, MIN, NBR>::get_enum_type_name () const
{
   return EnumDescription <EnumType>::use ().get_name ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
Enum <EnumType, MIN, NBR> &   Enum <EnumType, MIN, NBR>::operator = (EnumType val)
{
   assert (archi::Int64 (val) >= MIN);
   assert (archi::Int64 (val) < MIN + NBR);

   make_transaction_and_set (val);
   
   return *this;
}



/*
==============================================================================
Name : operator T *
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
Enum <EnumType, MIN, NBR>::operator EnumType () const
{
   return _val;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
bool  Enum <EnumType, MIN, NBR>::operator == (EnumType val) const
{
   return _val == val;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
bool  Enum <EnumType, MIN, NBR>::operator != (EnumType val) const
{
   return _val != val;
}



/*
==============================================================================
Name : did_value_change
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
bool  Enum <EnumType, MIN, NBR>::did_value_change () const
{
   return _val != _old_val;
}



/*
==============================================================================
Name : get_old_value
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
EnumType Enum <EnumType, MIN, NBR>::get_old_value () const
{
   return _old_val;
}



/*
==============================================================================
Name : backup
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::backup ()
{
   make_transaction_and_set (EnumType (MIN));
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::synchronize ()
{
   _old_val = _val;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::check_synchronized () const
{
   assert (_old_val == _val);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_val != _val)
   {
      ohm_util_TRACE_DEBUG ("  ENU");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*
==============================================================================
Name : trace
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::trace () const
{
   Object::trace ();
   
   const char * old_name_0 = 0;
   get_name_for_val (old_name_0, _old_val);
   
   const char * name_0 = 0;
   get_name_for_val (name_0, _val);
   
   ohm_util_TRACE_INFO2 (
      "ENU %1% -> %2%",
      old_name_0,
      name_0
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set_ref
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::set (archi::Int64 val)
{
   assert (_val != EnumType (val));
   assert (archi::Int64 (val) >= MIN);
   assert (archi::Int64 (val) < MIN + NBR);
   
   if (_old_val == _val)
   {
      _val = EnumType (val);
      
      invalidate ();
   }
   else if (_old_val == EnumType (val))
   {
      _val = EnumType (val);
      
      validate ();
   }
   else
   {
      _val = EnumType (val);
   }
}



/*
==============================================================================
Name : make_transaction_and_set
==============================================================================
*/

template <class EnumType, archi::Int64 MIN, archi::Int64 NBR>
void  Enum <EnumType, MIN, NBR>::make_transaction_and_set (EnumType val)
{
   if (_val == val)
   {
      return;
   }
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   MachineCode::build_enum_set (transaction, this_ref, _val, val);
   
   set (archi::Int64 (val));
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_Enum_CODEHEADER_INCLUDED

#undef ohm_flip_Enum_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
