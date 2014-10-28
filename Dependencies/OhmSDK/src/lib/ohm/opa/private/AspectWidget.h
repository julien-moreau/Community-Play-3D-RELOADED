/*****************************************************************************

        AspectWidget.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68114

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



#if ! defined (ohm_opa_AspectWidget_HEADER_INCLUDED)
#define  ohm_opa_AspectWidget_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/private/AspectWidgetCheckBox.h"
#include "ohm/opa/private/AspectWidgetComboBox.h"
#include "ohm/opa/private/AspectWidgetDialog.h"
#include "ohm/opa/private/AspectWidgetFader.h"
#include "ohm/opa/private/AspectWidgetLabel.h"
#include "ohm/opa/private/AspectWidgetList.h"
#include "ohm/opa/private/AspectWidgetMatrixBool.h"
#include "ohm/opa/private/AspectWidgetMenu.h"
#include "ohm/opa/private/AspectWidgetMusicTime.h"
#include "ohm/opa/private/AspectWidgetProgressBar.h"
#include "ohm/opa/private/AspectWidgetPushButton.h"
#include "ohm/opa/private/AspectWidgetPushButtonSmall.h"
#include "ohm/opa/private/AspectWidgetPushButtonTiny.h"
#include "ohm/opa/private/AspectWidgetScrollBar.h"
#include "ohm/opa/private/AspectWidgetSection.h"
#include "ohm/opa/private/AspectWidgetSpin.h"

#include "ohm/opal/Gradient.h"
#include "ohm/opak/base/AspectColorProcessorBase.h"



namespace ohm
{
namespace opa
{



class AspectWidget
:  public opak::AspectKeyedBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum ColorScheme
   {
                  ColorScheme_GRAY = 0,
                  ColorScheme_STRONG,
                  
                  ColorScheme_NBR_ELT
   };
   

                  AspectWidget ();
   virtual        ~AspectWidget ();
   
   // opak::AspectBase
   virtual void   make (opak::GraphicContext & context);

   const opal::Color &
                  get_color (ColorScheme color_scheme, float norm = 1.f) const;
   const opal::Color &
                  get_color_gray (float norm) const;
   const opal::Color &
                  get_color_strong (float norm) const;
   void           colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme) const;
   void           colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme, float norm) const;
   void           colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme, float norm_min, float norm_max) const;
   void           process_premul (opal::Bitmap & bitmap) const;
   
   bool           _cur_enabled_flag;
   
   AspectWidgetCheckBox
                  _check_box;
   AspectWidgetComboBox
                  _combo_box;
   AspectWidgetDialog
                  _dialog;
   AspectWidgetFader
                  _fader;
   AspectWidgetLabel
                  _label;
   AspectWidgetList
                  _list;
   AspectWidgetMatrixBool
                  _matrix_bool;
   AspectWidgetMenu
                  _menu;
   AspectWidgetMusicTime
                  _music_time;
   AspectWidgetProgressBar
                  _progress_bar;
   AspectWidgetPushButton
                  _push_button;
   AspectWidgetPushButtonSmall
                  _push_button_small;
   AspectWidgetPushButtonTiny
                  _push_button_tiny;
   AspectWidgetScrollBar
                  _scrollbar;
   AspectWidgetSection
                  _section;
   AspectWidgetSpin
                  _spin;
   
   opal::SprungImage
                  _tool_tip_back;
   opal::Image    _tool_tip_back2;
   opal::SprungImage
                  _tip_left;
   opal::SprungImage
                  _tip_up_left;
   opal::SprungImage
                  _tip_up_right;
   opal::SprungImage
                  _tip_right;
   opal::SprungImage
                  _tip_down_left;
   opal::SprungImage
                  _tip_down_right;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // opak::AspectBase
   virtual void   do_init (opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           make_color_schemes (const opak::AspectColorProcessorBase * aspect_ptr);
   void           make_color_scheme (ColorScheme color_scheme, const opal::Gradient <opal::ColorLab> & gradient, const opak::AspectColorProcessorBase * aspect_ptr);

   opak::AspectColorProcessorBase::Key
                  _aspect_color_proc_key;
   
   const opak::AspectColorProcessorBase *
                  _aspect_proc_ptr;
   opal::ColorEngine::ColorGradient
                  _color_gradient_arr [ColorScheme_NBR_ELT];
   opal::ColorEngine::PixArgbGradient
                  _pix_argb_gradient_arr [ColorScheme_NBR_ELT];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AspectWidget (const AspectWidget &other);
   AspectWidget &       operator = (const AspectWidget &other);
   bool           operator == (const AspectWidget &other);
   bool           operator != (const AspectWidget &other);

}; // class AspectWidget



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/private/AspectWidget.hpp"



#endif   // ohm_opa_AspectWidget_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
