/*****************************************************************************

        Delegate.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40608

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



#if defined (ohm_util_Delegate_CURRENT_CODEHEADER)
   #error Recursive inclusion of Delegate code header.
#endif
#define  ohm_util_Delegate_CURRENT_CODEHEADER

#if ! defined (ohm_util_Delegate_CODEHEADER_INCLUDED)
#define  ohm_util_Delegate_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <typeinfo>
#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*Method) ()>
void  Delegate::set (ReceiverType & receiver)
{
   assert (&receiver != 0);
   
   _receiver_ptr = &receiver;
   _param_type_info_ptr = 0;
   _stub = &Delegate::method_receiver_void <ReceiverType, Method>;
}



/*
==============================================================================
Name : param_set
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType)>
void  Delegate::param_set (ReceiverType & receiver)
{
   assert (&receiver != 0);
   
   _receiver_ptr = &receiver;
   _param_type_info_ptr = &typeid (ParamType);
   _stub = &Delegate::method_receiver_param <ReceiverType, ParamType, Method>;
}



/*
==============================================================================
Name : param_ref_set
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType &)>
void  Delegate::param_ref_set (ReceiverType & receiver)
{
   assert (&receiver != 0);
   
   _receiver_ptr = &receiver;
   _param_type_info_ptr = &typeid (ParamType &);
   _stub = &Delegate::method_receiver_param_ref <ReceiverType, ParamType, Method>;
}



/*
==============================================================================
Name : const_param_ref_set
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (const ParamType &)>
void  Delegate::const_param_ref_set (ReceiverType & receiver)
{
   assert (&receiver != 0);
   
   _receiver_ptr = &receiver;
   _param_type_info_ptr = &typeid (const ParamType &);
   _stub = &Delegate::method_receiver_const_param_ref <ReceiverType, ParamType, Method>;
}



/*
==============================================================================
Name : static_set
==============================================================================
*/

template <void (*Method) ()>
void  Delegate::static_set ()
{
   _receiver_ptr = 0;
   _param_type_info_ptr = 0;
   _stub = &Delegate::method_static_void <Method>;
}



/*
==============================================================================
Name : static_param_set
==============================================================================
*/

template <class ParamType, void (*Method) (ParamType)>
void  Delegate::static_param_set ()
{
   _receiver_ptr = 0;
   _param_type_info_ptr = &typeid (ParamType);
   _stub = &Delegate::method_static_param <ParamType, Method>;
}



/*
==============================================================================
Name : static_param_ref_set
==============================================================================
*/

template <class ParamType, void (*Method) (ParamType &)>
void  Delegate::static_param_ref_set ()
{
   _receiver_ptr = 0;
   _param_type_info_ptr = &typeid (ParamType &);
   _stub = &Delegate::method_static_param_ref <ParamType, Method>;
}



/*
==============================================================================
Name : static_const_param_ref_set
==============================================================================
*/

template <class ParamType, void (*Method) (const ParamType &)>
void  Delegate::static_const_param_ref_set ()
{
   _receiver_ptr = 0;
   _param_type_info_ptr = &typeid (const ParamType &);
   _stub = &Delegate::method_static_const_param_ref <ParamType, Method>;
}



/*
==============================================================================
Name : param_signal
==============================================================================
*/

template <class ParamType>
void  Delegate::param_signal (ParamType param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      assert (&typeid (ParamType) == _param_type_info_ptr);
      
      const void * param_ptr = &param;
      void * cparam_ptr = const_cast <void *> (param_ptr);
      
      // _receiver_ptr might be null
      //_stub (_receiver_ptr, &param);
      _stub (_receiver_ptr, cparam_ptr);
   }
}



/*
==============================================================================
Name : param_ref_signal
==============================================================================
*/

template <class ParamType>
void  Delegate::param_ref_signal (ParamType & param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      assert (&typeid (ParamType &) == _param_type_info_ptr);
      
      // _receiver_ptr might be null
      _stub (_receiver_ptr, &param);
   }
}



/*
==============================================================================
Name : const_param_ref_signal
==============================================================================
*/

template <class ParamType>
void  Delegate::const_param_ref_signal (const ParamType & param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      assert (&typeid (const ParamType &) == _param_type_info_ptr);
      
      ParamType * param_ptr = const_cast <ParamType *> (&param);
      
      // _receiver_ptr might be null
      _stub (_receiver_ptr, param_ptr);
   }
}



/*
==============================================================================
Name : param_signal_if_match
==============================================================================
*/

template <class ParamType>
void  Delegate::param_signal_if_match (ParamType param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      if (&typeid (param) == _param_type_info_ptr)
      {
         const void * param_ptr = &param;
         void * cparam_ptr = const_cast <void *> (param_ptr);
         
         // _receiver_ptr might be null
         //_stub (_receiver_ptr, &param);
         _stub (_receiver_ptr, cparam_ptr);
      }
   }
}



/*
==============================================================================
Name : param_ref_signal_if_match
==============================================================================
*/

template <class ParamType>
void  Delegate::param_ref_signal_if_match (ParamType & param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      if (&typeid (param) == _param_type_info_ptr)
      {
         // _receiver_ptr might be null
         _stub (_receiver_ptr, &param);
      }
   }
}



/*
==============================================================================
Name : const_param_ref_signal_if_match
==============================================================================
*/

template <class ParamType>
void  Delegate::const_param_ref_signal_if_match (const ParamType & param) const
{
   assert (&param != 0);
   
   if (_stub != 0)
   {
      if (&typeid (param) == _param_type_info_ptr)
      {
         ParamType * param_ptr = const_cast <ParamType *> (&param);
         
         // _receiver_ptr might be null
         _stub (_receiver_ptr, param_ptr);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : method_receiver_void
==============================================================================
*/

template <class ReceiverType, void (ReceiverType::*Method) ()>
void  Delegate::method_receiver_void (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr != 0);
   assert (param_ptr == 0);
   
   ReceiverType * casted_receiver_ptr = static_cast <ReceiverType *> (receiver_ptr);
   
   (casted_receiver_ptr->*Method) ();
}



/*
==============================================================================
Name : method_receiver_param
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType)>
void  Delegate::method_receiver_param (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr != 0);
   assert (param_ptr != 0);
   
   ReceiverType * casted_receiver_ptr = static_cast <ReceiverType *> (receiver_ptr);
   ParamType * casted_param_ptr = static_cast <ParamType *> (param_ptr);
   
   (casted_receiver_ptr->*Method) (*casted_param_ptr);
}



/*
==============================================================================
Name : method_receiver_param_ref
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType &)>
void  Delegate::method_receiver_param_ref (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr != 0);
   assert (param_ptr != 0);
   
   ReceiverType * casted_receiver_ptr = static_cast <ReceiverType *> (receiver_ptr);
   ParamType * casted_param_ptr = static_cast <ParamType *> (param_ptr);
   
   (casted_receiver_ptr->*Method) (*casted_param_ptr);
}



/*
==============================================================================
Name : method_receiver_const_param_ref
==============================================================================
*/

template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (const ParamType &)>
void  Delegate::method_receiver_const_param_ref (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr != 0);
   assert (param_ptr != 0);
   
   ReceiverType * casted_receiver_ptr = static_cast <ReceiverType *> (receiver_ptr);
   const ParamType * casted_param_ptr = static_cast <const ParamType *> (param_ptr);
   
   (casted_receiver_ptr->*Method) (*casted_param_ptr);
}



/*
==============================================================================
Name : method_static_void
==============================================================================
*/

template <void (*Method) ()>
void  Delegate::method_static_void (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr == 0);
   assert (param_ptr == 0);
   
   (*Method) ();
}



/*
==============================================================================
Name : method_static_param
==============================================================================
*/

template <class ParamType, void (*Method) (ParamType)>
void  Delegate::method_static_param (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr == 0);
   assert (param_ptr != 0);
   
   ParamType * casted_param_ptr = static_cast <ParamType *> (param_ptr);
   
   (*Method) (*casted_param_ptr);
}



/*
==============================================================================
Name : method_static_param_ref
==============================================================================
*/

template <class ParamType, void (*Method) (ParamType &)>
void  Delegate::method_static_param_ref (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr == 0);
   assert (param_ptr != 0);
   
   ParamType * casted_param_ptr = static_cast <ParamType *> (param_ptr);
   
   (*Method) (*casted_param_ptr);
}



/*
==============================================================================
Name : method_static_const_param_ref
==============================================================================
*/

template <class ParamType, void (*Method) (const ParamType &)>
void  Delegate::method_static_const_param_ref (void * receiver_ptr, void * param_ptr)
{
   assert (receiver_ptr == 0);
   assert (param_ptr != 0);
   
   const ParamType * casted_param_ptr = static_cast <const ParamType *> (param_ptr);
   
   (*Method) (*casted_param_ptr);
}



}  // namespace util
}  // namespace ohm



#endif   // ohm_util_Delegate_CODEHEADER_INCLUDED

#undef ohm_util_Delegate_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
