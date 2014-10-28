/*****************************************************************************

        Vect128MemOpUnalign.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18868

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



#if ! defined (ohm_archi_Vect128MemOpUnalign_HEADER_INCLUDED)
#define  ohm_archi_Vect128MemOpUnalign_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace archi
{



class Vect128MemOpUnalign
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   ohm_archi_FORCEINLINE static bool
                  check_address (const void *ptr);
   ohm_archi_FORCEINLINE static Vect128
                  load (const void *ptr);
   ohm_archi_FORCEINLINE static void
                  store (void *ptr, const Vect128 &val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Vect128MemOpUnalign ();
                  Vect128MemOpUnalign (const Vect128MemOpUnalign &other);
                  ~Vect128MemOpUnalign ();
   Vect128MemOpUnalign &
                  operator = (const Vect128MemOpUnalign &other);
   bool           operator == (const Vect128MemOpUnalign &other);
   bool           operator != (const Vect128MemOpUnalign &other);

}; // class Vect128MemOpUnalign



}  // namespace archi
}  // namespace ohm



#include "ohm/archi/Vect128MemOpUnalign.hpp"



#endif   // ohm_archi_Vect128MemOpUnalign_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
