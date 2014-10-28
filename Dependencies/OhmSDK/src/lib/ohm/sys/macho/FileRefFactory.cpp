/*****************************************************************************

        FileRefFactory.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68612

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

#include "ohm/sys/FileRefFactory.h"
#include "ohm/sys/macho/FileRefImpl.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace sys
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_path
==============================================================================
*/

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


   
/*
==============================================================================
Name : get_system_root
==============================================================================
*/

FileRefSPtr FileRefFactory::get_system_root ()
{
   return from_folder_type (kSystemDomain, kVolumeRootFolderType);
}



/*
==============================================================================
Name : get_working_dir
==============================================================================
*/

FileRefSPtr FileRefFactory::get_working_dir ()
{
   FileRefSPtr ret_val;
   char path_0 [PATH_MAX + 1];
   
   char * ptr = getcwd (path_0, PATH_MAX);
   assert (ptr != 0);
   assert (ptr == & path_0 [0]);
   
   ::FSRef fs_ref;
   
   Boolean dir_flag = false;
   OSStatus err = FSPathMakeRef ((UInt8 *) ptr, &fs_ref, &dir_flag);
   if (err == ::noErr)
   {
      assert (dir_flag);
      ret_val = FileRefFactory::from_fsref (fs_ref);
   }
   else
   {
      assert (false);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_user_documents
==============================================================================
*/

FileRefSPtr FileRefFactory::get_user_documents ()
{
   return from_folder_type (kUserDomain, kDocumentsFolderType);
}



/*
==============================================================================
Name : get_user_desktop
==============================================================================
*/

FileRefSPtr FileRefFactory::get_user_desktop ()
{
   return from_folder_type (kUserDomain, kDesktopFolderType);
}



/*
==============================================================================
Name: FileRefFactory::get_user_log
==============================================================================
*/
FileRefSPtr FileRefFactory::get_user_log ()
{
   return from_folder_type (kUserDomain, kLogsFolderType);
}



/*
==============================================================================
Name : get_user_preferences
==============================================================================
*/

FileRefSPtr FileRefFactory::get_user_preferences ()
{
   return from_folder_type (kUserDomain, kPreferencesFolderType);
}



/*
==============================================================================
Name: FileRefFactory::get_user_cache
==============================================================================
*/
FileRefSPtr FileRefFactory::get_user_cache ()
{
   return from_folder_type (kUserDomain, kCachedDataFolderType);
}



/*
==============================================================================
Name: FileRefFactory::get_fonts
==============================================================================
*/
FileRefSPtr FileRefFactory::get_fonts ()
{
   return from_folder_type (kLocalDomain, kFontsFolderType);
}



/*
==============================================================================
Name : get_common_preferences
==============================================================================
*/

FileRefSPtr FileRefFactory::get_common_preferences ()
{
   return from_folder_type (kLocalDomain, kPreferencesFolderType);
}



/*
==============================================================================
Name : get_temporary
==============================================================================
*/

FileRefSPtr FileRefFactory::get_temporary ()
{
   return from_folder_type (kUserDomain, kTemporaryFolderType);
}



/*
==============================================================================
Name : get_applications
==============================================================================
*/

FileRefSPtr FileRefFactory::get_applications ()
{
   return from_folder_type (kLocalDomain, kApplicationsFolderType);
}



/*
==============================================================================
Name : get_shared_app_data
==============================================================================
*/

FileRefSPtr FileRefFactory::get_shared_app_data ()
{
   return from_folder_type (kLocalDomain, kApplicationSupportFolderType);
}



/*
==============================================================================
Name : get_shared_user_datas
==============================================================================
*/

FileRefSPtr FileRefFactory::get_shared_user_datas ()
{
   return from_folder_type (kLocalDomain, kSharedUserDataFolderType);
}



/*
==============================================================================
Name : get_cur_application
==============================================================================
*/

FileRefSPtr FileRefFactory::get_cur_application ()
{
   FileRefSPtr ret_val;
   ::ProcessSerialNumber psn;
   ::OSErr err = ::GetCurrentProcess (&psn);
   
   if (err == 0)
   {
      ::FSRef fs_ref;
      ::ProcessInfoRec process_info;
      ::Str255 process_name_p;
      process_info.processInfoLength = sizeof(::ProcessInfoRec);
      process_info.processName = process_name_p;
#if defined (__LP64__)
      process_info.processAppRef = &fs_ref;
#else
      ::FSSpec process_fsspec;
      process_info.processAppSpec = &process_fsspec;
#endif      
      err = ::GetProcessInformation (&psn, &process_info);
#if !defined (__LP64__)
      if (err == 0)
      {
         
         err = ::FSpMakeFSRef (
            &process_fsspec,
            &fs_ref
         );
      }  
#endif      
      if (err == 0)
      {
         ret_val = FileRefFactory::from_fsref (fs_ref);
      }
   }
   
   assert (err == 0);
   
   return ret_val;
}



/*
==============================================================================
Name : from_fopen
==============================================================================
*/

FileRefSPtr FileRefFactory::from_utf8_path (const std::string & utf8_path)
{
   ::FSRef fs_ref;
   
   std::string fopen_path_mod = utf8_path;
   
   ::OSStatus err = ::FSPathMakeRef (
      reinterpret_cast <const ::UInt8 *> (fopen_path_mod.c_str ()),
      &fs_ref,
      0
   );
   
   FileRefSPtr ret_val_sptr;

   if (err != 0)
   {
      ohm_util_TRACE_ERROR2 (
         "FSPathMakeRef failed with error %1% for path %2%",
         err,
         utf8_path.c_str ()
      );
   }
   else
   {
      ret_val_sptr = FileRefFactory::from_fsref (fs_ref);
   }
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : from_file_url
==============================================================================
*/

FileRefSPtr FileRefFactory::from_file_url (const std::string & file_url)
{
   ::CFURLRef url_ref = 0;
   
   url_ref = CFURLCreateWithBytes (
      0,
      reinterpret_cast <const ::UInt8 *> (file_url.c_str ()),
      file_url.size (),
      kCFStringEncodingUTF8,
      0
   );
   
   FileRefSPtr ret_val_sptr;
   
   if (url_ref == 0)
   {
      ohm_util_TRACE_ERROR1 ("CFURLCreateWithBytes failed path %1%", file_url.c_str ());
   }
   else
   {
      ret_val_sptr = from_url_ref (url_ref);
      
      CFRelease (url_ref);
   }
   
   return ret_val_sptr;
}



/*
==============================================================================
Name : load
==============================================================================
*/

FileRefSPtr FileRefFactory::load (const std::vector <archi::UByte> & opaque_data)
{
   using namespace std;
   
#if 0
   // FSRefs are not persitent !
   assert (opaque_data.size () == sizeof (::FSRef));
   
   ::FSRef fs_ref;
   
   memcpy (&fs_ref, &opaque_data [0], opaque_data.size ());
   
   FileRefSPtr ret_val = FileRefFactory::from_fsref (fs_ref);
   
#elif 0
   // standard paths
   std::string fopen_path;
   fopen_path.resize (opaque_data.size ());
   
   memcpy (&fopen_path [0], &opaque_data [0], opaque_data.size ());

   FileRefSPtr ret_val  = FileRefFactory::from_fsref (from_fopen (fopen_path));

#elif 1
   // adviced way to do it from apple
   
   Handle alias_hnd = NewHandle (opaque_data.size ());
   
   ::memcpy (
      *alias_hnd,
      &opaque_data [0],
      opaque_data.size ()
   );
   
   Boolean was_changed = false;
   ::FSRef fs_ref;
   OSErr err = FSResolveAlias (0, (AliasHandle) alias_hnd, &fs_ref, &was_changed);
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR1 ("FSResolveAlias failed with error %1%", err);
   }
   
   
   DisposeHandle (alias_hnd);
   
   FileRefSPtr ret_val = FileRefFactory::from_fsref (fs_ref);
   
#endif
   
   return ret_val;
}



/*
==============================================================================
Name : from_fsref
==============================================================================
*/

FileRefSPtr FileRefFactory::from_fsref (FSRef fs_ref)
{
   FileRefImpl impl (fs_ref);
   
   FileRefSPtr ret_val = FileRefSPtr (new FileRef(&impl));
   
   return ret_val;
}



/*
==============================================================================
Name : from_url_ref
==============================================================================
*/

FileRefSPtr FileRefFactory::from_url_ref (CFURLRef url_ref)
{
   FSRef fs_ref;
   Boolean ok_flag = CFURLGetFSRef (url_ref, &fs_ref);
   assert (ok_flag);
   
   if (ok_flag)
   {
      FileRefImpl impl (fs_ref);
      
      FileRefSPtr ret_val = FileRefSPtr (new FileRef(&impl));
      
      return ret_val;
   }
   else
   {
      return FileRefSPtr ();
   }
}



FileRefSPtr FileRefFactory::from_folder_type (::FSVolumeRefNum v_ref_num, ::OSType folder_type)
{
   FileRefSPtr ret_val;
   ::FSRef fs_ref;
   
   ::OSStatus err = ::FSFindFolder (
      v_ref_num,
      folder_type,
      true,
      &fs_ref
   );
   
   if (err == ::noErr)
   {
      ret_val = FileRefFactory::from_fsref (fs_ref);
   }
   else
   {
      ohm_util_TRACE_ERROR3 (
         "FSFindFolder (%1% - %2%) failed with error %3%", 
         v_ref_num,
         folder_type,
         err
      );
      assert (false);
   }
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
