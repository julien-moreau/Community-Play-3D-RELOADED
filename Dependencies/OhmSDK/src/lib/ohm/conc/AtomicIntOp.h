/*****************************************************************************

        AtomicIntOp.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 26453

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



#if ! defined (ohm_conc_AtomicIntOp_HEADER_INCLUDED)
#define  ohm_conc_AtomicIntOp_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"



namespace ohm
{
namespace conc
{



template <class T, class F>
class AtomicIntOp
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static inline void
                  exec (AtomicInt <T> &atom, F &ftor);
   static inline T
                  exec_old (AtomicInt <T> &atom, F &ftor);
   static inline T
                  exec_new (AtomicInt <T> &atom, F &ftor);
   static inline void
                  exec_both (AtomicInt <T> &atom, F &ftor, T &val_old, T &val_new);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  AtomicIntOp ();
                  AtomicIntOp (const AtomicIntOp &other);
   AtomicIntOp &  operator = (const AtomicIntOp &other);
   virtual        ~AtomicIntOp () {}
   bool           operator == (const AtomicIntOp &other);
   bool           operator != (const AtomicIntOp &other);

}; // class AtomicIntOp



template <class T, class F>
inline void AtomicIntOp_exec (AtomicInt <T> &atom, F &ftor);
template <class T, class F>
inline T AtomicIntOp_exec_old (AtomicInt <T> &atom, F &ftor);
template <class T, class F>
inline T AtomicIntOp_exec_new (AtomicInt <T> &atom, F &ftor);
template <class T, class F>
inline void AtomicIntOp_exec_both (AtomicInt <T> &atom, F &ftor, T &val_old, T &val_new);



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/AtomicIntOp.hpp"



#endif   // ohm_conc_AtomicIntOp_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
