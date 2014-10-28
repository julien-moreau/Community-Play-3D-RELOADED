/*****************************************************************************

        LocalUseOle.cpp
        Copyright (c) 2011 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 60640

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN

#include "ohm/sys/win32/LocalUseOle.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#include <Objbase.h>
#include <windows.h>

#include <cassert>



namespace ohm
{
namespace sys
{
namespace win32
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



LocalUseOle::LocalUseOle (bool throw_on_failure_flag)
:  _success_flag (false)
{
   ::HRESULT         hres = ::OleInitialize (0);
   if (hres == S_OK || hres == S_FALSE)
   {
      _success_flag = true;
   }
   else
   {
      ohm_util_TRACE_ERROR1 (
         "ctor(): OleInitialize() failed. Returned code %1%.",
         hres
      );
   }

   if (! _success_flag && throw_on_failure_flag)
   {
      throw std::runtime_error (
         "ohm::sys::win32::LocalUseOle::LocalUseOle(): OleInitialize() failed."
      );
   }
}



LocalUseOle::~LocalUseOle ()
{
   if (_success_flag)
   {
      ::OleUninitialize ();
      _success_flag = false;
   }
}



bool  LocalUseOle::is_successfully_initialised () const
{
   return (_success_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace win32
}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
