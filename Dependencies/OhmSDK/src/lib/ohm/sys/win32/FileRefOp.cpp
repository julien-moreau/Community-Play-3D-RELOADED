/*****************************************************************************

        FileRefOp.cpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69449

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
#include "ohm/sys/FileRefOp.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/FilesystemSpec.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/trace.h"

#define  NOMINMAX
#define  NOGDI
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winerror.h>

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Returns file_ref if we are at the top
FileRefSPtr FileRefOp::up (const FileRef &file_ref)
{
   assert (&file_ref != 0);

   std::string    path (file_ref.to_utf8_path ());
   const long     delim_pos =
      FileUtilWin32::find_last_dir_delimiter (path.c_str ());

   if (delim_pos >= 0)
   {
      path = path.substr (0, delim_pos);
   }

   return (FileRefFactory::from_utf8_path (path));
}



FileRefSPtr FileRefOp::up (FileRefSPtr file_ref_sptr)
{
   assert (file_ref_sptr.get () != 0);
   
   FileRefSPtr ret_val;
   
   if (file_ref_sptr.get () != 0)
   {
      ret_val = up (*file_ref_sptr);
   }
   
   return ret_val;
}



FileRefSPtr FileRefOp::up (FileRefSPtr file_ref_sptr, int count)
{
   assert (file_ref_sptr.get () != 0);
   assert (count > 0);
   
   FileRefSPtr ret_val = file_ref_sptr;
   
   do
   {
      ret_val = up (ret_val);
      count--;
   }
   while (count > 0 && ret_val.get () != 0);
   
   return ret_val;
}



/*
==============================================================================
Name: FileRefOp::ls
Description: 
Throws: std::runtime_error
==============================================================================
*/
void  FileRefOp::ls (FileRefList &file_list, const FileRef &file_ref)
{
   assert (&file_list != 0);
   assert (&file_ref != 0);

   file_list.erase (file_list.begin (), file_list.end ());
   ls_rec (file_list, file_ref, false);
}



/*
==============================================================================
Name: FileRefOp::lsr
Description: 
Throws: std::runtime_error
==============================================================================
*/
void  FileRefOp::lsr (FileRefList &file_list, const FileRef &file_ref)
{
   assert (&file_list != 0);
   assert (&file_ref != 0);

   file_list.erase (file_list.begin (), file_list.end ());
   ls_rec (file_list, file_ref, true);
}



FileRefSPtr FileRefOp::mkdir (const FileRef &file_ref, const std::string &name)
{
   assert (&file_ref != 0);
   assert (&name != 0);
   assert (name.length () > 0);

   if (! file_ref.is_directory ())
   {
      ohm_util_TRACE_ERROR1 (
         "mkdir(): Path \"%1%\" is not a directory.",
         file_ref._impl_ptr->_path.c_str ()
      );
      throw std::runtime_error (
         "ohm::sys::mkdir(): Not a directory."
      );
   }

   const std::string fullpath =
        file_ref._impl_ptr->_path
      + FilesystemSpec::use_instance ().get_default_dir_delimiter ()
      + name;

   txt::String16  fullpath_w;
   const int      ret_val =
      txt::conv_utf8_to_utf16 (fullpath_w, fullpath.c_str ());
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR ("mkdir(): UTF-8 conversion error.");
      throw std::runtime_error (
         "ohm::sys::mkdir(): UTF-8 conversion error."
      );
   }

   const bool     ok_flag = (::CreateDirectoryW (fullpath_w.c_str (), 0) != 0);

   if (! ok_flag)
   {
      const unsigned long  err_code = ::GetLastError ();
      
      if (err_code == ERROR_ALREADY_EXISTS)
      {
         // Ignore : file already exists
         ohm_util_TRACE_INFO1 (
            "mkdir(): Path %1% already exists.",
            fullpath.c_str ()
         );
      }
      else
      {
         ohm_util_TRACE_ERROR2 (
            "mkdir(): Failed to create directory \"%1%\". "
            "Returned Windows error code 0x%2lm8X%.",
            fullpath.c_str (),
            err_code
         );

         throw std::runtime_error (
            "ohm::sys::FileRefOp::mkdir(): Failed to create directory."
         );
      }
   }

   // Should cause problem because there can't be a directory and a file of
   // the same name.

   return (FileRefFactory::from_utf8_path (fullpath));
}



FileRefSPtr FileRefOp::enter (const FileRef &file_ref, const std::string &name)
{
   assert (&file_ref != 0);
   assert (file_ref.is_directory ());
   assert (&name != 0);
   assert (name.length () > 0);

   std::string    fullname (file_ref.to_utf8_path ());
   fullname += FilesystemSpec::use_instance ().get_default_dir_delimiter ();
   fullname += name;

   return (FileRefFactory::from_utf8_path (fullname));
}



/*
==============================================================================
Name: FileRefOp::rm
Description: 
Throws: std::runtime_error
==============================================================================
*/
void  FileRefOp::rm (const FileRef &file_ref)
{
   assert (&file_ref != 0);

   const txt::Char16 *  pathname_w_0 = file_ref._impl_ptr->_path_16.c_str ();
   const char *   dir_or_file_0 = 0;
   bool           ok_flag = false;

   if (file_ref.is_directory ())
   {
      dir_or_file_0 = "directory";
      ok_flag = ::RemoveDirectoryW (pathname_w_0);
   }

   else
   {
      dir_or_file_0 = "file";
      ok_flag = ::DeleteFileW (pathname_w_0);
   }

   if (! ok_flag)
   {
      const unsigned long  err_code = ::GetLastError ();
      const char *   pathname_0 = file_ref._impl_ptr->_path.c_str ();
      ohm_util_TRACE_ERROR3 (
         "rm(): Failed to delete %3% \"%1%\". "
         "Returned Windows error code 0x%2lm8X%.",
         pathname_0,
         err_code,
         dir_or_file_0
      );

      throw std::runtime_error (
         "ohm::sys::FileRefOp::rm(): Failed to delete file or directory."
      );
   }
}



/*
==============================================================================
Name: FileRefOp::rmr
Description: 
Throws: std::runtime_error
==============================================================================
*/
void  FileRefOp::rmr (const FileRef &file_ref)
{
   assert (&file_ref != 0);


   if (file_ref.is_directory ())
   {
      FileRefList    file_list;
      ls_rec (file_list, file_ref, true);

      // Reverse order because tree leafs are stored last and we cannot
      // delete non-empty directories.
      for (FileRefList::reverse_iterator it = file_list.rbegin ()
      ;  it != file_list.rend ()
      ;  ++it)
      {
         rm (*it);
      }
   }

   rm (file_ref);
}



/*
==============================================================================
Name: FileRefOp::mv
Description:
   Limited to same volume only for directories.
   Files can be anywhere on the local filesystem.
==============================================================================
*/

FileRefSPtr FileRefOp::mv (const FileRef &file_ref, const FileRef &new_parent_ref, const std::string &name)
{
   assert (&file_ref != 0);
   assert (&new_parent_ref != 0);
   assert (&name != 0);
   assert (name.length () > 0);

   FileRefSPtr new_path_sptr;
   std::string    new_path_utf8;

   txt::String16  name_w;
   const txt::Char16 *  file_w_0 = 0;
   txt::String16  new_path ;
   int            ret_val = txt::conv_utf8_to_utf16 (name_w, name.c_str ());
   if (ret_val == sys::Err_OK)
   {
      file_w_0 = file_ref._impl_ptr->_path_16.c_str ();
      const txt::String16 &   parent_w = new_parent_ref._impl_ptr->_path_16;
      new_path = parent_w + txt::Char16 (L'\\') + name_w;
      ret_val = txt::conv_utf16_to_utf8 (new_path_utf8, new_path.c_str ());
   }

   if (ret_val == sys::Err_OK)
   {
      ::BOOL result = ::MoveFileExW (
         file_w_0,
         new_path.c_str (),
         MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED
      );

      if (result == 0)
      {
         const unsigned long  err_code = ::GetLastError ();

         ohm_util_TRACE_ERROR3 (
            "mv(): Failed to move %1% to %2%. "
            "Returned Windows error code 0x%3lm8X%.",
            file_ref._impl_ptr->_path.c_str (),
            new_path_utf8.c_str (),
            err_code
         );
         ret_val = sys::Err_CANNOT_OPEN_FILE;
      }
   }

   if (ret_val == sys::Err_OK)
   {
      new_path_sptr = FileRefFactory::from_utf8_path (new_path_utf8);
   }

   return (new_path_sptr);
}



/*
==============================================================================
Name: cp
Description: basic copy, synchronous.
==============================================================================
*/
FileRefSPtr FileRefOp::cp (const FileRef &file_ref, const FileRef &new_parent_ref, const std::string &name)
{
   assert (&file_ref != 0);
   assert (&new_parent_ref != 0);
   assert (&name != 0);
   assert (name.length () > 0);
   
   FileRefSPtr new_path_sptr;
   std::string    new_path_utf8;

   txt::String16  name_w;
   const txt::Char16 *  file_w_0 = 0;
   txt::String16  new_path ;
   int            ret_val = txt::conv_utf8_to_utf16 (name_w, name.c_str ());
   if (ret_val == txt::Err_OK)
   {
      file_w_0 = file_ref._impl_ptr->_path_16.c_str ();
      const txt::String16 &   parent_w = new_parent_ref._impl_ptr->_path_16;
      new_path = parent_w + txt::Char16 (L'\\') + name_w;
      ret_val = txt::conv_utf16_to_utf8 (new_path_utf8, new_path.c_str ());
   }

   if (ret_val == txt::Err_OK)
   {
      // Overwrite
      ::BOOL result = ::CopyFileW (file_w_0, new_path.c_str (), FALSE);

      if (result == 0)
      {
         const unsigned long  err_code = ::GetLastError ();

         ohm_util_TRACE_ERROR3 (
            "cp(): Failed to copy %1% to %2%. "
            "Returned Windows error code 0x%3lm8X%.",
            file_ref._impl_ptr->_path.c_str (),
            new_path_utf8.c_str (),
            err_code
         );
         ret_val = txt::Err_CANNOT_OPEN_FILE;
      }
   }

   if (ret_val == txt::Err_OK)
   {
      new_path_sptr = FileRefFactory::from_utf8_path (new_path_utf8);
   }

   return (new_path_sptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Appends new data to file_list.
void  FileRefOp::ls_rec (FileRefList &file_list, const FileRef &file_ref, bool rec_flag)
{
   assert (&file_list != 0);
   assert (&file_ref != 0);

   if (! file_ref.is_directory ())
   {
      ohm_util_TRACE_ERROR1 (
         "FileRefOp::ls_rec(): Path \"%1%\" is not a directory.",
         file_ref._impl_ptr->_path.c_str ()
      );
      throw std::runtime_error (
         "ohm::sys::FileRefOp::ls_rec(): Not a directory."
      );
   }

   ::_WIN32_FIND_DATAW  data;

   const std::string path = file_ref.to_utf8_path ();
   const std::string path_filter (path + "/*");

   txt::String16  path_filter_w;
   int            ret_val =
      txt::conv_utf8_to_utf16 (path_filter_w, path_filter.c_str ());
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "FileRefOp::ls_rec(): UTF-16 conversion error."
      );
      throw std::runtime_error (
         "ohm::sys::FileRefOp::ls_rec(): UTF-16 conversion error."
      );
   }

   ::HANDLE       handle = ::FindFirstFileW (path_filter_w.c_str (), &data);
   if (handle != INVALID_HANDLE_VALUE)
   {
      bool           ok_flag = true;
      do
      {
         // Skips self (".") and parent ("..") entries
         if (   (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0
             || data.cFileName [0] != txt::Char16 ('.')
             || (   data.cFileName [1] != 0
                 && (   data.cFileName [1] != txt::Char16 ('.')
                     || data.cFileName [2] != 0)))
         {
            std::string    filename;
            ret_val = txt::conv_utf16_to_utf8 (filename, data.cFileName);
            if (ret_val != Err_OK)
            {
               ohm_util_TRACE_ERROR (
                  "FileRefOp::ls_rec(): UTF-16 conversion error."
               );
            }

            else
            {
               // Gets file info and build FileRef object
               const std::string file_pathname (path + "\\" + filename);

               // Could be optimised to reuse collected -data-
               FileRefSPtr    file_ref_sub_sptr =
                  FileRefFactory::from_utf8_path (file_pathname);

               if (file_ref_sub_sptr.get () != 0)
               {
                  file_list.push_back (*file_ref_sub_sptr);

                  // Recursive listing, if required
                  if (rec_flag && file_ref_sub_sptr->is_directory ())
                  {
                     ls_rec (file_list, *file_ref_sub_sptr, rec_flag);
                  }
               }
            }
         }

         // Next file
         if (ret_val == Err_OK && ::FindNextFileW (handle, &data) == 0)
         {
            ok_flag = false;
            const ::DWORD  last_error = ::GetLastError ();
            if (last_error != ERROR_NO_MORE_FILES)
            {
               ohm_util_TRACE_ERROR1 (
                  "FileRefOp::ls_rec(): Cannot continue reading "
                  "directory \"%1%\".",
                  file_ref._impl_ptr->_path.c_str ()
               );
               ret_val = Err_CANNOT_READ_FILE;
            }
         }
      }
      while (ret_val == Err_OK && ok_flag);

      // Closes handle after the last file
      if (::FindClose (handle) == 0)
      {
         ohm_util_TRACE_ERROR1 (
            "FileRefOp::ls_rec(): Cannot close file handle \"%1%\".",
            file_ref._impl_ptr->_path.c_str ()
         );
         throw std::runtime_error (
            "ohm::sys::FileRefOp::ls_rec(): Cannot close handle."
         );
      }

      if (ret_val != Err_OK)
      {
         throw std::runtime_error (
            "ohm::sys::FileRefOp::ls_rec(): Failed."
         );
      }
   }
}



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
