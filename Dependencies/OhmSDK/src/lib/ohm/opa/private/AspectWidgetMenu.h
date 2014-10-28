/*****************************************************************************

        AspectWidgetMenu.h
        Copyright (c) 2010 Ohm Force

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



#if ! defined (ohm_opa_AspectWidgetMenu_HEADER_INCLUDED)
#define  ohm_opa_AspectWidgetMenu_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/SprungImage.h"

#include "ohm/opak/base/AspectKeyedBase.h"



namespace ohm
{
namespace opa
{



class AspectWidgetMenu
:  public opak::AspectBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Selection
   {
                  Selection_OFF = 0,
                  Selection_ON,

                  TOTAL_Selection_NBR
   };
   enum Enable
   {
                  Enable_OFF = 0,
                  Enable_ON,

                  TOTAL_Enable_NBR
   };
   
                  AspectWidgetMenu ();
   virtual        ~AspectWidgetMenu ();
   
   // opak::AspectBase
   virtual void   make (opak::GraphicContext & context);
   
   // root
   opal::SprungImage
                  _back;
   opal::Image    _arrow_left;
   opal::Image    _arrow_right;
   
   // item
   opal::SprungImage
                  _item_back;
   opal::Image    _item_mark_check_arr [TOTAL_Selection_NBR];
   opal::Image    _item_mark_check_multi_arr [TOTAL_Selection_NBR];
   opal::Image    _item_sub_arrow_arr [TOTAL_Selection_NBR];
   opal::Color    _item_text_arr [TOTAL_Selection_NBR][TOTAL_Enable_NBR];



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // opak::AspectBase
   virtual void   do_init (opak::GraphicContext & context);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class BaseElements
   {
   public:
      opal::Bitmap
                  _arrow_left;
      opal::Bitmap
                  _arrow_right;
      opal::Bitmap
                  _item_back;
      opal::Bitmap
                  _item_mark_check;
      opal::Bitmap
                  _item_mark_check_multi;
      opal::Bitmap
                  _item_sub_arrow;
   };
   
   static void    load_base_elements ();
   
   opak::AspectKeyedBase::Key
                  _aspect_widget_key;
   
   static BaseElements
                  _be;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AspectWidgetMenu (const AspectWidgetMenu &other);
   AspectWidgetMenu &         operator = (const AspectWidgetMenu &other);
   bool           operator == (const AspectWidgetMenu &other);
   bool           operator != (const AspectWidgetMenu &other);

}; // class AspectWidgetMenu



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/private/AspectWidgetMenu.hpp"



#endif   // ohm_opa_AspectWidgetMenu_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
