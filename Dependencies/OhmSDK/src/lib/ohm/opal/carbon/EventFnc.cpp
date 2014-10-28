/*****************************************************************************

        EventFnc.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70452

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/carbon/EventFnc.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/View.h"
#include "ohm/sys/SysLogger.h"
#include "ohm/opal/Key.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

EventFnc::~EventFnc ()
{
}



/*
==============================================================================
Name : use_instance
==============================================================================
*/

EventFnc &  EventFnc::use_instance ()
{
   static EventFnc instance;
   
   return instance;
}
   


/*
==============================================================================
Name : propagate_event_to_omfo_view
Description :
   Propagate 'event_ref' to all views in window that are part of the opal
   system with the following rules :
   - kEventMouseDown is only sent to one view, the one that is elligible by
     the system
   - 

   kEventMouseDown & kEventMouseMoved are only sent when the mouse is
   over the window structure (including decoration)
   kEventMouseDragged & kEventMouseUp are sent even when the mouse is
   outside the window structure.
==============================================================================
*/

OSStatus EventFnc::propagate_event_to_omfo_view (WindowRef window_ref, EventRef event_ref)
{
   HIViewRef view_ref = 0;
   
   HIViewGetViewForMouseEvent (HIViewGetRoot (window_ref), event_ref, &view_ref);
   
   OSStatus ret_val = eventNotHandledErr;
   
   if (view_ref != 0)
   {
      ViewImpl * view_impl_ptr = ViewImpl::get (view_ref);
      
      if (view_impl_ptr != 0)
      {
         ret_val = SendEventToEventTargetWithOptions (
            event_ref,
            HIObjectGetEventTarget ((HIObjectRef) view_ref),
            kEventTargetDontPropagate
         );
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_content_view_ref
==============================================================================
*/

HIViewRef   EventFnc::get_content_view_ref (WindowRef window_ref)
{
   HIViewRef root_ref = HIViewGetRoot (window_ref);
   assert (root_ref != 0);
   
   HIViewRef content_ref = 0;
   OSStatus err = HIViewFindByID (root_ref, kHIViewWindowContentID, &content_ref);
   assert (err == 0);
   assert (content_ref != 0);
   
   return content_ref;
}



/*
==============================================================================
Name : add_window_mouse_location
Description :
   Add the kEventParamWindowMouseLocation parameter if it is not already
   present.
==============================================================================
*/

/*void   EventFnc::add_window_mouse_location (WindowRef window_ref, EventRef event_ref)
{
   bool param_exist_flag = false;
   {
      opa::Point location;
      OSStatus err = GetEventParameter (
         event_ref,
         kEventParamWindowMouseLocation,
         typeQDPoint,
         0,
         sizeof (opa::Point),
         0,
         &location
      );
      assert ((err == noErr) || (err == eventParameterNotFoundErr));

      if (err == noErr)
      {
         param_exist_flag = true;
      }
   }
   
   if (!param_exist_flag)
   {
      opa::Point location;
      GetEventParameter (
         event_ref,
         kEventParamMouseLocation,
         typeQDPoint,
         0,
         sizeof (opa::Point),
         0,
         &location
      );
      
      ::Rect bounds;
      GetWindowBounds(
         window_ref,
         kWindowStructureRgn,
         &bounds
      );
      
      location.x -= bounds.left;
      location.y -= bounds.top;
      
      SetEventParameter (
         event_ref,
         kEventParamWindowRef,
         typeWindowRef,
         sizeof (WindowRef),
         &window_ref
      );
      
      SetEventParameter (
         event_ref,
         kEventParamWindowMouseLocation,
         typeQDPoint,
         sizeof (opa::Point),
         &location
      );
   }
}*/



/*
==============================================================================
Name : get_qd_mouse_location
==============================================================================
*/

::Point  EventFnc::get_qd_mouse_location (EventRef event_ref)
{
   ::Point location;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamMouseLocation,
      typeQDPoint,
      0,
      sizeof (::Point),
      0,
      &location
   );
   assert (err == 0);
   
   return location;
}



/*
==============================================================================
Name : get_mouse_location
==============================================================================
*/

opa::Point  EventFnc::get_mouse_location (EventRef event_ref)
{
   HIPoint location;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamMouseLocation,
      typeHIPoint,
      0,
      sizeof (HIPoint),
      0,
      &location
   );
   assert (err == 0);
   
   opa::Point ret_val;
   ret_val.from_CGPoint (location);
   
   return ret_val;
}



/*
==============================================================================
Name : get_mouse_window_location
==============================================================================
*/

opa::Point  EventFnc::get_mouse_window_location (EventRef event_ref)
{
   HIPoint location;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamWindowMouseLocation,
      typeHIPoint,
      0,
      sizeof (HIPoint),
      0,
      &location
   );
   assert (err == 0);
   
   opa::Point ret_val;
   ret_val.from_CGPoint (location);
   
   return ret_val;
}



/*
==============================================================================
Name : get_mouse_view_location
==============================================================================
*/

opa::Point  EventFnc::get_mouse_view_location (HIViewRef view_ref, EventRef event_ref)
{
   HIPoint location;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamWindowMouseLocation,
      typeHIPoint,
      0,
      sizeof (HIPoint),
      0,
      &location
   );
   assert ((err == 0) || (err == eventParameterNotFoundErr));
   
   if (err == eventParameterNotFoundErr)
   {
      err = GetEventParameter (
         event_ref,
         kEventParamMouseLocation,
         typeHIPoint,
         0,
         sizeof (HIPoint),
         0,
         &location
      );
      assert (err == 0);
      
      WindowRef window_ref = HIViewGetWindow (view_ref);
      assert (window_ref != 0);
      
      ::Rect bounds;
      GetWindowBounds(
         window_ref,
         kWindowStructureRgn,
         &bounds
      );
      
      location.x -= bounds.left;
      location.y -= bounds.top;
   }
   
   HIViewConvertPoint (&location, 0, view_ref);
   
   opa::Point ret_val;
   ret_val.from_CGPoint (location);
   
   return ret_val;
}



/*
==============================================================================
Name : get_mouse_view_location
==============================================================================
*/

opa::Point  EventFnc::get_mouse_view_location (HIViewRef view_ref, const opa::Point & global_location)
{
   WindowRef window_ref = HIViewGetWindow (view_ref);
   assert (window_ref != 0);

   ::Rect bounds;
      GetWindowBounds(
      window_ref,
      kWindowStructureRgn,
      &bounds
   );
   
   CGPoint location;
   location.x = global_location._x;
   location.y = global_location._y;

   location.x -= bounds.left;
   location.y -= bounds.top;
   
   HIViewConvertPoint (&location, 0, view_ref);
   
   opa::Point ret_val;
   ret_val.from_CGPoint (location);
   
   return ret_val;
}


/*
==============================================================================
Name : get_mouse_delta
==============================================================================
*/

opa::Point  EventFnc::get_mouse_delta (EventRef event_ref)
{
   HIPoint delta = {0, 0};
   GetEventParameter (
      event_ref,
      kEventParamMouseDelta,
      typeHIPoint,
      0,
      sizeof (HIPoint),
      0,
      &delta
   );
   
   opa::Point ret_val (delta.x, delta.y);
   
   return ret_val;
}



/*
==============================================================================
Name : has_key_modifiers_shift
==============================================================================
*/

bool  EventFnc::has_key_modifiers_shift (EventRef event_ref)
{
   ::UInt32 key_modifiers;
   GetEventParameter (
      event_ref,
      kEventParamKeyModifiers,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      &key_modifiers
   );
   
   bool ret_val = key_modifiers & shiftKey;
   
   return ret_val;
}



/*
==============================================================================
Name : has_key_modifiers_alt
==============================================================================
*/

bool  EventFnc::has_key_modifiers_alt (EventRef event_ref)
{
   ::UInt32 key_modifiers;
   GetEventParameter (
      event_ref,
      kEventParamKeyModifiers,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      &key_modifiers
   );
   
   bool ret_val = key_modifiers & optionKey;
   
   return ret_val;
}



/*
==============================================================================
Name : has_key_modifiers_command
==============================================================================
*/

bool  EventFnc::has_key_modifiers_command (EventRef event_ref)
{
   ::UInt32 key_modifiers;
   GetEventParameter (
      event_ref,
      kEventParamKeyModifiers,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      &key_modifiers
   );
   
   bool ret_val = key_modifiers & cmdKey;
   
   return ret_val;
}



/*
==============================================================================
Name : get_mouse_button
==============================================================================
*/

int   EventFnc::get_mouse_button (EventRef event_ref)
{
   ::UInt16 mouse_button;
   GetEventParameter (
      event_ref,
      kEventParamMouseButton,
      typeMouseButton,
      0,
      sizeof (::UInt16),
      0,
      &mouse_button
   );
   
   int ret_val = int (mouse_button);
   
   if (mouse_button == 1)
   {
      ::UInt32 key_modifiers;
      GetEventParameter (
         event_ref,
         kEventParamKeyModifiers,
         typeUInt32,
         0,
         sizeof (::UInt32),
         0,
         &key_modifiers
      );
      
      if (key_modifiers & controlKey)
      {
         ret_val = 2;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_click_count
==============================================================================
*/

int   EventFnc::get_click_count (EventRef event_ref)
{
   ::UInt32 click_count;
   GetEventParameter (
      event_ref,
      kEventParamClickCount,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      &click_count
   );
   
   int ret_val = int (click_count);
   
   return ret_val;
}



/*
==============================================================================
Name : get_wheel_delta
==============================================================================
*/

opa::Point  EventFnc::get_wheel_delta (EventRef event_ref)
{
   ::UInt16 wheel_axis;
   GetEventParameter (
      event_ref,
      kEventParamMouseWheelAxis,
      typeMouseWheelAxis,
      0,
      sizeof (::UInt16),
      0,
      &wheel_axis
   );
   
   ::SInt32 wheel_delta;
   GetEventParameter (
      event_ref,
      kEventParamMouseWheelDelta,
      typeSInt32,
      0,
      sizeof (::SInt32),
      0,
      &wheel_delta
   );
   
   opa::Point ret_val (0, 0);
   
   if (wheel_axis == 0)
   {
      // left or right
      ret_val._x = wheel_delta;
   }
   else if (wheel_axis == 1)
   {
      // up or down
      ret_val._y = wheel_delta;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : text_input_event_is_modifiers_changed
==============================================================================
*/

bool  EventFnc::text_input_event_is_modifiers_changed (EventRef event_ref)
{
   EventRef raw_keyboard_event_ref = 0;
   GetEventParameter (
      event_ref,
      kEventParamTextInputSendKeyboardEvent,
      typeEventRef,
      0,
      sizeof (EventRef),
      0,
      &raw_keyboard_event_ref
   );
   
   bool ret_val = false;
   
   if (raw_keyboard_event_ref != 0)
   {
      ret_val = (GetEventKind (raw_keyboard_event_ref) == kEventRawKeyModifiersChanged);
   }
   
   return ret_val;
}



/*
==============================================================================
Name: get_ascii_for_scan_code
==============================================================================
*/

char EventFnc::get_ascii_for_scan_code (::UInt32 scan_code)
{
   OSStatus err = 0;
   
   ::TISInputSourceRef input_src_ref = 0;
   
   if (err == 0)
   {
      input_src_ref = ::TISCopyCurrentASCIICapableKeyboardInputSource ();
      
      if (input_src_ref == 0)
      {
         err = -1;
      }
   }


   CFDataRef uchrDataRef = 0;
   
   if (err == 0)
   {
      uchrDataRef = CFDataRef (
         ::TISGetInputSourceProperty (input_src_ref, kTISPropertyUnicodeKeyLayoutData)
      );
      
      // happens for TIM Romaji

      if (uchrDataRef == 0)
      {
         err = -1;
      }
   }
   
   const ::UniCharCount max_unicode_len = 32;

   ::UniChar unicode_str_0 [max_unicode_len];
   unicode_str_0 [0] = 0;

   ::UniCharCount out_unicode_len = 0;
   
   if (err == 0)
   {
      assert (uchrDataRef != 0);
      
      const ::UCKeyboardLayout * uchr_layout_ptr
         = (const ::UCKeyboardLayout *) (CFDataGetBytePtr (uchrDataRef));
      assert (uchr_layout_ptr != 0);

      ::UInt32 dead_key_state = 0;

      err = ::UCKeyTranslate(
         uchr_layout_ptr, 
         scan_code, 
         kUCKeyActionDisplay,
         0, // modifiers >> 8, // no modifiers
         ::UInt32 (LMGetKbdType ()), 
         kUCKeyTranslateNoDeadKeysMask, 
         &dead_key_state, 
         max_unicode_len, 
         &out_unicode_len, 
         &unicode_str_0 [0]
      );
   }
   
   char ret_val = 0;
   
   if (err == 0)
   {
      ::CFStringRef str_ref = ::CFStringCreateWithCharacters (0, unicode_str_0, out_unicode_len);
      const int max_size = ::CFStringGetLength (str_ref) * 6 + 1;
      
      std::vector <char> buf;
      buf.resize (max_size);
      
      Boolean ok_flag = ::CFStringGetCString (str_ref, &buf [0], max_size, kCFStringEncodingUTF8);
      assert (ok_flag);
      
      CFRelease (str_ref);
      
      ret_val = buf [0];
   }
   
   return ret_val;
}



void  Event_convert_unicode_to_utf8 (std::string & out, const ::UInt16 * unicode_text_ptr, long unicode_char_count)
{
   ::CFStringRef  str_ref = ::CFStringCreateWithCharacters (0, unicode_text_ptr, unicode_char_count);
   const int      max_size = ::CFStringGetLength (str_ref) * 6 + 1;
   
   std::vector <char> buf;
   buf.resize (max_size);
   
   Boolean        ok_flag = ::CFStringGetCString (str_ref, &buf [0], max_size, kCFStringEncodingUTF8);
   assert (ok_flag);
   
   out.assign (&buf [0]);
   
   CFRelease (str_ref);
}



void  EventFnc::print_key (EventRef event_ref)
{
   if (GetEventKind (event_ref) == kEventRawKeyModifiersChanged)
   {
      return;
   }
   
   char           mac_char = 0;
   ::UInt32       scan_code = 0;
   ::UInt32       modifiers = 0;
   std::string    utf8;
   std::string    raw_utf8;
   std::string    raw_utf8_2;
   
   // mac char
   
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamKeyMacCharCodes,
      typeChar,
      0,
      sizeof (char),
      0,
      & mac_char
   );
   assert (err == 0);
   
   // scan code
   
   err = GetEventParameter (
      event_ref, 
      kEventParamKeyCode, 
      typeUInt32, 
      0, 
      sizeof(::UInt32), 
      0, 
      & scan_code
   );
   assert (err == 0);

   err = GetEventParameter (
      event_ref,
      kEventParamKeyModifiers,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      & modifiers
   );
   assert (err == 0);

   // unicode char (eventually none)
   {
      ::UniChar      unicode_text [32];
      ::ByteCount    out_byte_cnt = 0;

      err = GetEventParameter (
         event_ref, 
         kEventParamKeyUnicodes, 
         typeUnicodeText, 
         0, 
         sizeof(unicode_text), 
         & out_byte_cnt, 
         & unicode_text
      );
      assert (err == 0);
//    assert (out_byte_cnt <= 1);
      
      Event_convert_unicode_to_utf8 (utf8, unicode_text, out_byte_cnt / sizeof (unicode_text [0]));
   }
   
   // raw unicode char (without modifiers)
   
   {
      // Try to get the Raw Key Code (that is the unicode string for the pressed key without dead keys and modifiers:
      // deprecated in 10.5
      ::KeyboardLayoutRef  layout_ref;
      ::UCKeyboardLayout * uchr_layout_ptr = 0;
      ::UInt32             dead_key_state;
      const ::UniCharCount max_unicode_len = 32;
      ::UniCharCount       out_unicode_len = 0;
      ::UniChar            unicode_str_0 [max_unicode_len];

      // initialization
      err = ::KLGetCurrentKeyboardLayout (&layout_ref);
      assert (err == 0);
      err = ::KLGetKeyboardLayoutProperty (layout_ref, kKLuchrData, (const void **)(&uchr_layout_ptr));
      assert (err == 0);

      // if there is a 'uchr' for the current keyboard layout, 
      // use it
      if (uchr_layout_ptr != NULL)
      {
         err = UCKeyTranslate(
            uchr_layout_ptr, 
            ::UInt16 (scan_code), 
            kUCKeyActionDisplay,
            0, // modifier key state 
            ::UInt32 (LMGetKbdType ()), 
            0,
            & dead_key_state,
            max_unicode_len,
            & out_unicode_len, 
            & unicode_str_0 [0]
         );
         assert (err == 0);
         
         Event_convert_unicode_to_utf8 (raw_utf8, unicode_str_0, out_unicode_len);
      }
      else
      {
         printf ("**** no uchr  ***** ");
/*       // no 'uchr' resource, do something with 'KCHR'?
         // add your code here
         Ptr kchrKeyLayout;
         status = KLGetKeyboardLayoutProperty(layout_ref, kKLKCHRData, (const void **)(&kchrKeyLayout));
         if (kchrKeyLayout)
         {
            UInt32 state = 0;
            rawunicodetext = KeyTranslate(kchrKeyLayout, scan_code, &state);
         }
*/    }
   }

   // another method
   
#if 1
   {
      ::UInt32       keyboard_type = 0;
      const ::UniCharCount max_unicode_len = 32;
      ::UniCharCount out_unicode_len = 0;
      ::UniChar      unicode_str_0 [max_unicode_len];
      ::UInt32       dead_key_state = 0;
      const ::UCKeyboardLayout * 
                     uchr_layout_ptr = 0;
   
#if 0
      err = ::GetEventParameter(
         event_ref,
         kEventParamKeyboardType,
         typeUInt32, 
         0, 
         sizeof (::UInt32), 
         0, 
         & keyboard_type
      );
      assert (err == 0);
#else
      keyboard_type = ::UInt32 (LMGetKbdType ());
#endif

#if 0    
      // see http://developer.apple.com/mac/library/qa/qa2005/qa1446.html
      // deprecated in 10.5
      long     smv = ::GetScriptManagerVariable (smKeyScript);
      ::Handle uchrHandle = GetResource('uchr', ::GetScriptVariable(smv, smScriptKeys));
#else
      // Here what we do instead
      // see http://developer.apple.com/mac/library/documentation/TextFonts/Reference/TextInputSourcesReference/Reference/reference.html#//apple_ref/doc/uid/TP40006199-CH4-DontLinkElementID_2
//    ::TISInputSourceRef input_src_ref = ::TISCopyCurrentKeyboardInputSource ();
//    void * uchrHandle = ::TISGetInputSourceProperty (input_src_ref, kTISPropertyInputSourceLanguages);
      ::TISInputSourceRef input_src_ref = ::TISCopyCurrentASCIICapableKeyboardInputSource ();
      CFDataRef uchrDataRef = CFDataRef (::TISGetInputSourceProperty (input_src_ref, kTISPropertyUnicodeKeyLayoutData));
      assert (uchrDataRef != 0);
      uchr_layout_ptr = (const ::UCKeyboardLayout *) (CFDataGetBytePtr (uchrDataRef));
#endif
      err = ::UCKeyTranslate(
         uchr_layout_ptr, 
         scan_code, 
         kUCKeyActionDisplay,
         0, // modifiers >> 8, // no modifiers
         keyboard_type, 
         kUCKeyTranslateNoDeadKeysMask, 
         & dead_key_state, 
         max_unicode_len, 
         & out_unicode_len, 
         & unicode_str_0 [0]
      );
      assert (err == 0);

      Event_convert_unicode_to_utf8 (raw_utf8_2, unicode_str_0, out_unicode_len);
   }
#endif   
   
   printf ("scan - %lu\t0x%lx - ", scan_code, scan_code);
   printf ("macc - %d\t\'%c\' - ", mac_char, mac_char);
   printf ("utf8 - %s\t(%ld) - ", utf8.c_str (),utf8.size ());
   printf ("r--8 - %s\t(%ld) - ", raw_utf8.c_str (), raw_utf8.size ());
   printf ("r2-8 - %s\t(%ld) - ", raw_utf8_2.c_str (), raw_utf8_2.size ());
   printf ("\n");
}



/*
==============================================================================
Name : get_key_modifiers
==============================================================================
*/

archi::UInt32  EventFnc::get_key_modifiers (EventRef event_ref)
{
   ::UInt32 modifiers;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamKeyModifiers,
      typeUInt32,
      0,
      sizeof (::UInt32),
      0,
      &modifiers
   );
   assert (err == 0);
   
   return modifiers;
}



/*
==============================================================================
Name : get_characters
==============================================================================
*/

std::string EventFnc::get_characters (EventRef event_ref)
{
   UInt32 buf_size = 0;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamTextInputSendText,
      typeUTF8Text,
      0,
      0,
      &buf_size,
      0
   );
   assert (err == 0);
   
   std::string str;
   str.resize (buf_size);
   err = GetEventParameter (
      event_ref,
      kEventParamTextInputSendText,
      typeUTF8Text,
      0,
      buf_size,
      0,
      &str [0]
   );
   assert (err == 0);
   
   return str;
}



/*
==============================================================================
Name : get_control_part_code
==============================================================================
*/

ControlPartCode   EventFnc::get_control_part_code  (EventRef event_ref)
{
   ControlPartCode part_code;
   OSStatus err = GetEventParameter (
      event_ref,
      kEventParamControlPart,
      typeControlPartCode,
      0,
      sizeof (ControlPartCode),
      0,
      &part_code
   );
   assert (err == 0);
   
   return part_code;
}



/*
==============================================================================
Name : get_control_part_code
==============================================================================
*/

DragRef  EventFnc::get_drag_ref (EventRef event_ref)
{
   DragRef drag_ref = 0;
   
   OSStatus err =  GetEventParameter (
      event_ref,
      kEventParamDragRef,
      typeDragRef,
      0,
      sizeof (DragRef),
      0,
      &drag_ref
   );
   assert (err == 0);
   assert (drag_ref != 0);
   
   return drag_ref;
}



/*
==============================================================================
Name : attach_monitor
==============================================================================
*/

void  EventFnc::attach_monitor (EventTargetRef event_target_ref)
{
   attach_monitor (kEventClassMouse, event_target_ref);
   attach_monitor (kEventClassWindow, event_target_ref);
   attach_monitor (kEventClassKeyboard, event_target_ref);
   attach_monitor (kEventClassTextInput, event_target_ref);
   attach_monitor (kEventClassControl, event_target_ref);
   attach_monitor (kEventClassCommand, event_target_ref);
}



/*
==============================================================================
Name : attach_monitor
==============================================================================
*/

void  EventFnc::attach_monitor (OSType event_class, EventTargetRef event_target_ref)
{
   for (int i = 0 ; i < 9 ; ++i)
   {
      EventTypeSpec type = {event_class, i};
      
      OSStatus err = InstallEventHandler (
         event_target_ref,
         handle_event_proc,
         1,
         &type,
         0,
         0
      );
      assert (err == 0);
   }
}


/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

OSStatus EventFnc::handle_event_proc (EventHandlerCallRef, EventRef event_ref, void *)
{
   print_debug (event_ref);
   
   return eventNotHandledErr;
}




/*
==============================================================================
Name : print_debug
==============================================================================
*/

#define ADD_STR_CLASS(x) if (GetEventClass (event_ref) == x) str += ""#x""
#define ADD_STR_KIND(x) if (GetEventKind (event_ref) == x) str += ""#x""

void  EventFnc::print_debug (EventRef event_ref)
{
   std::string str;
   
   ADD_STR_CLASS (kEventClassMouse);
   ADD_STR_CLASS (kEventClassWindow);
   ADD_STR_CLASS (kEventClassKeyboard);
   ADD_STR_CLASS (kEventClassTextInput);
   ADD_STR_CLASS (kEventClassControl);
   ADD_STR_CLASS (kEventClassCommand);
   
   str += " ";
   
   switch (GetEventClass (event_ref))
   {
   case kEventClassMouse:
      ADD_STR_KIND (kEventMouseDown);
      ADD_STR_KIND (kEventMouseUp);
      ADD_STR_KIND (kEventMouseMoved);
      ADD_STR_KIND (kEventMouseDragged);
      ADD_STR_KIND (kEventMouseEntered);
      ADD_STR_KIND (kEventMouseExited);
      ADD_STR_KIND (kEventMouseWheelMoved);
      break;
   
   //kEventWindowGetClickActivation
   
   case kEventClassWindow:
      ADD_STR_KIND (kEventWindowActivated);
      ADD_STR_KIND (kEventWindowDeactivated);
      ADD_STR_KIND (kEventWindowBoundsChanging);
      ADD_STR_KIND (kEventWindowBoundsChanged);
      ADD_STR_KIND (kEventWindowClosed);
      ADD_STR_KIND (kEventWindowClickContentRgn);
      ADD_STR_KIND (kEventWindowClickStructureRgn);
      ADD_STR_KIND (kEventWindowFocusAcquired);
      ADD_STR_KIND (kEventWindowFocusRelinquish);
      ADD_STR_KIND (kEventWindowFocusContent);
      break;

   case kEventClassKeyboard:
      ADD_STR_KIND (kEventRawKeyDown);
      ADD_STR_KIND (kEventRawKeyRepeat);
      ADD_STR_KIND (kEventRawKeyUp);
      ADD_STR_KIND (kEventRawKeyModifiersChanged);
      break;

   case kEventClassTextInput:
      ADD_STR_KIND (kEventTextInputUnicodeForKeyEvent);
      ADD_STR_KIND (kEventTextInputUnicodeText);
      break;

   case kEventClassControl:
      ADD_STR_KIND (kEventControlHit);
      ADD_STR_KIND (kEventControlSimulateHit);
      ADD_STR_KIND (kEventControlHitTest);
      ADD_STR_KIND (kEventControlDraw);
      ADD_STR_KIND (kEventControlSetFocusPart);
      ADD_STR_KIND (kEventControlActivate);
      ADD_STR_KIND (kEventControlDeactivate);
      ADD_STR_KIND (kEventControlContextualMenuClick);
      ADD_STR_KIND (kEventControlClick);
      ADD_STR_KIND (kEventControlGetClickActivation);
      ADD_STR_KIND (kEventControlDragEnter);
      ADD_STR_KIND (kEventControlDragWithin);
      ADD_STR_KIND (kEventControlDragLeave);
      ADD_STR_KIND (kEventControlDragReceive);
      ADD_STR_KIND (kEventControlTrackingAreaEntered);
      ADD_STR_KIND (kEventControlTrackingAreaExited);
      ADD_STR_KIND (kEventControlBoundsChanged);
      break;

   case kEventClassCommand:
      ADD_STR_KIND (kEventCommandProcess);
      break;
   
   default:
      str += "Unknown Event";
      break;
   }
   
   str += " ";
   
   syslog (LOG_DEBUG, "%s", str.c_str ());
}
#undef ADD_STR_CLASS
#undef ADD_STR_KIND



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

EventFnc::EventFnc ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
