/*****************************************************************************

        MemMgrInterface.h
        Copyright (c) 2006 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 15581

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



#if ! defined (ohm_mem_mmgr_MemMgrInterface_HEADER_INCLUDED)
#define  ohm_mem_mmgr_MemMgrInterface_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cstddef>



namespace ohm
{
namespace mem
{
namespace mmgr
{



class MemMgrInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~MemMgrInterface () {}

   void *         allocate_bytes (ptrdiff_t len);
   void           deallocate_bytes (void *ptr);

   ptrdiff_t      get_total_mem () const;
   ptrdiff_t      get_allocated_mem () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual void * do_allocate_bytes (ptrdiff_t len) = 0;
   virtual void   do_deallocate_bytes (void *ptr) = 0;

   virtual ptrdiff_t
                  do_get_total_mem () const = 0;
   virtual ptrdiff_t
                  do_get_allocated_mem () const = 0;



}; // class MemMgrInterface



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



//#include  "ohm/mem/mmgr/MemMgrInterface.hpp"



#endif   // ohm_mem_mmgr_MemMgrInterface_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
