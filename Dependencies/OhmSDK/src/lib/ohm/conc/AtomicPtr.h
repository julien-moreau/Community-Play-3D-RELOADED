/*****************************************************************************

        AtomicPtr.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69933

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



#if ! defined (ohm_conc_AtomicPtr_HEADER_INCLUDED)
#define  ohm_conc_AtomicPtr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"
#include "ohm/lang/def.h"



namespace ohm
{
namespace conc
{



template <class T>
class AtomicPtr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   inline         AtomicPtr ();
   inline         AtomicPtr (T *ptr);
   inline AtomicPtr <T> &
                  operator = (T *other_ptr);

   inline         operator T * () const;

   bool           operator == (T *other_ptr) const;
   bool           operator != (T *other_ptr) const;

   inline T *     swap (T *other_ptr);
   inline T *     cas (T *other_ptr, T *comp_ptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_lang_CHECK_CST (PtrSize, (ohm_archi_WORD_SIZE == 8 * sizeof (T*)));
   
   ohm_archi_TYPEDEF_ALIGN (ohm_archi_WORD_SIZE_BYTE, T *, PtrAlign);

   inline T *     read_ptr () const;

   volatile PtrAlign
                  _ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           operator == (const AtomicPtr <T> &other);
   bool           operator != (const AtomicPtr <T> &other);

}; // class AtomicPtr



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/AtomicPtr.hpp"



#endif   // ohm_conc_AtomicPtr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
