/*****************************************************************************

        FileRefImpl.cpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69629

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

#include "ohm/sys/win32/FileRefImpl.h"
#include "ohm/sys/win32/FileUtilWin32.h"
#include "ohm/sys/Err.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/Date.h"
#include "ohm/util/FileOp.h"
#include "ohm/util/trace.h"
#include "ohm/util/TraceTools.h"

#include <stdexcept>

#include <cassert>
#include <cstdlib>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FileRefImpl::FileRefImpl ()
:  _path ()
,  _path_16 ()
,  _info_sptr ()
{
   // Nothing
}



// May throw
void  FileRefImpl::init_data (const std::string &fopen_path)
{
   assert (&fopen_path != 0);

   init_data_path (fopen_path);
   update_info ();
}



void  FileRefImpl::update_info ()
{
#if 0

   ::_WIN32_FIND_DATAW  data;
   ::HANDLE       handle = ::FindFirstFileW (_path_16.c_str (), &data);
   if (handle == INVALID_HANDLE_VALUE)
   {
      const long     err_code = ::GetLastError ();

      // FindFirstFile won't work on the root directory.
      // Try another way.
      const ::DWORD  file_attrib = ::GetFileAttributesW (_path_16.c_str ());
      if (file_attrib == INVALID_FILE_ATTRIBUTES)
      {
         ohm_util_TRACE_ERROR1 (
            "update_info(): Inexistent path. Error 0x%1rm8w0X%",
            err_code
         );
         throw std::runtime_error (
            "ohm::sys::FileRefImpl::update_info(): Inexistent path."
         );
      }
   }

   else if (::FindClose (handle) == 0)
   {
      const long     err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "update_info(): Cannot close handle. Error 0x%1rm8w0X%",
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::FileRefImpl::update_info(): Cannot close handle."
      );

      update_info (data);
   }

#else

   ::_BY_HANDLE_FILE_INFORMATION data;

   ::HANDLE       file_hnd = ::CreateFileW (
      _path_16.c_str (),
      0,                            // dwDesiredAccess
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,  // dwShareMode
      0,                            // lpSecurityAttributes
      OPEN_EXISTING,                // dwCreationDisposition
      FILE_FLAG_BACKUP_SEMANTICS,   // dwFlagsAndAttributes. Required to open directories.
      0                             // hTemplateFile
   );
   if (file_hnd == INVALID_HANDLE_VALUE)
   {
      const long     err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "update_info(): Inexistent path \"%2%\". "
         "Error 0x%1rm8w0X%",
         err_code,
         _path.c_str ()
      );
      throw std::runtime_error (
         "ohm::sys::FileRefImpl::update_info(): Inexistent path."
      );
   }
   else
   {
      const ::BOOL      ret_info = ::GetFileInformationByHandle (
         file_hnd,
         &data
      );
      if (ret_info == 0)
      {
         ohm_util_TRACE_ERROR1 (
            "update_info(): Inexistent path \"%1%\". "
            "Cannot retrieve path information.",
            _path.c_str ()
         );
         throw std::runtime_error (
            "ohm::sys::FileRefImpl::update_info(): "
            "Cannot retrieve path information."
         );
      }

      ::CloseHandle (file_hnd);
   }

   update_info (data, _path);

#endif
}



FileRefImpl *  FileRefImpl::clone () const
{
   FileRefImpl *  new_impl_ptr = new FileRefImpl;

   try
   {
      new_impl_ptr->_path = _path;
      new_impl_ptr->_path_16 = _path_16;
      new_impl_ptr->_info_sptr = InfoSPtr (new util::FileDescriptor (*_info_sptr));
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR1 (
         "clone(): Exception, path: \"%1%\").",
         _path.c_str ()
      );
      delete new_impl_ptr;
      new_impl_ptr = 0;
      throw;
   }

   return (new_impl_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  FileRefImpl::init_data_path (const std::string &fopen_path)
{
   assert (&fopen_path != 0);

   _path = fopen_path;
   remove_end_delim (_path);
   if (make_absolute (_path) != 0)
   {
      ohm_util_TRACE_ERROR1 (
         "init_data_path(): Invalid path \"%1%\".",
         fopen_path.c_str ()
      );
      throw std::runtime_error (
         "ohm::sys::FileRefImpl::init_data_path(): Invalid path."
      );
   }

   const int      ret_val = txt::conv_utf8_to_utf16 (_path_16, _path.c_str ());
   if (ret_val != Err_OK)
   {
      const int      max_len = 4095;
      char           txt_0 [max_len+1];
      util::TraceTools::dump_mem_as_hex (
         txt_0,
         max_len,
         _path.c_str (),
         long (_path.length ())
      );
      ohm_util_TRACE_ERROR1 (
         "init_data_path(): Invalid UTF-8 sequence: %1%.",
         txt_0
      );
      throw std::runtime_error (
         "ohm::sys::FileRefImpl::init_data_path(): Invalid path format."
      );
   }
}



#if 0
void  FileRefImpl::update_info (const ::_WIN32_FIND_DATAW &data)
#else
void  FileRefImpl::update_info (const ::_BY_HANDLE_FILE_INFORMATION &data, const std::string &path)
#endif
{
   assert (&data != 0);

   // Flags
   const bool     dir_flag =
      ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
   const bool     ro_flag =
      ((data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
   const bool     hidden_flag =
      ((data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
   const bool     system_flag =
      ((data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);

   const long     flags = util::FileDescriptor::make_flags (
      dir_flag,
      ro_flag,
      hidden_flag,
      system_flag
   );

   // Length
   archi::Int64   length = 0;
   if (! dir_flag)
   {
      const archi::Int64   length_h =
         static_cast <archi::Int64> (data.nFileSizeHigh);
      const archi::Int64   length_l =
         static_cast <archi::Int64> (data.nFileSizeLow);
      assert (length_l >= 0);

      length = (length_h * MAXDWORD) + length_l;
   }

   // Dates
   sys::UtcTime   date_creation;
   sys::UtcTime   date_modification;
   FileUtilWin32::convert_utc_time_from_file (
      date_creation,
      data.ftCreationTime
   );
   FileUtilWin32::convert_utc_time_from_file (
      date_modification,
      data.ftLastWriteTime
   );

   // Name
   std::string    filename;
#if 0
   const int      ret_val = txt::conv_utf16_to_utf8 (filename, data.cFileName);
   assert (ret_val == Err_OK);
#else
   std::string    dummy_path;
   util::FileOp::split_path_fname (dummy_path, filename, path);
#endif

   _info_sptr = InfoSPtr (new util::FileDescriptor (
      filename,
      date_creation,
      date_modification,
      length,
      flags
   ));
}



void  FileRefImpl::remove_end_delim (std::string &path)
{
   assert (&path != 0);

   bool           cont_flag = true;
   do
   {
      const std::string::size_type  len = path.length ();
      if (len > 0)
      {
         if (FileUtilWin32::is_dir_delimiter (path [len - 1]))
         {
            path = path.substr (0, len - 1);
         }
         else
         {
            cont_flag = false;
         }
      }
   }
   while (cont_flag);
}



// Output should not contain:
// - Forward slash delimiters (only backward)
// - Duplicated delimiters
// - \..\ and \.\ sequences
int   FileRefImpl::make_absolute (std::string &path)
{
   assert (&path != 0);

   int            ret_val = 0;

   // Operates only on non-drive root paths, so we make sure it won't be
   // extended with the current drive's path.
   if (path.empty () || path [path.length () - 1] != ':')
   {
      txt::String16  path_16;
      ret_val = txt::conv_utf8_to_utf16 (path_16, path.c_str ());

      if (ret_val == Err_OK)
      {
         const long     buf_len = 32768;
         txt::Char16    full_path_0 [buf_len];

         WCHAR *        dummy_fname_0;
         const ::DWORD  ret_gfpn = ::GetFullPathNameW (
            path_16.c_str (),
            buf_len,
            full_path_0,
            &dummy_fname_0
         );

         if (ret_gfpn == 0)
         {
            ohm_util_TRACE_ERROR1 (
               "make_absolute(): invalid path \"%1%\"",
               path.c_str ()
            );
            ret_val = Err_INVALID_PATH;
         }

         else
         {
            ret_val = txt::conv_utf16_to_utf8 (path, full_path_0);
         }
      }
   }

   if (ret_val == Err_OK)
   {
      assert (path.find ('/') == std::string::npos);
      assert (path.find ("\\\\") == std::string::npos);
      assert (path.find ("\\.\\") == std::string::npos);
      assert (path.find ("\\..\\") == std::string::npos);
   }

   return (ret_val);
}



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
