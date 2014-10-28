/*****************************************************************************

        ImageLoaderImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59383

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/ImageLoaderImpl.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/def.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/private/PngLoader.h"
#include "ohm/opal/win32/ApplicationImpl.h"

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
   ::HINSTANCE hinst = ApplicationImpl::use_instance ().get_hinst ();
   ::HRSRC hrsrc = ::FindResource (hinst, path.c_str (), RT_RCDATA);

   bool ok_flag = false;

   if (hrsrc != 0)
   {
      ::HGLOBAL hglobal = ::LoadResource (hinst, hrsrc);
      assert (hglobal != 0);
      ::DWORD rsrc_size = ::SizeofResource (hinst, hrsrc);
      assert (rsrc_size > 0);

      archi::UByte * data_ptr = (archi::UByte *) (::LockResource (hglobal));
      
      ok_flag = PngLoader::read_png (bitmap, data_ptr, rsrc_size);
   }
   
   /*if (!ok_flag)
   {
      ok_flag = load (bitmap, path, true);
   }*/

   if (ok_flag && premultiply_flag)
   {
      bitmap.premultiply ();
   }

   return ok_flag;
}



/*
==============================================================================
Name : load_from_path
==============================================================================
*/

bool  ImageLoaderImpl::load_from_path (Bitmap & bitmap, const std::string & path, bool premultiply_flag)
{
   bool ok_flag = PngLoader::read_png (bitmap, path);

   /*if (!ok_flag)
   {
      ok_flag = load (bitmap, path, false);
   }*/

   if (ok_flag && premultiply_flag)
   {
      bitmap.premultiply ();
   }

   return ok_flag;
}



/*
==============================================================================
Name : load_from_memory
==============================================================================
*/

bool  ImageLoaderImpl::load_from_memory (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size, bool premultiply_flag)
{
   bool ok_flag = PngLoader::read_png (bitmap, data_ptr, data_size);

   /*if (!ok_flag)
   {
      ok_flag = load (bitmap, data_ptr, data_size);
   }*/

   if (ok_flag && premultiply_flag)
   {
      bitmap.premultiply ();
   }

   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ImageLoaderImpl::ImageLoaderImpl ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load
==============================================================================
*/

bool  ImageLoaderImpl::load (Bitmap & bitmap, const std::string & path, bool resource_flag)
{
   // we should not call this method, as there will be multithreading issues
   // when called from a thread other than than the video thread
   // this happen when loading images for the cursors
   // in this case, DirectXEngine::use_instance () will assert
   
   return false;
   
   /*::IDirect3DDevice9 *     d3d_device_ptr = DirectXEngine::use_instance ().get_d3d_device ();
   ::IDirect3DTexture9 *   texture_ptr = 0;
   ::HRESULT               result = D3D_OK;
   
   if (resource_flag)
   {
      ::HINSTANCE          h_instance = ApplicationImpl::use_instance ().get_hinst ();

      // load from resource
      result = ::D3DXCreateTextureFromResourceEx (
         d3d_device_ptr,
         h_instance,
         path.c_str (),
         D3DX_DEFAULT_NONPOW2,   // width from file
         D3DX_DEFAULT_NONPOW2,   // height from file
         1, // mipmap level
         0, // default usage
         D3DFMT_A8R8G8B8,
         D3DPOOL_SYSTEMMEM,
         D3DX_FILTER_LINEAR,  // linear filter for subpixel
         D3DX_DEFAULT,  // no mip filter
         0, // no color key
         0, // no image info
         0, // no 256 colors
         &texture_ptr
      );
   }
   else
   {
      // load from file
      result = ::D3DXCreateTextureFromFileEx (
         d3d_device_ptr,
         path.c_str (),
         D3DX_DEFAULT_NONPOW2,   // width from file
         D3DX_DEFAULT_NONPOW2,   // height from file
         1, // mipmap level
         0, // default usage
         D3DFMT_A8R8G8B8,
         D3DPOOL_SYSTEMMEM,
         D3DX_FILTER_LINEAR,  // linear filter for subpixel
         D3DX_DEFAULT,  // no mip filter
         0, // no color key
         0, // no image info
         0, // no 256 colors
         &texture_ptr
      );
   }

   assert (result == D3D_OK);
   assert (texture_ptr != 0);
   
   ::IDirect3DSurface9 * surface_ptr = 0;
   result = texture_ptr->GetSurfaceLevel (0, &surface_ptr);
   assert (result == D3D_OK);
   assert (surface_ptr != 0);
   
   ::D3DLOCKED_RECT  locked_rect;
   result = surface_ptr->LockRect (&locked_rect, 0, 0);
   assert (result == D3D_OK);
   
   ::D3DSURFACE_DESC desc;
   texture_ptr->GetLevelDesc (0, &desc);
   
   archi::UByte * src_ptr = reinterpret_cast <archi::UByte *> (locked_rect.pBits);
   
   bitmap.resize (desc.Width, desc.Height);

   ::memcpy (bitmap.data (), src_ptr, 4 * bitmap.size ());
   
   result = surface_ptr->UnlockRect ();
   assert (result == D3D_OK);
   
   ULONG ref_cnt = surface_ptr->Release ();
// assert (ref_cnt == 1);
   
   ref_cnt = texture_ptr->Release ();
   assert (ref_cnt == 0);
   
   return true;*/
}



/*
==============================================================================
Name : load
==============================================================================
*/

bool  ImageLoaderImpl::load (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_size)
{
   // we should not call this method, as there will be multithreading issues
   // when called from a thread other than than the video thread
   // this happen when loading images for the cursors
   // in this case, DirectXEngine::use_instance () will assert
   
   return false;
   
   /*::IDirect3DDevice9 *     d3d_device_ptr = DirectXEngine::use_instance ().get_d3d_device ();
   ::IDirect3DTexture9 *   texture_ptr = 0;
   ::HRESULT               result = D3D_OK;
   
   // load from memory
   result = ::D3DXCreateTextureFromFileInMemoryEx (
      d3d_device_ptr,
      data_ptr, data_size,
      D3DX_DEFAULT_NONPOW2,   // width from file
      D3DX_DEFAULT_NONPOW2,   // height from file
      1, // mipmap level
      0, // default usage
      D3DFMT_A8R8G8B8,
      D3DPOOL_SYSTEMMEM,
      D3DX_FILTER_LINEAR,  // linear filter for subpixel
      D3DX_DEFAULT,  // no mip filter
      0, // no color key
      0, // no image info
      0, // no 256 colors
      &texture_ptr
   );

   assert (result == D3D_OK);
   assert (texture_ptr != 0);
   
   ::IDirect3DSurface9 * surface_ptr = 0;
   result = texture_ptr->GetSurfaceLevel (0, &surface_ptr);
   assert (result == D3D_OK);
   assert (surface_ptr != 0);
   
   ::D3DLOCKED_RECT  locked_rect;
   result = surface_ptr->LockRect (&locked_rect, 0, 0);
   assert (result == D3D_OK);
   
   ::D3DSURFACE_DESC desc;
   texture_ptr->GetLevelDesc (0, &desc);
   
   archi::UByte * src_ptr = reinterpret_cast <archi::UByte *> (locked_rect.pBits);
   
   bitmap.resize (desc.Width, desc.Height);

   ::memcpy (bitmap.data (), src_ptr, 4 * bitmap.size ());
   
   result = surface_ptr->UnlockRect ();
   assert (result == D3D_OK);
   
   ULONG ref_cnt = surface_ptr->Release ();
// assert (ref_cnt == 1);
   
   ref_cnt = texture_ptr->Release ();
   assert (ref_cnt == 0);
   
   return true;*/
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

