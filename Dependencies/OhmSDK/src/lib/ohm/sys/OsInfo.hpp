/*****************************************************************************

        OsInfo.hpp
        Copyright (c) 2004 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 10256

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



#if defined (ohm_sys_OsInfo_CURRENT_CODEHEADER)
   #error Recursive inclusion of OsInfo code header.
#endif
#define  ohm_sys_OsInfo_CURRENT_CODEHEADER

#if ! defined (ohm_sys_OsInfo_CODEHEADER_INCLUDED)
#define  ohm_sys_OsInfo_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  OsInfo::is_platform_win9x () const
{
   return (_platform_win9x_flag);
}



bool  OsInfo::is_platform_winnt () const
{
   return (_platform_winnt_flag);
}



bool  OsInfo::is_platform_macos_classic () const
{
   return (_platform_macos_classic_flag);
}



bool  OsInfo::is_platform_macos_x () const
{
   return (_platform_macos_x_flag);
}



OsInfo::OSType OsInfo::get_os_type () const
{
   return (_os_type);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



#endif   // ohm_sys_OsInfo_CODEHEADER_INCLUDED

#undef ohm_sys_OsInfo_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
