/*****************************************************************************

        CellPool.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 44882

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



#if ! defined (ohm_conc_CellPool_CODEHEADER_INCLUDED)
#define  ohm_conc_CellPool_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AioMax.h"
#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"
#include "ohm/sys/CritSec.h"

#include <cassert>



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
CellPool <T>::CellPool ()
:  _cell_stack ()
,  _zone_list ()
,  _nbr_avail_cells (0)
,  _nbr_zones (0)
,  _alloc_mutex ()
{
   ohm_lang_CHECK_CST (Growth, BASE_SIZE * GROW_RATE_NUM / GROW_RATE_DEN > BASE_SIZE);

   for (int zone_index = 0; zone_index < MAX_NBR_ZONES; ++zone_index)
   {
      _zone_list [zone_index] = 0;
   }
}



template <class T>
CellPool <T>::~CellPool ()
{
   clear_all ();
}



// Not thread-safe.
// All cells must have been returned before calling this function.
template <class T>
void  CellPool <T>::clear_all ()
{
#if !defined (NDEBUG)
   size_t nbr_total_cells = compute_total_size_for_zones (_nbr_zones);
   
   assert (_nbr_avail_cells == nbr_total_cells);
#endif
   
   while (_cell_stack.pop () != 0)
   {
      continue;
   }

   for (int zone_index = 0; zone_index < _nbr_zones; ++zone_index)
   {
      conc::AtomicPtr <CellType> &  zone_ptr_ref = _zone_list [zone_index];
      CellType *     zone_ptr = zone_ptr_ref;
      if (zone_ptr != 0)
      {
         delete [] zone_ptr;
         zone_ptr_ref = 0;
      }
   }
   _nbr_zones = 0;
   _nbr_avail_cells = 0;
}



// Thread-safe but locks
template <class T>
void  CellPool <T>::expand_to (size_t nbr_cells)
{
   assert (nbr_cells > 0);

   size_t         cur_size = BASE_SIZE;
   size_t         total_size = 0;
   int            zone_index = 0;
   while (total_size < nbr_cells && zone_index < MAX_NBR_ZONES)
   {
      conc::AtomicPtr <CellType> &  zone_ptr_ref = _zone_list [zone_index];
      CellType *     zone_ptr = zone_ptr_ref;
      if (zone_ptr == 0)
      {
         allocate_zone (zone_index, cur_size, zone_ptr_ref);
      }

      total_size += cur_size;
      cur_size = compute_grown_size (cur_size);
      ++ zone_index;
   }

   conc::AioMax <int>   ftor (zone_index);
   conc::AtomicIntOp_exec (_nbr_zones, ftor);
}



// Thread-safe
template <class T>
typename CellPool <T>::CellType *   CellPool <T>::take_cell (bool autogrow_flag)
{
   CellType *     cell_ptr = 0;
   
   const int      nbr_zones = _nbr_zones;

   do
   {
      cell_ptr = _cell_stack.pop ();

      if ((cell_ptr == 0) && autogrow_flag && (nbr_zones < MAX_NBR_ZONES))
      {
         const size_t   new_size = compute_total_size_for_zones (nbr_zones + 1);
         expand_to (new_size);
      }
   }
   while ((cell_ptr == 0) && autogrow_flag && (nbr_zones < MAX_NBR_ZONES));

   if (cell_ptr != 0)
   {
      conc::AioAdd <size_t>   ftor (-1);
      conc::AtomicIntOp_exec (_nbr_avail_cells, ftor);
   }

   return (cell_ptr);
}



// Thread-safe
template <class T>
void  CellPool <T>::return_cell (CellType &cell)
{
   assert (&cell != 0);

   _cell_stack.push (cell);

   conc::AioAdd <size_t>   ftor (1);
   conc::AtomicIntOp_exec (_nbr_avail_cells, ftor);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
void  CellPool <T>::allocate_zone (int zone_index, size_t cur_size, conc::AtomicPtr <CellType> & zone_ptr_ref)
{
   assert (zone_index >= 0);
   assert (zone_index < MAX_NBR_ZONES);

   sys::CritSec   lock (_alloc_mutex);

   CellType *     zone_ptr = new CellType [cur_size];

   if (zone_ptr_ref.cas (zone_ptr, 0) != (CellType *)0)
   {
      // CAS has failed, meaning that another thread is allocating this zone.
      delete [] zone_ptr;

      // Note: because of the mutex, this part shouldn't be accessed.
   }

   else
   {
      // Success: fill the stack with the new cells.
      for (size_t pos = 0; pos < cur_size; ++pos)
      {
         CellType &     cell = zone_ptr [pos];
         _cell_stack.push (cell);

         conc::AioAdd <size_t>   ftor (1);
         conc::AtomicIntOp_exec (_nbr_avail_cells, ftor);
      }
   }
}



template <class T>
size_t   CellPool <T>::compute_grown_size (size_t prev_size)
{
   assert (prev_size >= BASE_SIZE);

   return (prev_size * GROW_RATE_NUM / GROW_RATE_DEN);
}



template <class T>
size_t   CellPool <T>::compute_total_size_for_zones (int nbr_zones)
{
   assert (nbr_zones >= 0);
   assert (nbr_zones <= MAX_NBR_ZONES);

   size_t         cur_size = BASE_SIZE;
   size_t         total_size = 0;
   int            zone_index = 0;
   while (zone_index < nbr_zones)
   {
      total_size += cur_size;
      cur_size = compute_grown_size (cur_size);
      ++ zone_index;
   }

   return (total_size);
}



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_CellPool_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
