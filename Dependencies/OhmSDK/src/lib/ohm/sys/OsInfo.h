/*****************************************************************************

        OsInfo.h
        Copyright (c) 2004 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 17849

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



#if ! defined (ohm_sys_OsInfo_HEADER_INCLUDED)
#define  ohm_sys_OsInfo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace sys
{

class OsInfoImpl;



class OsInfo
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum OSType
   {
      OSType_NOT_VALID = -1,
      OSType_WIN_95 = 0,
      OSType_WIN_98,
      OSType_WIN_ME,
      OSType_WIN_NT_3,
      OSType_WIN_NT_4,
      OSType_WIN_2000,
      OSType_WIN_XP,
      OSType_WIN_SERVER_2003,
      OSType_WIN_VISTA
   };

   
   virtual        ~OsInfo ();


   static OsInfo& use_instance ();

   inline bool    is_platform_win9x () const;
   inline bool    is_platform_winnt () const;
   inline bool    is_platform_macos_classic () const;
   inline bool    is_platform_macos_x () const;
   
   inline OSType  get_os_type () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  OsInfo ();

   OsInfoImpl *   _impl_ptr;

   bool           _platform_win9x_flag;   // Windows 95, Windows 98, or Windows Me
   bool           _platform_winnt_flag;   // Windows NT, Windows 2000, Windows XP, Windows Server 2003, Windows Vista family.
   bool           _platform_macos_classic_flag; 
   bool           _platform_macos_x_flag; 

   OSType         _os_type;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  OsInfo (const OsInfo &other);
   OsInfo &       operator = (const OsInfo &other);
   bool           operator == (const OsInfo &other);
   bool           operator != (const OsInfo &other);

}; // class OsInfo



}  // namespace sys
}  // namespace ohm



#include "ohm/sys/OsInfo.hpp"



#endif   // ohm_sys_OsInfo_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
