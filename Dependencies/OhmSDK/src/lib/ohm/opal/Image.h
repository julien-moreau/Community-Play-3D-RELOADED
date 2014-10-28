/*****************************************************************************

        Image.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 63400

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



#if ! defined (ohm_opal_Image_HEADER_INCLUDED)
#define  ohm_opal_Image_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/opal/def.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/SprungMode.h"
#include "ohm/opal/FilterMode.h"
#include "ohm/opal/private/Engine.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include <string>
   
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <windowsx.h>
   #include <d3d9.h>
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include <AGL/agl.h>
#endif   // ohm_archi_SYS



namespace ohm
{
namespace opal
{



class Bitmap;
class GraphicContextInterface;

class Image
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Conf
   {
   public:
      SprungMode  _sprung_mode_x;
      SprungMode  _sprung_mode_y;
      FilterMode  _filter_mode;
   };

                  Image ();
                  Image (const Image & other);
                  Image (GraphicContextInterface & gc, const Bitmap & premultiplied_bitmap, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
                  Image (Engine & engine, const PixArgb * pix_ptr, size_t width, size_t scanline_width, size_t height, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);
#endif
   Image &        operator = (const Image & other);
   virtual        ~Image ();
   
   bool           operator == (const Image &other) const;
   bool           operator != (const Image &other) const;

   bool           empty () const;
   size_t         retain ();
   size_t         release ();
   
   void           create (Engine & engine, const PixArgb * buf_ptr, const Conf & conf, bool buf_owner_flag);
   
   const char *   _name_0; // maybe valid data, maybe not, used only for debugging
   
   size_t         _width;
   size_t         _height;

   size_t         _scanline_width;
   
   size_t         _tex_width;
   size_t         _tex_height;

   float          _tex_s;
   float          _tex_t;

   SprungMode     _sprung_mode_x;
   SprungMode     _sprung_mode_y;

   bool           _software_wrap_repeat_x_flag;
   bool           _software_wrap_repeat_y_flag;
   
   Engine *       _eng_ptr;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   FilterMode     _filter_mode;
   class Data
   {
   public:
      IDirect3DTexture9 *
                  _texture_ptr;
      size_t      _retain_cnt;
      const PixArgb *
                  _buf_ptr;
      bool        _buf_owner_flag;
   };
   typedef Data * DataRef;
   
   DataRef        _data_ref;

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   
   class Data
   {
   public:
      GLuint      _texture_id;
      GLenum      _tex_target;
      size_t      _retain_cnt;
      const PixArgb *
                  _buf_ptr;
      bool        _buf_owner_flag;
   };
   typedef Data * DataRef;
   
   DataRef        _data_ref;

#else
   #error Operating System not supported
#endif


   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class Image






}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/Image.hpp"



#endif   // ohm_opal_Image_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
