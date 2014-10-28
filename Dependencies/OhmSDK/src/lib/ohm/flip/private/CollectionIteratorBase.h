/*****************************************************************************

        CollectionIteratorBase.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_flip_CollectionIteratorBase_HEADER_INCLUDED)
#define  ohm_flip_CollectionIteratorBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/FindMask.h"

#include <map>



namespace ohm
{
namespace flip
{



template <class T>   class Collection;
class DocumentBase;

template <class T, class ParentRefType, class iterator_type>
class CollectionIteratorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                        CollectionIteratorBase ();
                        CollectionIteratorBase (const CollectionIteratorBase & other);
   CollectionIteratorBase &
                        operator = (const CollectionIteratorBase & other);
   CollectionIteratorBase &
                        operator ++ ();
   CollectionIteratorBase &
                        operator -- ();
   CollectionIteratorBase 
                        operator ++ (int);
   CollectionIteratorBase 
                        operator -- (int);
   CollectionIteratorBase &
                        operator += (int n);
   CollectionIteratorBase &
                        operator -= (int n);
   T &                  operator * ();
   T *                  operator -> ();
   bool                 operator == (const CollectionIteratorBase & other);
   bool                 operator != (const CollectionIteratorBase & other);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class Collection <T>;
                        CollectionIteratorBase (iterator_type it, iterator_type it_end, bool in_post_flag, FindMask find_mask);
   
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



#include "ohm/flip/private/CollectionIteratorBase.hpp"



#endif   // ohm_flip_CollectionIteratorBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
