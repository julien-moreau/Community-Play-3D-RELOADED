/*****************************************************************************

        AtomicIntOp.hpp
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



#if defined (ohm_conc_AtomicIntOp_CURRENT_CODEHEADER)
   #error Recursive inclusion of AtomicIntOp code header.
#endif
#define  ohm_conc_AtomicIntOp_CURRENT_CODEHEADER

#if ! defined (ohm_conc_AtomicIntOp_CODEHEADER_INCLUDED)
#define  ohm_conc_AtomicIntOp_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T, class F>
void  AtomicIntOp <T, F>::exec (AtomicInt <T> &atom, F &ftor)
{
   assert (&atom != 0);
   assert (&ftor != 0);

   T              val_new;
   T              val_old;
   exec_both (atom, ftor, val_old, val_new);
}



template <class T, class F>
T  AtomicIntOp <T, F>::exec_old (AtomicInt <T> &atom, F &ftor)
{
   assert (&atom != 0);
   assert (&ftor != 0);

   T              val_new;
   T              val_old;
   exec_both (atom, ftor, val_old, val_new);

   return (val_old);
}



template <class T, class F>
T  AtomicIntOp <T, F>::exec_new (AtomicInt <T> &atom, F &ftor)
{
   assert (&atom != 0);
   assert (&ftor != 0);

   T              val_new;
   T              val_old;
   exec_both (atom, ftor, val_old, val_new);

   return (val_new);
}



template <class T, class F>
void  AtomicIntOp <T, F>::exec_both (AtomicInt <T> &atom, F &ftor, T &val_old, T &val_new)
{
   assert (&atom != 0);
   assert (&ftor != 0);

   T              val_cur;
   do
   {
      val_cur = atom;
      val_new = ftor (val_cur);
      val_old = atom.cas (val_new, val_cur);
   }
   while (val_old != val_cur);
}



template <class T, class F>
void  AtomicIntOp_exec (AtomicInt <T> &atom, F &ftor)
{
   AtomicIntOp <T, F>::exec (atom, ftor);
}



template <class T, class F>
T  AtomicIntOp_exec_old (AtomicInt <T> &atom, F &ftor)
{
   return (AtomicIntOp <T, F>::exec_old (atom, ftor));
}



template <class T, class F>
T  AtomicIntOp_exec_new (AtomicInt <T> &atom, F &ftor)
{
   return (AtomicIntOp <T, F>::exec_new (atom, ftor));
}



template <class T, class F>
void  AtomicIntOp_exec_both (AtomicInt <T> &atom, F &ftor, T &val_old, T &val_new)
{
   AtomicIntOp <T, F>::exec_both (atom, ftor, val_old, val_new);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_AtomicIntOp_CODEHEADER_INCLUDED

#undef ohm_conc_AtomicIntOp_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
