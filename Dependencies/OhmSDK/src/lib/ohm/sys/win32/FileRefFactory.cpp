/*****************************************************************************

        FileRefFactory.cpp
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

#include "ohm/sys/Err.h"
#include "ohm/sys/File.h"
#include "ohm/sys/FileRef.h"
#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/win32/FileRefImpl.h"
#include "ohm/sys/win32/fnc_win32.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/trace.h"
#include "ohm/util/TraceTools.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#define  NOGDI
#include <shlobj.h>
#include <shlwapi.h>
#include <windows.h>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FileRefSPtr FileRefFactory::get_path (Path path)
{
   assert (path >= 0);
   assert (path < Path_NBR_ELT);

   FileRefSPtr ret_val;
   
   switch (path)
   {
   case Path_SYSTEM_ROOT:
      ret_val = get_system_root ();
      break;

   case Path_WORKING_DIR:
      ret_val = get_working_dir ();
      break;

   case Path_CUR_APPLICATION:
      ret_val = get_cur_application ();
      break;

   case Path_USER_DOCUMENTS:
      ret_val = get_user_documents ();
      break;

   case Path_USER_DESKTOP:
      ret_val = get_user_desktop ();
      break;

   case Path_USER_LOG:
      ret_val = get_user_log ();
      break;

   case Path_USER_PREFERENCES:
      ret_val = get_user_preferences ();
      break;

   case Path_USER_CACHE:
      ret_val = get_user_cache ();
      break;

   case Path_FONTS:
      ret_val = get_fonts ();
      break;

   case Path_COMMON_PREFERENCES:
      ret_val = get_common_preferences ();
      break;

   default:
      assert (false);
      break;
   }

   return ret_val;
}



FileRefSPtr FileRefFactory::get_system_root ()
{
   return (from_csidl (CSIDL_WINDOWS));
}



FileRefSPtr FileRefFactory::get_working_dir ()
{
   std::vector <txt::Char16>  tmp (MAX_PATH+1);
   bool           cont_flag = true;
   do
   {
      const long     len =
         ::GetCurrentDirectoryW (::DWORD (tmp.size ()), &tmp [0]);
      if (len >= long (tmp.size ()))
      {
         tmp.resize (len);
      }
      else
      {
         cont_flag = false;
         if (len != 0)
         {
            tmp [len] = 0;
         }
         else
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "get_working_dir(): "
               "Cannot retrieve current working directory. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            throw std::runtime_error (
               "ohm::sys::FileRefFactory::get_working_dir(): "
               "Cannot retrieve current working directory."
            );
         }
      }
   }
   while (cont_flag);

   std::string    path;
   const int      ret_val = txt::conv_utf16_to_utf8 (path, &tmp [0]);
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "get_working_dir(): Cannot convert path to UTF-8."
      );
      throw std::runtime_error (
         "ohm::sys::FileRefFactory::get_working_dir(): "
         "Cannot convert path to UTF-8."
      );
   }

   return FileRefFactory::from_utf8_path (path);
}



FileRefSPtr FileRefFactory::get_cur_application ()
{
   std::vector <txt::Char16>  tmp (MAX_PATH+1);
   bool           cont_flag = true;
   do
   {
      const long     len =
         ::GetModuleFileNameW (0, &tmp [0], ::DWORD (tmp.size ()));
      const unsigned long  err_code = ::GetLastError ();
      if (   len >= long (tmp.size ())
          && err_code == ERROR_INSUFFICIENT_BUFFER)
      {
         tmp.resize (len);
      }
      else
      {
         cont_flag = false;
         if (len != 0)
         {
            tmp [len] = 0;
         }
         else
         {
            ohm_util_TRACE_ERROR1 (
               "get_cur_application(): "
               "Cannot retrieve current application path. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            throw std::runtime_error (
               "ohm::sys::FileRefFactory::get_cur_application(): "
               "Cannot retrieve current working directory."
            );
         }
      }
   }
   while (cont_flag);

   std::string    path;
   const int      ret_val = txt::conv_utf16_to_utf8 (path, &tmp [0]);
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "get_cur_application(): Cannot convert path to UTF-8."
      );
      throw std::runtime_error (
         "ohm::sys::FileRefFactory::get_cur_application(): "
         "Cannot convert path to UTF-8."
      );
   }

   return FileRefFactory::from_utf8_path (path);
}



FileRefSPtr FileRefFactory::get_user_documents ()
{
   return (from_csidl (CSIDL_PERSONAL));
}



FileRefSPtr FileRefFactory::get_user_desktop ()
{
   return (from_csidl (CSIDL_DESKTOPDIRECTORY));
}



FileRefSPtr FileRefFactory::get_user_log ()
{
   //#* jno: pour l'instant, application data user. Voir si on trouve mieux, mais ya pas de log folder.
   return (from_csidl (CSIDL_LOCAL_APPDATA));
}



FileRefSPtr FileRefFactory::get_user_preferences ()
{
   return (from_csidl (CSIDL_LOCAL_APPDATA));
}



FileRefSPtr FileRefFactory::get_user_cache ()
{
   //#* jno: to do
   std::string path = win32::get_csidl_path (CSIDL_WINDOWS);
   path += "/Temp";

   return (from_utf8_path (path));
}



FileRefSPtr FileRefFactory::get_fonts ()
{
   return (from_csidl (CSIDL_FONTS));
}



FileRefSPtr FileRefFactory::get_common_preferences ()
{
   return (from_csidl (CSIDL_COMMON_APPDATA));
}



/*
==============================================================================
Name : get_temporary
==============================================================================
*/

FileRefSPtr FileRefFactory::get_temporary ()
{
   std::vector <txt::Char16>  tmp (MAX_PATH+1);
   bool           cont_flag = true;
   do
   {
      const long     len =
         ::GetTempPathW (::DWORD (tmp.size ()), &tmp [0]);
      if (len >= long (tmp.size ()))
      {
         tmp.resize (len);
      }
      else
      {
         cont_flag = false;
         if (len != 0)
         {
            tmp [len] = 0;
         }
         else
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "get_temporary(): "
               "Cannot retrieve temporary directory. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            throw std::runtime_error (
               "ohm::sys::FileRefFactory::get_temporary(): "
               "Cannot retrieve current working directory."
            );
         }
      }
   }
   while (cont_flag);

   std::string    path;
   const int      ret_val = txt::conv_utf16_to_utf8 (path, &tmp [0]);
   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "get_temporary(): Cannot convert path to UTF-8."
      );
      throw std::runtime_error (
         "ohm::sys::FileRefFactory::get_temporary(): "
         "Cannot convert path to UTF-8."
      );
   }

   return FileRefFactory::from_utf8_path (path);
}



/*
==============================================================================
Name : get_applications
==============================================================================
*/

FileRefSPtr FileRefFactory::get_applications ()
{
   return (from_csidl (CSIDL_PROGRAM_FILES));
}



/*
==============================================================================
Name : get_shared_app_data
==============================================================================
*/

FileRefSPtr FileRefFactory::get_shared_app_data ()
{
   return (from_csidl (CSIDL_COMMON_APPDATA));
}



/*
==============================================================================
Name : get_shared_user_datas
==============================================================================
*/

FileRefSPtr FileRefFactory::get_shared_user_datas ()
{
   return (from_csidl (CSIDL_COMMON_APPDATA));
}



FileRefSPtr FileRefFactory::from_utf8_path (const std::string & utf8_path)
{
   assert (&utf8_path != 0);

   FileRefSPtr    fref_sptr;

   if (utf8_path.empty ())
   {
      ohm_util_TRACE_ERROR ("from_utf8_path(): empty path.");
   }

   else
   {
      const int   req_type_mask =
           File::ExistTypeFlag_FILE
         | File::ExistTypeFlag_DIR;

      if (File::exist (utf8_path.c_str (), req_type_mask))
      {
         FileRefImpl *  impl_ptr = 0;
         try
         {
            impl_ptr = new FileRefImpl;
            impl_ptr->init_data (utf8_path);

            fref_sptr = FileRefSPtr (new FileRef (impl_ptr));
         }
         catch (...)
         {
            ohm_util_TRACE_ERROR1 (
               "from_utf8_path(): exception with path \"%1%\"",
               utf8_path.c_str ()
            );
         }

         delete impl_ptr;
         impl_ptr = 0;
      }
   }

   return (fref_sptr);
}



// URL must be encoded in UTF-8. %-escaping is locale-dependent.
FileRefSPtr FileRefFactory::from_file_url (const std::string & file_url)
{
   assert (&file_url != 0);

   FileRefSPtr    fref_sptr;

   txt::String16  file_url_utf16;
   int            ret_val =
      txt::conv_utf8_to_utf16 (file_url_utf16, file_url.c_str ());
   if (ret_val != txt::Err_OK)
   {
      assert (false);
      ohm_util_TRACE_ERROR ("from_file_url(): invalid UTF-8 string.");
   }

   else
   {
      ::WCHAR        filepath_utf16_0 [MAX_PATH + 1];
      ::DWORD        path_len = MAX_PATH;

      ::HRESULT hresult = ::PathCreateFromUrlW (
         file_url_utf16.c_str (),
         filepath_utf16_0,
         &path_len,
         0
      );

      if (hresult == S_OK)
      {
         assert (path_len >= 0);
         assert (path_len <= MAX_PATH);
         assert (filepath_utf16_0 [path_len] == '\0');

         std::string    filepath;
         ret_val = txt::conv_utf16_to_utf8 (filepath, filepath_utf16_0);
         if (ret_val != txt::Err_OK)
         {
            ohm_util_TRACE_ERROR ("from_file_url(): invalid UTF-8 path.");
         }
         else
         {
            fref_sptr = from_utf8_path (filepath.c_str ());
         }
      }
   }

   return (fref_sptr);
}



FileRefSPtr FileRefFactory::load (const std::vector <archi::UByte> & opaque_data)
{
   assert (&opaque_data != 0);

   FileRefSPtr    fref_sptr;
   const size_t   len = opaque_data.size ();

   std::string    utf8_path (len, ' ');
   for (size_t pos = 0; pos < len; ++pos)
   {
       utf8_path [pos] = opaque_data [pos];
   }

   fref_sptr = FileRefFactory::from_utf8_path (utf8_path);

   if (fref_sptr.get () == 0)
   {
      char           txt_0 [1023+1];
      util::TraceTools::dump_mem_as_filtered_string (
         txt_0,
         sizeof (txt_0),
         &opaque_data [0],
         long (len)
      );
      ohm_util_TRACE_ERROR1 (
         "load(): data is not a valid path - \"%1%\"",
         txt_0
      );
   }

   return (fref_sptr);
}



FileRefSPtr  FileRefFactory::from_csidl (int csidl)
{
   std::string path = win32::get_csidl_path (csidl);

   return (from_utf8_path (path));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
