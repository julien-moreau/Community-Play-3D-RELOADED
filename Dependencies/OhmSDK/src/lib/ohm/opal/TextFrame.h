/*****************************************************************************

        TextFrame.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 33891

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



#if ! defined (ohm_opal_TextFrame_HEADER_INCLUDED)
#define  ohm_opal_TextFrame_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace ohm
{
namespace opa
{
class TextBundle;
}
namespace opak
{
class TextField;
}



namespace opal
{



class TextFrame
:  public Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TextFrame (opa::TextBundle & text_bundle, opak::TextField & text_field);
   virtual        ~TextFrame ();

   void           push_state_font (const Font & font);
   void           clear_state_font ();
   void           set_state_color (const Color & color);
   
   // delegates
   void           mouse_button_pressed (Event & event);
   void           mouse_button_released (Event & event);
   void           mouse_dragged (Event & event);
   void           mouse_button_pressed_twice (Event & event);
   void           mouse_button_released_twice (Event & event);
   
   void           focus_key_pressed (Event & event);
   void           focus_key_repeated (Event & event);
   void           focus_key_released (Event & event);
   void           focus_characters_typed (Event & event);

   void           user_input_lost (Event & event);
   
   // Frame
   virtual void   keyboard_focus_lost (const Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           begin_edit ();
   void           end_edit ();
   
   void           scripture_changed ();
   void           update_typesetter ();
   
   void           erase_between_carets ();

   void           fix_caret ();
   void           animate_caret ();

   void           focus_lost ();
   void           focus_gain ();
   
   opa::TextBundle &
                  _bundle;
   opak::TextField &
                  _text_field;
                  
   EventDispatcherStandard <TextFrame>
                  _dispatcher;
                  
   bool           _editing_flag;
   
   Scripture      _scripture;
   std::auto_ptr <Typesetter>
                  _typesetter_aptr;
   bool           _typesetter_need_layout_flag;
   size_t         _first_pos;
   size_t         _second_pos;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TextFrame ();
                  TextFrame (const TextFrame &other);
   TextFrame &    operator = (const TextFrame &other);
   bool           operator == (const TextFrame &other);
   bool           operator != (const TextFrame &other);

}; // class TextFrame



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_TextFrame_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
