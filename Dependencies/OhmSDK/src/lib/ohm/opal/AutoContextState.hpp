/*****************************************************************************

        AutoContextState.hpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18515

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



#if defined (ohm_opal_AutoContextState_CURRENT_CODEHEADER)
   #error Recursive inclusion of AutoContextState code header.
#endif
#define  ohm_opal_AutoContextState_CURRENT_CODEHEADER

#if ! defined (ohm_opal_AutoContextState_CODEHEADER_INCLUDED)
#define  ohm_opal_AutoContextState_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



AutoContextState::AutoContextState (GraphicContextInterface &context)
:  _context (context)
{
   assert (&context != 0);

   _context.save_graphic_state ();
}



AutoContextState::~AutoContextState ()
{
   _context.restore_graphic_state ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_AutoContextState_CODEHEADER_INCLUDED

#undef ohm_opal_AutoContextState_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
