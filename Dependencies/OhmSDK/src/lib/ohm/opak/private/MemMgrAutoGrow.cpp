/*****************************************************************************

        MemMgrAutoGrow.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38760

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
#include "ohm/opak/private/MemMgrAutoGrow.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

MemMgrAutoGrow::MemMgrAutoGrow ()
:  _total_len (0)
,  _alloc_mem (0)
{
   // Nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

MemMgrAutoGrow::~MemMgrAutoGrow ()
{
   // Nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

void *   MemMgrAutoGrow::do_allocate_bytes (ptrdiff_t len)
{
   using namespace std;
   
   assert (len > 0);

   archi::UByte * data_ptr = new archi::UByte [len + sizeof (ptrdiff_t)];
   
   std::memcpy (data_ptr, &len, sizeof (ptrdiff_t));
   
   void * ptr = reinterpret_cast <void *> (data_ptr + sizeof (ptrdiff_t));
   
   _total_len += len;
   _alloc_mem += len;
   
   return (ptr);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  MemMgrAutoGrow::do_deallocate_bytes (void *ptr)
{
   assert (ptr != 0);

   archi::UByte * data_ptr = reinterpret_cast <archi::UByte *> (ptr);
   data_ptr -= sizeof (ptrdiff_t);
   
   ptrdiff_t len = 0;
   std::memcpy (&len, data_ptr, sizeof (ptrdiff_t));
   
   _total_len -= len;
   _alloc_mem -= len;
   
   delete [] data_ptr;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ptrdiff_t   MemMgrAutoGrow::do_get_total_mem () const
{
   return (_total_len);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ptrdiff_t   MemMgrAutoGrow::do_get_allocated_mem () const
{
   return (_alloc_mem);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
