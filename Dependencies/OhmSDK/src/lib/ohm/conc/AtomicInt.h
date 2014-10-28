/*****************************************************************************

        AtomicInt.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 39813

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



#if ! defined (ohm_conc_AtomicInt_HEADER_INCLUDED)
#define  ohm_conc_AtomicInt_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"
#include "ohm/lang/def.h"
#include "ohm/conc/AtomicMem.h"



namespace ohm
{
namespace conc
{



template <class T>
class AtomicInt
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  DataType;

   inline         AtomicInt ();
   inline explicit
                  AtomicInt (T val);
   inline         AtomicInt (const AtomicInt <T> &other);
   inline AtomicInt <T> &
                  operator = (T other);

   inline         operator T () const;

   inline T       swap (T other);
   inline T       cas (T other, T comp);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         SZ    = sizeof (T)   };
   enum {         SL2   = (SZ > 16) ? -1 : ((SZ > 8) ? 4 : ((SZ > 4) ? 3 : ((SZ > 2) ? 2 : ((SZ > 1) ? 1 : 0))))  };

   typedef  AtomicMem <SL2>   StoredTypeWrapper;
   typedef  typename StoredTypeWrapper::DataType   StoredType;
   typedef  typename StoredTypeWrapper::DataTypeAlign StoredTypeAlign;

   ohm_lang_CHECK_CST (Size, sizeof (T) <= sizeof (StoredType));

   volatile StoredTypeAlign
                  _val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class AtomicInt



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/AtomicInt.hpp"



#endif   // ohm_conc_AtomicInt_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
