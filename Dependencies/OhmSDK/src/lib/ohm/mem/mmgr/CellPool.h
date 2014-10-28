/*****************************************************************************

        CellPool.h
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



#if ! defined (ohm_mem_mmgr_CellPool_HEADER_INCLUDED)
#define  ohm_mem_mmgr_CellPool_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/Vector.h"



namespace ohm
{
namespace mem
{
namespace mmgr
{



class MemMgrInterface;

template <class T>
class CellPool
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  Cell;

                  CellPool ();
   explicit       CellPool (MemMgrInterface &mem_mgr);
   virtual        ~CellPool ();

   CellPool <T> & swap (CellPool <T> &other);

   inline void    set_mem_mgr (MemMgrInterface &mem_mgr);
   inline MemMgrInterface &
                  use_mem_mgr () const;
   inline bool    is_valid () const;

   void           reserve (long nbr_elt);
   inline long    get_capacity () const;

   inline bool    has_free_cell () const;
   inline T &     use_free_cell ();
   inline T &     buy_new_cell ();

   inline void    free_cell (T &cell);
   void           free_all_cells ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class ZoneDesc
   {
   public:
      ZoneDesc ()
      :  _zone_ptr (0)
      ,  _capacity (0)
      ,  _nbr_elt (0)
      {
         // Nothing
      }

      ZoneDesc (const ZoneDesc &other)
      :  _zone_ptr (other._zone_ptr)
      ,  _capacity (other._capacity)
      ,  _nbr_elt (other._nbr_elt)
      {
         // Nothing
      }

      Cell *         _zone_ptr;
      long           _capacity;
      long           _nbr_elt;
   };
   typedef  Vector <ZoneDesc> ZoneDescArray;

   void           add_zone (long zone_len);
   void           erase_all_zones ();
   void           erase_one_zone_cells (long zone_index);

   ZoneDescArray  _zone_arr;
   Cell *         _empty_ptr;
   long           _capacity;     // Total (sum of all zone capacities)



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CellPool (const CellPool &other);
   CellPool &     operator = (const CellPool &other);
   bool           operator == (const CellPool &other);
   bool           operator != (const CellPool &other);

}; // class CellPool



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#include "ohm/mem/mmgr/CellPool.hpp"



#endif   // ohm_mem_mmgr_CellPool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
