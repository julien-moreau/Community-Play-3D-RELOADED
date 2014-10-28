/*****************************************************************************

        ImageManager.h
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



#if ! defined (ohm_opal_ImageManager_HEADER_INCLUDED)
#define  ohm_opal_ImageManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/SprungMode.h"
#include "ohm/opal/FilterMode.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/util/SingletonBase.h"

#include <list>
#include <string>
#include <map>



namespace ohm
{
namespace opal
{



class Bitmap;
class GraphicContextInterface;

class ImageManager
:  public util::SingletonBase <ImageManager>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (*ImageProcessor) (Bitmap & bitmap);
   
   virtual        ~ImageManager ();
   
   void           release (Engine & engine);
   void           release_all ();
   
   static void    image_processor_to_gray (Bitmap & bitmap);
   
   Image          get_image (GraphicContextInterface & gc, const std::string & path, ImageProcessor processor = 0, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);
   Image          get_image_fit (GraphicContextInterface & gc, const std::string & path, const opa::Size & fit_size, ImageProcessor processor = 0, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);
   SprungImage    get_sprung_image (GraphicContextInterface & gc, const std::string & path, const opa::Rect & rect, ImageProcessor processor = 0, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  ImageManager ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (ImageManager)
   
   class Key
   {
   public:
      bool        operator < (const Key & other) const;
      
      Engine *    _eng_ptr;
      std::string _path;
      ImageProcessor
                  _processor;
      SprungMode  _sprung_mode_x;
      SprungMode  _sprung_mode_y;
      FilterMode  _filter_mode;
      bool        _sprung_flag;
      opa::Rect   _sprung_rect;
      bool        _fit_flag;
      opa::Size   _fit_size;
   };
   
   class ImageContainer
   {
   public:
                  ImageContainer ();
      virtual     ~ImageContainer ();
      Image *     _image_ptr;
      SprungImage *
                  _sprung_image_ptr;
   };
   
   typedef std::map <Key, ImageContainer *> KeyImageContainerPtrMap;
   
   ImageContainer *
                  get (GraphicContextInterface & gc, const Key & key);
   
   KeyImageContainerPtrMap
                  _key_image_container_ptr_map;
   
   static Image   _empty_image;
   static SprungImage
                  _empty_sprung_image;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ImageManager (const ImageManager &other);
   ImageManager & operator = (const ImageManager &other);
   bool           operator == (const ImageManager &other);
   bool           operator != (const ImageManager &other);

}; // class ImageManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ImageManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
