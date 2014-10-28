/*****************************************************************************

        ReadWriteLock.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40228

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



#if defined (ohm_sys_ReadWriteLock_CURRENT_HEADER)
   #error Recursive inclusion of ohm/sys/ReadWriteLock header.
#endif
#define  ohm_sys_ReadWriteLock_CURRENT_HEADER

#if ! defined (ohm_sys_ReadWriteLock_HEADER_INCLUDED)
#define  ohm_sys_ReadWriteLock_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/Array.h"
#include "ohm/archi/def.h"

namespace ohm
{
namespace sys
{



class ReadWriteLock
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

#if (ohm_archi_WORD_SIZE == 64)
   enum {         MAX_IMPL_DATA_SIZE   = 208 }; // Maximum data size
#else
   enum {         MAX_IMPL_DATA_SIZE   = 128 }; // Maximum data size
#endif
                  ReadWriteLock ();
                  ~ReadWriteLock ();

   void           lock_read () const;
   void           unlock_read () const;

   void           lock_write () const;
   void           unlock_write () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   mutable mem::Array <char, MAX_IMPL_DATA_SIZE>
                  _impl;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ReadWriteLock (const ReadWriteLock &other);
   ReadWriteLock &operator = (const ReadWriteLock &other);
   bool           operator == (const ReadWriteLock &other);
   bool           operator != (const ReadWriteLock &other);

}; // class ReadWriteLock



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_ReadWriteLock_HEADER_INCLUDED

#undef ohm_sys_ReadWriteLock_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
