/*****************************************************************************

        CachePool.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59363

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



#if ! defined (ohm_opak_CachePool_HEADER_INCLUDED)
#define  ohm_opak_CachePool_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/CellPool.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/opal/SprungMode.h"
#include "ohm/opal/FilterMode.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/util/SingletonBase.h"

#include <map>



namespace ohm
{
namespace opal
{
class Cache;
}



namespace opak
{



class GraphicContext;

class CachePool
:  public util::SingletonBase <CachePool>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Item
   {
   public:
                  Item ();
                  Item (const Item & other);
      virtual     ~Item ();
      
      Item &      operator = (const Item & other);
      
      bool        is_empty ();
      
      opal::Cache &
                  use_cache ();
      GraphicContext &
                  use_context ();
   
   private:
      friend class CachePool;
      mem::SharedPtr <opal::Cache>
                  _cache_sptr;
      mem::SharedPtr <GraphicContext>
                  _context_sptr;
      void *      _cell_ptr;
      void *      _cell_pool_ptr;
   };
   
   virtual        ~CachePool ();
   
   void           release (opal::Engine & engine);
   
   Item           take (GraphicContext & context, opal::SprungMode sprung_mode_x = opal::SprungMode_DISTEND, opal::SprungMode sprung_mode_y = opal::SprungMode_DISTEND, opal::FilterMode filter_mode = opal::FilterMode_LINEAR);
   void           give_back (Item & item);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  CachePool ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (CachePool);
   
   typedef conc::CellPool <Item>::CellType   Cell;
   
   class CellPoolArr
   {
   public:
                  CellPoolArr () {}
                  CellPoolArr (const CellPoolArr & other) { /* actually nothing */}
      conc::CellPool <Item>
                  _  [opal::SprungMode_NBR_ELT][opal::SprungMode_NBR_ELT][opal::FilterMode_NBR_ELT];
   };
   
   typedef std::map <opal::Engine *, CellPoolArr> EngineCellPoolArrMap;
   
   EngineCellPoolArrMap
                  _cell_pool_arr_map;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CachePool (const CachePool &other);
   CachePool &    operator = (const CachePool &other);
   bool           operator == (const CachePool &other) const;
   bool           operator != (const CachePool &other) const;

}; // class CachePool



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/CachePool.hpp"



#endif   // ohm_opak_CachePool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
