/*****************************************************************************

        FileUtilWin32.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 17833

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



#if ! defined (ohm_sys_FileUtilWin32_HEADER_INCLUDED)
#define  ohm_sys_FileUtilWin32_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>

#include <string>



namespace ohm
{
namespace sys
{



class UtcTime;

class FileUtilWin32
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    is_path_absolute (const char *path_0);
   static bool    is_dir_delimiter (char c);
   static long    find_last_dir_delimiter (const char *path_0);
   static long    find_last_dir_delimiter (const std::string &path);
   static void    convert_utc_time_from_file (UtcTime &utc_time, const ::FILETIME &date_file);
   static void    convert_utc_time_to_file (::FILETIME &date_file, const UtcTime &utc_time);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FileUtilWin32 ();
   virtual        ~FileUtilWin32 ();
                  FileUtilWin32 (const FileUtilWin32 &other);
   FileUtilWin32& operator = (const FileUtilWin32 &other);
   bool           operator == (const FileUtilWin32 &other);
   bool           operator != (const FileUtilWin32 &other);

}; // class FileUtilWin32



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/FileUtilWin32.hpp"



#endif   // ohm_sys_FileUtilWin32_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
