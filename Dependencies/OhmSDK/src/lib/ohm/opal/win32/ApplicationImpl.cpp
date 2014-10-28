/*****************************************************************************

        ApplicationImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70971

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/win32/ApplicationImpl.h"
#include "ohm/opal/win32/WindowImpl.h"
#include "ohm/opal/Window.h"
#include "ohm/sys/win32/ExceptionHandler.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/win32/RunLoopGuiImpl.h"
#include "ohm/txt/fnc.h"
#include "ohm/txt/String16.h"

#include <vector>

#include <commdlg.h>
#include <shlobj.h>
#include <objbase.h>

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ApplicationImpl::~ApplicationImpl ()
{
   // Nothing

   if (_clipboard_hwnd)
   {
      if (::DestroyWindow (_clipboard_hwnd) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "dtor(): Cannot destroy clipboard window. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         assert (false);
      }
      _clipboard_hwnd = 0;
   }
   
   if (_hinst == 0)
   {
      ohm_util_TRACE_ERROR ("dtor: _hinst hasn't been set.");
      assert (false);
   }
   else
   {
      if (::UnregisterClassW (_clipboard_class_name_0, _hinst) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "dtor(): Cannot unregister clipboard window class. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         assert (false);
      }
      _hinst = 0;
   }
}



ApplicationImpl & ApplicationImpl::use_instance ()
{
   static ApplicationImpl  instance;
   
   return (instance);
}




void  ApplicationImpl::quit ()
{
   ::PostQuitMessage (0);
}
   


/*
==============================================================================
Name : set_ui_mode_windowed
Description :
   "Normal" User Interface Mode (same as a normal application launch)
Throws : Nothing
==============================================================================
*/

void  ApplicationImpl::set_ui_mode_windowed ()
{
   //SetSystemUIMode (kUIModeNormal, 0);
}



/*
==============================================================================
Name : set_ui_mode_immersive
Description :
   Dock is hidden and cannot be reached in the activate dock aera.
   Menubar is not visible.
Throws : Nothing
==============================================================================
*/

void  ApplicationImpl::set_ui_mode_immersive ()
{
   //SetSystemUIMode (kUIModeAllHidden, 0);
}



/*
==============================================================================
Name : set_system_pasteboard
Description :
   Set the clipboard to the content of data, translating the content of
   data to whatever might be suitable for the system.
Throws : Nothing
==============================================================================
*/

void  ApplicationImpl::set_system_pasteboard (const Data & data)
{
   ::BOOL w_ret = ::OpenClipboard (_clipboard_hwnd);
   assert (w_ret != 0);
   
   if (w_ret != 0)
   {
      w_ret = ::EmptyClipboard ();
      assert (w_ret != 0);
   }
   
   if (w_ret != 0 && !data._mime_type_data_map_arr.empty ())
   {
      const Data::MimeTypeDataMap & mime_type_data_map = data._mime_type_data_map_arr.front ();
      
      if (mime_type_data_map.find ("text/plain") != mime_type_data_map.end ())
      {
         Data::MimeTypeDataMap::const_iterator it = mime_type_data_map.find ("text/plain");

         const std::vector <archi::UByte> & flavor_data = it->second;
         
         // ensure null terminated
         std::string str_utf8;
         str_utf8.assign ((const char *) & flavor_data [0], flavor_data.size ());

         txt::String16 str_utf16;
         int ret_val = txt::conv_utf8_to_utf16 (str_utf16, str_utf8.c_str ());

         if (ret_val == txt::Err_OK && str_utf16.size () > 0)
         {
            const size_t wchar_count = str_utf16.size ();
            ::HGLOBAL hglobal = 0;
            
            {
               // add room for terminating null wide char
               const size_t data_length 
                  = (wchar_count + 1) * sizeof (txt::String16::value_type);

               hglobal = ::GlobalAlloc (GMEM_MOVEABLE, data_length);
            }
            
            if (hglobal == 0)
            {  
               w_ret = 0;
            }
            
            if (w_ret != 0)
            {
               txt::Char16 * data_ptr 
                  = (txt::Char16 *) (::GlobalLock (hglobal));
               const size_t wchar_len 
                  = wchar_count * sizeof (txt::String16::value_type);
               
               ::memcpy (data_ptr, str_utf16.c_str (), wchar_len);
               
               data_ptr [wchar_count] = 0;

               ::GlobalUnlock (hglobal); 
               
               ::SetClipboardData (CF_UNICODETEXT, hglobal);
            }
         }
      }
   }
   
   assert (w_ret != 0);
   
   w_ret = ::CloseClipboard ();
   
   assert (w_ret != 0);
}



/*
==============================================================================
Name : is_system_pasteboard_external_available
Description :
   Returns 'true' if the system clipboard contains data that were not
   set by us (to avoid a forward-and-back translation that might
   lose some data)
Throws : Nothing
==============================================================================
*/

bool  ApplicationImpl::is_system_pasteboard_external_available ()
{
   // same as mac os x implementation?
   return true;
}



/*
==============================================================================
Name : get_system_pasteboard
Description :
   Fill 'data' with the system clipboard in whatever different flavor
   the system give us.
Throws : Nothing
==============================================================================
*/

void  ApplicationImpl::get_system_pasteboard (Data & data)
{
   // only text for now.
   // we just handle utf16
   
   ::BOOL w_ret = ::OpenClipboard (_clipboard_hwnd);
   assert (w_ret != 0);
   
   bool has_text_data_flag = true;
   
   if (w_ret != 0)
   {
      // returns the next format after CF_TEXT, or 0 if end of enumeration or error.
      
      ::UINT next_format = ::EnumClipboardFormats (CF_UNICODETEXT);
      
      if (next_format == 0)
      {
         // distinguish between the end of the enumeration and an error
         
         ::DWORD error = ::GetLastError ();
         
         if (error != ERROR_SUCCESS)
         {
            has_text_data_flag = false;
         }
      }
   }
   
   if ((w_ret != 0) && has_text_data_flag)
   {
      ::HGLOBAL hglobal = ::GetClipboardData (CF_UNICODETEXT);
      
      if (hglobal == 0)
      { 
         w_ret = 0;
      }
      
      if (w_ret != 0)
      { 
         txt::Char16 * utf16_0 = (txt::Char16 *)(::GlobalLock (hglobal));
         
         if (utf16_0 != 0)
         {
            std::string str_utf8;
            
            int ret_val 
               = txt::conv_utf16_to_utf8 (str_utf8, utf16_0);

            if (ret_val == txt::Err_OK && str_utf8.size () > 0)
            {
               std::vector <archi::UByte> flavor_data;
               flavor_data.resize (str_utf8.size ());
               ::memcpy (& flavor_data [0], str_utf8.c_str (), str_utf8.size ());
               
               Data::MimeTypeDataMap mime_type_data_map;
               mime_type_data_map [Data::_mime_text_plain_0] = flavor_data;
               
               data._mime_type_data_map_arr.push_back (mime_type_data_map);
            }
            
            ::GlobalUnlock (hglobal); 
         } 
      }
   }
   
   w_ret = ::CloseClipboard ();
   
   assert (w_ret != 0);
}



void  ApplicationImpl::set_cursor (StandardMouseCursor cursor)
{
   // #warning to do

   switch (cursor)
   {
   case StandardMouseCursor_DEFAULT:
      set_default_cursor ();
      break;

   case StandardMouseCursor_IBEAM:
      ::SetCursor (::LoadCursor (0, IDC_IBEAM));
      break;

   case StandardMouseCursor_POINTING_HAND:
      ::SetCursor (::LoadCursor (0, IDC_ARROW));
      break;

   case StandardMouseCursor_CUSTOM:
   default:
      assert (false);
      break;
   }
}



void  ApplicationImpl::set_default_cursor ()
{
   ::SetCursor (::LoadCursor (0, IDC_ARROW));
}



void  ApplicationImpl::show_cursor ()
{
   if (_cursor_hidden_flag)
   {
      ::ShowCursor (TRUE);
      _cursor_hidden_flag = false;
   }
}



void  ApplicationImpl::hide_cursor ()
{
   if (! _cursor_hidden_flag)
   {
      ::ShowCursor (FALSE);
      _cursor_hidden_flag = true;
   }
}



/*
==============================================================================
Name : obscure_cursor
==============================================================================
*/

void  ApplicationImpl::obscure_cursor ()
{
   // to do
}



/*
==============================================================================
Name : unobscure_cursor
==============================================================================
*/

void  ApplicationImpl::unobscure_cursor ()
{
   // to do
}



void  ApplicationImpl::reposition_mouse (const opa::Point & global_location)
{
   ::SetCursorPos (int (global_location._x), int (global_location._y));
}



void  ApplicationImpl::choose_file (std::string & path, const std::string & original_path, bool (*filter_proc) (const char * /*path_0*/), const std::string & window_title, const std::string & message)
{
   // LDS for now 'file_format_list' in 'choose_file' was ignored.
   // we just forward to this function, ignoring the filter
   
   Application::FileFormatList file_format_list;
   
   choose_file (path, original_path, file_format_list, window_title, message);
}



void  ApplicationImpl::choose_file (std::string & path, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   txt::String16  window_title_utf16;
   txt::conv_utf8_to_utf16 (window_title_utf16, window_title.c_str ());
   txt::String16  original_path_utf16;
   txt::conv_utf8_to_utf16 (original_path_utf16, original_path.c_str ());

   const long     filename_max_size = 32767;
   std::vector <txt::Char16> file_buf (filename_max_size + 1, '\0');
   
   std::vector <txt::Char16> path_name;
   path_name.assign (original_path_utf16.begin (), original_path_utf16.end ());
   path_name.resize (filename_max_size + 1, '\0');
   
   ::OPENFILENAMEW   ofn;
   ofn.lStructSize = sizeof (ofn);
   ofn.hwndOwner = ::GetForegroundWindow ();
   ofn.hInstance = 0;
   ofn.lpstrFilter = L"All Files\0*.*\0\0";  // for now
   ofn.lpstrCustomFilter = 0;
   ofn.nMaxCustFilter = 0;
   ofn.nFilterIndex = 1;
   ofn.lpstrFile = &file_buf [0];
   ofn.nMaxFile = filename_max_size;
   ofn.lpstrFileTitle = 0;
   ofn.nMaxFileTitle = 0;
   ofn.lpstrInitialDir = &path_name [0];
   ofn.lpstrTitle = window_title_utf16.c_str ();
   ofn.Flags = OFN_ENABLESIZING | OFN_LONGNAMES | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;
   ofn.nFileOffset = 0;
   ofn.nFileExtension = 0;
   ofn.lpstrDefExt = 0;
   ofn.lCustData = 0;
   ofn.lpfnHook = 0;
   ofn.lpTemplateName = 0;
#if (_WIN32_WINNT >= 0x0500)
   ofn.dwReserved = 0;  
   ofn.pvReserved = NULL;
#endif // (_WIN32_WINNT >= 0x0500)

   int ret = ::GetOpenFileNameW (&ofn);
   
   if (ret == 0)
   {
      // cancel or error
      
      path.clear ();
   }
   else
   {
      // ok
      ret = txt::conv_utf16_to_utf8 (path, ofn.lpstrFile);
      if (ret != txt::Err_OK)
      {
         path.clear ();
      }
   }
}



void  ApplicationImpl::choose_files (std::list <std::string> & path_list, const std::string & original_path, const Application::FileFormatList & file_format_list, const std::string & window_title, const std::string & message)
{
   // #warning to do
   assert (false);
}



static int CALLBACK  DirectorySelectorImpl_callback_proc (HWND hwnd, UINT msg, LPARAM param, LPARAM data_ptr);
static void DirectorySelectorImpl_conv_scr_to_cli (HWND hwnd, ::RECT &rect);
static void DirectorySelectorImpl_move_window (HWND hwnd, ::RECT &rect, BOOL repaint_flag);



void  ApplicationImpl::choose_folder (std::string & path, const std::string & original_path, const std::string & window_title, const std::string & message)
{
   txt::String16  window_title_utf16;
   txt::conv_utf8_to_utf16 (window_title_utf16, window_title.c_str ());
   txt::String16  original_path_utf16;
   txt::conv_utf8_to_utf16 (original_path_utf16, original_path.c_str ());

   HRESULT ret_val = ::CoInitializeEx (0,  COINIT_APARTMENTTHREADED);
   
   assert ((ret_val == S_OK) || (ret_val == S_FALSE));
   
   bool deinit_com_flag = (ret_val == S_OK) || (ret_val == S_FALSE);
   
   ::LPMALLOC     imalloc_ptr = 0;
   // Must be balanced with a call to imalloc_ptr->Release ()
   const HRESULT result = ::SHGetMalloc (&imalloc_ptr);

   assert ( result == NOERROR);

   if (result == NOERROR)
   {
      const long filename_max_size = MAX_PATH;
      std::vector <txt::Char16>  dir_path_0 (filename_max_size + 1);
      txt::strncpy_0 (
         &dir_path_0 [0],
         original_path_utf16.c_str (),
         filename_max_size + 1
      );

      using namespace std;

      // launch the dialog box
      BROWSEINFOW    bi;
      memset (&bi, 0, sizeof (bi));
      bi.hwndOwner = ::GetForegroundWindow ();  // Handle to the owner window for the dialog box.
      bi.pidlRoot = 0;                          // Pointer to an item identifier list (PIDL) specifying the location of the root folder from which to start browsing. 
      bi.pszDisplayName = &dir_path_0 [0];      // Address of a buffer to receive the display name of the folder selected by the user. The size of this buffer is assumed to be MAX_PATH characters.
      bi.lpszTitle = window_title_utf16.c_str ();  // Address of a null-terminated string that is displayed above the tree view control in the dialog box. This string can be used to specify instructions to the user.
      bi.ulFlags = BIF_RETURNONLYFSDIRS;        // Flags specifying the options for the dialog box.  
      bi.lpfn = &DirectorySelectorImpl_callback_proc; // Address of an application-defined function that the dialog box calls when an event occurs. For more information, see the BrowseCallbackProc function. This member can be NULL.
      bi.lParam = reinterpret_cast <LPARAM> (original_path.c_str ());   // Application-defined value that the dialog box passes to the callback function, if one is specified. 
      bi.iImage = 0;                            // Variable to receive the image associated with the selected folder. The image is specified as an index to the system image list. 

      LPITEMIDLIST id_list_ptr = ::SHBrowseForFolderW (&bi);

      // now analyse the returned PIDL
      if (id_list_ptr == 0)
      {
         // user cancelled the action
         
         path.clear ();
      }
      else
      {
         // get the path
         if (::SHGetPathFromIDListW (id_list_ptr, &dir_path_0 [0]) == TRUE)
         {
            const int      ret = txt::conv_utf16_to_utf8 (path, &dir_path_0 [0]);
            if (ret != txt::Err_OK)
            {
               path.clear ();
            }

            // free the structure
            if (id_list_ptr != 0)
            {
               imalloc_ptr->Free (id_list_ptr);
            }
         }
         else
         {
            path.clear ();
         }
      }

      // release the interface
      imalloc_ptr->Release ();
   }

   if (deinit_com_flag)
   {
      ::CoUninitialize ();
   }
}



static int CALLBACK  DirectorySelectorImpl_callback_proc (HWND hwnd, UINT msg, LPARAM param, LPARAM data_ptr)
{
   switch (msg)
   {
      // Sent when the browse dialog box has finished initializing. 
      case  BFFM_INITIALIZED:
      {
         // Set initial directory
         ::SendMessage (hwnd, BFFM_SETSELECTION, TRUE, data_ptr);

         // Remove context help button from dialog caption
         LONG_PTR          style = ::GetWindowLongPtr (hwnd, GWL_STYLE);
         style &= ~DS_CONTEXTHELP;
         ::SetWindowLongPtr (hwnd, GWL_STYLE, style);

         style = ::GetWindowLongPtr (hwnd, GWL_EXSTYLE);
         style &= ~WS_EX_CONTEXTHELP;
         ::SetWindowLongPtr (hwnd, GWL_EXSTYLE, style);

         // Find the folder tree and make dialog larger
         const HWND     hwnd_tree =
            ::FindWindowEx (hwnd, 0, "SysTreeView32", 0);
         if (hwnd_tree != 0)
         {
            // Make the dialog larger
            ::RECT         rect_dlg;
            ::GetWindowRect (hwnd, &rect_dlg);
            rect_dlg.right += 40;
            rect_dlg.bottom += 30;
            DirectorySelectorImpl_move_window (hwnd, rect_dlg, TRUE);
            ::GetClientRect (hwnd, &rect_dlg);

            // Move the Cancel button
            ::RECT         rect_cancel;
            memset (&rect_cancel, 0, sizeof (rect_cancel));
            HWND           hwnd_cancel = ::GetDlgItem (hwnd, IDCANCEL);
            if (hwnd_cancel)
            {
               ::GetWindowRect (hwnd_cancel, &rect_cancel);
            }
            DirectorySelectorImpl_conv_scr_to_cli (hwnd, rect_cancel);
            const int      w = rect_cancel.right - rect_cancel.left;
            const int      h = rect_cancel.bottom - rect_cancel.top;
            rect_cancel.bottom = rect_dlg.bottom - 5;
            rect_cancel.top = rect_cancel.bottom - h;
            rect_cancel.right = rect_dlg.right - 5;
            rect_cancel.left = rect_cancel.right - w;
            if (hwnd_cancel != 0)
            {
               DirectorySelectorImpl_move_window (hwnd_cancel, rect_cancel, FALSE);
            }

            // Move the OK button
            ::RECT         rect_ok;
            memset (&rect_ok, 0, sizeof (rect_ok));
            HWND           hwnd_ok = ::GetDlgItem (hwnd, IDOK);
            if (hwnd_ok != 0)
            {
               ::GetWindowRect (hwnd_ok, &rect_ok);
            }
            DirectorySelectorImpl_conv_scr_to_cli (hwnd, rect_ok);
            rect_ok.bottom = rect_dlg.bottom - 5;
            rect_ok.top = rect_ok.bottom - h;
            rect_ok.right = rect_cancel.left - 10;
            rect_ok.left = rect_ok.right - w;
            if (hwnd_ok)
            {
               DirectorySelectorImpl_move_window (hwnd_ok, rect_ok, FALSE);
            }

            // Expand the folder tree to fill the dialog
            ::RECT         rect_tree;
            memset (&rect_tree, 0, sizeof (rect_tree));
            ::GetWindowRect (hwnd_tree, &rect_tree);
            DirectorySelectorImpl_conv_scr_to_cli (hwnd, rect_tree);
            rect_tree.top = 5;
            rect_tree.left= 5;
            rect_tree.bottom = rect_ok.top - 5;
            rect_tree.right = rect_dlg.right - 5;
            DirectorySelectorImpl_move_window (hwnd_tree, rect_tree, FALSE);
         }
      }
      break;
   }

   return (0);
}



void  DirectorySelectorImpl_conv_scr_to_cli (HWND hwnd, ::RECT &rect)
{
   assert (::IsWindow (hwnd));
   assert (&rect != 0);

   ::POINT        p_1;
   p_1.x = rect.left;
   p_1.y = rect.top;
   ::ScreenToClient (hwnd, &p_1);
   rect.left = p_1.x;
   rect.top = p_1.y;

   ::POINT        p_2;
   p_2.x = rect.right;
   p_2.y = rect.bottom;
   ::ScreenToClient (hwnd, &p_2);
   rect.right = p_2.x;
   rect.bottom = p_2.y;
}



void  DirectorySelectorImpl_move_window (HWND hwnd, ::RECT &rect, BOOL repaint_flag)
{
   assert (::IsWindow (hwnd));
   assert (&rect != 0);

   ::MoveWindow (
      hwnd,
      rect.left,
      rect.top,
      rect.right - rect.left,
      rect.bottom - rect.top,
      repaint_flag
   );
}



void  ApplicationImpl::choose_volume (std::string & path, const std::string & window_title, const std::string & message)
{
   // #warning to do
}



void  ApplicationImpl::put_file (std::string & path, const std::string & original_path, const std::string & original_filename, const std::string & window_title, const std::string & message, const std::vector <std::string> &filter)
{
   assert ((filter.size () & 1) == 0);

   txt::String16  window_title_utf16;
   txt::conv_utf8_to_utf16 (window_title_utf16, window_title.c_str ());
   txt::String16  original_path_utf16;
   txt::conv_utf8_to_utf16 (original_path_utf16, original_path.c_str ());
   txt::String16  original_filename_utf16;
   txt::conv_utf8_to_utf16 (original_filename_utf16, original_filename.c_str ());

   std::vector <txt::Char16>  filter_string;
   for (int pos = 0; pos < int (filter.size ()); ++pos)
   {
      txt::String16     tmp_16;
      txt::conv_utf8_to_utf16 (tmp_16, filter [pos].c_str ());
      filter_string.insert (filter_string.end (), tmp_16.begin (), tmp_16.end ());
      filter_string.push_back (L'\0');
   }
   if (filter_string.empty ())
   {
      txt::String16     tmp_16;
      tmp_16 = L"All files";
      filter_string.insert (filter_string.end (), tmp_16.begin (), tmp_16.end ());
      filter_string.push_back (L'\0');
      tmp_16 = L"*.*";
      filter_string.insert (filter_string.end (), tmp_16.begin (), tmp_16.end ());
      filter_string.push_back (L'\0');
   }
   filter_string.push_back (L'\0');

   const long     filename_max_size = 32767;
   std::vector <txt::Char16> file_buf;
   file_buf.assign (original_filename_utf16.begin (), original_filename_utf16.end ());
   file_buf.resize (filename_max_size + 1, '\0');
   
   std::vector <txt::Char16> path_name;
   path_name.assign (original_path_utf16.begin (), original_path_utf16.end ());
   path_name.resize (filename_max_size + 1, '\0');
   
   OPENFILENAMEW  ofn;
   ofn.lStructSize = sizeof (ofn);
   ofn.hwndOwner = ::GetForegroundWindow ();
   ofn.hInstance = 0;
   ofn.lpstrFilter = &filter_string [0];
   ofn.lpstrCustomFilter = 0;
   ofn.nMaxCustFilter = 0;
   ofn.nFilterIndex = 1;
   ofn.lpstrFile = &file_buf [0];
   ofn.nMaxFile = filename_max_size;
   ofn.lpstrFileTitle = 0;
   ofn.nMaxFileTitle = 0;
   ofn.lpstrInitialDir = &path_name [0];
   ofn.lpstrTitle = window_title_utf16.c_str ();
   ofn.Flags = OFN_ENABLESIZING | OFN_LONGNAMES | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;
   ofn.nFileOffset = 0;
   ofn.nFileExtension = 0;
   ofn.lpstrDefExt = L"";  // Poorly documented feature: the string content is
   // actually ignored and replaced by the extension of the selected filter.
   ofn.lCustData = 0;
   ofn.lpfnHook = 0;
   ofn.lpTemplateName = 0;
#if (_WIN32_WINNT >= 0x0500)
   ofn.dwReserved = 0;  
   ofn.pvReserved = NULL;
#endif // (_WIN32_WINNT >= 0x0500)

   int ret = ::GetSaveFileNameW (&ofn);
   
   if (ret == 0)
   {
      // cancel or error
      path.clear ();
   }
   else
   {
      // ok
      const int      ret = txt::conv_utf16_to_utf8 (path, ofn.lpstrFile);
      if (ret != txt::Err_OK)
      {
         path.clear ();
      }
   }
}



void  ApplicationImpl::set_hinst (::HINSTANCE hinst)
{
   assert (_hinst == 0);

   _hinst = hinst;
   
   assert (_clipboard_hwnd == 0);
   
   // register the clipboard windows, and create the window
   
   if (_clipboard_hwnd == 0)
   {
      ::WNDCLASSEXW wcex;

      wcex.cbSize          = sizeof (wcex); 
      wcex.style           = CS_DBLCLKS;
      wcex.lpfnWndProc     = handle_event_proc;
      wcex.cbClsExtra      = 0;
      wcex.cbWndExtra      = 0;
      wcex.hInstance       = _hinst;
      wcex.hIcon           = 0;
      wcex.hCursor         = 0;
      wcex.hbrBackground   = HBRUSH (COLOR_APPWORKSPACE + 1);
      wcex.lpszMenuName    = 0;
      wcex.lpszClassName   = _clipboard_class_name_0;
      wcex.hIconSm         = 0;

      if (::RegisterClassExW (&wcex) == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "set_hinst(): Cannot register clipboard window class. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         assert (false);
      }
      else
      {
         _clipboard_hwnd = ::CreateWindowExW (
            0, // style_ex,
            _clipboard_class_name_0,
            L"",
            0, //style,
            0, 0,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0, // parent window
            0,
            _hinst,
            0
         );
         if (_clipboard_hwnd == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "set_hinst(): Cannot create clipboard window. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            assert (false);
         }

         ::ShowWindow (_clipboard_hwnd, SW_HIDE);
      }
   }
}



::HINSTANCE ApplicationImpl::get_hinst ()
{
   assert (_hinst != 0);
   
   return _hinst;
}



View *   ApplicationImpl::get_keyboard_focus_ptr ()
{
   return 0;
}



void  ApplicationImpl::synthetize_events (View & view)
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ApplicationImpl::ApplicationImpl ()
:  _hinst (0)
,  _clipboard_hwnd (0)
,  _cursor_hidden_flag (false)
{
   task::RunLoopGuiImpl & run_loop = task::RunLoopGui::use_instance ().use_impl ();
   
   run_loop.bind_crumb (handle_crumb_proc, this);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const txt::Char16 ApplicationImpl::_clipboard_class_name_0 [] = L"Ohm_Force_Opal_ApplicationImpl_Clip_Class_Name";


/*
==============================================================================
Name : handle_event_proc
Throws : Nothing
==============================================================================
*/

LRESULT CALLBACK  ApplicationImpl::handle_event_proc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
   LRESULT ret_val = 0;

   __try
   {
      // trace (hwnd, message, wparam, lparam);
      
      ret_val = ::DefWindowProcW (hwnd, message, wparam, lparam);
   }
   __except (sys::win32::ExceptionHandler::exec (GetExceptionInformation ()))
   {
      assert (false);
   }


   return ret_val;
}



/*
==============================================================================
Name : handle_crumb_proc
==============================================================================
*/

void  ApplicationImpl::handle_crumb_proc (const ::MSG & msg, void * user_data)
{
   assert (user_data != 0);
   
   ApplicationImpl * this_ptr = (ApplicationImpl *)user_data;
   
   this_ptr->handle_crumb (msg);
}



/*
==============================================================================
Name : handle_crumb
==============================================================================
*/

void  ApplicationImpl::handle_crumb (const ::MSG & msg)
{
   HIDEvent hid_event;
   
   switch (msg.message)
   {
   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
   case WM_KEYUP:
   case WM_SYSKEYUP:
      handle_crumb_keyboard (hid_event, msg);
      break;

   case WM_CHAR:
   case WM_UNICHAR:
      handle_crumb_character (hid_event, msg);
      break;
   }

   Application::Event event;
   event._type = Application::Event::Type_HID_EVENT_ARR;
   event._hid_event_arr.push_back (hid_event);
   
   Application::use_instance ().req_signal (event);
}



/*
==============================================================================
Name : handle_crumb_keyboard
==============================================================================
*/

void  ApplicationImpl::handle_crumb_keyboard (HIDEvent & event, const ::MSG & msg)
{
   const bool repeated_flag = ((msg.lParam >> 30) & 1) == 1;
   
   switch (msg.message)
   {
   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
      if (repeated_flag)
      {
         event._type = HIDEvent::Type_KEY_REPEATED;
      }
      else
      {
         event._type = HIDEvent::Type_KEY_PRESSED;
      }
      break;

   case WM_KEYUP:
   case WM_SYSKEYUP:
      event._type = HIDEvent::Type_KEY_RELEASED;
      break;
   }
   
   event._key = Key (msg.wParam, msg.lParam);
}



/*
==============================================================================
Name : handle_crumb_character
==============================================================================
*/

void  ApplicationImpl::handle_crumb_character (HIDEvent & event, const ::MSG & msg)
{
   event._type = HIDEvent::Type_CHARACTERS_TYPED;
   
   ohm::txt::unicode::Char ucs4 = msg.wParam;
   
   std::string utf8;
   int err = ohm::txt::utf8::Codec8::encode_char (utf8, ucs4);
   assert (err == 0);
   
   event._str = utf8;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  ApplicationImpl::trace (::HWND hwnd, ::UINT message, ::WPARAM wparam, ::LPARAM lparam)
{
   ohm_util_TRACE_INFO1 ("ApplicationImpl::trace WM %1%", message);  
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
