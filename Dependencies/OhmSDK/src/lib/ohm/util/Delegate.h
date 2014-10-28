/*****************************************************************************

        Delegate.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44887

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



#if ! defined (ohm_util_Delegate_HEADER_INCLUDED)
#define  ohm_util_Delegate_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <typeinfo>



namespace ohm
{
namespace util
{



class Delegate
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Delegate ();
                  Delegate (const Delegate &other);
   virtual        ~Delegate ();

   Delegate &     operator = (const Delegate &other);
   bool           operator == (const Delegate &other) const;
   bool           operator != (const Delegate &other) const;
   bool           operator < (const Delegate &other) const;
   
   template <class ReceiverType, void (ReceiverType::*Method) ()>
   void           set (ReceiverType & receiver);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType)>
   void           param_set (ReceiverType & receiver);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType &)>
   void           param_ref_set (ReceiverType & receiver);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (const ParamType &)>
   void           const_param_ref_set (ReceiverType & receiver);

   template <void (*Method) ()>
   void           static_set ();
   template <class ParamType, void (*Method) (ParamType)>
   void           static_param_set ();
   template <class ParamType, void (*Method) (ParamType &)>
   void           static_param_ref_set ();
   template <class ParamType, void (*Method) (const ParamType &)>
   void           static_const_param_ref_set ();
   
   void           reset ();
   
   bool           is_set () const;
   
   void *         get_receiver_ptr () const;
   
   void           signal () const;
   void           cast_signal (void * param_ptr) const;
   template <class ParamType>
   void           param_signal (ParamType param) const;
   template <class ParamType>
   void           param_ref_signal (ParamType & param) const;
   template <class ParamType>
   void           const_param_ref_signal (const ParamType & param) const;
   template <class ParamType>
   void           param_signal_if_match (ParamType param) const;
   template <class ParamType>
   void           param_ref_signal_if_match (ParamType & param) const;
   template <class ParamType>
   void           const_param_ref_signal_if_match (const ParamType & param) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef void (*StubType) (void * receiver_ptr, void * param_ptr);
   
   template <class ReceiverType, void (ReceiverType::*Method) ()>
   static void    method_receiver_void (void * receiver_ptr, void * param_ptr);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType)>
   static void    method_receiver_param (void * receiver_ptr, void * param_ptr);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (ParamType &)>
   static void    method_receiver_param_ref (void * receiver_ptr, void * param_ptr);
   template <class ReceiverType, class ParamType, void (ReceiverType::*Method) (const ParamType &)>
   static void    method_receiver_const_param_ref (void * receiver_ptr, void * param_ptr);

   template <void (*Method) ()>
   static void    method_static_void (void * receiver_ptr, void * param_ptr);
   template <class ParamType, void (*Method) (ParamType)>
   static void    method_static_param (void * receiver_ptr, void * param_ptr);
   template <class ParamType, void (*Method) (ParamType &)>
   static void    method_static_param_ref (void * receiver_ptr, void * param_ptr);
   template <class ParamType, void (*Method) (const ParamType &)>
   static void    method_static_const_param_ref (void * receiver_ptr, void * param_ptr);
   
   void *         _receiver_ptr;
   const std::type_info *
                  _param_type_info_ptr;
   StubType       _stub;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Delegate



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/Delegate.hpp"


#endif   // ohm_util_Delegate_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
