/*****************************************************************************

        MemMgrAutoGrow.h
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



#if ! defined (ohm_opak_MemMgrAutoGrow_HEADER_INCLUDED)
#define  ohm_opak_MemMgrAutoGrow_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/MemMgrInterface.h"

#include <vector>



namespace ohm
{
namespace opak
{



class MemMgrAutoGrow
:  public mem::mmgr::MemMgrInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  MemMgrAutoGrow ();
   virtual        ~MemMgrAutoGrow ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // mem::mmgr::MemMgrInterface
   virtual void * do_allocate_bytes (ptrdiff_t len);
   virtual void   do_deallocate_bytes (void *ptr);

   virtual ptrdiff_t
                  do_get_total_mem () const;
   virtual ptrdiff_t
                  do_get_allocated_mem () const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ptrdiff_t      _total_len;
   ptrdiff_t      _alloc_mem;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  MemMgrAutoGrow (const MemMgrAutoGrow &other);
   MemMgrAutoGrow &        operator = (const MemMgrAutoGrow &other);
   bool           operator == (const MemMgrAutoGrow &other);
   bool           operator != (const MemMgrAutoGrow &other);

}; // class MemMgrAutoGrow



}  // namespace opak
}  // namespace ohm



//#include  "ohm/mem/mmgr/MemMgrAutoGrow.hpp"



#endif   // ohm_opak_MemMgrAutoGrow_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
