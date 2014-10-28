/*****************************************************************************

        CellPool.hpp
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



#if defined (ohm_mem_mmgr_CellPool_CURRENT_CODEHEADER)
   #error Recursive inclusion of CellPool code header.
#endif
#define  ohm_mem_mmgr_CellPool_CURRENT_CODEHEADER

#if ! defined (ohm_mem_mmgr_CellPool_CODEHEADER_INCLUDED)
#define  ohm_mem_mmgr_CellPool_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace mem
{
namespace mmgr
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
CellPool <T>::CellPool ()
:  _zone_arr ()
,  _empty_ptr (0)
,  _capacity (0)
{
   // Nothing
}



template <class T>
CellPool <T>::CellPool (MemMgrInterface &mem_mgr)
:  _zone_arr (mem_mgr)
,  _empty_ptr (0)
,  _capacity (0)
{
   assert (&mem_mgr != 0);
}



template <class T>
CellPool <T>::~CellPool ()
{
   if (is_valid ())
   {
      erase_all_zones ();
   }
}



template <class T>
CellPool <T> & CellPool <T>::swap (CellPool <T> &other)
{
   _zone_arr.swap (        other._zone_arr);
   std::swap (_empty_ptr , other._empty_ptr);
   std::swap (_capacity  , other._capacity);

   return (*this);
}



template <class T>
void  CellPool <T>::set_mem_mgr (MemMgrInterface &mem_mgr)
{
   assert (&mem_mgr != 0);

   _zone_arr.set_mem_mgr (mem_mgr);
}



template <class T>
MemMgrInterface & CellPool <T>::use_mem_mgr () const
{
   assert (is_valid ());

   return (_zone_arr.use_mem_mgr ());
}



template <class T>
bool  CellPool <T>::is_valid () const
{
   return (_zone_arr.is_valid ());
}



template <class T>
void  CellPool <T>::reserve (long nbr_elt)
{
   assert (is_valid ());
   assert (nbr_elt >= 0);

   if (nbr_elt > _capacity)
   {
      const long     zone_len = nbr_elt - _capacity;
      add_zone (zone_len);
   }
}



template <class T>
long  CellPool <T>::get_capacity () const
{
   return (_capacity);
}



template <class T>
bool  CellPool <T>::has_free_cell () const
{
   return (_empty_ptr != 0);
}



// The cell could not be "clean", it hasn't just been contructed, meaning that
// misc. connections and other members could not be at their default values.
template <class T>
T &   CellPool <T>::use_free_cell ()
{
   assert (has_free_cell ());

   Cell &         cell = *_empty_ptr;
   _empty_ptr = _empty_ptr->get_child ();

   return (cell);
}



// Here the cell is freshly constructed.
template <class T>
T &   CellPool <T>::buy_new_cell ()
{
   bool           growth_flag = (_capacity == 0);
   if (! growth_flag)
   {
      const ZoneDesc &  last_zone = _zone_arr.use_back ();
      growth_flag = (last_zone._nbr_elt == last_zone._capacity);
   }
   if (growth_flag)
   {
      // x1.5 growth factor should be good.
      const long     zone_len = 1 + (_capacity >> 1);
      add_zone (zone_len);
   }

   ZoneDesc &     last_zone = _zone_arr.use_back ();
   assert (last_zone._nbr_elt < last_zone._capacity);

   Cell *         cell_ptr = &last_zone._zone_ptr [last_zone._nbr_elt];
   ::new (cell_ptr) Cell ();
   ++ last_zone._nbr_elt;

   return (*cell_ptr);
}



template <class T>
void  CellPool <T>::free_cell (T &cell)
{
   assert (&cell != 0);

   cell.connect (_empty_ptr);
   _empty_ptr = &cell;
}



template <class T>
void  CellPool <T>::free_all_cells ()
{
   const long     nbr_zones = _zone_arr.get_size ();

   _empty_ptr = 0;
   for (long zone_index = 0; zone_index < nbr_zones - 1; ++zone_index)
   {
      ZoneDesc &     zone = _zone_arr [zone_index];
      Cell *         zone_ptr = zone._zone_ptr;
      if (zone_ptr != 0)
      {
         const long     zone_len = zone._nbr_elt;
         for (long pos = 0; pos < zone_len; ++pos)
         {
            free_cell (zone_ptr [pos]);
         }
      }
   }

   if (nbr_zones > 0)
   {
      erase_one_zone_cells (nbr_zones - 1);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
void  CellPool <T>::add_zone (long zone_len)
{
   assert (zone_len > 0);

   _zone_arr.push_back ();
   ZoneDesc &     new_zone = _zone_arr.back ();

   MemMgrInterface & mem_mgr = _zone_arr.use_mem_mgr ();
   new_zone._zone_ptr = AllocTools <Cell>::allocate_mem (
      zone_len,
      mem_mgr
   );
   new_zone._capacity = zone_len;
   assert (new_zone._nbr_elt == 0);

   _capacity += zone_len;
}



template <class T>
void  CellPool <T>::erase_all_zones ()
{
   const long     nbr_zones = _zone_arr.get_size ();
   for (long zone_index = 0; zone_index < nbr_zones; ++zone_index)
   {
      erase_one_zone_cells (zone_index);

      ZoneDesc &     zone = _zone_arr [zone_index];
      Cell *         zone_ptr = zone._zone_ptr;
      if (zone_ptr != 0)
      {
         MemMgrInterface & mem_mgr = _zone_arr.use_mem_mgr ();
         mem_mgr.deallocate_bytes (zone_ptr);
      }
   }

   _zone_arr.clear ();
}



template <class T>
void  CellPool <T>::erase_one_zone_cells (long zone_index)
{
   assert (zone_index >= 0);
   assert (zone_index < _zone_arr.get_size ());

   ZoneDesc &     zone = _zone_arr [zone_index];
   Cell *         zone_ptr = zone._zone_ptr;
   if (zone_ptr != 0)
   {
      const long     zone_len = zone._nbr_elt;
      for (long pos = 0; pos < zone_len; ++pos)
      {
         Cell *         cell_ptr = &zone_ptr [pos];
         cell_ptr->Cell::~Cell ();
      }

      zone._nbr_elt = 0;
   }
   else
   {
      assert (zone._nbr_elt == 0);
   }
}



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_mmgr_CellPool_CODEHEADER_INCLUDED

#undef ohm_mem_mmgr_CellPool_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
