/*****************************************************************************

        LockFreeBuffer.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 26462

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



#if defined (ohm_conc_LockFreeBuffer_CURRENT_CODEHEADER)
   #error Recursive inclusion of LockFreeBuffer code header.
#endif
#define  ohm_conc_LockFreeBuffer_CURRENT_CODEHEADER

#if ! defined (ohm_conc_LockFreeBuffer_CODEHEADER_INCLUDED)
#define  ohm_conc_LockFreeBuffer_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AioAnd.h"
#include "ohm/conc/AioOr.h"
#include "ohm/conc/AioSetBits.h"
#include "ohm/conc/AtomicIntOp.h"

#include <cassert>



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T> 
LockFreeBuffer <T>::LockFreeBuffer ()
:  _obj_arr ()
,  _flags (Flags_POS_R)
{
   // Nothing
}


template <class T> 
void  LockFreeBuffer <T>::init (const T &t)
{
   assert (&t != 0);

   _obj_arr [0] = t;
   _obj_arr [1] = t;
}



template <class T> 
void  LockFreeBuffer <T>::write (const T &t)
{
   assert (&t != 0);

   const AioOr <FlagType>  ftor_start (Flags_WRITE);
   const int      f = AtomicIntOp_exec_old (_flags, ftor_start);

   int            pos_w = (f >> Flags_POS_W_INDEX) & 1;
   const int      pos_r = (f >> Flags_POS_R_INDEX) & 1;
   if (pos_w == pos_r)
   {
      if ((f & Flags_CHANGE) != 0)
      {
         pos_w = 1 - pos_w;
         const AioSetBits <FlagType>   ftor_set (
            pos_w << Flags_POS_W_INDEX,
            Flags_POS_W | Flags_CHANGE
         );
         AtomicIntOp_exec (_flags, ftor_set);
      }
   }
   _obj_arr [pos_w] = t;

   const AioAnd <FlagType> ftor_end (~Flags_WRITE);
   AtomicIntOp_exec (_flags, ftor_end);
}



template <class T> 
const T &   LockFreeBuffer <T>::obtain_access_from_write_thread () const
{
   const int      f = _flags;

   const int      pos_w = (f >> Flags_POS_W_INDEX) & 1;

   return (_obj_arr [pos_w]);
}



template <class T> 
bool  LockFreeBuffer <T>::is_modified () const
{
   const int      f = _flags;

   const int      pos_w = (f >> Flags_POS_W_INDEX) & 1;
   const int      pos_r = (f >> Flags_POS_R_INDEX) & 1;

   return (pos_w != pos_r);
}



template <class T> 
bool  LockFreeBuffer <T>::read_if_new (T &t) const
{
   assert (&t != 0);

   int            pos_r;

   return (read_if_new_internal (&t, pos_r));
}



template <class T> 
bool  LockFreeBuffer <T>::read_always (T &t) const
{
   assert (&t != 0);

   int            pos_r;
   const bool     really_changed_flag = read_if_new_internal (&t, pos_r);
   if (! really_changed_flag)
   {
      t = _obj_arr [pos_r];
   }

   return (really_changed_flag);
}



template <class T> 
bool  LockFreeBuffer <T>::reset_read ()
{
   int            pos_r;

   return (read_if_new_internal (0, pos_r));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// if t_ptr == 0, no read occurs.
template <class T> 
bool  LockFreeBuffer <T>::read_if_new_internal (T *t_ptr, int &pos_r) const
{
   assert (&pos_r != 0);

   const AioOr <FlagType>  ftor_chg (Flags_CHANGE);
   const int      f = AtomicIntOp_exec_old (_flags, ftor_chg);

   pos_r = (f >> Flags_POS_R_INDEX) & 1;
   const int      pos_w = (f >> Flags_POS_W_INDEX) & 1;
   const bool     really_changed_flag =
      ((pos_r != pos_w) && ((f & Flags_WRITE) == 0));

   if (really_changed_flag)
   {
      pos_r = 1 - pos_r;

      if (t_ptr != 0)
      {
         *t_ptr = _obj_arr [pos_r];
      }

      const AioSetBits <FlagType>   ftor_set (
         pos_r << Flags_POS_R_INDEX,
         Flags_POS_R
      );
      AtomicIntOp_exec (_flags, ftor_set);
   }

   return (really_changed_flag);
}



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_LockFreeBuffer_CODEHEADER_INCLUDED

#undef ohm_conc_LockFreeBuffer_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
