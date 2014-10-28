/*****************************************************************************

        CachePool.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Cache.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/opak/base/GraphicContext.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

CachePool::~CachePool ()
{
   try
   {
      EngineCellPoolArrMap::iterator it = _cell_pool_arr_map.begin ();
      const EngineCellPoolArrMap::iterator it_end = _cell_pool_arr_map.end ();
      
      for (; it != it_end ; ++it)
      {
         CellPoolArr & cell_pool_arr = it->second;
         
         for (int i = 0 ; i < opal::SprungMode_NBR_ELT ; ++i)
         {
            for (int j = 0 ; j < opal::SprungMode_NBR_ELT ; ++j)
            {
               for (int k = 0 ; k < opal::FilterMode_NBR_ELT ; ++k)
               {
                  conc::CellPool <Item> & cell_pool
                     = cell_pool_arr._ [i][j][k];
                  
                  assert (cell_pool.take_cell (false) == 0);
               }
            }
         }
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  CachePool::release (opal::Engine & engine)
{
   EngineCellPoolArrMap::iterator it = _cell_pool_arr_map.find (&engine);
   
   if (it != _cell_pool_arr_map.end ())
   {
      CellPoolArr & cell_pool_arr = it->second;
      
      for (int i = 0 ; i < opal::SprungMode_NBR_ELT ; ++i)
      {
         for (int j = 0 ; j < opal::SprungMode_NBR_ELT ; ++j)
         {
            for (int k = 0 ; k < opal::FilterMode_NBR_ELT ; ++k)
            {
               conc::CellPool <Item> & cell_pool
                  = cell_pool_arr._ [i][j][k];
               
               cell_pool.clear_all ();
            }
         }
      }
   }
}



/*
==============================================================================
Name : take
==============================================================================
*/

CachePool::Item   CachePool::take (GraphicContext & context, opal::SprungMode sprung_mode_x, opal::SprungMode sprung_mode_y, opal::FilterMode filter_mode)
{
   opal::Engine & engine = context.get ().use_engine ();
   
   // might create if needed
   CellPoolArr & cell_pool_arr = _cell_pool_arr_map [&engine];
   
   conc::CellPool <Item> & cell_pool
      = cell_pool_arr._ [sprung_mode_x][sprung_mode_y][filter_mode];
   
   Cell * cell_ptr = cell_pool.take_cell (true);
   assert (cell_ptr != 0);
   
   Item & item = cell_ptr->_val;
   
   if (item._cache_sptr.get () == 0)
   {
      assert (item._context_sptr.get () == 0);
      assert (item._cell_ptr == 0);
      assert (item._cell_pool_ptr == 0);
      
      item._cache_sptr = mem::SharedPtr <opal::Cache> (
         new opal::Cache (context.get (), sprung_mode_x, sprung_mode_y, filter_mode)
      );
      item._context_sptr = mem::SharedPtr <GraphicContext> (
         new GraphicContext (*item._cache_sptr)
      );
      item._cell_ptr = cell_ptr;
      item._cell_pool_ptr = &cell_pool;
   }

   assert (item._cache_sptr.get () != 0);
   assert (item._context_sptr.get () != 0);
   assert (item._cell_ptr != 0);
   assert (item._cell_pool_ptr != 0);
   
   assert (item._context_sptr->get_stack_size () == 1);
   
   return item;
}



/*
==============================================================================
Name : give_back
==============================================================================
*/

void  CachePool::give_back (Item & item)
{
   assert (item._cache_sptr.get () != 0);
   assert (item._context_sptr.get () != 0);
   assert (item._cell_ptr != 0);
   assert (item._cell_pool_ptr != 0);
   
   assert (item._context_sptr->get_stack_size () == 1);
   
   conc::CellPool <Item> * cell_pool_ptr 
      = reinterpret_cast <conc::CellPool <Item> *> (item._cell_pool_ptr);
   
   Cell * cell_ptr = reinterpret_cast <Cell *> (item._cell_ptr);
   
   cell_pool_ptr->return_cell (*cell_ptr);
   
   item._cell_ptr = 0;
   item._cell_pool_ptr = 0;
}



/*
==============================================================================
Name : Item::ctor
==============================================================================
*/

CachePool::Item::Item ()
:  _cache_sptr ()
,  _context_sptr ()
,  _cell_ptr (0)
,  _cell_pool_ptr (0)
{
}



/*
==============================================================================
Name : Item::ctor
==============================================================================
*/

CachePool::Item::Item (const Item & other)
:  _cache_sptr (other._cache_sptr)
,  _context_sptr (other._context_sptr)
,  _cell_ptr (other._cell_ptr)
,  _cell_pool_ptr (other._cell_pool_ptr)
{
   assert (_cache_sptr.get () != 0);
   assert (_context_sptr.get () != 0);
   assert (_cell_ptr != 0);
   assert (_cell_pool_ptr != 0);
}



/*
==============================================================================
Name : Item::dtor
==============================================================================
*/

CachePool::Item::~Item ()
{
   try
   {
      _cache_sptr.destroy ();
      _context_sptr.destroy ();
      
      _cell_ptr = 0;
      _cell_pool_ptr = 0;
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : Item::operator =
==============================================================================
*/

CachePool::Item & CachePool::Item::operator = (const Item & other)
{
   _cache_sptr = other._cache_sptr;
   _context_sptr = other._context_sptr;
   _cell_ptr = other._cell_ptr;
   _cell_pool_ptr = other._cell_pool_ptr;

   assert (_cache_sptr.get () != 0);
   assert (_context_sptr.get () != 0);
   assert (_cell_ptr != 0);
   assert (_cell_pool_ptr != 0);
   
   return *this;
}



/*
==============================================================================
Name : Item::is_empty
==============================================================================
*/

bool  CachePool::Item::is_empty ()
{
   return (_cell_ptr == 0);
}



/*
==============================================================================
Name : Item::use_cache
==============================================================================
*/

opal::Cache &  CachePool::Item::use_cache ()
{
   assert (_cache_sptr.get () != 0);
   assert (_context_sptr.get () != 0);
   assert (_cell_ptr != 0);
   assert (_cell_pool_ptr != 0);
   
   return *_cache_sptr;
}



/*
==============================================================================
Name : Item::use_context
==============================================================================
*/

GraphicContext &  CachePool::Item::use_context ()
{
   assert (_cache_sptr.get () != 0);
   assert (_context_sptr.get () != 0);
   assert (_cell_ptr != 0);
   assert (_cell_pool_ptr != 0);
   
   return *_context_sptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

CachePool::CachePool ()
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
