/*****************************************************************************

        ExceptionHandler.h
        Copyright (c) 2012 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 67309

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



#if ! defined (ohm_sys_win32_ExceptionHandler_HEADER_INCLUDED)
#define  ohm_sys_win32_ExceptionHandler_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN

#include "Windows.h"



namespace ohm
{
namespace sys
{
namespace win32
{



class ExceptionHandler
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  ::LONG (WINAPI *ExHndPtr) (::EXCEPTION_POINTERS *exinfo_ptr);

   static void    set (ExHndPtr ex_hnd_ptr);
   static ::LONG  exec (::EXCEPTION_POINTERS *exinfo_ptr);

   static volatile ExHndPtr
                  _exception_hnd_ptr;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ExceptionHandler ();
   virtual        ~ExceptionHandler () {}
                  ExceptionHandler (const ExceptionHandler &other);
   ExceptionHandler &
                  operator = (const ExceptionHandler &other);
   bool           operator == (const ExceptionHandler &other) const;
   bool           operator != (const ExceptionHandler &other) const;

}; // class ExceptionHandler



}  // namespace win32
}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/win32/ExceptionHandler.hpp"



#endif   // ohm_sys_win32_ExceptionHandler_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
