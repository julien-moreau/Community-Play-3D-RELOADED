/*****************************************************************************

        Range.hpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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



#if defined (model_Range_CURRENT_CODEHEADER)
   #error Recursive inclusion of Range code header.
#endif
#define  model_Range_CURRENT_CODEHEADER

#if ! defined (model_Range_CODEHEADER_INCLUDED)
#define  model_Range_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Range <T>::Range ()
:  _min (T (0))
,  _max (T (0))
{
}

   
/*
==============================================================================
Name: Range
==============================================================================
*/

template <class T>
Range <T>::Range (T min, T max)
:  _min (min)
,  _max (max)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
Range <T>::Range (const Range &other)
:  _min (other._min)
,  _max (other._max)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
Range <T>::~Range ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator = (const Range & other)
{
   _min = other._min;
   _max = other._max;
   
   return *this;
}



/*
==============================================================================
Name : operator &=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator &= (const Range & other)
{
   using namespace ohm;
   
   lang::max_in_place (_min, other._min);
   lang::min_in_place (_max, other._max);
   
   return *this;
}



/*
==============================================================================
Name : operator &=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator |= (const Range & other)
{
   using namespace ohm;
   
   lang::min_in_place (_min, other._min);
   lang::max_in_place (_max, other._max);
   
   return *this;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator += (const Range & other)
{
   _min += other._min;
   _max += other._max;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator -= (const Range & other)
{
   _min -= other._max;
   _max -= other._min;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator += (T val)
{
   _min += val;
   _max += val;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

template <class T>
Range <T> & Range <T>::operator -= (T val)
{
   _min -= val;
   _max -= val;
   
   return *this;
}



/*
==============================================================================
Name : operator &=
==============================================================================
*/

template <class T>
bool  Range <T>::operator == (const Range &other) const
{
   return (_min == other._min) && (_max == other._max);
}



/*
==============================================================================
Name : operator &=
==============================================================================
*/

template <class T>
bool  Range <T>::operator != (const Range &other) const
{
   return (_min != other._min) || (_max != other._max);
}



/*
==============================================================================
Name : is_null
==============================================================================
*/

template <class T>
bool  Range <T>::is_null () const
{
   return _max < _min;
}



/*
==============================================================================
Name : is_empty
==============================================================================
*/

template <class T>
bool  Range <T>::is_empty () const
{
   return _max <= _min;
}



/*
==============================================================================
Name : limit_in_place
==============================================================================
*/

template <class T>
void  Range <T>::limit_in_place (T & val) const
{
   using namespace ohm;
   
   assert (!is_null ());
   
   lang::limit_in_place (val, _min, _max);
}



/*
==============================================================================
Name: limit
==============================================================================
*/

template <class T>
T Range <T>::limit (const T & val) const
{
   using namespace ohm;
   
   assert (!is_null ());
   
   return lang::limit (val, _min, _max);
}



/*
==============================================================================
Name: contains
==============================================================================
*/

template <class T>
bool Range <T>::contains (const T & val) const
{
   return (
      (val >= _min) && (val <= _max)
   );
}



template <class T>
const Range <T> Range <T>::_zero (0, 0);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator &
==============================================================================
*/

template <class T>
Range <T>   operator & (const Range <T> & l_op, const Range <T> & r_op)
{
   using namespace ohm;
   
   Range <T> ret_val;
   ret_val._min = lang::max (l_op._min, r_op._min);
   ret_val._max = lang::min (l_op._max, r_op._max);
   
   return ret_val;
}



/*
==============================================================================
Name : operator |
==============================================================================
*/

template <class T>
Range <T>   operator | (const Range <T> & l_op, const Range <T> & r_op)
{
   using namespace ohm;
   
   Range <T> ret_val;
   ret_val._min = lang::min (l_op._min, r_op._min);
   ret_val._max = lang::max (l_op._max, r_op._max);
   
   return ret_val;
}



/*
==============================================================================
Name : operator +
==============================================================================
*/

template <class T>
Range <T>   operator + (const Range <T> & l_op, const Range <T> & r_op)
{
   Range <T> ret_val;
   ret_val._min = l_op._min + r_op._min;
   ret_val._max = l_op._max + r_op._max;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

template <class T>
Range <T>   operator - (const Range <T> & l_op, const Range <T> & r_op)
{
   Range <T> ret_val;
   ret_val._min = l_op._min - r_op._max;
   ret_val._max = l_op._max - r_op._min;
   
   return ret_val;
}



}  // namespace model



#endif   // model_Range_CODEHEADER_INCLUDED

#undef model_Range_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
