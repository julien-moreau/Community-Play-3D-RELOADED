/*****************************************************************************

        FilesystemSpec.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 54701

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

#include "ohm/archi/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/FilesystemSpec.h"
#include "ohm/sys/OsInfo.h"
#include "ohm/txt/fnc.h"
#include "ohm/txt/utf8/utf8.h"
#include "ohm/util/trace.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <windows.h>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FilesystemSpec::~FilesystemSpec ()
{
// delete _impl_ptr;
   _impl_ptr = 0;
}



bool  FilesystemSpec::is_case_sensitive () const
{
   return (false);
}



long  FilesystemSpec::get_max_filename_length () const
{
   return (MAX_PATH);
}



long  FilesystemSpec::get_max_pathname_length () const
{
   long        max_pathname_len = 65535;  // Actually 32767 wide characters

   const OsInfo & os_info = OsInfo::use_instance ();
   if (os_info.is_platform_win9x ())
   {
      max_pathname_len = MAX_PATH;
   }

   return (max_pathname_len);
}



// Makes also the path "unique" (but could be still case sensitive)
// Don't accept semi-relative path (with only drive specified, eg "c:toto")
// The presence of a terminal delimiter is preserved.
int   FilesystemSpec::simplify_path (std::string &path) const
{
   assert (&path != 0);

   int            ret_val = Err_OK;

   try
   {
      const std::string::size_type  drive_end_pos = path.find (':');

      txt::find_replace (path, "\\", "/");
      txt::find_replace (path, "/./", "/");
      txt::find_replace (path, "//", "/", 1);

      if (drive_end_pos != std::string::npos)
      {
         assert (drive_end_pos + 1 < path.length ());
         assert (path [drive_end_pos + 1] == '/');
      }

      // Remove all the unused "/xxx/.." but not "/../.."
      std::string::size_type  pos = 0;
      while (pos != std::string::npos)
      {
         pos = path.find ("/..", pos);
         if (pos != std::string::npos && pos > 0)
         {
            const std::string::size_type  beg_pos = path.rfind ('/', pos - 1);
            bool           replace_flag = (beg_pos != std::string::npos);
            std::string::size_type  erase_pos = beg_pos + 1;

            // Special case for relative pathnames
            if (   beg_pos == std::string::npos
                && drive_end_pos == std::string::npos)
            {
               erase_pos = 0;
               replace_flag = true;
            }

            // Do not erase "../.." patterns
            if (   replace_flag
                && pos - erase_pos == 2
                && path [erase_pos    ] == '.'
                && path [erase_pos + 1] == '.')
            {
               replace_flag = false;
            }

            if (replace_flag)
            {
               const std::string::size_type  erase_len = pos + 3 - (erase_pos);
               path.erase (erase_pos, erase_len);
               pos = lang::max (erase_pos - 1, std::string::size_type (0));
            }
            else
            {
               pos += 3;
            }
         }
      }
   }

   catch (...)
   {
      ohm_util_TRACE_ERROR ("simplify_path(): Exception.");
      ret_val = Err_EXCEPTION;
      assert (false);
   }

   return (ret_val);
}



bool  FilesystemSpec::is_dir_delimiter (const wchar_t c) const
{
   return (c == L'/' || c == L'\\');
}



char  FilesystemSpec::get_default_dir_delimiter () const
{
   return ('\\');
}



FilesystemSpec &  FilesystemSpec::use_instance ()
{
   static FilesystemSpec   instance;

   return (instance);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FilesystemSpec::FilesystemSpec ()
:  _impl_ptr (0)
{
   // Nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
