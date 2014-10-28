/*****************************************************************************

        FileRefFactory.h
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62485

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



#if ! defined (ohm_sys_FileRefFactory_HEADER_INCLUDED)
#define  ohm_sys_FileRefFactory_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/sys/FileRef.h"
#include "ohm/sys/FileRefSPtr.h"

#include <vector>



namespace ohm
{
namespace sys
{

class FileRef;

class FileRefFactory
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Path
   {
      Path_INVALID = -1,

      Path_SYSTEM_ROOT = 0,   // root of disk where system was started
      Path_WORKING_DIR,       // Current working directory
      Path_CUR_APPLICATION,   // path to current running application
      Path_USER_DOCUMENTS,    // user document in multiple user env, else root doc if any, else root
      Path_USER_DESKTOP,      // user desktop in multiple user env, else desktop if any, else root
      Path_USER_LOG,          // user log directory.
      Path_USER_PREFERENCES,  // user preferences in multiple user env, else preferences if any, else root
      Path_USER_CACHE,        // user cache in multiple user env, else preferences if any, else nothing
      Path_FONTS,             
      Path_COMMON_PREFERENCES,// common preferences if any, else nothing

      Path_NBR_ELT
   };

   static FileRefSPtr
                     get_path (Path path);
      
   static FileRefSPtr
                     get_system_root ();
   static FileRefSPtr
                     get_working_dir ();
   static FileRefSPtr
                     get_cur_application ();
   static FileRefSPtr
                     get_user_documents ();
   static FileRefSPtr
                     get_user_desktop ();
   static FileRefSPtr
                     get_user_log ();
   static FileRefSPtr
                     get_user_preferences ();
   static FileRefSPtr
                     get_user_cache ();
   static FileRefSPtr
                     get_fonts ();
   static FileRefSPtr
                     get_common_preferences ();
   static FileRefSPtr
                     get_temporary ();
   static FileRefSPtr
                     get_applications ();
   static FileRefSPtr
                     get_shared_app_data ();
   static FileRefSPtr
                     get_shared_user_datas ();
   
   static FileRefSPtr
                     from_utf8_path (const std::string & utf8_path);
   static FileRefSPtr
                     from_file_url (const std::string & file_url);
   static FileRefSPtr
                     load (const std::vector <archi::UByte> & opaque_data);

   
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
#if (ohm_archi_ARCH == ohm_archi_ARCH_MACHO)
   static FileRefSPtr
                     from_fsref (::FSRef fs_ref);
   static FileRefSPtr
                     from_url_ref (::CFURLRef url_ref);
   static FileRefSPtr
                     from_folder_type (::FSVolumeRefNum v_ref_num, ::OSType folder_type);
#endif   // ohm_archi_ARCH
#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   static FileRefSPtr
                     from_csidl (int csidl);
#endif   // ohm_archi_SYS



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FileRefFactory ();
                  FileRefFactory (const FileRefFactory & other);
   FileRefFactory &
                  operator = (const FileRefFactory & other);
   bool           operator == (const FileRefFactory &other);
   bool           operator != (const FileRefFactory &other);

}; // class FileRefFactory



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_FileRefFactory_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
