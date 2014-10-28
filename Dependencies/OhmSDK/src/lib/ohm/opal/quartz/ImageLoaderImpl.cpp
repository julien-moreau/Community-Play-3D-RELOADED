/*****************************************************************************

        ImageLoaderImpl.cpp
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

#if (ohm_opal_API != ohm_opal_API_OPENGL)
   #error Unsupported API
#endif

#include "ohm/opal/quartz/ImageLoaderImpl.h"
/*#include "ohm/opal/quartz/MainDeviceSpace.h"*/
#include "ohm/opal/Bitmap.h"
#include "ohm/math/def.h"
#include "ohm/opal/def.h"
#include "ohm/opal/private/PngLoader.h"

#include "ohm/opal/opengl/OpenGLEngine.h"


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

ImageLoaderImpl::~ImageLoaderImpl ()
{
}



/*
==============================================================================
Name : load_from_resource
==============================================================================
*/

bool  ImageLoaderImpl::load_from_resource (Bitmap & bitmap, const std::string & path, bool premultiply_flag)
{
   assert (path.size () > 0);
   assert (path [0] != '/');
   
   return load_from_path (bitmap, _base_rsrc_path + path, premultiply_flag);
}



/*
==============================================================================
Name : load_from_path
==============================================================================
*/

bool  ImageLoaderImpl::load_from_path (Bitmap & bitmap, const std::string & path, bool premultiply_flag)
{
   if (PngLoader::read_png (bitmap, path))
   {
      if (premultiply_flag)
      {
         bitmap.premultiply ();
      }
      
      return true;
   }
   
   CFURLRef url_ref = CFURLCreateFromFileSystemRepresentation (
      0, (::UInt8 *) &path [0], path.size (), false
   );
   assert (url_ref != 0);
   
   CGImageSourceRef image_source_ref = CGImageSourceCreateWithURL (url_ref, 0);
   Image ret_val;
   
   CFRelease (url_ref);
   
   CGImageRef image_ref = 0;
   
   if (image_source_ref != 0)
   {
      image_ref = CGImageSourceCreateImageAtIndex (image_source_ref, 0, 0);
      // ret_val might be 0
      
      CFRelease (image_source_ref);
      image_source_ref = 0;
   }
   
   if (image_ref != 0)
   {
      size_t image_width = CGImageGetWidth (image_ref);
      size_t image_height = CGImageGetHeight (image_ref);
      
      bitmap.resize (image_width, image_height);
   
      /*CGColorSpaceRef main_device_space_ref = MainDeviceSpace::use_instance ().get ();
      assert (main_device_space_ref != 0);*/
      
      CGBitmapInfo info = kCGBitmapByteOrder32Host;
      
      if (premultiply_flag)
      {
         info |= kCGImageAlphaPremultipliedFirst;
      }
      else
      {
         info |= kCGImageAlphaFirst;
      }
      
      CGContextRef context_ref = CGBitmapContextCreate (
         bitmap.data (),
         bitmap.get_width (),
         bitmap.get_height (),
         8,
         bitmap.get_width () * sizeof (PixArgb),
         CGImageGetColorSpace (image_ref), //main_device_space_ref,
         info
      );
      assert (context_ref != 0);

      CGContextConcatCTM (
         context_ref,
         CGAffineTransformMakeTranslation (0, ret_val._tex_height - ret_val._height)
      );

      CGContextDrawImage (
         context_ref, CGRectMake (0, 0, image_width, image_height), image_ref
      );
      
      CGImageRelease (image_ref);
      
      CGContextRelease (context_ref);
      context_ref = 0;
   }
   
   return image_ref != 0;
}



/*
==============================================================================
Name : load_from_memory
==============================================================================
*/

bool  ImageLoaderImpl::load_from_memory (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size, bool premultiply_flag)
{
   if (PngLoader::read_png (bitmap, data_ptr, data_size))
   {
      if (premultiply_flag)
      {
         bitmap.premultiply ();
      }
      
      return true;
   }
   
   CFDataRef data_ref = CFDataCreateWithBytesNoCopy (
      kCFAllocatorDefault,
      data_ptr, data_size,
      kCFAllocatorNull
   );
   assert (data_ref != 0);
   
   CGImageSourceRef image_source_ref = CGImageSourceCreateWithData (data_ref, 0);
   Image ret_val;
   
   CFRelease (data_ref);
   
   CGImageRef image_ref = 0;
   
   if (image_source_ref != 0)
   {
      image_ref = CGImageSourceCreateImageAtIndex (image_source_ref, 0, 0);
      // ret_val might be 0
      
      CFRelease (image_source_ref);
      image_source_ref = 0;
   }
   
   if (image_ref != 0)
   {
      size_t image_width = CGImageGetWidth (image_ref);
      size_t image_height = CGImageGetHeight (image_ref);
      
      bitmap.resize (image_width, image_height);
   
      /*CGColorSpaceRef main_device_space_ref = MainDeviceSpace::use_instance ().get ();
      assert (main_device_space_ref != 0);*/
      
      CGBitmapInfo info = kCGBitmapByteOrder32Host;
      
      if (premultiply_flag)
      {
         info |= kCGImageAlphaPremultipliedFirst;
      }
      else
      {
         info |= kCGImageAlphaFirst;
      }
      
      CGContextRef context_ref = CGBitmapContextCreate (
         bitmap.data (),
         bitmap.get_width (),
         bitmap.get_height (),
         8,
         bitmap.get_width () * sizeof (PixArgb),
         CGImageGetColorSpace (image_ref), //main_device_space_ref,
         info
      );
      assert (context_ref != 0);

      CGContextConcatCTM (
         context_ref,
         CGAffineTransformMakeTranslation (0, ret_val._tex_height - ret_val._height)
      );

      CGContextDrawImage (
         context_ref, CGRectMake (0, 0, image_width, image_height), image_ref
      );
      
      CGImageRelease (image_ref);
      
      CGContextRelease (context_ref);
      context_ref = 0;
   }
   
   return image_ref != 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ImageLoaderImpl::ImageLoaderImpl ()
{
   CFURLRef url_ref = CFBundleCopyResourcesDirectoryURL (CFBundleGetMainBundle ());
   assert (url_ref != 0);
   
   char buf_0 [4097];
   bool ok_flag = CFURLGetFileSystemRepresentation (url_ref, true, (::UInt8 *) buf_0, 4097);
   assert (ok_flag);
   
   CFRelease (url_ref);
   
   _base_rsrc_path = std::string (buf_0) + "/";
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
