/*****************************************************************************

        AllocToolsBase.hpp
        Copyright (c) 2007 Ohm Force

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



#if defined (ohm_mem_mmgr_AllocToolsBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of AllocToolsBase code header.
#endif
#define  ohm_mem_mmgr_AllocToolsBase_CURRENT_CODEHEADER

#if ! defined (ohm_mem_mmgr_AllocToolsBase_CODEHEADER_INCLUDED)
#define  ohm_mem_mmgr_AllocToolsBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/MemMgrInterface.h"

#include <new>

#include <cassert>



namespace ohm
{
namespace mem
{
namespace mmgr
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
T *   AllocToolsBase <T>::allocate_one (MemMgrInterface &mem_mgr)
{
   assert (&mem_mgr != 0);

   const long     size_to_alloc = sizeof (T);
   void *         tmp_ptr = mem_mgr.allocate_bytes (size_to_alloc);
   if (tmp_ptr == 0)
   {
      throw std::bad_alloc ();
   }

   return (reinterpret_cast <T *> (tmp_ptr));
}



template <class T>
T *   AllocToolsBase <T>::allocate_mem (long len, MemMgrInterface &mem_mgr)
{
   assert (len > 0);
   assert (&mem_mgr != 0);

   const long     size_to_alloc = len * sizeof (T);
   void *         tmp_ptr = mem_mgr.allocate_bytes (size_to_alloc);
   if (tmp_ptr == 0)
   {
      throw std::bad_alloc ();
   }

   return (reinterpret_cast <T *> (tmp_ptr));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_mmgr_AllocToolsBase_CODEHEADER_INCLUDED

#undef ohm_mem_mmgr_AllocToolsBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
