/*****************************************************************************

        File.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68168

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

#include "ohm/lang/fnc.h"
#include "ohm/sys/win32/FileImpl.h"
#include "ohm/sys/win32/FileUtilWin32.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/File.h"
#include "ohm/sys/FileRef.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/FilesystemSpec.h"
#include "ohm/sys/UtcTime.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/fnc.h"
#include "ohm/txt/String16.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



File::File ()
:  _impl_ptr (new FileImpl)
{
   // Nothing
}



File::~File ()
{
   if (is_open ())
   {
      close ();
   }

   delete _impl_ptr;
   _impl_ptr = 0;
}



// Throws: nothing
// A file can be open only once with write access.
int   File::open (const char *pathname_0, long flags)
{
   assert (_impl_ptr != 0);
   assert (pathname_0 != 0);
   assert (pathname_0 [0] != '\0');
   assert (FileUtilWin32::is_path_absolute (pathname_0));

   int            ret_val = Err_OK;

   bool           create_flag = ((flags & Flags_CREATE) != 0);
   bool           erase_flag = ((flags & Flags_ERASE) != 0);
   bool           wr_flag = ((flags & Flags_WRITE) != 0);
   bool           rd_flag = ((flags & Flags_READ) != 0);

   ::DWORD        access_mode = 0;
   ::DWORD        share_mode = 0;
   
   if (wr_flag)
   {
      share_mode |= FILE_SHARE_READ;
      access_mode |= GENERIC_WRITE;
      access_mode |= GENERIC_READ;
      flags |= Flags_READ; // write implies read.
   } 
   else if (rd_flag)
   {
      share_mode |= FILE_SHARE_READ;
      share_mode |= FILE_SHARE_WRITE;
      access_mode |= GENERIC_READ;
   }
   else
   {
      assert (false);
      ret_val = Err_CANNOT_OPEN_FILE;
      ohm_util_TRACE_ERROR1 (
         "open(): Cannot open with no read and no write permission. "
         "Path: \"%1%\"",
         pathname_0
      );
   }

   ::DWORD        create_mode = 0;
   
   if (wr_flag)
   {
      switch (flags & (Flags_CREATE | Flags_ERASE))
      {
      case  Flags_CREATE:
         create_mode = OPEN_ALWAYS;
         break;
      case  Flags_ERASE:
      case  Flags_CREATE | Flags_ERASE:
         create_mode = CREATE_ALWAYS;
         break;
      default:
         create_mode = OPEN_EXISTING;
         break;
      }
   }
   else if (rd_flag)
   {
      create_mode = OPEN_EXISTING;
      
      if (erase_flag || create_flag)
      {
         ret_val = Err_CANNOT_OPEN_FILE;
         ohm_util_TRACE_ERROR1 (
            "open(): We forbid opening a file with create "
            "or erase flags without write flag. Path: \"%1%\"",
            pathname_0
         );
      }
   }

   if (is_open())
   {
      assert (false);
      ret_val = Err_CANNOT_OPEN_FILE;
      ohm_util_TRACE_ERROR ("open(): already opened.");
   }
   
   if (ret_val == Err_OK)
   {  
      _impl_ptr->_flags = flags;

      try
      {
         txt::String16  pathname_w;
         ret_val = txt::conv_utf8_to_utf16 (pathname_w, pathname_0);

         if (ret_val == Err_OK)
         {
            _impl_ptr->_full_path = pathname_0;
            _impl_ptr->_file_h = ::CreateFileW (
               pathname_w.c_str (), // pointer to name of the file
               access_mode,         // access (read-write) mode
               share_mode,          // share mode
               0,                   // pointer to security attributes
               create_mode,         // how to create
               FILE_ATTRIBUTE_NORMAL,  // file attributes
               0                    // handle to file with attributes to copy
            );

            if (_impl_ptr->_file_h == INVALID_HANDLE_VALUE)
            {
               const unsigned long  err_code = ::GetLastError ();
               ohm_util_TRACE_ERROR3 (
                  "open(): Cannot open or create file \"%1%\" "
                  "with flags %2r8w0b%b. "
                  "Returned Windows error code 0x%3rm8w0X%.",
                  pathname_0,
                  flags,
                  err_code
               );
               ret_val = Err_CANNOT_OPEN_FILE;
            }
         }
      }

      catch (...)
      {
         ohm_util_TRACE_ERROR2 (
            "open(): Exception while trying to open or create "
            "file \"%1%\" with flags %2rm8w0b%b.",
            pathname_0,
            flags
         );
         ret_val = Err_EXCEPTION;
      }
   }

   if (ret_val == 0)
   {
      ohm_util_TRACE_INFO2 (
         "open(): file \"%1%\" opened successfully "
         "with flags %2rm8w0b%b.",
         pathname_0,
         flags
      );
   }

   return (ret_val);
}



// Throws: nothing
int   File::close ()
{
   assert (is_open ());

   int            ret_val = Err_OK;

   try
   {
      if (::CloseHandle (_impl_ptr->_file_h) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR3 (
            "close(): Cannot close file \"%1%\" "
            "with flags %2rm8w0b%b. "
            "Returned Windows error code 0x%3rm8w0X%.",
            _impl_ptr->_full_path.c_str (),
            _impl_ptr->_flags,
            err_code
         );
         ret_val = Err_CANNOT_CLOSE_FILE;
      }
   }

   catch (...)
   {
      ohm_util_TRACE_ERROR ("close(): Exception.");
      ret_val = Err_EXCEPTION;
   }

   if (ret_val == 0)
   {
      ohm_util_TRACE_INFO1 (
         "close(): file \"%1%\" successfully closed.",
         _impl_ptr->_full_path.c_str ()
      );
      _impl_ptr->_file_h = INVALID_HANDLE_VALUE;
   }

   return (ret_val);
}



bool  File::is_open () const
{
   assert (_impl_ptr != 0);

   return (_impl_ptr->_file_h != INVALID_HANDLE_VALUE);
}



// Must have read access.
int   File::read (archi::UByte data_ptr [], long len)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_READ) != 0);
   assert (data_ptr != 0);
   assert (len > 0);

   int            ret_val = Err_OK;

   ::DWORD        read_len;
   const bool     ok_flag = ::ReadFile (
      _impl_ptr->_file_h,  // handle of file to read
      data_ptr,            // pointer to buffer that receives data
      len,                 // number of bytes to read
      &read_len,           // pointer to number of bytes read
      0                    // pointer to structure for data
   );

   if (! ok_flag)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR3 (
         "read(): Cannot read file \"%1%\" "
         "with flags %2rm8w0b%b. "
         "Returned Windows error code 0x%3rm8w0X%.",
         _impl_ptr->_full_path.c_str (),
         _impl_ptr->_flags,
         err_code
      );

      ret_val = Err_CANNOT_READ_FILE;
   }

   else if (long (read_len) < len)
   {
      ohm_util_TRACE_ERROR2 (
         "read(): Attempt to read file \"%1%\" past its end. "
         "Requested %2% bytes.",
         _impl_ptr->_full_path.c_str (),
         len
      );
      ret_val = Err_CANNOT_READ_PAST_FILE_END;
   }

   return (ret_val);
}



// Must have write access.
int   File::write (const archi::UByte data_ptr [], long len)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_WRITE) != 0);
   assert (data_ptr != 0);
   assert (len > 0);

   int            ret_val = Err_OK;

   ::DWORD        written_len;
   const bool     ok_flag = ::WriteFile (
      _impl_ptr->_file_h,  // handle to file to write to
      data_ptr,            // pointer to data to write to file
      len,                 // number of bytes to write
      &written_len,        // pointer to number of bytes written
      0                    // pointer to structure for overlapped I/O
   );

   if (! ok_flag)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR3 (
         "write(): Cannot write file \"%1%\" "
         "with flags %2rm8w0b%b. "
         "Returned Windows error code 0x%3rm8w0X%.",
         _impl_ptr->_full_path.c_str (),
         _impl_ptr->_flags,
         err_code
      );

      ret_val = Err_CANNOT_WRITE_FILE;
   }

   else if (long (written_len) < len)
   {
      ohm_util_TRACE_ERROR3 (
         "write(): function mysteriously reported less bytes "
         "written as requested for file \"%1%\": %2% vs %3%.",
         _impl_ptr->_full_path.c_str (),
         written_len,
         len
      );

      ret_val = Err_CANNOT_WRITE_FILE;
   }

   return (ret_val);
}



// Must have read or write access.
int   File::get_pos (archi::Int64 &pos) const
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & (Flags_READ | Flags_WRITE)) != 0);
   assert (&pos != 0);

   int            ret_val = Err_OK;

   ::LONG         high = 0;

   const ::DWORD  low = ::SetFilePointer (
      _impl_ptr->_file_h,  // handle of file
      0,                   // number of bytes to move file pointer
      &high,               // pointer to high-order DWORD of distance to move
      FILE_CURRENT         // how to move
   );

   const unsigned long  err_code = ::GetLastError ();
   if (   err_code != NO_ERROR
       && static_cast <unsigned long> (low) == 0xFFFFFFFFUL)
   {
      ohm_util_TRACE_ERROR2 (
         "get_pos(): Cannot get position for file \"%1%\" "
         "Returned Windows error code 0x%2rm8w0X%.",
         _impl_ptr->_full_path.c_str (),
         err_code
      );

      pos = 0;
      ret_val = Err_CANNOT_GET_FILE_POSITION;
   }

   if (ret_val == 0)
   {
      pos =   static_cast <unsigned long> (low)
            + (static_cast <archi::Int64> (high) << 32);
   }

   return (ret_val);
}



// Must have read or write access.
// Position should not be located past the file end, but it could be equal
// to its length (no check however, because the function is used internally
// to extend the length).
int   File::seek (archi::Int64 pos)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & (Flags_READ | Flags_WRITE)) != 0);
   assert (pos >= 0);

   int            ret_val = Err_OK;

   ::LONG         high = static_cast <::LONG> (pos >> 32);
   ::DWORD        low = static_cast <::DWORD> (pos & 0xFFFFFFFFUL);

   low = ::SetFilePointer (
      _impl_ptr->_file_h,  // handle of file
      low,                 // number of bytes to move file pointer
      &high,               // pointer to high-order DWORD of distance to move
      FILE_BEGIN           // how to move
   );

   const unsigned long  err_code = ::GetLastError ();
   if (   static_cast <unsigned long> (low) == 0xFFFFFFFFUL
       && err_code != NO_ERROR)
   {
      ohm_util_TRACE_ERROR3 (
         "seek(): Cannot seek to position %1% on file \"%2%\" "
         "Returned Windows error code 0x%3rm8w0X%.",
         pos,
         _impl_ptr->_full_path.c_str (),
         err_code
      );

      ret_val = Err_CANNOT_SET_FILE_POSITION;
   }

   if (ret_val == 0 && (low != (pos & 0xFFFFFFFFUL) || high != (pos >> 32)))
   {
      const archi::Int64   real_pos = ohm_archi_MAKE_INT64 (high, low);
      ohm_util_TRACE_ERROR3 (
         "seek(): New file position (%1%) differs of the requested one (%2%) for file \"%3%\" ",
         real_pos,
         pos,
         _impl_ptr->_full_path.c_str ()
      );
      ret_val = Err_CANNOT_SET_FILE_POSITION;
   }

   return (ret_val);
}



int   File::get_length (archi::Int64 &len) const
{
   assert (is_open ());
   assert (&len != 0);

   int            ret_val = Err_OK;

   ::DWORD        high;
   const ::DWORD  low = ::GetFileSize (
      _impl_ptr->_file_h,  // handle of file to get size of
      &high                // pointer to high-order word for file size
   );

   const unsigned long  err_code = ::GetLastError ();
   if (   static_cast <unsigned long> (low) == 0xFFFFFFFFUL
       && err_code != NO_ERROR)
   {
      ohm_util_TRACE_ERROR3 (
         "get_length(): Cannot get length of file \"%1%\" "
         "with flags %2rm8w0b%b. "
         "Returned Windows error code 0x%3rm8w0X%.",
         _impl_ptr->_full_path.c_str (),
         _impl_ptr->_flags,
         err_code
      );

      ret_val = Err_CANNOT_GET_FILE_LENGTH;
      len = 0;
   }

   if (ret_val == Err_OK)
   {
      len =   static_cast <unsigned long> (low)
            + (static_cast <archi::Int64> (high) << 32);
   }

   return (ret_val);
}



// Must have write access.
// Can truncate or extend file.
// Current position is not affected, but may move back if it is in a
// truncated zone.
int   File::set_length (archi::Int64 len)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_WRITE) != 0);
   assert (len >= 0);

   int            ret_val = Err_OK;
   bool           seek_flag = false;

   archi::Int64   saved_pos;
   ret_val = get_pos (saved_pos);

   if (ret_val == Err_OK)
   {
      ret_val = seek (len);
   }

   if (ret_val == Err_OK)
   {
      seek_flag = true;

      if (::SetEndOfFile (_impl_ptr->_file_h) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR3 (
            "truncate(): Cannot truncate file \"%1%\" "
            "with flags %2rm8w0b%b. "
            "Returned Windows error code 0x%3rm8w0X%.",
            _impl_ptr->_full_path.c_str (),
            _impl_ptr->_flags,
            err_code
         );

         ret_val = Err_CANNOT_SET_FILE_LENGTH;
      }
   }

   if (ret_val == Err_OK)
   {
      lang::max_in_place (saved_pos, len);
   }

   if (seek_flag)
   {
      const int      seek_ret_val = seek (saved_pos);
      if (ret_val == Err_OK)
      {
         ret_val = seek_ret_val;
      }
   }

   return (ret_val);
}



int   File::get_fileref (FileRefSPtr &fref_sptr) const
{
   assert (is_open ());
   assert (&fref_sptr != 0);

   int            ret_val = Err_OK;

   fref_sptr = FileRefFactory::from_utf8_path (_impl_ptr->_full_path);
   if (fref_sptr.get () == 0)
   {
      ohm_util_TRACE_ERROR2 (
         "get_fileref(): Error for file \"%1%\" "
         "with flags %2rm8w0b%b.",
         _impl_ptr->_full_path.c_str (),
         _impl_ptr->_flags
      );

      ret_val = Err_CANNOT_CONVERT_PATH_TO_FILEREF;
   }

   return (ret_val);
}



long  File::get_flags () const
{
   assert (is_open ());

   return (_impl_ptr->_flags);
}



// Must have write rights
int   File::set_creation_date (const sys::UtcTime &date_cre)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_WRITE) != 0);
   assert (&date_cre != 0);
   assert (date_cre.is_valid ());

   int            ret_val = Err_OK;

   ::FILETIME     date_file;
   try
   {
      FileUtilWin32::convert_utc_time_to_file (date_file, date_cre);
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("set_creation_date(): Cannot convert date.");
      ret_val = Err_DATE_CONVERSION_ERROR;
   }

   if (ret_val == Err_OK)
   {
      const int      result = ::SetFileTime (
         _impl_ptr->_file_h,
         &date_file,
         0,
         0
      );
      if (result == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "set_creation_date(): Cannot set date. "
            "Returned Windows error code 0x%3rm8w0X%.",
            err_code
         );

         ret_val = Err_CANNOT_CHANGE_FILE_CREATION_DATE;
      }
   }

   return (ret_val);
}



// Must have write rights
int   File::set_modification_date (const sys::UtcTime &date_mod)
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_WRITE) != 0);
   assert (&date_mod != 0);
   assert (date_mod.is_valid ());

   int            ret_val = Err_OK;

   ::FILETIME     date_file;
   try
   {
      FileUtilWin32::convert_utc_time_to_file (date_file, date_mod);
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("set_modification_date(): Cannot convert date.");
      ret_val = Err_DATE_CONVERSION_ERROR;
   }

   if (ret_val == Err_OK)
   {
      const int      result = ::SetFileTime (
         _impl_ptr->_file_h,
         0,
         0,
         &date_file
      );
      if (result == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "set_modification_date(): Cannot set date. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );

         ret_val = Err_CANNOT_CHANGE_FILE_MODIFICATION_DATE;
      }
   }

   return (ret_val);
}



// Must have read rights
int   File::get_creation_date (sys::UtcTime &date_cre) const
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_READ) != 0);
   assert (&date_cre != 0);

   int            ret_val = Err_OK;

   ::FILETIME     date_file;
   const int      result = ::GetFileTime (
      _impl_ptr->_file_h,
      &date_file,
      0,
      0
   );
   if (result == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "get_creation_date(): Cannot set date. "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code
      );

      ret_val = Err_CANNOT_READ_FILE_CREATION_DATE;
   }

   try
   {
      FileUtilWin32::convert_utc_time_from_file (date_cre, date_file);
      assert (date_cre.is_valid ());
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("get_creation_date(): Cannot convert date.");
      ret_val = Err_DATE_CONVERSION_ERROR;
   }

   return (ret_val);
}



// Must have read rights
int   File::get_modification_date (sys::UtcTime &date_mod) const
{
   assert (is_open ());
   assert ((_impl_ptr->_flags & Flags_READ) != 0);
   assert (&date_mod != 0);

   int            ret_val = Err_OK;

   ::FILETIME     date_file;
   const int      result = ::GetFileTime (
      _impl_ptr->_file_h,
      0,
      0,
      &date_file
   );
   if (result == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "get_modification_date(): Cannot set date. "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code
      );

      ret_val = Err_CANNOT_READ_FILE_MODIFICATION_DATE;
   }

   try
   {
      FileUtilWin32::convert_utc_time_from_file (date_mod, date_file);
      assert (date_mod.is_valid ());
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("get_modification_date(): Cannot convert date.");
      ret_val = Err_DATE_CONVERSION_ERROR;
   }

   return (ret_val);
}



bool  File::exist (const char *pathname_0, int req_type_mask)
{
   assert (pathname_0 != 0);
   assert (req_type_mask != 0);

   txt::String16  path_16;
   const int      ret_val = txt::conv_utf8_to_utf16 (path_16, pathname_0);
   if (ret_val != Err_OK)
   {
      assert (false);
      ohm_util_TRACE_ERROR ("exist(): invalid UTF-8 path.");
      throw std::runtime_error (
         "ohm::sys::File::exist(): Invalid UTF-8 path."
      );
   }

   FilesystemSpec &  fss = FilesystemSpec::use_instance ();
   while (   path_16.length () > 0
          && fss.is_dir_delimiter (path_16 [path_16.length () - 1]))
   {
      path_16 = path_16.substr (0, path_16.length () - 1);
   }

   if (path_16.empty ())
   {
      assert (false);
      ohm_util_TRACE_ERROR ("exist(): Empty path (after trailing slash removal).");
      throw std::runtime_error (
         "ohm::sys::File::exist(): Empty path (after trailing slash removal)."
      );
   }

   bool           exist_flag = false;

#if 0    // Previous method

   ::_WIN32_FIND_DATAW  data;
   ::DWORD        file_attrib = INVALID_FILE_ATTRIBUTES;
   ::HANDLE       handle = ::FindFirstFileW (path_16.c_str (), &data);
   if (handle == INVALID_HANDLE_VALUE)
   {
#if 0
      const long     err_code = ::GetLastError ();
      if (err_code != ERROR_FILE_NOT_FOUND)
      {
         ohm_util_TRACE_ERROR1 (
            "exist(): Error during handle opening. Error 0x%1rm8w0X%",
            err_code
         );
      }
#endif

      // FindFirstFile cannot retrieve root information. Therefore
      // in case of failure, we retry with GetFileAttributes.
      file_attrib = ::GetFileAttributesW (path_16.c_str ());
   }
   else
   {
      if (::FindClose (handle) == 0)
      {
         const long     err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "exist(): Cannot close handle. Error 0x%1rm8w0X%",
            err_code
         );
         throw std::runtime_error (
            "ohm::sys::File::exist(): Cannot close handle."
         );
      }

      file_attrib = data.dwFileAttributes;
   }

   if (file_attrib != INVALID_FILE_ATTRIBUTES)
   {
      const ExistTypeFlag  found_type =
           ((file_attrib & FILE_ATTRIBUTE_DIRECTORY) != 0)
         ? ExistTypeFlag_DIR
         : ExistTypeFlag_FILE;

      exist_flag = ((found_type & req_type_mask) != 0);
   }

#else    // Current method

   ::HANDLE       file_hnd = ::CreateFileW (
      path_16.c_str (),
      0,                            // dwDesiredAccess
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,  // dwShareMode
      0,                            // lpSecurityAttributes
      OPEN_EXISTING,                // dwCreationDisposition
      FILE_FLAG_BACKUP_SEMANTICS,   // dwFlagsAndAttributes. Required to open directories.
      0                             // hTemplateFile
   );

   if (file_hnd != INVALID_HANDLE_VALUE)
   {
      ::_BY_HANDLE_FILE_INFORMATION data;
      const ::BOOL      ret_info = ::GetFileInformationByHandle (
         file_hnd,
         &data
      );
      if (ret_info != 0)
      {
         const ExistTypeFlag  found_type =
              ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            ? ExistTypeFlag_DIR
            : ExistTypeFlag_FILE;

         exist_flag = ((found_type & req_type_mask) != 0);
      }

      ::CloseHandle (file_hnd);
   }
   else
   {
      const long     err_code = ::GetLastError ();
   }

#endif   // Current method

   return (exist_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
