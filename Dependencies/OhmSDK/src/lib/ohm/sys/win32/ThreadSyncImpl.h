/*****************************************************************************

        ThreadSyncImpl.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64466

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



#if ! defined (ohm_sys_ThreadSyncImpl_HEADER_INCLUDED)
#define  ohm_sys_ThreadSyncImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  NOGDI
#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>



namespace ohm
{
namespace sys
{



class ThreadSyncImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ThreadSyncImpl ();
   virtual        ~ThreadSyncImpl ();

   void           signal (int cnt);
   bool           wait (double timeout = -1.0);    // timeout is measured in seconds, and negative if no timeout required.



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ::HANDLE       _handle;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ThreadSyncImpl (const ThreadSyncImpl &other);
   ThreadSyncImpl &
                  operator = (const ThreadSyncImpl &other);
   bool           operator == (const ThreadSyncImpl &other);
   bool           operator != (const ThreadSyncImpl &other);

}; // class ThreadSyncImpl



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/win32/ThreadSyncImpl.hpp"



#endif   // ohm_sys_ThreadSyncImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
