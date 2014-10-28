/*****************************************************************************

        FileRefImpl.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68320

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

#include "ohm/sys/macho/FileRefImpl.h"
#include "ohm/sys/macho/MacTimeFnc.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <cassert>



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

FileRefImpl::FileRefImpl (FSRef fs_ref)
:  _fs_ref (fs_ref)
,  _file_desc_aptr ()
,  _update_flag (true)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

FileRefImpl* FileRefImpl::clone () const
{
   return new FileRefImpl(_fs_ref);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FileRefImpl::~FileRefImpl ()
{
}



/*
==============================================================================
Name : is_directory
==============================================================================
*/

bool  FileRefImpl::is_directory () const
{
   update ();
   
   return _file_desc_aptr->is_directory ();
}



/*
==============================================================================
Name : is_hidden
==============================================================================
*/

bool  FileRefImpl::is_hidden () const
{
   update ();
   
   return _file_desc_aptr->is_hidden ();
}



/*
==============================================================================
Name : is_read_only
==============================================================================
*/

bool  FileRefImpl::is_read_only () const
{
   update ();
   
   return _file_desc_aptr->is_read_only ();
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

std::string FileRefImpl::get_name () const
{
   update ();
   
   return _file_desc_aptr->get_name ();
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  FileRefImpl::save (std::vector <archi::UByte> & opaque_data) const
{
   using namespace std;

#if 0
   // FSRefs are not persitent !
   
   assert (is_valid ());
   check_validity ();

   opaque_data.resize (sizeof (::FSRef));

   memcpy (
      &opaque_data [0],
      &_fs_ref,
      opaque_data.size ()
   );

#elif 0
   // standard paths
   std::string fopen_path = to_fopen ();
   
   opaque_data.resize (fopen_path.size ());

   memcpy (
      &opaque_data [0],
      fopen_path.c_str (),
      opaque_data.size ()
   );

#elif 1
   // adviced way to do it from apple
   AliasHandle alias_hnd;
   OSErr err = FSNewAlias (0, &_fs_ref, &alias_hnd);
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR1 ("FSNewAliasMinimal failed with error %1%", err);
   }
   
   if (alias_hnd == 0)
   {
      ohm_util_TRACE_ERROR ("FSNewAliasMinimal failed : alias_hnd is 0");
   }
   
   assert (err == 0);
   assert (alias_hnd != 0);
   
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1040 // 10.4
   Size size = GetAliasSize (alias_hnd);
#else
   Size size = ((**alias_hnd).aliasSize);
#endif
   
   AliasPtr alias_ptr = *alias_hnd;
   
   opaque_data.resize (size);
   
   memcpy (
      &opaque_data [0],
      alias_ptr,
      opaque_data.size ()
   );
   
   DisposeHandle ((Handle) alias_hnd);
   
#endif
}



/*
==============================================================================
Name : to_fopen
==============================================================================
*/

std::string FileRefImpl::to_fopen () const
{
   char fopen_path_0 [4096 + 1];
   
   ::OSStatus err = ::FSRefMakePath (
      &_fs_ref,
      reinterpret_cast < ::UInt8 *> (fopen_path_0),
      4096
   );
   assert (err == ::noErr);

   if (err != ::noErr)
   {
      throw std::runtime_error ("ohm::sys::FileRefImpl::conv_to_fopen : FSRefMakePath failed");
   }
   
   return std::string (fopen_path_0);
}



/*
==============================================================================
Name : get_fs_ref
==============================================================================
*/

const ::FSRef &   FileRefImpl::get_fs_ref () const
{
   return _fs_ref;
}



/*
==============================================================================
Name : get_info
==============================================================================
*/

const util::FileDescriptor &  FileRefImpl::get_info () const
{
   update ();
   
   return *_file_desc_aptr;
}



/*
==============================================================================
Name : update_info
==============================================================================
*/

void  FileRefImpl::update_info ()
{
   // force update
   _update_flag = true;
   
   update ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  FileRefImpl::update () const
{
   if (!_update_flag)
   {
      // abort
      return;
   }
   
   ::FSCatalogInfo info;
   ::HFSUniStr255 uni_name;
   uni_name.length = 0;
   
   ::FSCatalogInfoBitmap info_flags = 0;

#if 0 
   info_flags |= ::kFSCatInfoGettableInfo;

#elif 1  
   info_flags |= ::kFSCatInfoNodeFlags;
   info_flags |= ::kFSCatInfoFinderInfo;
   info_flags |= ::kFSCatInfoCreateDate;
   info_flags |= ::kFSCatInfoContentMod;
   info_flags |= ::kFSCatInfoDataSizes;

#endif
   
   ::OSErr err = ::FSGetCatalogInfo (
      &_fs_ref,
      info_flags,
      &info,
      &uni_name,
      0,
      0
   );
   
   if (err != 0)
   {
      ohm_util_TRACE_ERROR1 ("FSGetCatalogInfo failed with error %1%", err);
   }
   
   assert (err == ::noErr);
   
   ::FInfo * finder_info_ptr = (FInfo *) info.finderInfo;
   assert (finder_info_ptr != 0);
   
   ::UInt16 finder_flags = finder_info_ptr->fdFlags;
   
   bool dir_flag = (info.nodeFlags & ::kFSNodeIsDirectoryMask) != 0;
   // suppose that bundles are not directories
   dir_flag &= (finder_flags & kHasBundle) == 0;
   bool read_only_flag = (info.nodeFlags & ::kFSNodeLockedMask) != 0;
   bool hidden_flag = (finder_flags & kIsInvisible) != 0;
   bool system_flag = false;
   
   sys::UtcTime creation_date;
   MacTimeFnc::UTCDateTime_to_UtcTime (creation_date, info.createDate);
   
   sys::UtcTime modification_date;
   MacTimeFnc::UTCDateTime_to_UtcTime (modification_date, info.contentModDate);
   
   archi::Int64 length = info.dataLogicalSize;
   
   CFStringRef uni_str_ref = CFStringCreateWithCharacters (
      0, uni_name.unicode, uni_name.length
   );
   assert (uni_str_ref != 0);
   
   char utf8_name_0 [2048 + 1];
   Boolean ok_flag = CFStringGetCString (uni_str_ref, utf8_name_0, 2048, kCFStringEncodingUTF8);
   assert (ok_flag);
   
   CFRelease (uni_str_ref);
   
   std::string utf8_name (utf8_name_0);
   
   if (utf8_name.size () > 0)
   {
      if (utf8_name [0] == '.')
      {
         hidden_flag = true;
      }
   }
   
   long flags = util::FileDescriptor::make_flags (dir_flag, read_only_flag, hidden_flag, system_flag);
   
   util::FileDescriptor * file_desc_ptr = new util::FileDescriptor (
      utf8_name, creation_date, modification_date, length, flags
   );
   
   _file_desc_aptr = std::auto_ptr <util::FileDescriptor> (file_desc_ptr);
   
   _update_flag = false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
