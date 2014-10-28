/*****************************************************************************

        TypeTraits.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if ! defined (ohm_flip_TypeTraits_HEADER_INCLUDED)
#define  ohm_flip_TypeTraits_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Float64.h"
#include "ohm/flip/Bool.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Enum.h"



namespace ohm
{
namespace flip
{



template <class Type>
class TypeTraits
{
public:
   typedef void   BasicType;

}; // class TypeTraits



template <>
class TypeTraits <flip::Float64>
{
public:
   typedef archi::Float64  BasicType;
   
}; // class TypeTraits



template <>
class TypeTraits <flip::Bool>
{
public:
   typedef bool   BasicType;
   
}; // class TypeTraits



template <>
class TypeTraits <flip::Int64>
{
public:
   typedef archi::Int64 BasicType;
   
}; // class TypeTraits



template <>
class TypeTraits <flip::Blob>
{
public:
   typedef flip::Blob::Data BasicType;
   
}; // class TypeTraits



template <class T>
class TypeTraits <flip::ObjectRef <T> >
{
public:
   typedef archi::Int64 BasicType;
   
}; // class TypeTraits



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/TypeTraits.hpp"



#endif   // ohm_flip_TypeTraits_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
