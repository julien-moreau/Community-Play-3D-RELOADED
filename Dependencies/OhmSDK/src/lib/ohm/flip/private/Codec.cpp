/*****************************************************************************

        Codec.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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

#include "ohm/flip/private/Codec.h"
#include "ohm/txt/ascii/ConvDigit.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/Base64.h"

#include "zlib.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : encode
==============================================================================
*/

int   Codec::encode (Data & dst, const archi::UByte * src_ptr, size_t src_size, const std::vector <Encoder> & encoder_arr)
{
   const int encoder_arr_size = encoder_arr.size ();
   
   if (encoder_arr_size == 0)
   {
      dst.assign (src_ptr, src_ptr + src_size);
      
      return Err_OK;
   }
   
   int err = Err_OK;
   
   if (encoder_arr_size == 1)
   {
      err = encode (dst, src_ptr, src_size, encoder_arr [0]);
      if (err != Err_OK)   return err;
   }
   else if (encoder_arr_size % 2 == 1)
   {
      Data buf_0;
      Data buf_1;
      
      size_t loop = (encoder_arr_size - 1) / 2;
      
      size_t encoder_nbr = 0;
      
      err = encode (buf_0, src_ptr, src_size, encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
      ++encoder_nbr;
      
      for (size_t i = 0 ; i < loop ; ++i)
      {
         err = encode (buf_1, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         ++encoder_nbr;
      
         Data & buf = (i == loop - 1) ? dst : buf_0;
         err = encode (buf, &buf_1 [0], buf_1.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         ++encoder_nbr;
      }
   }
   else
   {
      Data buf_0;
      Data buf_1;
      
      size_t loop = (encoder_arr_size - 2) / 2;
      
      size_t encoder_nbr = 0;
      
      err = encode (buf_0, src_ptr, src_size, encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
      ++encoder_nbr;
      
      for (size_t i = 0 ; i < loop ; ++i)
      {
         err = encode (buf_1, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         ++encoder_nbr;
      
         err = encode (buf_0, &buf_1 [0], buf_1.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         ++encoder_nbr;
      }

      err = encode (dst, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
   }
   
   return err;
}



/*
==============================================================================
Name : decode
==============================================================================
*/

int   Codec::decode (Data & dst, const archi::UByte * src_ptr, size_t src_size, const std::vector <Encoder> & encoder_arr)
{
   const int encoder_arr_size = encoder_arr.size ();
   
   if (encoder_arr_size == 0)
   {
      dst.assign (src_ptr, src_ptr + src_size);
      
      return Err_OK;
   }
   
   int err = Err_OK;
   
   if (encoder_arr_size == 1)
   {
      err = decode (dst, src_ptr, src_size, encoder_arr [0]);
      if (err != Err_OK)   return err;
   }
   else if (encoder_arr_size % 2 == 1)
   {
      Data buf_0;
      Data buf_1;
      
      size_t loop = (encoder_arr_size - 1) / 2;
      
      size_t encoder_nbr = encoder_arr_size - 1;
      
      err = decode (buf_0, src_ptr, src_size, encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
      --encoder_nbr;
      
      for (size_t i = 0 ; i < loop ; ++i)
      {
         err = decode (buf_1, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         --encoder_nbr;
      
         Data & buf = (i == loop - 1) ? dst : buf_0;
         err = decode (buf, &buf_1 [0], buf_1.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         --encoder_nbr;
      }
   }
   else
   {
      Data buf_0;
      Data buf_1;
      
      size_t loop = (encoder_arr_size - 2) / 2;
      
      size_t encoder_nbr = encoder_arr_size - 1;
      
      err = decode (buf_0, src_ptr, src_size, encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
      --encoder_nbr;
      
      for (size_t i = 0 ; i < loop ; ++i)
      {
         err = decode (buf_1, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         --encoder_nbr;
      
         err = decode (buf_0, &buf_1 [0], buf_1.size (), encoder_arr [encoder_nbr]);
         if (err != Err_OK)   return err;
         --encoder_nbr;
      }

      err = decode (dst, &buf_0 [0], buf_0.size (), encoder_arr [encoder_nbr]);
      if (err != Err_OK)   return err;
   }
   
   return err;
}



/*
==============================================================================
Name : encode
==============================================================================
*/

int   Codec::encode (Data & dst, const archi::UByte * src_ptr, size_t src_size, Encoder encoder)
{
   int err = Err_OK;
   
   switch (encoder)
   {
   case Encoder_BASE64:
      err = encode_base64 (dst, src_ptr, src_size);
      break;

   case Encoder_DEFLATE:
      err = encode_deflate (dst, src_ptr, src_size);
      break;

   case Encoder_ESCAPED_ASCII:
      err = encode_escaped_ascii (dst, src_ptr, src_size);
      break;
   
   case Encoder_HEXADECIMAL:
      err = encode_hexadecimal (dst, src_ptr, src_size);
      break;
   
   default:
      err = Err_CODEC_UNKNOWN;
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : decode
==============================================================================
*/

int   Codec::decode (Data & dst, const archi::UByte * src_ptr, size_t src_size, Encoder encoder)
{
   int err = Err_OK;
   
   switch (encoder)
   {
   case Encoder_BASE64:
      err = decode_base64 (dst, src_ptr, src_size);
      break;

   case Encoder_DEFLATE:
      err = decode_deflate (dst, src_ptr, src_size);
      break;

   case Encoder_ESCAPED_ASCII:
      err = decode_escaped_ascii (dst, src_ptr, src_size);
      break;
   
   case Encoder_HEXADECIMAL:
      err = decode_hexadecimal (dst, src_ptr, src_size);
      break;
   
   default:
      err = Err_CODEC_UNKNOWN;
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : encode_base64
==============================================================================
*/

int   Codec::encode_base64 (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   std::string dst_base64;
   util::Base64::encode (dst_base64, src_ptr, src_size, 76, "\n");
   
   dst.resize (dst_base64.size ());
   memcpy (&dst [0], &dst_base64 [0], dst_base64.size ());
   
   return Err_OK;
}



/*
==============================================================================
Name : encode_deflate
==============================================================================
*/

int   Codec::encode_deflate (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   dst.resize (compressBound (src_size) + 4);
         
   uLongf dest_len = dst.size () - 4;
   
   int err = compress2 (
      &dst [4], &dest_len,
      src_ptr, src_size,
      9
   );
   
   assert (err == Z_OK);
   if (err != Z_OK)
   {
      return Err_CODEC_DEFLATE;
   }
   
   dst.resize (dest_len + 4);
   
   dst [0] = src_size >> 24;
   dst [1] = src_size >> 16;
   dst [2] = src_size >> 8;
   dst [3] = src_size;
   
   return Err_OK;
}



/*
==============================================================================
Name : encode_escaped_ascii
==============================================================================
*/

int   Codec::encode_escaped_ascii (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   for (size_t i = 0 ; i < src_size ; ++i)
   {
      archi::UByte c = src_ptr [i];
      
      // from space to ~
      if ((c >= 32) && (c <= 126) && c != '\\')
      {
         dst.push_back (c);
      }
      else if (c == '\\')
      {
         dst.push_back ('\\');
         dst.push_back ('\\');
      }
      else if (c == '\n')
      {
         dst.push_back ('\\');
         dst.push_back ('n');
      }
      else if (c == '\r')
      {
         dst.push_back ('\\');
         dst.push_back ('r');
      }
      else if (c == '\t')
      {
         dst.push_back ('\\');
         dst.push_back ('t');
      }
      else
      {
         dst.push_back ('\\');
         dst.push_back ('x');
         
         archi::UByte c0 = (c >> 4) & 15;
         
         if (c0 < 10)
         {
            dst.push_back ('0' + c0);
         }
         else
         {
            dst.push_back ('a' + c0 - 10);
         }
         
         archi::UByte c1 = c & 15;
         
         if (c1 < 10)
         {
            dst.push_back ('0' + c1);
         }
         else
         {
            dst.push_back ('a' + c1 - 10);
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : encode_hexadecimal
==============================================================================
*/

int   Codec::encode_hexadecimal (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   char buf_0 [3];
   txt::out::PrintNa printer (buf_0, 2);

   for (size_t i = 0 ; i < src_size ; ++i)
   {
      archi::UByte c = src_ptr [i];
      
      printer.print ("%1m2rw0x%").arg (c);
      
      dst.push_back (buf_0 [0]);
      dst.push_back (buf_0 [1]);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : decode_base64
==============================================================================
*/

int   Codec::decode_base64 (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   std::string    src_base64;
   
   for (size_t i = 0 ; i < src_size ; ++i)
   {
      src_base64 += char (src_ptr [i]);
   }
   
   util::Base64::decode (dst, src_base64);
   
   return Err_OK;
}



/*
==============================================================================
Name : decode_deflate
==============================================================================
*/

int   Codec::decode_deflate (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   if (src_size < 4)
   {
      return Err_CODEC_BOOTLEG;
   }
   
   archi::UInt32 dst_size = 0;
   dst_size += (src_ptr [0] << 24);
   dst_size += (src_ptr [1] << 16);
   dst_size += (src_ptr [2] << 8);
   dst_size += (src_ptr [3]);
   
   dst.resize (dst_size);
   
   uLongf dest_len = dst_size;
   
   int err = uncompress (
      &dst [0], &dest_len,
      &src_ptr [4], src_size - 4
   );
   
   assert (err == Z_OK);
   if (err != Z_OK)
   {
      return Err_CODEC_DEFLATE;
   }
   
   if (dst.size () != dest_len)
   {
      return Err_CODEC_BOOTLEG;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : decode_escaped_ascii
==============================================================================
*/

int   Codec::decode_escaped_ascii (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   for (size_t i = 0 ; i < src_size ; ++i)
   {
      archi::UByte c = src_ptr [i];
      
      if (c == '\\')
      {
         ++i;
         
         if (i >= src_size)
         {
            return Err_CODEC_BOOTLEG;
         }
         
         c = src_ptr [i];
         
         if (c == '\\')
         {
            dst.push_back ('\\');
         }
         else if (c == 'n')
         {
            dst.push_back ('\n');
         }
         else if (c == 'r')
         {
            dst.push_back ('\r');
         }
         else if (c == 't')
         {
            dst.push_back ('\t');
         }
         else
         {
            if (c != 'x')
            {
               return Err_CODEC_BAD_DATA;
            }
            
            ++i;
            
            if (i >= src_size)
            {
               return Err_CODEC_BOOTLEG;
            }
         
            c = src_ptr [i];
               
            archi::UByte c0 = 0;
            
            if ((c >= '0') && (c <= '9'))
            {
               c0 = c - '0';
            }
            else if ((c >= 'a') && (c <= 'f'))
            {
               c0 = c - 'a' + 10;
            }
            else
            {
               return Err_CODEC_BAD_DATA;
            }
            
            ++i;
         
            if (i >= src_size)
            {
               return Err_CODEC_BOOTLEG;
            }
            
            c = src_ptr [i];
            
            archi::UByte c1;
            
            if ((c >= '0') && (c <= '9'))
            {
               c1 = c - '0';
            }
            else if ((c >= 'a') && (c <= 'f'))
            {
               c1 = c - 'a' + 10;
            }
            else
            {
               return Err_CODEC_BAD_DATA;
            }
            
            dst.push_back ((c0 << 4) | c1);
         }
      }
      else
      {
         dst.push_back (c);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : decode_hexadecimal
==============================================================================
*/

int   Codec::decode_hexadecimal (Data & dst, const archi::UByte * src_ptr, size_t src_size)
{
   for (size_t i = 0 ; i < src_size ; i += 2)
   {
      if (i + 1 >= src_size)
      {
         return Err_CODEC_BOOTLEG;
      }
      
      archi::UByte c0 = src_ptr [i];
      archi::UByte c1 = src_ptr [i + 1];
      
      bool ok_flag = ((c0 >= '0') && (c0 <= '9')) || ((c0 >= 'a') && (c0 <= 'f'));
      
      if (!ok_flag)
      {
         return Err_CODEC_BAD_DATA;
      }
      
      ok_flag = ((c1 >= '0') && (c1 <= '9')) || ((c1 >= 'a') && (c1 <= 'f'));
      
      if (!ok_flag)
      {
         return Err_CODEC_BAD_DATA;
      }
      
      archi::UByte b0 = txt::ascii::ConvDigit::conv_char_to_digit (c0);
      archi::UByte b1 = txt::ascii::ConvDigit::conv_char_to_digit (c1);
      archi::UByte b = (b0 << 4) | b1;
      
      dst.push_back (b);
   }
   
   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
