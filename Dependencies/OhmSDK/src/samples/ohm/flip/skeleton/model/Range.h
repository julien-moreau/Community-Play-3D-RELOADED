/*****************************************************************************

        Range.h
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



#if ! defined (model_Range_HEADER_INCLUDED)
#define  model_Range_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace model
{



template <class T>
class Range
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Range ();
                  Range (T min, T max);
                  Range (const Range &other);
   virtual        ~Range ();
   
   Range &        operator = (const Range & other);
   
   Range &        operator &= (const Range & other);
   Range &        operator |= (const Range & other);
   Range &        operator += (const Range & other);
   Range &        operator -= (const Range & other);

   Range &        operator += (T val);
   Range &        operator -= (T val);

   bool           operator == (const Range &other) const;
   bool           operator != (const Range &other) const;
   
   bool           is_null () const;
   bool           is_empty () const;
   void           limit_in_place (T & val) const;
   T              limit (const T & val) const;
   bool           contains (const T & val) const;
   
   T              _min;
   T              _max;
   
   static const Range
                  _zero;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class Range


typedef Range <ohm::archi::Float64> RangeFloat64;
typedef Range <ohm::archi::Int64>      RangeInt64;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <class T>
Range <T>               operator & (const Range <T> & l_op, const Range <T> & r_op);

template <class T>
Range <T>               operator | (const Range <T> & l_op, const Range <T> & r_op);

template <class T>
Range <T>               operator + (const Range <T> & l_op, const Range <T> & r_op);

template <class T>
Range <T>               operator - (const Range <T> & l_op, const Range <T> & r_op);

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "model/Range.hpp"



#endif   // model_Range_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
