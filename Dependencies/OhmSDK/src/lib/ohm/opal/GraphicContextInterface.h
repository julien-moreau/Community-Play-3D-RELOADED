/*****************************************************************************

        GraphicContextInterface.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if defined (ohm_opal_GraphicContextInterface_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opal/GraphicContextInterface header.
#endif
#define  ohm_opal_GraphicContextInterface_CURRENT_HEADER

#if ! defined (ohm_opal_GraphicContextInterface_HEADER_INCLUDED)
#define  ohm_opal_GraphicContextInterface_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/FragmentProgramProc.h"
#include "ohm/opal/private/Engine.h"
#include "ohm/opal/def.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/Image.h"

#include <vector>
#include <string>



namespace ohm
{
namespace opa
{
class Size;
class Rect;
class Point;
class Quad;
class Path;
}



namespace opal
{



class Color;
class LabelString;
class Image;
class SprungImage;
class Cache;

class GraphicContextInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~GraphicContextInterface () {}
   
   virtual Engine &
                  use_engine () = 0;
   
   // Graphic State
   virtual void   set_alpha (float alpha) = 0;
   virtual void   concat_alpha (float alpha) = 0;
   virtual void   set_stroke_color (const Color & color) = 0;
   virtual void   set_fill_color (const Color & color) = 0;
   virtual void   set_clear_color (const Color & color) = 0;

   virtual void   set_blend_mode_normal () = 0;
   virtual void   set_blend_mode_multiply () = 0;
   virtual void   set_blend_mode_screen () = 0;
   virtual void   set_blend_mode_replace () = 0;
   virtual void   set_blend_mode_add () = 0;
   virtual void   set_blend_mode_max () = 0;
   virtual void   set_blend_mode_min () = 0;
   
   virtual void   set_fragment_program (FragmentProgramProc proc) = 0;
   virtual void   set_fragment_local_vars (float var_0, float var_1, float var_2, float var_3) = 0;
   
   virtual void   clip (const opa::Rect & rect) = 0;
   virtual opa::Rect
                  get_clip_bounding_box () = 0;
   virtual void   concat_translate (float tx, float ty) = 0;
   
   virtual void   save_graphic_state () = 0;
   virtual void   restore_graphic_state () = 0;
   
   // Vectorial Drawing
   virtual void   fill (const opa::Path & path) = 0;
   virtual void   fill (const opa::Rect & rect) = 0;
   virtual void   fill (const opa::Quad & vertices) = 0;

   virtual void   stroke (const opa::Path & path) = 0;
   
   virtual void   clear () = 0;
   
   // Raster Drawing
   virtual void   draw (const Image & image, const opa::Point & point) = 0;
   virtual void   draw (const Image & image, const opa::Rect & rect) = 0;
   virtual void   draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords) = 0;
   virtual void   draw (const SprungImage & sprung_image, const opa::Rect & rect) = 0;

   virtual void   draw (const Image & mask, const opa::Point & point, const Color & color) = 0;
   virtual void   draw (const Image & mask, const opa::Rect & rect, const Color & color) = 0;
   virtual void   draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color) = 0;
   virtual void   draw (const SprungImage & sprung_mask, const opa::Rect & rect, const Color & color) = 0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           operator == (const GraphicContextInterface &other);
   bool           operator != (const GraphicContextInterface &other);



}; // class GraphicContextInterface



/*\\\ PUBLIC OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_GraphicContextInterface_HEADER_INCLUDED

#undef ohm_opal_GraphicContextInterface_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
