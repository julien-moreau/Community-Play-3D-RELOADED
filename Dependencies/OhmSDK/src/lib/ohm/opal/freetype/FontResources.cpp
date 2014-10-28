/*****************************************************************************

        FontResources.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53922

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

#include "ohm/opal/freetype/FontResources.h"
#include "ohm/opal/freetype/FTLibrary.h"
#include "ohm/util/trace.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/ApplicationImpl.h"
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FontResources::~FontResources ()
{
}



/*
==============================================================================
Name : get
==============================================================================
*/

const FontResources::Result & FontResources::get (const std::string & name) const
{
   NameResultMap::const_iterator it = _name_result_map.find (name);
   assert (it != _name_result_map.end ());
   
   return it->second;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FontResources::FontResources ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   // enumerate .ttf rsrc
   
   CFBundleRef bundle_ref = CFBundleGetMainBundle ();
   
   CFMutableArrayRef array_ref = CFArrayCreateMutable (0, 0, &kCFTypeArrayCallBacks);
   
   {
      CFArrayRef sub_array_ref = CFBundleCopyResourceURLsOfType (
         bundle_ref,
         CFSTR(""),
         0
      );
      
      CFArrayAppendArray (
         array_ref, sub_array_ref, CFRangeMake (0, CFArrayGetCount (sub_array_ref))
      );
      
      CFRelease (sub_array_ref);
      sub_array_ref = 0;
   }
   
   CFIndex array_size = CFArrayGetCount (array_ref);
   
   for (CFIndex i = 0 ; i < array_size ; ++i)
   {
      CFURLRef url_ref = (CFURLRef) CFArrayGetValueAtIndex (array_ref, i);
      
      const size_t max_buf_len = 2048;
      
      char buf_0 [max_buf_len + 1];
      
      Boolean ok_flag = CFURLGetFileSystemRepresentation (
         url_ref,
         true, // return absolute path name
         (UInt8 *) buf_0,
         max_buf_len
      );
      assert (ok_flag);
      
      FT_Library & library = FTLibrary::use_instance ().get ();
   
      FT_Face face = 0;
      FT_Error err = FT_New_Face (
         library,
         buf_0,
         0,
         &face
      );

      if (err == 0)
      {
         const char * font_name_0 = FT_Get_Postscript_Name (face);
         
         //ohm_util_TRACE_INFO1 ("Found %1%", font_name_0);
         
         Result result;
         result._memory_flag = false;
         result._path = buf_0;
         result._data_ptr = 0;
         result._data_size = 0;
         
         _name_result_map [font_name_0] = result;
         
         err = FT_Done_Face (face);
         assert (err == 0);
      }
   }
   
   CFRelease (array_ref);
   array_ref = 0;

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)

   ::HINSTANCE    h_instance = ApplicationImpl::use_instance ().get_hinst ();
   
   ::EnumResourceNames (
      h_instance,
      MAKEINTRESOURCE (RT_RCDATA),  // type
      enum_resource_name_proc,
      long (this)
   );


#else
   #error Unsupported operating system.
#endif
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
BOOL CALLBACK FontResources::enum_resource_name_proc (::HMODULE hinst, ::LPCTSTR type_0, ::LPTSTR name_0, ::LONG_PTR lparam)
{
   BOOL ret_val = true;
   
   try
   {
      FontResources *   this_ptr = reinterpret_cast <FontResources *> (lparam);
      assert (this_ptr != 0);
      
      ret_val = this_ptr->enum_resource_name (hinst, type_0, name_0);
   }
   catch (...)
   {
      assert (false);
   }
   
   return ret_val;
}



BOOL CALLBACK FontResources::enum_resource_name (::HMODULE hinst, ::LPCTSTR type_0, ::LPTSTR name_0)
{
   std::string name = std::string (name_0);
   
   if (name.size () > 5)
   {
      bool ok_flag = false;
      ok_flag |= (name.rfind (".FONT") == name.size () - 5);
      ok_flag |= (name.rfind (".TTF") == name.size () - 4);

      if (ok_flag)
      {
         ::HRSRC hrsrc = ::FindResource (hinst, name_0, type_0);
         assert (hrsrc != 0);

         ::HGLOBAL hglobal = ::LoadResource (hinst, hrsrc);
         assert (hglobal != 0);
         
         ::DWORD rsrc_size = ::SizeofResource (hinst, hrsrc);
         assert (rsrc_size > 0);

         archi::UByte * data_ptr = (archi::UByte *) (::LockResource (hglobal));
         // no unlocking possible. We eat the memory until the process ends...
         // see http://msdn.microsoft.com/en-us/library/ms648044(VS.85).aspx

         FT_Library & library = FTLibrary::use_instance ().get ();
      
         FT_Face face = 0;
         FT_Error err = FT_New_Memory_Face (
            library,
            data_ptr,
            rsrc_size,
            0,
            &face
         );

         if (err == 0)
         {
            const char * font_name_0 = FT_Get_Postscript_Name (face);
            
            //ohm_util_TRACE_INFO1 ("Found %1%", font_name_0);
            
            Result result;
            result._memory_flag = true;
            result._path = "";
            result._data_ptr = data_ptr;
            result._data_size = rsrc_size;
            
            _name_result_map [font_name_0] = result;
            
            err = FT_Done_Face (face);
            assert (err == 0);
         }
      }
   }

   return TRUE;
}



#endif



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
