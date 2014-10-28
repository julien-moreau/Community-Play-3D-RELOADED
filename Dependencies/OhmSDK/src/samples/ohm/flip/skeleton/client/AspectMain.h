/*****************************************************************************

        AspectMain.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70465

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



#if ! defined (client_AspectMain_HEADER_INCLUDED)
#define  client_AspectMain_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/ColorLab.h"
#include "ohm/opal/Gradient.h"
#include "ohm/opal/SprungImage.h"

#include "ohm/opak/base/AspectColorProcessorBase.h"



namespace client
{



class AspectMain
:  public ohm::opak::AspectKeyedBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Selection
   {
                  Selection_OFF = 0,
                  Selection_ON,
                  Selection_OTHER,
                  Selection_MIXED,
                  Selection_CONFLICT,
                  
                  Selection_NBR_ELT
   };
   
   enum ColorScheme
   {
                  ColorScheme_GRAY = 0,
                  ColorScheme_LIGHT,
                  ColorScheme_STRONG,
                  ColorScheme_OTHER,
                  ColorScheme_CONFLICT,
                  ColorScheme_RED,
                  
                  ColorScheme_NBR_ELT
   };
   
                  AspectMain ();
   virtual        ~AspectMain ();
   
   // ohm::opak::AspectBase
   virtual void   make (ohm::opak::GraphicContext & context);

   static int     get_selection (bool selected_flag, bool ext_selected_flag, bool conflict_flag);
   
   const ohm::opal::Color &
                  get_color (ColorScheme color_scheme, float norm = 1.f) const;
   const ohm::opal::Color &
                  get_color_gray (float norm) const;
   void           colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme) const;
   void           colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme, float norm) const;
   void           colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme, float norm_min, float norm_max) const;
   void           process_premul (ohm::opal::Bitmap & bitmap) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // ohm::opak::AspectBase
   virtual void   do_init (ohm::opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           make_color_schemes (const ohm::opak::AspectColorProcessorBase * aspect_ptr);
   void           make_color_scheme (ColorScheme color_scheme, const ohm::opal::Gradient <ohm::opal::ColorLab> & gradient, const ohm::opak::AspectColorProcessorBase * aspect_ptr);
   
   ohm::opak::AspectColorProcessorBase::Key
                  _aspect_color_proc_key;
   
   const ohm::opak::AspectColorProcessorBase *
                  _aspect_proc_ptr;
   ohm::opal::ColorEngine::ColorGradient
                  _color_gradient_arr [ColorScheme_NBR_ELT];
   ohm::opal::ColorEngine::PixArgbGradient
                  _pix_argb_gradient_arr [ColorScheme_NBR_ELT];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AspectMain (const AspectMain &other);
   AspectMain &   operator = (const AspectMain &other);
   bool           operator == (const AspectMain &other);
   bool           operator != (const AspectMain &other);

}; // class AspectMain



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/AspectMain.hpp"



#endif   // client_AspectMain_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
