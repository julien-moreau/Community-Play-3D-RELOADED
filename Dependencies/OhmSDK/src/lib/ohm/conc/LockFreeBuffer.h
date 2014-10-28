/*****************************************************************************

        LockFreeBuffer.h
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



#if ! defined (ohm_conc_LockFreeBuffer_HEADER_INCLUDED)
#define  ohm_conc_LockFreeBuffer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/conc/AtomicInt.h"
#include "ohm/mem/Array.h"



namespace ohm
{
namespace conc
{



template <class T>
class LockFreeBuffer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   inline         LockFreeBuffer ();
   virtual        ~LockFreeBuffer () {}

   inline void    init (const T &t);

   inline void    write (const T &t);
   inline const T &
                  obtain_access_from_write_thread () const;

   inline bool    is_modified () const;
   inline bool    read_if_new (T &t) const;
   inline bool    read_always (T &t) const;
   inline bool    reset_read ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum Flags
   {
      Flags_POS_W_INDEX = 0,
      Flags_POS_R_INDEX,
      Flags_CHANGE_INDEX,
      Flags_WRITE_INDEX,

      Flags_POS_W    = 1 << Flags_POS_W_INDEX,  // Modified by write
      Flags_POS_R    = 1 << Flags_POS_R_INDEX,  // Modified by read
      Flags_CHANGE   = 1 << Flags_CHANGE_INDEX, // Set by read, cleared by write
      Flags_WRITE    = 1 << Flags_WRITE_INDEX   // Modified by write
   };

   typedef  archi::UInt32  FlagType;

   inline bool    read_if_new_internal (T *t_ptr, int &pos_r) const;

   mem::Array <T, 2>
                  _obj_arr;

   mutable AtomicInt <FlagType>
                  _flags;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LockFreeBuffer (const LockFreeBuffer &other);
   LockFreeBuffer &
                  operator = (const LockFreeBuffer &other);
   bool           operator == (const LockFreeBuffer &other);
   bool           operator != (const LockFreeBuffer &other);

}; // class LockFreeBuffer



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/LockFreeBuffer.hpp"



#endif   // ohm_conc_LockFreeBuffer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
