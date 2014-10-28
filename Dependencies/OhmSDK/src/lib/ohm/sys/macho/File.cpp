/*****************************************************************************

        File.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65977

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

#include "ohm/sys/File.h"
#include "ohm/sys/Err.h"
#include "ohm/sys/macho/FileImpl.h"
#include "ohm/sys/FileRef.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/util/trace.h"
#include "ohm/sys/macho/MacTimeFnc.h"

#include <cassert>
#include <sys/stat.h>
#include <AvailabilityMacros.h>

namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

File::File ()
:  _impl_ptr (0)
{
   _impl_ptr = new FileImpl;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

File::~File ()
{
   if (is_open ())
   {
      close ();
   }
   
   delete _impl_ptr;
   _impl_ptr = 0;
}



/*
==============================================================================
Name: File::open
Description: opens a file.
Returns int: see ohm/sys/Err.h
==============================================================================
*/
int   File::open (const char *pathname_0, long flags)
{
   assert (_impl_ptr != 0);
   assert (pathname_0 != 0);
   assert (pathname_0 [0] != '\0');
   
   
   int ret_val = Err_OK;
      
   bool     create_flag = ((flags & Flags_CREATE) != 0);
   bool     erase_flag = ((flags & Flags_ERASE) != 0);
   bool     wr_flag = ((flags & Flags_WRITE) != 0);
   bool     rd_flag = ((flags & Flags_READ) != 0);

   int      open_flags = O_NONBLOCK | O_NOFOLLOW;
   mode_t   perm_flags = 0;

   if (wr_flag)
   {
      open_flags |= (O_RDWR | O_EXCL | O_EXLOCK);
      flags |= Flags_READ; // write implies read.
   }
   else if (rd_flag)
   {
      open_flags |= O_RDONLY; // we ignore shared lock for reading. We accept to have inconsistencies if a write occurs. /* | O_SHLOCK*/
   }
   else
   {
      assert (false);
      ret_val = Err_CANNOT_OPEN_FILE;
      ohm_util_TRACE_ERROR ("Cannot open with no read and no write permission");
   }
   
   if (wr_flag)
   {
      if (erase_flag && create_flag)
      {
         open_flags |= O_CREAT | O_EXCL | O_TRUNC;
         perm_flags = S_IRUSR | S_IWUSR;
      }
      else if (create_flag)
      {
         open_flags |= O_CREAT;
         perm_flags = S_IRUSR | S_IWUSR;
      }
      else if (erase_flag)
      {
         open_flags |= O_TRUNC;
      }
   }
   else if (erase_flag || create_flag)
   {
      ret_val = Err_CANNOT_OPEN_FILE;
      ohm_util_TRACE_ERROR ("We forbid opening a file with create or erase flags if no write flag.");
   }
      
   if (is_open())
   {
      assert (false);
      ret_val = Err_CANNOT_OPEN_FILE;
      ohm_util_TRACE_ERROR ("self already opened");
   }
   
   if (ret_val == Err_OK)
   {  
      int file_descriptor = 0;
      
      ::umask (mode_t(0x22));
      
      if (perm_flags != 0)
      {
         // we're creating the file. If it already exist, simply trunc
         file_descriptor = ::open (pathname_0, open_flags, perm_flags);
         
         // If it does not exist, evenutally trunc it.
         if (file_descriptor < 0)
         {
            open_flags &= ~ O_CREAT;
            file_descriptor = ::open (pathname_0, open_flags);
         }
      }
      else
      {
         file_descriptor = ::open (pathname_0, open_flags);
      }
      
      if (file_descriptor < 0)
      {
         ret_val = Err_CANNOT_OPEN_FILE;
         ohm_util_TRACE_ERROR4 (
            "::open () failed. "
            "File : %1%. "
            "errno : %2% - %3% "
            "open flags : %4X%",
            pathname_0,
            errno,
            ::strerror (errno),
            open_flags
         );
      }
      
      if (ret_val == Err_OK)
      {
         std::string open_mode_str;
         
         if (wr_flag)
         {
            open_mode_str = "r+";
         }
         else if (rd_flag)
         {
            open_mode_str = "r";
         }
         
         _impl_ptr->_stream = ::fdopen (file_descriptor, open_mode_str.c_str ());
         
         if (_impl_ptr->_stream == 0)
         {
            ret_val = Err_CANNOT_OPEN_FILE;
            ohm_util_TRACE_ERROR4 (
               "::fopen () failed. "
               "File : %1%. "
               "errno : %2% - %3% "
               "open flags : %4%",
               pathname_0,
               errno,
               ::strerror (errno),
               open_mode_str.c_str ()
           );
         }
         else
         {
            _impl_ptr->_pathname = std::string (pathname_0);
            _impl_ptr->_flags = flags;
            ::Boolean   is_dir_flag = false;
            ::FSRef &   fs_ref = _impl_ptr->_fs_ref;
            ::OSStatus  err = FSPathMakeRef ((::UInt8 *) pathname_0, &fs_ref, &is_dir_flag);
            assert (err == 0);
            assert (is_dir_flag == false);
         }
      }
   }
   
   return ret_val;
}



int   File::close ()
{
   int ret_val = Err_OK;
   assert (is_open ());
   
   if (!is_open ())
   {
      ohm_util_TRACE_ERROR ("fails: self is not opened");
      ret_val = Err_CANNOT_CLOSE_FILE;
   }
   
   if (::fclose (_impl_ptr->_stream) != 0)
   {
      ret_val = Err_CANNOT_CLOSE_FILE;
      ohm_util_TRACE_ERROR3 (
        "::fclose () failed. with errno : %2% - %3% "
        "File : %1% ",
        _impl_ptr->_pathname.c_str (),
        errno,
        ::strerror (errno)
      );
   }
   else
   {
      _impl_ptr->_stream = 0;
   }
   
   return ret_val;
}



bool  File::is_open () const
{
   const ::FILE * stream = _impl_ptr->_stream;

   return (stream != 0);
}



int   File::read (archi::UByte data_ptr [], long len)
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   assert (data_ptr != 0);
   assert (len > 0);

   int         ret_val = Err_OK;
   FILE *      file = _impl_ptr->_stream;
   size_t      read_count = ::fread (data_ptr, 1, len, file);

   if (read_count < size_t (len))
   {
      if (::feof (file))
      {
         ret_val = Err_CANNOT_READ_PAST_FILE_END;
         ohm_util_TRACE_ERROR3 (
           "read(): Attempt to read file \"%1%\" past its end. "
           "Requested %2% bytes, read %3%.",
           _impl_ptr->_pathname.c_str (),
           len,
           read_count
        );
      }
      else
      {
         ret_val = Err_CANNOT_READ_FILE;
         ohm_util_TRACE_ERROR1 (
           "::fread failed for file \"%1%\".",
           _impl_ptr->_pathname.c_str ()
        );
      }
   }
   
   return ret_val;
}



int   File::write (const archi::UByte data_ptr [], long len)
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   assert ((_impl_ptr->_flags & Flags_WRITE) != 0);
   assert (data_ptr != 0);
   assert (len > 0);

   int         ret_val = Err_OK;
   FILE *      file = _impl_ptr->_stream;
   size_t      write_count = ::fwrite (data_ptr, 1, len, file);
   
   if (write_count < size_t (len))
   {
      ret_val = Err_CANNOT_WRITE_FILE;
      ohm_util_TRACE_ERROR1 (
         "::fwrite failed for file \"%1%\".",
         _impl_ptr->_pathname.c_str ()
      );
   }
   
   return ret_val;
}



int   File::get_pos (archi::Int64 &pos) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
      
   int      ret_val = Err_OK;
   ::off_t  offset = ::ftello (_impl_ptr->_stream);
   
   if (offset < 0)
   {
      ret_val = Err_CANNOT_GET_FILE_POSITION;
      ohm_util_TRACE_ERROR3 (
         "::ftello failed [errno = %2% - %3%] "
         "File \"%1%\".",
         _impl_ptr->_pathname.c_str (),
         errno,
         ::strerror (errno)
      );
   }
   else
   {
      pos = offset;
   }
      
   return ret_val;
}



int   File::seek (archi::Int64 pos)
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   int      ret_val = Err_OK;
   if (::fseeko (_impl_ptr->_stream, pos, SEEK_SET) == -1)
   {
      ret_val = Err_CANNOT_SET_FILE_POSITION;
      ohm_util_TRACE_ERROR3 (
         "::fseeko failed [errno = %2% - %3%] "
         "File \"%1%\".",
         _impl_ptr->_pathname.c_str (),
         errno,
         ::strerror (errno)
      );
   }
   
   return ret_val;
}



#if (MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_4)

int   File::get_length (archi::Int64 &len) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }

   assert (_impl_ptr->_stream->_file >= 0);
   
   int               ret_val = Err_OK;
   FILE *            file_ptr = _impl_ptr->_stream;
   struct ::stat     buf_stat;
   
   if ((_impl_ptr->_flags & Flags_WRITE) != 0)
   {
      if (::fflush (file_ptr) != 0)
      {
         ohm_util_TRACE_WARNING3 (
            "::fflush failed [errno = %2% - %3%] "
            "File: \"%1%\". Length might not be accurate",
            _impl_ptr->_pathname.c_str (),
            errno,
            ::strerror (errno)
         );
      }
   }
   
   if (::fstat (file_ptr->_file, &buf_stat) != 0)
   {
      ret_val = Err_CANNOT_GET_FILE_LENGTH;
      ohm_util_TRACE_ERROR3 (
         "::fstat failed [errno = %2% - %3%] "
         "File \"%1%\".",
         _impl_ptr->_pathname.c_str (),
         errno,
         ::strerror (errno)
      );
   }
   else
   {
      len = buf_stat.st_size;
   }
   
   return ret_val;
}
   
#else // #if (MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_4)

int   File::get_length (archi::Int64 &len) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }

   assert (_impl_ptr->_stream->_file >= 0);
   
   int               ret_val = Err_OK;
   FILE *            file_ptr = _impl_ptr->_stream;
   struct ::stat64   buf_stat;
   
   if ((_impl_ptr->_flags & Flags_WRITE) != 0)
   {
      if (::fflush (file_ptr) != 0)
      {
         ohm_util_TRACE_WARNING3 (
            "::fflush failed [errno = %2% - %3%] "
            "File: \"%1%\". Length might not be accurate",
            _impl_ptr->_pathname.c_str (),
            errno,
            ::strerror (errno)
         );
      }
   }
   
   if (::fstat64 (file_ptr->_file, &buf_stat) != 0)
   {
      ret_val = Err_CANNOT_GET_FILE_LENGTH;
      ohm_util_TRACE_ERROR3 (
         "::fstat64 failed [errno = %2% - %3%] "
         "File \"%1%\".",
         _impl_ptr->_pathname.c_str (),
         errno,
         ::strerror (errno)
      );
   }
   else
   {
      len = buf_stat.st_size;
   }
   
   return ret_val;
}

#endif // MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_4



int   File::set_length (archi::Int64 len)
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }

   assert (_impl_ptr->_stream->_file >= 0);
   
   int               ret_val = Err_OK;
   
   if (::ftruncate (_impl_ptr->_stream->_file, len) != 0)
   {
      ret_val = Err_CANNOT_SET_FILE_LENGTH;
      ohm_util_TRACE_ERROR3 (
         "::ftruncate failed [errno = %2% - %3%] "
         "File \"%1%\".",
         _impl_ptr->_pathname.c_str (),
         errno,
         ::strerror (errno)
      );
   }
   
   return ret_val;
}



int   File::get_fileref (FileRefSPtr &fref_sptr) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   assert (&fref_sptr != 0);
   
   int            ret_val = Err_OK;
   
   fref_sptr = FileRefFactory::from_fsref (_impl_ptr->_fs_ref);
   if (fref_sptr.get () == 0)
   {
      ohm_util_TRACE_ERROR2 (
         "get_fileref(): Error for file \"%1%\" "
         "with flags %2rm8w0b%b.",
         _impl_ptr->_pathname.c_str (),
         _impl_ptr->_flags
      );
      
      ret_val = Err_CANNOT_CONVERT_PATH_TO_FILEREF;
   }
   
   return (ret_val);
}
   
   
   
long  File::get_flags () const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
      
   long flags = _impl_ptr->_flags;
   
   return flags;
}



int   File::set_creation_date (const sys::UtcTime &date_cre)
{
   assert (is_open ());

   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   int ret_val = Err_OK;
   
   ::UTCDateTime utc_date_time;
   
   MacTimeFnc::UtcTime_to_UTCDateTime (utc_date_time, date_cre);
   
   FSCatalogInfo info;
   info.createDate = utc_date_time;
   
   OSStatus err = ::FSSetCatalogInfo (
      &_impl_ptr->_fs_ref,
      ::kFSCatInfoCreateDate,
      &info
   );
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "set_creation_date(): Cannot set create date of file \"%1%\" "
         " because FSSetCatalogInfo "
         "returned Mac error code %2%.",
         _impl_ptr->_pathname.c_str (),
         err
      );
      
      ret_val = Err_CANNOT_CHANGE_FILE_CREATION_DATE;
   }
   
   return ret_val;
}



int   File::set_modification_date (const sys::UtcTime &date_mod)
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   
   int ret_val = Err_OK;
   
   ::UTCDateTime utc_date_time;
   
   MacTimeFnc::UtcTime_to_UTCDateTime (utc_date_time, date_mod);
   
   FSCatalogInfo info;
   info.contentModDate = utc_date_time;
   
   OSStatus err = ::FSSetCatalogInfo (
      &_impl_ptr->_fs_ref,
      ::kFSCatInfoContentMod,
      &info
   );
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "set_creation_date(): Cannot set modification date of file \"%1%\" "
         " because FSSetCatalogInfo "
         "returned Mac error code %2%.",
         _impl_ptr->_pathname.c_str (),
         err
      );
      
      ret_val = Err_CANNOT_CHANGE_FILE_MODIFICATION_DATE;
   }
   
   return ret_val;
}



int   File::get_creation_date (sys::UtcTime &date_cre) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
   
   int ret_val = Err_OK;
   
   FSCatalogInfo info;
   
   OSStatus err = ::FSGetCatalogInfo (
      &_impl_ptr->_fs_ref,
      ::kFSCatInfoCreateDate,
      &info, 0, 0, 0
   );

   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "set_creation_date(): Cannot set modification date of file \"%1%\" "
         " because FSSetCatalogInfo "
         "returned Mac error code %2%.",
         _impl_ptr->_pathname.c_str (),
         err
      );
      
      ret_val = Err_CANNOT_READ_FILE_CREATION_DATE;
   }
   else
   {
      MacTimeFnc::UTCDateTime_to_UtcTime (date_cre, info.createDate);
   }
   
   return ret_val;
}



int   File::get_modification_date (sys::UtcTime &date_mod) const
{
   assert (is_open ());
   
   if (! is_open ())
   {
      // abort
      ohm_util_TRACE_ERROR ("File is not opened");
      return Err_GENERIC_ERROR;
   }
      
   int ret_val = Err_OK;
   
   FSCatalogInfo info;
   
   OSStatus err = ::FSGetCatalogInfo (
      &_impl_ptr->_fs_ref,
      ::kFSCatInfoContentMod,
      &info, 0, 0, 0
   );

   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "set_creation_date(): Cannot set modification date of file \"%1%\" "
         " because FSSetCatalogInfo "
         "returned Mac error code %2%.",
         _impl_ptr->_pathname.c_str (),
         err
      );
      
      ret_val = Err_CANNOT_READ_FILE_MODIFICATION_DATE;
   }
   else
   {
      MacTimeFnc::UTCDateTime_to_UtcTime (date_mod, info.contentModDate);
   }
   
   return ret_val;
}



bool  File::exist (const char *pathname_0, int req_type_mask)
{
   ::Boolean directory_flag = false;
   
   FSRef fs_ref;
   
   ::OSStatus err = ::FSPathMakeRef (
      reinterpret_cast <const ::UInt8 *> (pathname_0),
      &fs_ref,
      &directory_flag
   );
   
   bool exist_flag = false;
   
   if (err == 0)
   {
      switch (req_type_mask)
      {
      case ExistTypeFlag_FILE:
         exist_flag = !directory_flag;
         break;

      case ExistTypeFlag_DIR:
         exist_flag = directory_flag;
         break;
      
      default:
         assert (false);
         break;
      }
   }
   
   return exist_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
