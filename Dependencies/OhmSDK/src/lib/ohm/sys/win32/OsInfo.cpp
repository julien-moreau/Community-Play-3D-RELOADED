/*****************************************************************************

        OsInfo.cpp
        Copyright (c) 2004 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 17743

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/OsInfo.h"

#include <cassert>

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



OsInfo::~OsInfo ()
{
   // Nothing
}



OsInfo & OsInfo::use_instance ()
{
   static OsInfo  instance;

   return (instance); 
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



OsInfo::OsInfo ()
:  _impl_ptr (0) // No use here
,  _platform_win9x_flag (false)
,  _platform_winnt_flag (false)
,  _platform_macos_classic_flag (false)
,  _platform_macos_x_flag (false)
,  _os_type (OSType_NOT_VALID)
{
   ::OSVERSIONINFO   ver_info;
   ver_info.dwOSVersionInfoSize = sizeof (ver_info);
   ::GetVersionEx (&ver_info);

   switch (ver_info.dwPlatformId)
   {
   case  VER_PLATFORM_WIN32_WINDOWS:
      _platform_win9x_flag = true;
      break;

   case  VER_PLATFORM_WIN32_NT:
      _platform_winnt_flag = true;
      break;

   default :
      // Windows 3.x not supported. nor future OSes
      assert (false);
      break;
   }

   if (_platform_win9x_flag)
   {
      assert (ver_info.dwMajorVersion == 4);
      switch (ver_info.dwMinorVersion)
      {
      case 0 :
         _os_type = OSType_WIN_95;
         break;
      
      case 10:
         _os_type = OSType_WIN_98;
         break;

      case 90:
         _os_type = OSType_WIN_ME;
         break;

      default: 
         assert (false);
         break;
      }
   }

   if (_platform_winnt_flag)
   {
      switch (ver_info.dwMajorVersion)
      {
      case 3:
         _os_type = OSType_WIN_NT_3;
         assert (ver_info.dwMinorVersion == 51);
         break;

      case 4:
         _os_type = OSType_WIN_NT_4;
         assert (ver_info.dwMinorVersion == 0);
         break;

      case 5:
         {
            switch (ver_info.dwMinorVersion)
            {
            case 0 :
               _os_type = OSType_WIN_2000;
               break;

            case 1 :
               _os_type = OSType_WIN_XP;
               break;

            case 2 :
               _os_type = OSType_WIN_SERVER_2003;
               break;

            default:
               // Unknown operating system
               assert (false);
               _os_type = OSType_WIN_SERVER_2003;  // Set to the most recent known
               break;
            }
         }
         break;

      case 6:
         _os_type = OSType_WIN_VISTA;
         break;

      default :
         // Unknown operating system
         assert (false);
         _os_type = OSType_WIN_VISTA;  // Set to the most recent known
         break;
      }
   }
}



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
