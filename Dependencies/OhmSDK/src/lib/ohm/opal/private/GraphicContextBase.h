/*****************************************************************************

        GraphicContextBase.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60935

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



#if ! defined (ohm_opal_GraphicContextBase_HEADER_INCLUDED)
#define  ohm_opal_GraphicContextBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/Color.h"
#include "ohm/opa/Rect.h"
#include "ohm/opa/Point.h"
#include "ohm/opal/private/GraphicState.h"
#include "ohm/opal/private/GraphicStateStack.h"


#include <stack>



namespace ohm
{
namespace opal
{



class GraphicContextBase
:  public GraphicContextInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  GraphicContextBase ();
   virtual        ~GraphicContextBase ();
   
   Engine &       use_engine ();

   void           set_clip_rect (const opa::Rect & rect);
   
   // Graphic State
   virtual void   set_alpha (float alpha);
   virtual void   concat_alpha (float alpha);
   virtual void   set_stroke_color (const Color & color);
   virtual void   set_fill_color (const Color & color);
   virtual void   set_clear_color (const Color & color);

   virtual void   set_blend_mode_normal ();
   virtual void   set_blend_mode_multiply ();
   virtual void   set_blend_mode_screen ();
   virtual void   set_blend_mode_replace ();
   virtual void   set_blend_mode_add ();
   virtual void   set_blend_mode_max ();
   virtual void   set_blend_mode_min ();

   virtual void   set_fragment_program (FragmentProgramProc proc);
   virtual void   set_fragment_local_vars (float var_0, float var_1, float var_2, float var_3);

   virtual void   clip (const opa::Rect & rect);
   virtual opa::Rect
                  get_clip_bounding_box ();
   virtual void   concat_translate (float tx, float ty);
   
   virtual void   save_graphic_state ();
   virtual void   restore_graphic_state ();
   
   // Vectorial Drawing
   virtual void   fill (const opa::Path & path);
   virtual void   fill (const opa::Rect & rect);
   virtual void   fill (const opa::Quad & vertices);

   virtual void   stroke (const opa::Path & path);
   
   virtual void   clear ();
   
   // Raster Drawing
   virtual void   draw (const Image & image, const opa::Point & point);
   virtual void   draw (const Image & image, const opa::Rect & rect);
   virtual void   draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords);
   virtual void   draw (const SprungImage & sprung_image, const opa::Rect & rect);

   virtual void   draw (const Image & mask, const opa::Point & point, const Color & color);
   virtual void   draw (const Image & mask, const opa::Rect & rect, const Color & color);
   virtual void   draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color);
   virtual void   draw (const SprungImage & sprung_mask, const opa::Rect & rect, const Color & color);
   
   /*virtual void draw (const LabelString & label_str, const opa::Point & point, float flush_factor = 0.f);*/
         
   // debug
   const GraphicState & 
                  use_stack_top () const;

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual Engine &
                  do_use_engine () = 0;

   virtual void   do_update_context (const GraphicState & graphic_state) = 0;
   virtual const opa::Size &
                  do_get_context_size () = 0;
   
   virtual void   do_fill (const opa::Path & path, const Color & color) = 0;
   virtual void   do_fill (const opa::Rect & rect, const Color & color) = 0;
   virtual void   do_fill (const opa::Quad & vertices, const Color & color) = 0;

   virtual void   do_stroke (const opa::Path & path, const Color & color) = 0;
   
   virtual void   do_draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha) = 0;
   virtual void   do_draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color) = 0;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class T>
   void           draw (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color);
   template <class T>
   void           draw_hardware_repeat (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color);
   template <class T>
   void           draw_software_repeat (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color);
   template <class T>
   void           draw (const GraphicState & graphic_state, const SprungImage & image_or_mask, const opa::Rect & rect, const T & alpha_or_color);
   
   static float   _min_alpha;
   static float   _max_alpha;
   
   GraphicStateStack
                  _graphic_state_stack;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  GraphicContextBase (const GraphicContextBase &other);
   GraphicContextBase &
                  operator = (const GraphicContextBase &other);
   bool           operator == (const GraphicContextBase &other);
   bool           operator != (const GraphicContextBase &other);

}; // class GraphicContextBase



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/GraphicContextBase.hpp"



#endif   // ohm_opal_GraphicContextBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
