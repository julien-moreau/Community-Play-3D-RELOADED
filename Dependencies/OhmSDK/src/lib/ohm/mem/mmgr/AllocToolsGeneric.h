/*****************************************************************************

        AllocToolsGeneric.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18963

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



#if ! defined (ohm_mem_mmgr_AllocToolsGeneric_HEADER_INCLUDED)
#define  ohm_mem_mmgr_AllocToolsGeneric_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/AllocToolsBase.h"



namespace ohm
{
namespace mem
{
namespace mmgr
{



template <class T>
class AllocToolsGeneric
:  public AllocToolsBase <T>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static void    copy_zone (T dest_ptr [], const T src_ptr [], long len);
   static void    init_zone (T zone_ptr [], long len);
   static void    copy_and_init_zone (T dest_ptr [], const T src_ptr [], long len, long copy_len);
   static void    delete_zone (T zone_ptr [], long len);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  AllocToolsGeneric ();
                  AllocToolsGeneric (const AllocToolsGeneric &other);
//                ~AllocToolsGeneric ();
   AllocToolsGeneric &
                  operator = (const AllocToolsGeneric &other);
   bool           operator == (const AllocToolsGeneric &other);
   bool           operator != (const AllocToolsGeneric &other);

}; // class AllocToolsGeneric



}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



#include "ohm/mem/mmgr/AllocToolsGeneric.hpp"



#endif   // ohm_mem_mmgr_AllocToolsGeneric_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
