/*****************************************************************************

        ImageLoader.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 51504

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

#include "ohm/opal/ImageLoader.h"
#include "ohm/opa/Size.h"
#include "ohm/opa/Rect.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/directx/ImageLoaderImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/quartz/ImageLoaderImpl.h"

#else
   #error Operating System not supported

#endif

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

ImageLoader::~ImageLoader ()
{
}



/*
==============================================================================
Name : push_back_base_path
Description :
   push a base path to search. This must have an absolute directory
   style so that base_path + path is a valid absolute path to a file
   in the fopen format
==============================================================================
*/

void  ImageLoader::push_base_path (const std::string & base_path)
{
   _base_path_arr.push_back (base_path);
}



/*
==============================================================================
Name : load
Description :
   fill 'bitmap' with image pointed by 'path' if it exists.
   Returns true if it succeeded
==============================================================================
*/

bool  ImageLoader::load (Bitmap & bitmap, const std::string & path, bool premultiply_flag)
{
   bool ok_flag = false;
   
   if (!ok_flag)
   {
      ok_flag = ImageLoaderImpl::use_instance ()
         .load_from_resource (bitmap, path, premultiply_flag);
   }

   if (!ok_flag)
   {
      ok_flag = ImageLoaderImpl::use_instance ()
         .load_from_path (bitmap, path, premultiply_flag);
   }

   if (!ok_flag)
   {
      PathList::const_iterator it = _base_path_arr.begin ();
      const PathList::const_iterator it_end = _base_path_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         std::string base_path = *it;
         
         std::string absolute_path = base_path + path;
         
         ok_flag = ImageLoaderImpl::use_instance ()
            .load_from_path (bitmap, absolute_path, premultiply_flag);
            
         if (ok_flag)
         {
            break;
         }
      }
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : load
Description :
   fill 'bitmap' with image described by 'data_ptr'
   Returns true if it succeeded
==============================================================================
*/

bool  ImageLoader::load (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size, bool premultiply_flag)
{
   bool ok_flag = ImageLoaderImpl::use_instance ()
      .load_from_memory (bitmap, data_ptr, data_size, premultiply_flag);
   
   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ImageLoader::ImageLoader ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
