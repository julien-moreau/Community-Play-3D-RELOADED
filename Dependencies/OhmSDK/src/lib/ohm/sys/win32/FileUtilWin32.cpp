/*****************************************************************************

        FileUtilWin32.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 29282

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

#include "ohm/math/fnc.h"
#include "ohm/sys/win32/FileUtilWin32.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#include <cassert>
#include <cctype>
#include <cstring>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  FileUtilWin32::is_path_absolute (const char *path_0)
{
   using namespace std;

   assert (path_0 != 0);
   assert (! isspace (path_0 [0]));

   return (   path_0 [0] != ':'
           && strchr (path_0, ':') != 0);
}



bool  FileUtilWin32::is_dir_delimiter (char c)
{
   return (c == '\\' || c == '/');
}



// -1 if not found
long  FileUtilWin32::find_last_dir_delimiter (const char *path_0)
{
   assert (path_0 != 0);

   long           delim_pos = -1;
   long           pos = 0;
   while (path_0 [pos] != '\0')
   {
      const char     c = path_0 [pos];
      if (is_dir_delimiter (c))
      {
         delim_pos = pos;
      }
      ++ pos;
   }

   return (delim_pos);
}



// -1 if not found
long  FileUtilWin32::find_last_dir_delimiter (const std::string &path)
{
   assert (&path != 0);

   long           delim_pos = -1;
   const long     path_len = long (path.length ());
   long           pos = path_len - 1;
   while (pos >= 0 && delim_pos < 0)
   {
      const char     c = path [pos];
      if (is_dir_delimiter (c))
      {
         delim_pos = pos;
      }
      -- pos;
   }

   return (delim_pos);
}



void  FileUtilWin32::convert_utc_time_from_file (UtcTime &utc_time, const ::FILETIME &date_file)
{
   assert (&utc_time != 0);
   assert (&date_file != 0);

   archi::Int64   val;
   val = static_cast <archi::Int64> (date_file.dwHighDateTime) << 32;
   val += static_cast <archi::Int64> (date_file.dwLowDateTime);

   utc_time.set (val);
}



void  FileUtilWin32::convert_utc_time_to_file (::FILETIME &date_file, const UtcTime &utc_time)
{
   assert (&date_file != 0);
   assert (&utc_time != 0);

   const archi::Int64   val = utc_time.get ();

   date_file.dwHighDateTime = static_cast <DWORD> (val >> 32);
   date_file.dwLowDateTime  = static_cast <DWORD> (val & 0xFFFFFFFFUL);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
