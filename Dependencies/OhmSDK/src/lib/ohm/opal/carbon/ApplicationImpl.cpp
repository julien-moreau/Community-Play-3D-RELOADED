/*****************************************************************************

        ApplicationImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71118

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

#include "ohm/opal/carbon/ApplicationImpl.h"
#include "ohm/opal/carbon/EventFnc.h"
#include "ohm/opal/carbon/WindowImpl.h"
#include "ohm/opal/Window.h"
#include "ohm/opal/View.h"
#include "ohm/sys/FilesystemSpec.h"
#include "ohm/util/trace.h"
#include <cassert>

#import <Cocoa/Cocoa.h>



namespace ohm
{
namespace opal
{

static NSAutoreleasePool *
   ApplicationImpl_auto_release_pool_ptr;


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

ApplicationImpl::~ApplicationImpl ()
{
   if (_app_event_handler_ref != 0)
   {
      RemoveEventHandler (_app_event_handler_ref);
      _app_event_handler_ref = 0;
   }
   [ApplicationImpl_auto_release_pool_ptr release];
}



/*
==============================================================================
Name : use_instance
==============================================================================
*/

ApplicationImpl & ApplicationImpl::use_instance ()
{
   static ApplicationImpl instance;
   
   return instance;
}



/*
==============================================================================
Name : set_ui_mode_windowed
Description :
   "Normal" User Interface Mode (same as a normal application launch)
==============================================================================
*/

void  ApplicationImpl::set_ui_mode_windowed ()
{
   SetSystemUIMode (kUIModeNormal, 0);
}



/*
==============================================================================
Name : set_ui_mode_immersive
Description :
   Dock is hidden and cannot be reached in the activate dock aera.
   Menubar is not visible.
==============================================================================
*/

void  ApplicationImpl::set_ui_mode_immersive ()
{
   // kUIModeAllHidden
   
   //SetSystemUIMode (kUIModeAllSuppressed, 0);
   SetSystemUIMode (kUIModeAllHidden, kUIOptionAutoShowMenuBar);
}



/*
==============================================================================
Name : set_system_pasteboard
Description :
   Set the clipboard to the content of data, translating the content of
   data to whatever might be suitable for the system.
==============================================================================
*/

void  ApplicationImpl::set_system_pasteboard (const Data & data)
{
   OSStatus err = PasteboardClear (_global_paste_board_ref);
   assert (err == 0);
   
   // UTIs :
   // file:///Developer/ADC%20Reference%20Library/documentation/Carbon/Conceptual/understanding_utis/utilist/chapter_4_section_1.html
   
   // mime types :
   // http://www.iana.org/assignments/media-types/
   
   if (!data._mime_type_data_map_arr.empty ())
   {
      const Data::MimeTypeDataMap & mime_type_data_map = data._mime_type_data_map_arr.front ();
      
      if (mime_type_data_map.find ("application/octet-stream") != mime_type_data_map.end ())
      {
         Data::MimeTypeDataMap::const_iterator it = mime_type_data_map.find ("application/octet-stream");
         
         const std::vector <archi::UByte> & flavor_data = it->second;
         
         CFDataRef data_ref = CFDataCreate (0, &flavor_data [0], flavor_data.size ());
         
         err = PasteboardPutItemFlavor (
            _global_paste_board_ref,
            (void *)1,  // item id
            #warning rename
            CFSTR ("public.data"),  // uniform type identifier
            data_ref,   // data
            kPasteboardFlavorSenderOnly   // private
         );
         assert (err == 0);
         
         CFRelease (data_ref);
      }

      if (mime_type_data_map.find ("text/plain") != mime_type_data_map.end ())
      {
         Data::MimeTypeDataMap::const_iterator it = mime_type_data_map.find ("text/plain");
         
         const std::vector <archi::UByte> & flavor_data = it->second;
         
         CFDataRef data_ref = CFDataCreate (0, &flavor_data [0], flavor_data.size ());
         
         err = PasteboardPutItemFlavor (
            _global_paste_board_ref,
            (void *)1,  // item id
            kUTTypeUTF8PlainText,   // uniform type identifier
            data_ref,   // data
            kPasteboardFlavorNoFlags
         );
         assert (err == 0);
         
         CFRelease (data_ref);
      }
   }
}



/*
==============================================================================
Name : is_system_pasteboard_external_available
Description :
   Returns 'true' if the system clipboard contains data that were not
   set by us (to avoid a forward-and-back translation that might
   lose some data)
==============================================================================
*/

bool  ApplicationImpl::is_system_pasteboard_external_available ()
{
   /*assert (_local_paste_board_ref != 0);
   
   PasteboardSyncFlags sync = PasteboardSynchronize (_local_paste_board_ref);
   
   bool ret_val = false;
   
   if (sync == kPasteboardModified)
   {
      ret_val = true;
   }
   
   if (sync == kPasteboardClientIsOwner)
   {
      ret_val = false;
   }

   if (ret_val)
   {
      ItemCount item_count = 0;
      OSStatus err = PasteboardGetItemCount (_local_paste_board_ref, &item_count);
      assert (err == 0);
      
      ret_val = item_count > 0;
   }
   
   return ret_val;*/
   
   return true;
}



/*
==============================================================================
Name : get_system_pasteboard
Description :
   Fill 'data' with the system clipboard in whatever different flavor
   the system give us.
==============================================================================
*/

void  ApplicationImpl::get_system_pasteboard (Data & data)
{
   //assert (is_system_pasteboard_external_available ());
   
   PasteboardSynchronize (_global_paste_board_ref);
   
   ItemCount item_count = 0;
   OSStatus err = PasteboardGetItemCount (_global_paste_board_ref, &item_count);
   assert (err == 0);
   
   for (ItemCount i = 0 ; i < item_count ; ++i)
   {
      Data::MimeTypeDataMap mime_type_data_map;
            
      PasteboardItemID item = 0;
      err = PasteboardGetItemIdentifier (_global_paste_board_ref, i + 1, &item);
      assert (err == 0);
      
      CFArrayRef array_ref = 0;
      err = PasteboardCopyItemFlavors (_global_paste_board_ref, item, &array_ref);
      assert (err == 0);
      
      CFIndex array_count = CFArrayGetCount (array_ref);
      
      for (CFIndex j = 0 ; j < array_count ; ++j)
      {
         CFStringRef uti_ref = (CFStringRef) CFArrayGetValueAtIndex (array_ref, j);
         
         if (CFStringCompare (uti_ref, kUTTypeUTF8PlainText, 0) == kCFCompareEqualTo)
         {
            CFDataRef data_ref = 0;
            err = PasteboardCopyItemFlavorData (_global_paste_board_ref, item, kUTTypeUTF8PlainText, &data_ref);
            assert (err == 0);
            
            std::vector <archi::UByte> flavor_data (CFDataGetLength (data_ref));
            CFDataGetBytes (data_ref, CFRangeMake (0, CFDataGetLength (data_ref)), &flavor_data [0]);
            
            mime_type_data_map [Data::_mime_text_plain_0] = flavor_data;
            
            CFRelease (data_ref);
         }
      }
      
      CFRelease (array_ref);
      
      data._mime_type_data_map_arr.push_back (mime_type_data_map);
   }
}



/*
==============================================================================
Name : set_cursor
==============================================================================
*/

void  ApplicationImpl::set_cursor (StandardMouseCursor cursor)
{
   switch (cursor)
   {
   case StandardMouseCursor_DEFAULT:
      set_default_cursor ();
      break;

   case StandardMouseCursor_IBEAM:
      SetThemeCursor (kThemeIBeamCursor);
      break;

   case StandardMouseCursor_POINTING_HAND:
      SetThemeCursor (kThemePointingHandCursor);
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : set_default_cursor
==============================================================================
*/

void  ApplicationImpl::set_default_cursor ()
{
   SetThemeCursor (kThemeArrowCursor);
}



/*
==============================================================================
Name : show_cursor
==============================================================================
*/

void  ApplicationImpl::show_cursor ()
{
   if (!CGCursorIsVisible ())
   {
      CGDisplayErr err = CGDisplayShowCursor (kCGDirectMainDisplay);
      assert (err == 0);
   }
}



/*
==============================================================================
Name : hide_cursor
==============================================================================
*/

void  ApplicationImpl::hide_cursor ()
{
   if (CGCursorIsVisible ())
   {
      CGDisplayErr err = CGDisplayHideCursor (kCGDirectMainDisplay);
      assert (err == 0);
   }
}



/*
==============================================================================
Name : obscure_cursor
==============================================================================
*/

void  ApplicationImpl::obscure_cursor ()
{
   [NSCursor setHiddenUntilMouseMoves:YES];
}



/*
==============================================================================
Name : unobscure_cursor
==============================================================================
*/

void  ApplicationImpl::unobscure_cursor ()
{
   [NSCursor setHiddenUntilMouseMoves:NO];
}



/*
==============================================================================
Name : choose_file
==============================================================================
*/

void  ApplicationImpl::choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message)
{
   _filter_proc = filter_proc;
   
   NavDialogCreationOptions options;
   OSStatus err = NavGetDefaultDialogCreationOptions (&options);
   assert (err == 0);
   
   CFStringRef window_title_ref = CFStringCreateWithCString (0, window_title.c_str (), kCFStringEncodingUTF8);
   CFStringRef message_ref = CFStringCreateWithCString (0, message.c_str (), kCFStringEncodingUTF8);
   
   options.windowTitle = window_title_ref;
   options.message = message_ref;
   
   NavDialogRef dialog_ref = 0;
   err = NavCreateChooseFileDialog (
      &options,
      0,
      0, 0,
      filter_file_proc, this,
      &dialog_ref
   );
   assert (err == 0);
   
   if (original_path != "")
   {
      FSRef fs_ref;
      err = FSPathMakeRef ((::UInt8 *) original_path.c_str (), &fs_ref, 0);
      assert (err == 0);
      
      AEDesc original_desc;
      err = AECreateDesc (typeFSRef, &fs_ref, sizeof (FSRef), &original_desc);
      assert (err == 0);
      
      err = NavCustomControl (dialog_ref, kNavCtlSetLocation, &original_desc);
      assert (err == 0);
   }

   err = NavDialogRun (dialog_ref);
   
   NavUserAction action = NavDialogGetUserAction (dialog_ref);
   
   if (action == kNavUserActionCancel)
   {
      path = "";
   }
   else
   {
      NavReplyRecord reply;
      err = NavDialogGetReply (dialog_ref, &reply);
      assert (err == 0);
      
      AEDesc dir_desc = { typeNull, NULL };
      err = AEGetNthDesc (&reply.selection, 1, typeFSRef, 0, &dir_desc);
      assert (err == 0);
      
      FSRef dir_ref;
      err = AEGetDescData (&dir_desc, &dir_ref, sizeof (FSRef));
      AEDisposeDesc (&dir_desc);
      assert (err == 0);
         
      char path_0 [2049];
      err = FSRefMakePath (&dir_ref, (UInt8 *) path_0, 2048);
      assert (err == 0);
      
      path = std::string (path_0);
      
      NavDisposeReply (&reply);
   }
   
   CFRelease (window_title_ref);
   CFRelease (message_ref);
   NavDialogDispose (dialog_ref);
   
   _filter_proc = 0;
}



/*
==============================================================================
Name : choose_file
==============================================================================
*/

void  ApplicationImpl::choose_file (std::string & path, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   _file_format_list = file_format_list;
   
   NavDialogCreationOptions options;
   OSStatus err = NavGetDefaultDialogCreationOptions (&options);
   assert (err == 0);
   
   CFStringRef window_title_ref = CFStringCreateWithCString (0, window_title.c_str (), kCFStringEncodingUTF8);
   CFStringRef message_ref = CFStringCreateWithCString (0, message.c_str (), kCFStringEncodingUTF8);
   
   options.windowTitle = window_title_ref;
   options.message = message_ref;
   
   NavDialogRef dialog_ref = 0;
   err = NavCreateChooseFileDialog (
      &options,
      0,
      0, 0,
      filter_file_proc, this,
      &dialog_ref
   );
   assert (err == 0);
   
   if (original_path != "")
   {
      FSRef fs_ref;
      err = FSPathMakeRef ((::UInt8 *) original_path.c_str (), &fs_ref, 0);
      assert (err == 0);
      
      AEDesc original_desc;
      err = AECreateDesc (typeFSRef, &fs_ref, sizeof (FSRef), &original_desc);
      assert (err == 0);
      
      err = NavCustomControl (dialog_ref, kNavCtlSetLocation, &original_desc);
      assert (err == 0);
   }

   err = NavDialogRun (dialog_ref);
   
   NavUserAction action = NavDialogGetUserAction (dialog_ref);
   
   if (action == kNavUserActionCancel)
   {
      path = "";
   }
   else
   {
      NavReplyRecord reply;
      err = NavDialogGetReply (dialog_ref, &reply);
      assert (err == 0);
      
      AEDesc dir_desc = { typeNull, NULL };
      err = AEGetNthDesc (&reply.selection, 1, typeFSRef, 0, &dir_desc);
      assert (err == 0);
      
      FSRef dir_ref;
      err = AEGetDescData (&dir_desc, &dir_ref, sizeof (FSRef));
      AEDisposeDesc (&dir_desc);
      assert (err == 0);
         
      char path_0 [2049];
      err = FSRefMakePath (&dir_ref, (UInt8 *) path_0, 2048);
      assert (err == 0);
      
      path = std::string (path_0);
      
      NavDisposeReply (&reply);
   }
   
   CFRelease (window_title_ref);
   CFRelease (message_ref);
   NavDialogDispose (dialog_ref);

   _file_format_list.clear ();
}



/*
==============================================================================
Name : choose_files
==============================================================================
*/

void  ApplicationImpl::choose_files (std::list <std::string> & path_list, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   #warning to do
}



/*
==============================================================================
Name : choose_folder
==============================================================================
*/

void  ApplicationImpl::choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message)
{
   NavDialogCreationOptions options;
   OSStatus err = NavGetDefaultDialogCreationOptions (&options);
   assert (err == 0);
   
   CFStringRef window_title_ref = CFStringCreateWithCString (0, window_title.c_str (), kCFStringEncodingUTF8);
   CFStringRef message_ref = CFStringCreateWithCString (0, message.c_str (), kCFStringEncodingUTF8);
   
   options.windowTitle = window_title_ref;
   options.message = message_ref;
   
   NavDialogRef dialog_ref = 0;
   err = NavCreateChooseFolderDialog (
      &options,
      0, 0, 0,
      &dialog_ref
   );
   assert (err == 0);
   
   if (original_path != "")
   {
      FSRef fs_ref;
      err = FSPathMakeRef ((::UInt8 *) original_path.c_str (), &fs_ref, 0);
      assert (err == 0);
      
      AEDesc original_desc;
      err = AECreateDesc (typeFSRef, &fs_ref, sizeof (FSRef), &original_desc);
      assert (err == 0);
      
      err = NavCustomControl (dialog_ref, kNavCtlSetLocation, &original_desc);
      assert (err == 0);
   }

   err = NavDialogRun (dialog_ref);
   
   NavUserAction action = NavDialogGetUserAction (dialog_ref);
   
   if (action == kNavUserActionCancel)
   {
      path = "";
   }
   else
   {
      NavReplyRecord reply;
      err = NavDialogGetReply (dialog_ref, &reply);
      assert (err == 0);
      
      AEDesc dir_desc = { typeNull, NULL };
      err = AEGetNthDesc (&reply.selection, 1, typeFSRef, 0, &dir_desc);
      assert (err == 0);
      
      FSRef dir_ref;
      err = AEGetDescData (&dir_desc, &dir_ref, sizeof (FSRef));
      AEDisposeDesc (&dir_desc);
      assert (err == 0);
         
      char path_0 [2049];
      err = FSRefMakePath (&dir_ref, (UInt8 *) path_0, 2048);
      assert (err == 0);
      
      path = std::string (path_0) + "/";
      
      NavDisposeReply (&reply);
   }
   
   CFRelease (window_title_ref);
   CFRelease (message_ref);
   NavDialogDispose (dialog_ref);
}



/*
==============================================================================
Name : choose_volume
==============================================================================
*/

void  ApplicationImpl::choose_volume (std::string & path, const std::string & window_title, const std::string & message)
{
   NavDialogCreationOptions options;
   OSStatus err = NavGetDefaultDialogCreationOptions (&options);
   assert (err == 0);
   
   CFStringRef window_title_ref = CFStringCreateWithCString (0, window_title.c_str (), kCFStringEncodingUTF8);
   CFStringRef message_ref = CFStringCreateWithCString (0, message.c_str (), kCFStringEncodingUTF8);
   
   options.windowTitle = window_title_ref;
   options.message = message_ref;
   
   NavDialogRef dialog_ref = 0;
   err = NavCreateChooseVolumeDialog (
      &options,
      0, 0, 0,
      &dialog_ref
   );
   assert (err == 0);
   
   err = NavDialogRun (dialog_ref);
   
   NavUserAction action = NavDialogGetUserAction (dialog_ref);
   
   if (action == kNavUserActionCancel)
   {
      path = "";
   }
   else
   {
      NavReplyRecord reply;
      err = NavDialogGetReply (dialog_ref, &reply);
      assert (err == 0);
      
      AEDesc dir_desc = { typeNull, NULL };
      err = AEGetNthDesc (&reply.selection, 1, typeFSRef, 0, &dir_desc);
      assert (err == 0);
      
      FSRef dir_ref;
      err = AEGetDescData (&dir_desc, &dir_ref, sizeof (FSRef));
      AEDisposeDesc (&dir_desc);
      assert (err == 0);
         
      char path_0 [2049];
      err = FSRefMakePath (&dir_ref, (UInt8 *) path_0, 2048);
      assert (err == 0);
      
      path = std::string (path_0) + "/";
      
      NavDisposeReply (&reply);
   }
   
   CFRelease (window_title_ref);
   CFRelease (message_ref);
   NavDialogDispose (dialog_ref);
}



/*
==============================================================================
Name : put_file
==============================================================================
*/

void  ApplicationImpl::put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &/*filter*/)
{
   NavDialogCreationOptions options;
   OSStatus err = NavGetDefaultDialogCreationOptions (&options);
   assert (err == 0);
   
   CFStringRef window_title_ref = CFStringCreateWithCString (0, window_title.c_str (), kCFStringEncodingUTF8);
   CFStringRef message_ref = CFStringCreateWithCString (0, message.c_str (), kCFStringEncodingUTF8);
   CFStringRef save_filename_ref = CFStringCreateWithCString (0, original_filename.c_str (), kCFStringEncodingUTF8);
   
   options.windowTitle = window_title_ref;
   options.message = message_ref;
   options.saveFileName = save_filename_ref;
   
   NavDialogRef dialog_ref = 0;
   err = NavCreatePutFileDialog (
      &options,
      kNavGenericSignature, kNavGenericSignature,
      0, 0,
      &dialog_ref
   );
   assert (err == 0);
   
   if (original_path != "")
   {
      FSRef fs_ref;
      err = FSPathMakeRef ((::UInt8 *) original_path.c_str (), &fs_ref, 0);
      assert (err == 0);
      
      AEDesc original_desc;
      err = AECreateDesc (typeFSRef, &fs_ref, sizeof (FSRef), &original_desc);
      assert (err == 0);
      
      err = NavCustomControl (dialog_ref, kNavCtlSetLocation, &original_desc);
      assert (err == 0);
   }

   err = NavDialogRun (dialog_ref);
   
   NavUserAction action = NavDialogGetUserAction (dialog_ref);
   
   if (action == kNavUserActionCancel)
   {
      path = "";
   }
   else
   {
      NavReplyRecord reply;
      err = NavDialogGetReply (dialog_ref, &reply);
      assert (err == 0);
      
      AEDesc dir_desc = { typeNull, NULL };
      err = AEGetNthDesc (&reply.selection, 1, typeFSRef, 0, &dir_desc);
      assert (err == 0);
      
      FSRef dir_ref;
      err = AEGetDescData (&dir_desc, &dir_ref, sizeof (FSRef));
      AEDisposeDesc (&dir_desc);
      assert (err == 0);
         
      char path_0 [2049];
      err = FSRefMakePath (&dir_ref, (UInt8 *) path_0, 2048);
      assert (err == 0);
      
      CFStringRef filename_ref = reply.saveFileName;
      char filename_0 [2049];
      
      bool ok_flag = CFStringGetCString (
         filename_ref, filename_0, 2049, kCFStringEncodingUTF8
      );
      assert (ok_flag);
      
      path = std::string (path_0);
      path += sys::FilesystemSpec::use_instance ().get_default_dir_delimiter ();
      path += filename_0;
      
      NavDisposeReply (&reply);
   }
   
   CFRelease (save_filename_ref);
   CFRelease (window_title_ref);
   CFRelease (message_ref);
   NavDialogDispose (dialog_ref);
}



/*
==============================================================================
Name : begin_mouse_capture
==============================================================================
*/

void  ApplicationImpl::begin_mouse_capture (HIViewRef view_ref, bool relative_hidden_flag, bool block_flag)
{
   assert (_capture_gesture_ref == 0);
   
   _capture_gesture_ref = view_ref;
   _relative_hidden_flag = relative_hidden_flag;
   _capture_flag = true;
   _block_flag = block_flag;
   
   if (_relative_hidden_flag)
   {
      CGAssociateMouseAndMouseCursorPosition (false);
      hide_cursor ();
   }
}



/*
==============================================================================
Name : end_mouse_capture
==============================================================================
*/

void  ApplicationImpl::end_mouse_capture (HIViewRef view_ref)
{
   assert (_capture_gesture_ref != 0);
   assert (_capture_gesture_ref == view_ref);
   
   _capture_gesture_ref = 0;
   _capture_flag = false;
   _block_flag = false;
   
   if (_relative_hidden_flag)
   {
      CGAssociateMouseAndMouseCursorPosition (true);
      show_cursor ();
   }
}



/*
==============================================================================
Name : end_mouse_capture
==============================================================================
*/

void  ApplicationImpl::end_mouse_capture (HIViewRef view_ref, const opa::Point & view_rel_pt)
{
   assert (_capture_gesture_ref != 0);
   assert (_capture_gesture_ref == view_ref);
   
   CGPoint pt = view_rel_pt.to_CGPoint ();
   
   HIViewConvertPoint (&pt, view_ref, 0);
   // now pt is in window content relative coords
   // (includes decoration)
   
   ::Rect bounds;
   OSStatus err = GetWindowBounds(
      HIViewGetWindow (view_ref),
      kWindowStructureRgn,
      &bounds
   );
   assert (err == 0);
   
   
   CGPoint global_point;
   global_point.x = float (bounds.left) + pt.x;
   global_point.y = float (bounds.top) + pt.y;

   {
      // the following line will move the cursor but will
      // not generate an event.
      // HOWEVER the next mouse moved or mouse dragged event
      // will contain an erroneous delta of the total displacement
      // from the position of the mouse before it was moved
      // with CGWarpMouseCursorPosition
      
      // we've been testing some different solutions, and none
      // will work as expected, they are :
      // - calling CGWarpMouseCursorPosition twice
      // - posting a CGEventRef from CGEventCreateMouseEvent
      //   this will not work as for the delta with any
      //   combinaison of factors :
      //   - CGEventSetIntegerValueField (event_ref, kCGMouseEventDeltaX, 0);
      //   - kCGHIDEventTap, kCGSessionEventTap, kCGAnnotatedSessionEventTap
      // - posting a CGEventRef from CGEventCreateMouseEvent twice
      // - using CGSetLocalEventsSuppressionInterval with any interval value
      // - enclosing the move in a CGAssociateMouseAndMouseCursorPosition true/false
      
      // the only solution found for now is to flag the movement upper in
      // ViewImpl to ignore the mouse delta contained in the event for
      // the next mouse moved or mouse dragged event
      
      CGError err = CGWarpMouseCursorPosition (global_point);
      assert (err == 0);
   }
   
   _capture_gesture_ref = 0;
   _capture_flag = false;
   _block_flag = false;
   
   if (_relative_hidden_flag)
   {
      CGAssociateMouseAndMouseCursorPosition (true);
      show_cursor ();
   }
}



/*
==============================================================================
Name : begin_mouse_gesture
==============================================================================
*/

void  ApplicationImpl::begin_mouse_gesture (HIViewRef view_ref, bool block_flag)
{
   assert (_capture_gesture_ref == 0);
   
   _capture_gesture_ref = view_ref;
   _relative_hidden_flag = false;
   _capture_flag = false;
   _block_flag = block_flag;
}



/*
==============================================================================
Name : end_mouse_capture
==============================================================================
*/

void  ApplicationImpl::end_mouse_gesture (HIViewRef view_ref)
{
   assert (_capture_gesture_ref != 0);
   assert (_capture_gesture_ref == view_ref);
   
   _capture_gesture_ref = 0;
   _capture_flag = false;
   _block_flag = false;
}



/*
==============================================================================
Name : post_synthesized_mouse_move_event
==============================================================================
*/

void  ApplicationImpl::post_synthesized_mouse_move_event ()
{
   CGPoint mouse_cursor_position;
   
   // not quite cool, might get deprecated, but here is the problem :
   // we want to know the position of the mouse at the moment
   // the application is switched in
   ::Point pt;
   GetGlobalMouse (&pt);
   
   mouse_cursor_position.x = pt.h;
   mouse_cursor_position.y = pt.v;
   
   CGEventRef event_ref = CGEventCreateMouseEvent (
      0, // source event
      kCGEventMouseMoved,
      mouse_cursor_position,
      0  // mouse button, ignored
   );
   assert (event_ref != 0);
   
   CGEventPost (
      kCGHIDEventTap,
      event_ref
   );
   
   CFRelease (event_ref);
   event_ref = 0;
}



/*
==============================================================================
Name : post_synthesized_key_event
==============================================================================
*/

void  ApplicationImpl::post_synthesized_key_event (::CGKeyCode key_code, bool key_down_flag)
{
   CGEventRef event_ref = CGEventCreateKeyboardEvent (
      0, // source event
      key_code,
      key_down_flag
   );
   assert (event_ref != 0);
   
   CGEventPost (
      kCGHIDEventTap,
      event_ref
   );
   
   CFRelease (event_ref);
   event_ref = 0;
}



/*
==============================================================================
Name : get_keyboard_focus_ptr
Description :
   Return the current opal view which acquired keyboard focus, if any.
==============================================================================
*/

View *   ApplicationImpl::get_keyboard_focus_ptr ()
{
   View * view_ptr = 0;
   
   // only search the active window
   
   ::WindowRef window_ref = ::ActiveNonFloatingWindow ();
   
   if (window_ref != 0)
   {
      ::HIViewRef view_ref = 0;
      
      ::GetKeyboardFocus (window_ref, &view_ref);
      
      if (view_ref != 0)
      {
         ViewImpl * view_impl_ptr = ViewImpl::get (view_ref);
         
         if (view_impl_ptr != 0)
         {
            view_ptr = &view_impl_ptr->use_view ();
         }
      }
   }
   
   return view_ptr;
}



/*
==============================================================================
Name : synthetize_events
==============================================================================
*/

void  ApplicationImpl::synthetize_events (View & view)
{
   // the current state, as seen by the view
   
   const Event & event = view.get_event ();
   
   // the current state, as seen by MacOS X
   
   CGEventSourceStateID event_source = kCGEventSourceStateCombinedSessionState;
   
   bool command_flag = ::CGEventSourceKeyState (event_source, kVK_Command);
   bool shift_flag = ::CGEventSourceKeyState (event_source, kVK_Shift);
   bool alt_flag = ::CGEventSourceKeyState (event_source, kVK_Option);
   
   Key key = event._key;
   
   if (command_flag != event._key.has_command_modifier ())
   {
      post_synthesized_key_event (kVK_Command, command_flag);
   }
   
   if (shift_flag != event._key.has_shift_modifier ())
   {
      post_synthesized_key_event (kVK_Shift, shift_flag);
   }
   
   if (alt_flag != event._key.has_alt_modifier ())
   {
      post_synthesized_key_event (kVK_Option, alt_flag);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ApplicationImpl::ApplicationImpl ()
:  _local_paste_board_ref (0)
,  _global_paste_board_ref (0)
,  _app_event_handler_ref (0)
,  _capture_gesture_ref (0)
,  _relative_hidden_flag (false)
,  _capture_flag (false)
,  _block_flag (false)
{
   NSApplicationLoad ();
   
   ApplicationImpl_auto_release_pool_ptr = [[NSAutoreleasePool alloc] init];

   OSStatus err = PasteboardCreate (0, &_local_paste_board_ref);
   assert (err == 0);

   err = PasteboardCreate (kPasteboardClipboard, &_global_paste_board_ref);
   assert (err == 0);

   static const EventTypeSpec app_event_type_arr [] = {
      {kEventClassApplication, kEventAppActivated},
      {kEventClassApplication, kEventAppDeactivated},

      //{kEventClassWindow, kEventWindowClose},

      {kEventClassTextInput, kEventTextInputUnicodeForKeyEvent},

      {kEventClassCommand, kEventCommandProcess},
      
      // only for redirects
      {kEventClassKeyboard, kEventRawKeyDown},
      {kEventClassKeyboard, kEventRawKeyRepeat},
      {kEventClassKeyboard, kEventRawKeyUp},
      {kEventClassKeyboard, kEventRawKeyModifiersChanged},
   };
   
   // we may need to put kEventWindowClose in the window event handler
   // for dock reasons
   
   err = InstallEventHandler (
      GetApplicationEventTarget (),
      handle_event_proc,
      GetEventTypeCount (app_event_type_arr),
      app_event_type_arr,
      this,
      &_app_event_handler_ref
   );
   assert (err == 0);
   
   // See definition of SRefCon
   // we match exactly the declaration of the function
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : notify_activated
Description :
   Notification when the application becomes the front application, that
   is the one which is the primary target for user inputs.
==============================================================================
*/

void  ApplicationImpl::notify_activated ()
{
   post_synthesized_mouse_move_event ();
}



/*
==============================================================================
Name : notify_deactivated
Description :
   Notification when the application is no longer the front application, that
   is the one which is the primary target for user inputs.
==============================================================================
*/

void  ApplicationImpl::notify_deactivated ()
{
   WindowRef window_ref = GetWindowList ();
   
   while (window_ref != 0)
   {
      WindowImpl * window_impl_ptr = WindowImpl::get (window_ref);
         
      if (window_impl_ptr != 0)
      {
         window_impl_ptr->user_input_lost ();
      }
      
      window_ref = GetNextWindow (window_ref);
   }
}



/*
==============================================================================
Name : filter_file_proc
==============================================================================
*/

::Boolean   ApplicationImpl::filter_file_proc (::AEDesc *item_ptr, void * info, ::NavCallBackUserData user_data, ::NavFilterModes filter_mode)
{
   ApplicationImpl * this_ptr = reinterpret_cast <ApplicationImpl *> (user_data);
   assert (this_ptr != 0);
   
   ::Boolean ret_val = this_ptr->filter_file (item_ptr, info, user_data, filter_mode);
   
   return ret_val;
}



/*
==============================================================================
Name : filter_file
==============================================================================
*/

::Boolean   ApplicationImpl::filter_file (::AEDesc * item_ptr, void * info, ::NavCallBackUserData user_data, ::NavFilterModes filter_mode)
{
   ::Boolean ret_val = false;
   
   bool ok_flag = false;
   ok_flag |= filter_mode == kNavFilteringBrowserList;
   ok_flag |= filter_mode == kNavFilteringFavorites;
   ok_flag |= filter_mode == kNavFilteringRecents;
   
   if (ok_flag)
   {
      ::AEDesc fsref_desc;
      
      ::OSStatus err = ::AECoerceDesc (item_ptr, ::typeFSRef, &fsref_desc);
      
      if (err == 0)
      {
         ::NavFileOrFolderInfo * fofi = (::NavFileOrFolderInfo *) info;
         
         if (fofi->isFolder)
         {
            ret_val = true;
         }
         else
         {
            FSRef    fs_ref;
            char     path_0 [4096+1];
            err = ::AEGetDescData (&fsref_desc, &fs_ref, sizeof(fs_ref));
            assert (err == 0);
            
            err = ::FSRefMakePath (
                &fs_ref,
                reinterpret_cast < ::UInt8 *> (path_0),
                4096
             );
            assert (err == ::noErr);
            
            path_0 [4096] = 0;
            
            if (_filter_proc != 0)
            {
               ret_val = _filter_proc (path_0);
            }
            else
            {
               std::string file_path (path_0);
                        
               ret_val = Application::use_instance ().is_matching (file_path, _file_format_list);
            }
         }
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

OSStatus ApplicationImpl::handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data)
{
   OSStatus ret_val = noErr;
   
   try
   {
      ret_val = ApplicationImpl::use_instance ().handle_event (handler_call_ref, event_ref);
   }
   catch (...)
   {
      assert (false);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_event (EventHandlerCallRef handler_call_ref, EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventClass (event_ref))
   {
   case kEventClassApplication:
      ret_val = handle_application_event (event_ref);
      break;

   case kEventClassMouse:
      ret_val = handle_mouse_event (event_ref);
      break;

   case kEventClassWindow:
      ret_val = handle_window_event (event_ref);
      break;
      
   case kEventClassKeyboard:
      ret_val = handle_keyboard_event (event_ref);
      break;
      
   case kEventClassTextInput:
      ret_val = handle_text_input_event (event_ref);
      break;
      
   case kEventClassCommand:
      ret_val = handle_command_event (event_ref);
      break;
      
   default:
      assert (false);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_application_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_application_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventAppActivated:
      notify_activated ();
      break;

   case kEventAppDeactivated:
      notify_deactivated ();
      break;

   default:
      assert (false);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_mouse_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_mouse_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   return ret_val;
}



/*
==============================================================================
Name : handle_window_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_window_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventWindowClose:
      {
         WindowRef window_ref = 0;
         GetEventParameter (
            event_ref,
            kEventParamDirectObject,
            typeWindowRef,
            0,
            sizeof (WindowRef),
            0,
            &window_ref
         );
         assert (window_ref != 0);
         
         WindowImpl * window_impl_ptr = WindowImpl::get (window_ref);
         
         if (window_impl_ptr != 0)
         {
            Window & window = window_impl_ptr->get_window ();
            
            assert (false);
            
            /*if (window.can_close_window ())
            {
               DisposeWindow (window_ref);
            }*/
         }
      }
      break;

   default:
      assert (false);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_keyboard_event
Model :
   Each key pressed will generate, if not eat by an external view, the
   following sequence :

   kEventRawKeyDown
   kEventTextInputUnicodeForKeyEvent
   kEventRawKeyUp
   
   or 

   kEventRawKeyDown
   kEventTextInputUnicodeForKeyEvent
   kEventRawKeyRepeat
   kEventTextInputUnicodeForKeyEvent
   kEventRawKeyUp
   
   Any other sequence means that the external view eat the event.
==============================================================================
*/

OSStatus ApplicationImpl::handle_keyboard_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   // If we receive the event here, this means that every views let it
   // slipped. we are going to let it slip also so that a text input
   // event is generated.
   // if no one takes the text input, we are going to send them
   // there can be multiple keyboard event issued before a text input
   // is generated
   
   ::UInt32 event_kind = GetEventKind (event_ref);
   
   if (event_kind == kEventRawKeyModifiersChanged)
   {
      return ret_val;   // ignore, it will be resynthetized
   }
   
   if (event_kind == kEventRawKeyDown)
   {
      // start of sequence
      
      _hid_event_keyboard_arr.clear ();
   }
   
   HIDEvent & hid_event = *_hid_event_keyboard_arr.insert (
      _hid_event_keyboard_arr.end (), HIDEvent ()
   );
   
   switch (event_kind)
   {
   case kEventRawKeyDown:
      hid_event._type = HIDEvent::Type_KEY_PRESSED;
      break;

   case kEventRawKeyRepeat:
      hid_event._type = HIDEvent::Type_KEY_REPEATED;
      break;

   case kEventRawKeyUp:
      hid_event._type = HIDEvent::Type_KEY_RELEASED;
      break;

   default:
      assert (false);
      break;
   }
   
   hid_event._key = Key (event_ref);
   
   return ret_val;
}



/*
==============================================================================
Name : handle_text_input_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_text_input_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   // the keys were ignored, send everything we have
   
   HIDEvent & hid_event = *_hid_event_keyboard_arr.insert (
      _hid_event_keyboard_arr.end (), HIDEvent ()
   );
   
   hid_event._type = HIDEvent::Type_CHARACTERS_TYPED;
   hid_event._str = EventFnc::use_instance ().get_characters (event_ref);
   
   Application::Event event;
   event._type = Application::Event::Type_HID_EVENT_ARR;
   event._hid_event_arr = _hid_event_keyboard_arr;
   
   Application::use_instance ().req_signal (event);
   
   _hid_event_keyboard_arr.clear ();
   
   switch (GetEventKind (event_ref))
   {
   case kEventTextInputUnicodeForKeyEvent:
      if ((_capture_gesture_ref != 0) && (_block_flag))
      {
         // block everything except modifiers changes
         if (!EventFnc::use_instance ().text_input_event_is_modifiers_changed (event_ref))
         {
            ret_val = noErr;
         }
      }
      break;
   
   default:
      assert (false);
      break;
   }
         
   return ret_val;
}



/*
==============================================================================
Name : handle_command_event
==============================================================================
*/

OSStatus ApplicationImpl::handle_command_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventCommandProcess:
      if ((_capture_gesture_ref != 0) && (_block_flag))
      {
         // block
         ret_val = noErr;
      }
      else
      {
         HICommandExtended hi_command;
      
         OSStatus err = GetEventParameter (
            event_ref,
            kEventParamDirectObject,
            typeHICommand,
            0,
            sizeof (HICommand),
            0,
            &hi_command
         );
         
         assert (err == ::noErr);
         
         ret_val = eventNotHandledErr;
         
         if (hi_command.commandID != 'ocmd')
         {
            // propagate to next and to post work
            #warning just not handling should be ok
            // ret_val = CallNextEventHandler (handler_call_ref, event_ref);
         }
         
         if (ret_val == eventNotHandledErr)
         {
            std::string command_str;
            
            if (hi_command.attributes == kHICommandFromMenu)
            {
               ::UInt32 size = 0;
               OSStatus err = GetMenuItemPropertySize (
                  hi_command.source.menu.menuRef,
                  hi_command.source.menu.menuItemIndex, 
                  'OmFo', 'cstr', &size
               );
               
               if ((err == 0) && (size != 0))
               {
                  command_str.resize (size);
                  err = GetMenuItemProperty (
                     hi_command.source.menu.menuRef,
                     hi_command.source.menu.menuItemIndex,
                     'OmFo', 'cstr', size, &size, &command_str [0]
                  );
                  assert (err == 0);
               }
            }
            else if (hi_command.attributes == kHICommandFromControl)
            {
               ::UInt32 size = 0;
               OSStatus err = GetControlPropertySize (
                  hi_command.source.control,
                  'OmFo', 'cstr', &size
               );
               
               if ((err == 0) && (size != 0))
               {
                  command_str.resize (size);
                  err = GetControlProperty (
                     hi_command.source.control,
                     'OmFo', 'cstr', size, &size, &command_str [0]
                  );
                  assert (err == 0);
               }
            }
            
            if (command_str != "")
            {
               Application::use_instance ().process_command (command_str);
               ret_val = noErr;
            }
         }
      }
      break;
   
   default:
      assert (false);
      break;
   }
         
   return ret_val;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
