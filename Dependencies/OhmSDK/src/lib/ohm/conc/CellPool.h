/*****************************************************************************

        CellPool.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 26655

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



#if ! defined (ohm_conc_CellPool_HEADER_INCLUDED)
#define  ohm_conc_CellPool_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"
#include "ohm/conc/LockFreeCell.h"
#include "ohm/conc/LockFreeStack.h"
#include "ohm/sys/Mutex.h"

#include <cstddef>



namespace ohm
{
namespace conc
{



template <class T>
class CellPool
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  DataType;
   typedef  conc::LockFreeCell <T>  CellType;

                  CellPool ();
   virtual        ~CellPool ();

   void           clear_all ();
   void           expand_to (size_t nbr_cells);

   inline CellType *
                  take_cell (bool autogrow_flag = false);
   inline void    return_cell (CellType &cell);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         MAX_NBR_ZONES  = 64  };
   enum {         GROW_RATE_NUM  = 3   };
   enum {         GROW_RATE_DEN  = 2   };
   enum {         BASE_SIZE      = 64  };    // Number of cells for the first zone

   typedef  conc::LockFreeStack <T> CellStack;
   typedef  conc::AtomicInt <size_t>   CountCells;
   typedef  conc::AtomicInt <int>      CountZones;
   typedef  mem::Array <conc::AtomicPtr <CellType>, 64>  ZoneList;

   void           allocate_zone (int zone_index, size_t cur_size, conc::AtomicPtr <CellType> & zone_ptr_ref);

   static inline size_t
                  compute_grown_size (size_t prev_size);
   static inline size_t
                  compute_total_size_for_zones (int nbr_zones);

   CellStack      _cell_stack;
   ZoneList       _zone_list;
   CountCells     _nbr_avail_cells;
   CountZones     _nbr_zones;
   sys::Mutex     _alloc_mutex;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CellPool (const CellPool &other);
   CellPool &     operator = (const CellPool &other);
   bool           operator == (const CellPool &other);
   bool           operator != (const CellPool &other);

}; // class CellPool



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/CellPool.hpp"



#endif   // ohm_conc_CellPool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
