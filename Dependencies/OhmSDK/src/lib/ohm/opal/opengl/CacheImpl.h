/*****************************************************************************

        CacheImpl.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59382

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



#if ! defined (ohm_opal_CacheImpl_HEADER_INCLUDED)
#define  ohm_opal_CacheImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/PixArgb.h"

#include <vector>



namespace ohm
{
namespace opa
{
class Path;
}



namespace opal
{



class Color;
class GraphicState;
class OpenGLEngine;

class CacheImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  CacheImpl (GraphicContextInterface & gc, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode);
   virtual        ~CacheImpl ();
   
   void           resize (const opa::Size & size);
   const opa::Size &
                  get_size ();
   
   Image          get_image () const;

   bool           is_dirty () const;
   void           invalidate_all ();
   void           invalidate (const opa::Rect & rect);
   void           validate ();
   
   const opa::Rect &
                  get_invalid_rect () const;
   
   void           update_context (const GraphicState & graphic_state);

   void           fill (const opa::Path & path, const Color & color);
   void           fill (const opa::Rect & rect, const Color & color);
   void           fill (const opa::Quad & vertices, const Color & color);

   void           stroke (const opa::Path & path, const Color & color);
   
   void           draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha);
   void           draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           release ();
   
   OpenGLEngine & _engine;
   
   Image::Conf    _conf;
   
   GLuint         _frame_buffer_id;
   
   opa::Size      _size;
   mutable Image  _image;

   bool           _invalid_flag;
   opa::Rect      _invalid_rect;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CacheImpl (const CacheImpl &other);
   CacheImpl &    operator = (const CacheImpl &other);
   bool           operator == (const CacheImpl &other);
   bool           operator != (const CacheImpl &other);

}; // class CacheImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_CacheImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
