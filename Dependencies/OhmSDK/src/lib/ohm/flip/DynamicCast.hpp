/*****************************************************************************

        DynamicCast.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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



#if defined (ohm_flip_DynamicCast_CURRENT_CODEHEADER)
   #error Recursive inclusion of DynamicCast code header.
#endif
#define  ohm_flip_DynamicCast_CURRENT_CODEHEADER

#if ! defined (ohm_flip_DynamicCast_CODEHEADER_INCLUDED)
#define  ohm_flip_DynamicCast_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentBase.h"

#include <cassert>

#include <typeinfo>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
DynamicCast <T>::DynamicCast (DocumentBase & document, ohm::flip::Ref ref)
:  _document (document)
,  _ref (ref)
{
   assert (&_document != 0);
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
DynamicCast <T>::DynamicCast (const DynamicCast &other)
:  _document (other._document)
,  _ref (other._ref)
{
   assert (&_document != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
DynamicCast <T>::~DynamicCast ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
DynamicCast <T> & DynamicCast <T>::operator = (const DynamicCast &other)
{
   _ref = other._ref;
   
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
DynamicCast <T> & DynamicCast <T>::operator = (ohm::flip::Ref ref)
{
   _ref = ref;
   
   return *this;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T>
T *   DynamicCast <T>::operator -> () const
{
   assert (_ref != 0);
   assert (&_document != 0);
   
   if (_ref == 0)
   {
      throw std::bad_cast ();
   }
   
   Object * obj_ptr = _document.get_object (_ref);
   
   assert (obj_ptr != 0);
   
   if (obj_ptr == 0)
   {
      throw std::bad_cast ();
   }
   
   T * cast_ptr = dynamic_cast <T *> (obj_ptr);

   assert (cast_ptr != 0);
   
   if (cast_ptr == 0)
   {
      throw std::bad_cast ();
   }
   
   return cast_ptr;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T>
T &   DynamicCast <T>::operator * () const
{
   return *operator -> ();
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T *   DynamicCast <T>::get () const
{
   assert (&_document != 0);

   T * ptr = 0;
   
   if (_ref != 0)
   {
      Object * obj_ptr = _document.get_object (_ref);
   
      if (obj_ptr != 0)
      {
         ptr = dynamic_cast <T *> (obj_ptr);
      }
   }
   
   return ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_DynamicCast_CODEHEADER_INCLUDED

#undef ohm_flip_DynamicCast_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
