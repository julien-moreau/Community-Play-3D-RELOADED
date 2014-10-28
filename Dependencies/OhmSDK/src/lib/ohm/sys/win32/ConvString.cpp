/*****************************************************************************

        ConvString.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 62577

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

#include "ohm/sys/ConvString.h"
#include "ohm/sys/Err.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

#include <vector>

#include <cassert>

#define  NOMINMAX
#define  NOGDI
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>



namespace ohm
{
namespace sys
{



static const int  ConvString_conv_multipurpose (std::string &dst, const char *src_0, int cp_dst, int cp_src, ::BOOL *bad_conv_flag_ptr)
{
   assert (&dst != 0);
   assert (src_0 != 0);

   int            ret_val = Err_OK;

   if (src_0 [0] == '\0')
   {
      dst.erase (dst.begin (), dst.end ());
   }

   else
   {
      // Counts the number of characters
      const int      nbr_widechar = ::MultiByteToWideChar (
         cp_src,
         0,
         src_0,
         -1,
         0,
         0
      );
      std::vector <WCHAR>  buf_w;
      if (nbr_widechar == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "ConvString_conv_multipurpose(): "
            "1st string length retrieval failed. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         ret_val = Err_CANNOT_CONVERT_STRING;
      }
      else
      {
         // Converts the string
         buf_w.resize (nbr_widechar);
         const int      success = ::MultiByteToWideChar (
            cp_src,
            0,
            src_0,
            -1,
            &buf_w [0],
            nbr_widechar
         );
         if (success == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "ConvString_conv_multipurpose(): "
               "1st string conversion failed. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ret_val = Err_CANNOT_CONVERT_STRING;
         }
      }

      int            nbr_char8;
      if (ret_val == Err_OK)
      {
         // Counts the number of characters
         nbr_char8 = ::WideCharToMultiByte (
            cp_dst,
            0,
            &buf_w [0],
            -1,
            0,
            0,
            0,
            0
         );
         if (nbr_char8 == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "ConvString_conv_multipurpose(): "
               "2nd string lenth retrieval failed. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ret_val = Err_CANNOT_CONVERT_STRING;
         }
      }
      std::vector <char>   buf_8;
      if (ret_val == Err_OK)
      {
         // Converts the string
         buf_8.resize (nbr_char8);
         const int      success = ::WideCharToMultiByte (
            cp_dst,
            0,
            &buf_w [0],
            -1,
            &buf_8 [0],
            nbr_char8,
            0,
            bad_conv_flag_ptr
         );
         if (success == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "ConvString_conv_multipurpose(): "
               "2nd string conversion failed. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ret_val = Err_CANNOT_CONVERT_STRING;
         }
      }
      if (ret_val == Err_OK)
      {
         dst = &buf_8 [0];
      }
   }

   return (ret_val);
}



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



int   ConvString::conv_native_string_to_utf8 (std::string &utf8, const char *native_0)
{
   assert (&utf8 != 0);
   assert (native_0 != 0);

   return (ConvString_conv_multipurpose (utf8, native_0, CP_UTF8, CP_ACP, 0));
}



int   ConvString::conv_utf8_string_to_native (std::string &native, const char *utf8_0, bool allow_unmapped_char_flag)
{
   assert (&native != 0);
   assert (utf8_0 != 0);

   ::BOOL         conv_err_val = FALSE;
   int            ret_val = ConvString_conv_multipurpose (
      native,
      utf8_0,
      CP_ACP,
      CP_UTF8,
      (allow_unmapped_char_flag) ? 0 : &conv_err_val
   );

   if (conv_err_val != FALSE)
   {
      ret_val = Err_CANNOT_CONVERT_STRING;
   }

   return (ret_val);
}



int   ConvString::conv_system_string_to_utf8 (std::string &utf8, const char *system_0)
{
   return conv_native_string_to_utf8 (utf8, system_0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
