/*****************************************************************************

        Vector.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 16078

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



#if ! defined (ohm_mem_mmgr_Vector_HEADER_INCLUDED)
#define  ohm_mem_mmgr_Vector_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace mem
{
namespace mmgr
{



class MemMgrInterface;

template <class T>
class Vector
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  Element;
   typedef  Vector <T>  ThisType;

                  Vector ();
   explicit       Vector (MemMgrInterface &mem_mgr);
                  Vector (MemMgrInterface &mem_mgr, long size);
                  Vector (MemMgrInterface &mem_mgr, long size, long capacity);
                  Vector (const Vector <T> &other);
                  ~Vector ();

   Vector <T> &   operator = (const Vector <T> &other);
   Vector <T> &   swap (const Vector <T> &other);

   void           set_mem_mgr (MemMgrInterface &mem_mgr);
   MemMgrInterface &
                  use_mem_mgr () const;
   bool           is_valid () const;

   inline long    get_capacity () const;
   inline long    capacity () const;   // For std::vector compatibility
   void           kill ();
   void           reserve (long nbr_elt);

   inline long    get_size () const;
   inline long    size () const;       // For std::vector compatibility
   void           clear ();
   void           resize (long nbr_elt);

   void           push_back (const T &val = T());
   T &            use_back ();
   const T &      use_back () const;
   T &            back ();             // For std::vector compatibility
   const T &      back () const;       // For std::vector compatibility

   inline T &     operator [] (long pos);
   inline const T&operator [] (long pos) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           reserve_and_copy (long new_capacity, MemMgrInterface &mem_mgr, T src_ptr [], long copy_len);
   void           resize_internal (long nbr_elt, const T new_elt_arr []);

   MemMgrInterface *
                  _mem_mgr_ptr;
   T *            _data_ptr;
   long           _len;
   long           _capacity;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           operator == (const Vector &other);
   bool           operator != (const Vector &other);

}; // class Vector



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#include "ohm/mem/mmgr/Vector.hpp"



#endif   // ohm_mem_mmgr_Vector_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
