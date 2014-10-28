/*****************************************************************************

        ImageManager.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62268

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

#include "ohm/opal/ImageManager.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/dsp/Downsampler.h"
#include "ohm/lang/fnc.h"
#include "ohm/util/trace.h"

#include <cassert>

#define OHM_OPAL_PRINT_LEAKS



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

ImageManager::~ImageManager ()
{
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  ImageManager::release (Engine & engine)
{
   KeyImageContainerPtrMap::iterator it = _key_image_container_ptr_map.begin ();
   KeyImageContainerPtrMap::iterator it_end = _key_image_container_ptr_map.end ();
   
   for (; it != it_end ;)
   {
      KeyImageContainerPtrMap::iterator it_next = it;
      ++it_next;
      
      ImageContainer * container_ptr = it->second;
      
      if (container_ptr != 0)
      {
         bool release_flag = false;
         
         if (container_ptr->_image_ptr != 0)
         {
            assert (container_ptr->_image_ptr->_eng_ptr != 0);
            
            release_flag = container_ptr->_image_ptr->_eng_ptr == &engine;
         }
         
         if (container_ptr->_sprung_image_ptr != 0)
         {
            assert (container_ptr->_sprung_image_ptr->_eng_ptr != 0);
            
            release_flag = container_ptr->_sprung_image_ptr->_eng_ptr == &engine;
         }
         
         if (release_flag)
         {
            delete container_ptr;
            
            _key_image_container_ptr_map.erase (it);
         }
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name : release_all
==============================================================================
*/

void  ImageManager::release_all ()
{
   KeyImageContainerPtrMap::iterator it = _key_image_container_ptr_map.begin ();
   KeyImageContainerPtrMap::iterator it_end = _key_image_container_ptr_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ImageContainer * container_ptr = it->second;
      
      delete container_ptr;
   }
   
   _key_image_container_ptr_map.clear ();
}



/*
==============================================================================
Name : image_processor_to_gray
==============================================================================
*/

void  ImageManager::image_processor_to_gray (Bitmap & bitmap)
{
   size_t bsize = bitmap.size ();
   
   PixArgb * pix_ptr = bitmap.data ();
   
   for (size_t i = 0 ; i < bsize ; ++i)
   {
      PixArgb & pix = pix_ptr [i];
      
      int gray = int (pix._r) + int (pix._g) + int (pix._b);
      
      gray = (gray * 85) >> 8;
      
      pix._r = gray;
      pix._g = gray;
      pix._b = gray;
   }
}



/*
==============================================================================
Name : get_image
==============================================================================
*/

Image ImageManager::get_image (GraphicContextInterface & gc, const std::string & path, ImageProcessor processor, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
{
   assert (&gc != 0);
   assert (&gc.use_engine () != 0);
   
   Key key;
   key._eng_ptr = &gc.use_engine ();
   key._path = path;
   key._processor = processor;
   key._sprung_mode_x = sprung_mode_x;
   key._sprung_mode_y = sprung_mode_y;
   key._filter_mode = filter_mode;
   key._sprung_flag = false;
   key._sprung_rect = opa::Rect::_zero;
   key._fit_flag = false;
   key._fit_size = opa::Size::_zero;
   
   ImageContainer * image_container_ptr = get (gc, key);
   
   assert (image_container_ptr != 0);
   
   if (image_container_ptr != 0)
   {
      assert (image_container_ptr->_image_ptr != 0);
   
      return *image_container_ptr->_image_ptr;
   }
   else
   {
      return _empty_image;
   }
}



/*
==============================================================================
Name : get_image
Description :
   Downsample image so that it fits in 'fit_rect'
==============================================================================
*/

Image ImageManager::get_image_fit (GraphicContextInterface & gc, const std::string & path, const opa::Size & fit_size, ImageProcessor processor, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
{
   // filter gaussian, then sample nearest
   // crop or distord to nearest so that no pixels are transparent

   assert (&gc != 0);
   
   Key key;
   key._eng_ptr = &gc.use_engine ();
   key._path = path;
   key._processor = processor;
   key._sprung_mode_x = sprung_mode_x;
   key._sprung_mode_y = sprung_mode_y;
   key._filter_mode = filter_mode;
   key._sprung_flag = false;
   key._sprung_rect = opa::Rect::_zero;
   key._fit_flag = true;
   key._fit_size = fit_size;
   
   ImageContainer * image_container_ptr = get (gc, key);
   
   assert (image_container_ptr != 0);
   
   if (image_container_ptr != 0)
   {
      assert (image_container_ptr->_image_ptr != 0);
   
      return *image_container_ptr->_image_ptr;
   }
   else
   {
      return _empty_image;
   }
}



/*
==============================================================================
Name : get_sprung_image
==============================================================================
*/

SprungImage ImageManager::get_sprung_image (GraphicContextInterface & gc, const std::string & path, const opa::Rect & rect, ImageProcessor processor, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
{
   assert (&gc != 0);
   
   Key key;
   key._eng_ptr = &gc.use_engine ();
   key._path = path;
   key._processor = processor;
   key._sprung_mode_x = sprung_mode_x;
   key._sprung_mode_y = sprung_mode_y;
   key._filter_mode = filter_mode;
   key._sprung_flag = true;
   key._sprung_rect = rect;
   key._fit_flag = false;
   key._fit_size = opa::Size::_zero;
   
   ImageContainer * image_container_ptr = get (gc, key);
   
   assert (image_container_ptr != 0);
   
   if (image_container_ptr != 0)
   {
      assert (image_container_ptr->_sprung_image_ptr != 0);
   
      return *image_container_ptr->_sprung_image_ptr;
   }
   else
   {
      return _empty_sprung_image;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ImageManager::ImageManager ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name : get
==============================================================================
*/

ImageManager::ImageContainer *   ImageManager::get (GraphicContextInterface & gc, const Key & key)
{
   KeyImageContainerPtrMap::iterator it = _key_image_container_ptr_map.find (key);
   
   if (it == _key_image_container_ptr_map.end ())
   {
      Bitmap bitmap;
      bool premultiply_flag = true;
      bool ok_flag = ImageLoader::use_instance ().load (
         bitmap, key._path, premultiply_flag
      );
      assert (ok_flag);
      
      if (!ok_flag)
      {
         return 0;
      }
      
      if (key._fit_flag)
      {
         opa::Rect fit_rect (opa::Rect (opa::Point::_zero, key._fit_size));
         opa::Rect image_rect (opa::Rect (
            opa::Point::_zero,
            opa::Size (float (bitmap.get_width ()), float (bitmap.get_height ()))
         ));
         
         if (!fit_rect.contains (image_rect))
         {
            Downsampler downsampler;
            downsampler.set (archi::UInt32 (key._fit_size._width), archi::UInt32 (key._fit_size._height));
            
            Bitmap downsampled_bitmap;
            downsampler.process (downsampled_bitmap, bitmap);
            
            bitmap.swap (downsampled_bitmap);
         }
      }
      
      if (key._processor != 0)
      {
         key._processor (bitmap);
      }
      
      if (key._sprung_flag)
      {
         SprungImage * sprung_image_ptr = new SprungImage (
            gc, bitmap,
            key._sprung_rect,
            key._sprung_mode_x,
            key._sprung_mode_y,
            key._filter_mode
         );
         
         ImageContainer * image_container_ptr = new ImageContainer;
         image_container_ptr->_sprung_image_ptr = sprung_image_ptr;
         
         std::pair <KeyImageContainerPtrMap::iterator, bool> ret
            = _key_image_container_ptr_map.insert (
            std::make_pair (key, image_container_ptr)
         );
         assert (ret.second);
         
         it = ret.first;
      }
      else
      {
         Image * image_ptr = new Image (
            gc, bitmap,
            key._sprung_mode_x,
            key._sprung_mode_y,
            key._filter_mode
         );
         
         char * name_0 = new char [key._path.size () + 1];
         memcpy (name_0, key._path.c_str (), key._path.size () + 1);
         
         image_ptr->_name_0 = name_0;
         
         ImageContainer * image_container_ptr = new ImageContainer;
         image_container_ptr->_image_ptr = image_ptr;
         
         std::pair <KeyImageContainerPtrMap::iterator, bool> ret
            = _key_image_container_ptr_map.insert (
            std::make_pair (key, image_container_ptr)
         );
         assert (ret.second);
         
         it = ret.first;
      }
   }
   
   return it->second;
}
   


/*
==============================================================================
Name : ImageContainer::ctor
==============================================================================
*/

bool  ImageManager::Key::operator < (const Key & other) const
{
   int c = lang::cmp (_eng_ptr, other._eng_ptr);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_path, other._path);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_processor, other._processor);
   if (c != 0) return c < 0;

   c = lang::cmp (_sprung_mode_x, other._sprung_mode_x);
   if (c != 0) return c < 0;

   c = lang::cmp (_sprung_mode_y, other._sprung_mode_y);
   if (c != 0) return c < 0;

   c = lang::cmp (_filter_mode, other._filter_mode);
   if (c != 0) return c < 0;

   c = lang::cmp (_sprung_flag, other._sprung_flag);
   if (c != 0) return c < 0;

   c = lang::cmp (_sprung_rect, other._sprung_rect);
   if (c != 0) return c < 0;

   c = lang::cmp (_fit_flag, other._fit_flag);
   if (c != 0) return c < 0;

   c = lang::cmp (_fit_size, other._fit_size);
   return c < 0;
}



/*
==============================================================================
Name : ImageContainer::ctor
==============================================================================
*/

ImageManager::ImageContainer::ImageContainer ()
:  _image_ptr (0)
,  _sprung_image_ptr (0)
{
}



/*
==============================================================================
Name : ImageContainer::dtor
==============================================================================
*/

ImageManager::ImageContainer::~ImageContainer ()
{
   if (_image_ptr != 0)
   {
      size_t ref_cnt = _image_ptr->release ();

      const char * name_0 = _image_ptr->_name_0;
         
#if defined (OHM_OPAL_PRINT_LEAKS)
      if (ref_cnt != 0)
      {
         if (name_0 == 0)
         {
            name_0 = "<Unknown>";
         }
         
         ohm_util_TRACE_ERROR1 ("Image %1% leaked.", name_0);
      }
#else
      assert (ref_cnt == 0);
#endif
   
      if (name_0 != 0)
      {
         delete [] name_0;
      }
      
      delete _image_ptr;
      _image_ptr = 0;
   }
   
   if (_sprung_image_ptr != 0)
   {
      //_sprung_image_ptr->release_check_total ();
      _sprung_image_ptr->release ();
      
      delete _sprung_image_ptr;
      _sprung_image_ptr = 0;
   }
}



Image ImageManager::_empty_image;
SprungImage ImageManager::_empty_sprung_image;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
