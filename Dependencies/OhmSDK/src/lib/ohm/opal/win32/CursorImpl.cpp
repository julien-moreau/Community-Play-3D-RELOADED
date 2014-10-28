/*****************************************************************************

        CursorImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 54310

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

#include "ohm/opal/win32/CursorImpl.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opa/Point.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

CursorImpl::CursorImpl ()
:  _hicon (0)
{
}



CursorImpl::~CursorImpl ()
{
   assert (_hicon == 0);
   // to do
}

   

void  CursorImpl::create (const Bitmap & bitmap, const opa::Point & hot_point)
{

   const int   w = bitmap.get_width ();
   const int   h = bitmap.get_height ();
   PixArgb *   data_ptr = 0;

   // the color bitmap

   ::HDC screen_hdc = ::GetDC (0);
   ::HDC hdc = ::CreateCompatibleDC (screen_hdc);
   ::BITMAPINFOHEADER   bitmap_info_header =
   {
      sizeof (bitmap_info_header),
      w,
      -h,
      1,
      32,
      BI_RGB,
      0,
      0,
      0,
      0,
      0
   };

   ::BITMAPINFO   bitmap_info;
   bitmap_info.bmiHeader = bitmap_info_header;

   ::HBITMAP hbitmap = ::CreateDIBSection (
      hdc, &bitmap_info, DIB_RGB_COLORS, (void **) &data_ptr, 0, 0
   );
   assert (hbitmap != 0);
   assert (data_ptr != 0);

   ::memcpy (data_ptr, bitmap.data (), 4 * w * h);
   
   // the mask

   const size_t byte_cnt = (2 * w * h + 15) / 16;
   archi::UByte * mask_ptr = new archi::UByte [byte_cnt];

   ::memset (mask_ptr, 0xFF, byte_cnt);

   ::HBITMAP hbitmap_mask = ::CreateBitmap (
      w, h, 1, 1, (void *) (mask_ptr)
   );
   if (hbitmap_mask == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "create(): Cannot create bitmap mask. "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code
      );
      assert (false);
   }

   else
   {
      // the icon

      ::ICONINFO cursor_info = 
      {
         FALSE,
         DWORD (hot_point._x),
         DWORD (hot_point._y),
         hbitmap_mask,  
         hbitmap
      };

      _hicon = ::CreateIconIndirect (&cursor_info);
      assert (_hicon != 0);

      ::DeleteObject (hbitmap_mask);
   }

   delete [] mask_ptr;
   ::DeleteObject (hbitmap);
   ::DeleteDC (hdc);
   ::ReleaseDC (0, screen_hdc);
}



void  CursorImpl::release ()
{
   if (_hicon != 0)
   {
      BOOL ret_val = ::DestroyIcon (_hicon);
      _hicon = 0;
   }
}


void  CursorImpl::set ()
{
/* ::HCURSOR old_cursor = */
   if (_hicon != 0)
   {
      ::SetCursor (_hicon);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

