/*****************************************************************************

        CacheManager.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63406

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

#include "ohm/archi/types.h"
#include "ohm/math/def.h"
#include "ohm/math/fnc.h"
#include "ohm/opal/directx/CacheManager.h"
#include "ohm/opal/win32/ViewImpl.h"
#include "ohm/opal/View.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Throws : ?
==============================================================================
*/

CacheManager::CacheManager ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

CacheManager::~CacheManager ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  CacheManager::add (CacheImpl & cache)
{
   assert (&cache != 0);
   assert (_cache_ptr_set.find (&cache) == _cache_ptr_set.end ());
   
   _cache_ptr_set.insert (&cache);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  CacheManager::remove (CacheImpl & cache)
{
   assert (&cache != 0);
   assert (_cache_ptr_set.find (&cache) != _cache_ptr_set.end ());
   
   _cache_ptr_set.erase (&cache);
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   CacheManager::size () const
{
   return _cache_ptr_set.size ();
}



/*
==============================================================================
Name : release_all
Description :
   release all caches
==============================================================================
*/

void  CacheManager::release_all ()
{
   CachePtrSet::iterator it = _cache_ptr_set.begin ();
   const CachePtrSet::iterator it_end = _cache_ptr_set.end ();
   
   for (; it != it_end ;)
   {
      CachePtrSet::iterator it_next = it;
      ++it_next;
      
      CacheImpl * cache_ptr = *it;
      assert (cache_ptr != 0);
      
      cache_ptr->release ();
      
      it = it_next;
   }
}



/*
==============================================================================
Name : save_all
==============================================================================
*/

void  CacheManager::save_all ()
{
   CachePtrSet::iterator it = _cache_ptr_set.begin ();
   const CachePtrSet::iterator it_end = _cache_ptr_set.end ();
   
   for (; it != it_end ;)
   {
      CachePtrSet::iterator it_next = it;
      ++it_next;
      
      CacheImpl * cache_ptr = *it;
      assert (cache_ptr != 0);
      
      cache_ptr->save ();
      
      it = it_next;
   }
}



/*
==============================================================================
Name : restore_all
==============================================================================
*/

void  CacheManager::restore_all ()
{
   CachePtrSet::iterator it = _cache_ptr_set.begin ();
   const CachePtrSet::iterator it_end = _cache_ptr_set.end ();
   
   for (; it != it_end ;)
   {
      CachePtrSet::iterator it_next = it;
      ++it_next;
      
      CacheImpl * cache_ptr = *it;
      assert (cache_ptr != 0);
      
      cache_ptr->restore ();
      
      it = it_next;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
