/*****************************************************************************

        BitmapContext.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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



#if ! defined (ohm_opal_BitmapContext_HEADER_INCLUDED)
#define  ohm_opal_BitmapContext_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/GraphicStateStack.h"
#include "ohm/opal/PixArgb.h"



namespace ohm
{
namespace opal
{



class Bitmap;

class BitmapContext
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BitmapContext (Bitmap & dst);
   virtual        ~BitmapContext ();
   
   void           set_alpha (float alpha);
   void           set_fill_color (const Color & color);
   void           set_clear_color (const Color & color);
   
   void           set_blend_mode_normal ();
   void           set_blend_mode_multiply ();
   void           set_blend_mode_screen ();
   void           set_blend_mode_replace ();
   void           set_blend_mode_add ();
   void           set_blend_mode_max ();
   void           set_blend_mode_min ();
   
   void           clip (const opa::Rect & rect);
   
   void           save_graphic_state ();
   void           restore_graphic_state ();
   
   void           fill (const opa::Rect & rect);
   
   void           clear ();
   
   void           draw (const Bitmap & bitmap, const opa::Point & point = opa::Point::_zero);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum BlendEquation
   {
                  BlendEquation_ADD = 0,
                  BlendEquation_MAX,
                  BlendEquation_MIN,
   };
   
   enum BlendFunc
   {
                  BlendFunc_ONE_ONE_MINUS_SRC_ALPHA = 0,
                  BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA,
                  BlendFunc_ONE_MINUS_DST_COLOR_ONE,
                  BlendFunc_ONE_ZERO,
                  BlendFunc_ONE_ONE,
   };

   void           do_update_context (const GraphicState & graphic_state);
   void           do_fill (size_t x, size_t y, size_t width, size_t height, const Color & color);
   void           do_draw (size_t dst_x, size_t dst_y, size_t width, size_t height, const Bitmap & bitmap, size_t src_x, size_t src_y);
   void           blend (PixArgb & dst, const PixArgb & src);
   
   static float   _min_alpha;
   static float   _max_alpha;

   Bitmap &       _dst;
   
   GraphicStateStack
                  _graphic_state_stack;

   BlendEquation  _cur_blend_equation;
   BlendFunc      _cur_blend_func;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BitmapContext ();
                  BitmapContext (const BitmapContext &other);
   BitmapContext &operator = (const BitmapContext &other);
   bool           operator == (const BitmapContext &other);
   bool           operator != (const BitmapContext &other);

}; // class BitmapContext



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_BitmapContext_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
