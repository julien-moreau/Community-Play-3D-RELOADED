/*****************************************************************************

        Mutex.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 56377

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



/*

Because mutex can be used to both read and write data, it would be more
intuitive to make the lock/unlock function const. Therefore it is possible
to use a const mutex to read data. That means that implementation data should
be mutable.

*/


#if defined (ohm_sys_Mutex_CURRENT_HEADER)
   #error Recursive inclusion of ohm/sys/Mutex header.
#endif
#define  ohm_sys_Mutex_CURRENT_HEADER

#if ! defined (ohm_sys_Mutex_HEADER_INCLUDED)
#define  ohm_sys_Mutex_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/Array.h"

namespace ohm
{
namespace sys
{



class MutexImpl;

class Mutex
{

   friend class MutexImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         MAX_IMPL_DATA_SIZE   = 64  }; // Maximum data size

                  Mutex ();
                  ~Mutex ();

   void           lock () const;
   void           unlock () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   mutable mem::Array <char, MAX_IMPL_DATA_SIZE>
                  _impl;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Mutex (const Mutex &other);
   bool           operator == (const Mutex &other);
   bool           operator != (const Mutex &other);
   Mutex &        operator = (const Mutex &other);

}; // class Mutex



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_Mutex_HEADER_INCLUDED

#undef ohm_sys_Mutex_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
