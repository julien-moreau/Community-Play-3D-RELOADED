/*****************************************************************************

        FrameTextField.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if ! defined (ohm_opa_FrameTextField_HEADER_INCLUDED)
#define  ohm_opa_FrameTextField_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/CursorLink.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/Key.h"
#include "ohm/opal/MenuStdIdLink.h"
#include "ohm/opal/base/EventDispatcherStandard.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opal
{
class Scripture;
}



namespace opa
{



class FrameTextField
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef txt::unicode::Char (*CharProc) (txt::unicode::Char c);
   
   class Config
   {
   public:
                  Config ();
      bool        _commit_on_outside_click_flag;
      bool        _commit_on_tab_key_flag;
      bool        _end_edit_on_commit_flag;
      bool        _edit_on_double_click_flag;
      bool        _allow_copy_to_clipboard_flag;
   };
   
   enum Mode
   {
                  Mode_FIXED = 0,
                  Mode_FIXED_WIDTH,
   };
   
   class Event
   {
   public:

      enum Type
      {
                  Type_BEGIN = 0,
                  Type_COMMIT,
                  Type_REVERT,
                  Type_END,
                  Type_SCRIPTURE_CHANGED,
                  Type_CARETS_POSITIONS_CHANGED,
                  Type_WIDTH_CHANGED,
                  Type_HEIGHT_CHANGED,

                  Type_ENTERED,
                  Type_EXITED,
      };
      
                  Event (FrameTextField & frame);
      
      FrameTextField &
                  _frame;
      Type        _type;
      float       _height;
      size_t      _first_caret_pos;
      size_t      _second_caret_pos;
      opal::Key   _key; // revert or commit
   };
   
                  FrameTextField (opal::Scripture & scripture, opal::Scripture * processed_scripture_ptr = 0);
   virtual        ~FrameTextField ();

   template <class T, void (T::*text_edit) (Event &)>
   void           bind_edit (T & receiver);
   
   void           add_key_bind_commit (const opal::Key & key);
   void           add_key_bind_revert (const opal::Key & key);
   
   void           set_mode (Mode mode);
   
   const Config & get_config () const;
   void           set_config (const Config & config);
   
   float          get_height () const;
   size_t         get_first_caret_pos () const;
   size_t         get_second_caret_pos () const;
   
   void           set (const std::string & str);
   
   void           set_char_proc (CharProc char_proc);

   void           select_all ();
   void           deselect_all ();
   
   void           start_edit ();
   void           cancel_edit ();

   void           handle_backspace ();
   void           handle_suppr ();
   void           handle_left_move ();
   void           handle_right_move ();
   void           handle_home_move ();
   void           handle_end_move ();
   void           handle_left_select ();
   void           handle_right_select ();
   void           handle_home_select ();
   void           handle_end_select ();
   void           handle_characters (const std::string & str);

   // delegates
   void           focus_key_pressed (opal::Event & event);
   void           focus_key_released (opal::Event & event);
   void           focus_characters_typed (opal::Event & event);
   void           catch_focus (opal::Event & event);
   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);
   void           mouse_button_pressed_twice (opal::Event & event);
   void           mouse_button_released_twice (opal::Event & event);
   void           mouse_dragged_twice (opal::Event & event);
   void           mouse_moved (opal::Event & event);
   void           mouse_cursor (opal::Event & event);
   void           user_input_lost (opal::Event & event);
   
   // opal::Frame
   virtual void   mouse_entered (opal::Event & event);
   virtual void   mouse_exited (opal::Event & event);
   virtual void   mouse_target_lost (const opal::Event & event);
   virtual void   keyboard_focus_lost (const opal::Event & event);
   
   // menu delegates
   void           req_menu_cut (opal::MenuEvent & event);
   void           req_menu_copy (opal::MenuEvent & event);
   void           req_menu_paste (opal::MenuEvent & event);
   void           req_menu_select_all (opal::MenuEvent & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <opal::Key, bool> KeyFlagMap;

   void           begin_edit ();
   void           end_edit ();
   
   void           focus_lost ();
   void           focus_gain ();

   void           scripture_changed ();
   void           carets_positions_changed ();
   void           update_typesetter ();

   void           erase_between_carets ();

   opal::Scripture &
                  _scripture;
   opal::Scripture *
                  _processed_scripture_ptr;

   opal::EventDispatcherStandard <FrameTextField>
                  _dispatcher;
   
   Mode           _mode;
   Config         _config;
                                                
   bool           _editing_flag;
   std::auto_ptr <opal::Typesetter>
                  _typesetter_aptr;
   bool           _typesetter_need_layout_flag;
   float          _height;
   size_t         _first_pos;
   size_t         _second_pos;
   
   bool           _tracking_flag;
   
   KeyFlagMap     _key_map_flag;
   
   CharProc       _char_proc;
   
   util::Delegate _delegate;
   
   opal::MenuStdIdLink
                  _menu_link_cut;
   opal::MenuStdIdLink
                  _menu_link_copy;
   opal::MenuStdIdLink
                  _menu_link_paste;
   opal::MenuStdIdLink
                  _menu_link_select_all;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameTextField ();
                  FrameTextField (const FrameTextField &other);
   FrameTextField &  
                  operator = (const FrameTextField &other);
   bool           operator == (const FrameTextField &other) const;
   bool           operator != (const FrameTextField &other) const;

}; // class FrameTextField



}  // namespace opa
}  // namespace ohm



#include "ohm/opa/frame/FrameTextField.hpp"



#endif   // ohm_opa_FrameTextField_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

