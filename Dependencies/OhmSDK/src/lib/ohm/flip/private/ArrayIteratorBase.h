/*****************************************************************************

        ArrayIteratorBase.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55967

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



#if ! defined (ohm_flip_ArrayIteratorBase_HEADER_INCLUDED)
#define  ohm_flip_ArrayIteratorBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/FindMask.h"

#include <map>



namespace ohm
{
namespace flip
{



template <class T>   class Array;
class DocumentBase;

template <class T, class iterator_type>
class ArrayIteratorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                        ArrayIteratorBase ();
                        ArrayIteratorBase (const ArrayIteratorBase & other);
   ArrayIteratorBase &  operator = (const ArrayIteratorBase & other);
   ArrayIteratorBase &  operator ++ ();
   ArrayIteratorBase &  operator -- ();
   ArrayIteratorBase    operator ++ (int);
   ArrayIteratorBase    operator -- (int);
   ArrayIteratorBase &  operator += (int n);
   ArrayIteratorBase &  operator -= (int n);
   T &                  operator * ();
   T *                  operator -> ();
   bool                 operator == (const ArrayIteratorBase & other) const;
   bool                 operator != (const ArrayIteratorBase & other) const;
   
   bool                 was_added () const;
   bool                 was_removed () const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class Array <T>;
                        ArrayIteratorBase (iterator_type it, iterator_type it_end, bool in_post_flag, FindMask find_mask);

   bool                 is_matching_find_mask () const;
   
   iterator_type        _it;
   iterator_type        _it_end;
   bool                 _all_flag;
   bool                 _added_flag;
   bool                 _resident_flag;
   bool                 _removed_flag;
};



}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/flip/private/ArrayIteratorBase.hpp"



#endif   // ohm_flip_ArrayIteratorBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
