/*****************************************************************************

        PngLoader.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 69624

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

#include "ohm/archi/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/opal/private/PngLoader.h"
#include "ohm/opal/bitmap.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <csetjmp>
#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: read_png
==============================================================================
*/

bool  PngLoader::read_png (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_len)
{
   assert (&bitmap != 0);
   assert (data_ptr != 0);
   assert (data_len > 0);

   bool           ok_flag = true;

   png_structp png_ptr = 0;
   png_infop   info_ptr = 0;
   init_png (png_ptr, info_ptr);

   if (setjmp (png_jmpbuf (png_ptr)))
   {
      png_destroy_read_struct (&png_ptr, &info_ptr, 0);
      ohm_util_TRACE_ERROR ("PngLoader::read_png(): decoding error.");
      ok_flag = false;
   }

   // libpng access a non const buffer, but won't write in it.
   
   archi::UByte * cst_data_ptr = const_cast <archi::UByte *> (data_ptr);

   // init via custom read fnc
   
   ReadCbData read_cb_data;
   read_cb_data._data_ptr = cst_data_ptr; 
   read_cb_data._data_size = data_len;
   read_cb_data._cur_pos = 0;

   if (ok_flag)
   {
      png_set_read_fn (
         png_ptr,
         (void *) &read_cb_data, // user data
         read_cb                 // user read fn
      );

      // check png magic
      
      size_t   sig_check_len = lang::min (size_t (8), data_len);
      ok_flag = (png_sig_cmp (cst_data_ptr, 0, sig_check_len) == 0);
   }
   
   if (ok_flag)
   {
      process (bitmap, png_ptr, info_ptr);
   }
   
   restore_png (png_ptr, info_ptr);

   return ok_flag;
}



/*
==============================================================================
Name: read_png
==============================================================================
*/

bool PngLoader::read_png (Bitmap & bitmap, const std::vector <archi::UByte> & data)
{
   assert (&bitmap != 0);
   assert (&data != 0);
   
   bool ok_flag = !data.empty ();
   
   if (ok_flag)
   {
      ok_flag = read_png (bitmap, & data [0], data.size ());
   }
   
   return ok_flag;
}



/*
==============================================================================
Name: read_png
==============================================================================
*/

bool PngLoader::read_png (Bitmap & bitmap, const std::string & path)
{
   assert (&bitmap != 0);
   assert (&path != 0);
   
   bool           ok_flag = true;
   FILE *         fp = 0;

   png_structp png_ptr = 0;
   png_infop info_ptr = 0;
   init_png (png_ptr, info_ptr);

   if (setjmp (png_jmpbuf (png_ptr)))
   {
      png_destroy_read_struct (&png_ptr, &info_ptr, 0);
      ohm_util_TRACE_ERROR ("PngLoader::read_png(): decoding error.");
      ok_flag = false;
      // To check: if fp contains the correct file pointer when
      // we jump here.
   }

   if (ok_flag)
   {
      fp = fopen (path.c_str (), "rb");
      ok_flag = (fp != 0);
   }
   
   if (ok_flag)
   {
      // init via standard io

      png_init_io (png_ptr, fp);

      // check png magic

      unsigned char buf [8];
      
      if (::fread(buf, 1, 8, fp) == 8)
      {
         ok_flag = (png_sig_cmp (buf, 0, 8) == 0);
         png_set_sig_bytes (png_ptr, 8);
      }
      else
      {
         ok_flag = false;
      }
   }

   if (ok_flag)
   {
      process (bitmap, png_ptr, info_ptr);
   }
   
   restore_png (png_ptr, info_ptr);
   
   if (fp != 0)
   {
      fclose (fp);      
   }

   return ok_flag;   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*
==============================================================================
Name: init_png
==============================================================================
*/

void PngLoader::init_png (png_structp & png_ptr, png_infop & info_ptr)
{
   png_ptr = png_create_read_struct (
      PNG_LIBPNG_VER_STRING,
      0, // user error ptr
      0, // user error fn
      0  // user warning fn
   );
   
   if (png_ptr == 0)
   {
      throw std::runtime_error (
         "PngLoader::init_png : png_create_read_struct fails"
      );
   }
   
   info_ptr = png_create_info_struct (png_ptr);
   
   if (info_ptr == 0)
   {
      png_destroy_read_struct (&png_ptr, 0, 0);
      throw std::runtime_error (
         "PngLoader::init_png : png_create_info_struct fails"
      );
   }
}



/*
==============================================================================
Name: restore_png
==============================================================================
*/

void PngLoader::restore_png (png_structp & png_ptr, png_infop & info_ptr)
{
   png_destroy_read_struct (&png_ptr, &info_ptr, 0);
}



/*
==============================================================================
Name: process
==============================================================================
*/

bool PngLoader::process (Bitmap & bitmap, png_structp & png_ptr, png_infop & info_ptr)
{
   assert (&bitmap != 0);
   assert (png_ptr != 0);
   assert (info_ptr != 0);

   png_uint_32 width;
   png_uint_32 height;
   int bit_depth;
   int color_type;
   
   png_read_info (png_ptr, info_ptr);

   png_get_IHDR (
     png_ptr,
     info_ptr,
     &width,
     &height,
     &bit_depth,
     &color_type,
     0,  // interlace type
     0,  // compression method
     0   // filter method
  );

   bitmap.resize (width, height);   
   
#if (ohm_archi_ENDIAN == ohm_archi_ENDIAN_BIG)
   PixArgb * data_ptr = bitmap.data ();
#elif (ohm_archi_ENDIAN == ohm_archi_ENDIAN_LITTLE)
   PixArgbBE * data_ptr = new PixArgbBE [width*height];
#endif

   png_bytep * row_pointer_ptr_arr = new png_bytep [height];
   
   for (size_t i = 0 ; i < height ; ++i)
   {
      row_pointer_ptr_arr [i] = png_bytep (& data_ptr [i * width]);
   }
   
   if (color_type == PNG_COLOR_TYPE_PALETTE)
   {
      png_set_expand (png_ptr);
   }
   
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
   {
      png_set_expand (png_ptr);
   }
   
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
   {
      png_set_expand (png_ptr);
   }
   
   if (bit_depth == 16)
   {
      png_set_strip_16 (png_ptr);
   }
   
   if (
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA
   )
   {
      png_set_gray_to_rgb (png_ptr);
   }
   
   if (color_type == PNG_COLOR_TYPE_RGB)
   {
      png_set_filler (png_ptr, 0xFF, PNG_FILLER_BEFORE);
   }
   
   if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
   {
      png_set_swap_alpha (png_ptr);
   }
   
   png_read_update_info (png_ptr, info_ptr);
   
   png_read_image (png_ptr, row_pointer_ptr_arr);
   
   png_read_end (png_ptr, info_ptr);
   
#if (ohm_archi_ENDIAN == ohm_archi_ENDIAN_LITTLE)

   PixArgb *   bm_data_ptr = bitmap.data ();

   for (size_t i = 0; i < width * height; ++i)
   {
      bm_data_ptr [i] = data_ptr [i];
   }

   delete [] data_ptr;

#endif

   delete [] row_pointer_ptr_arr;
      
   return true;
}



/*
==============================================================================
Name: read_cb
==============================================================================
*/

void PNGAPI PngLoader::read_cb (png_structp png_ptr, png_bytep data_ptr, png_size_t length)
{
   ReadCbData * cb_data_ptr
      = reinterpret_cast <ReadCbData *> (png_get_io_ptr (png_ptr));
   
   const long remaining = cb_data_ptr->_data_size - cb_data_ptr->_cur_pos;
   
   if (remaining < long (length))
   {
      png_error (png_ptr, "Read Error !");
   }
   else
   {
      using namespace std;
      
      memcpy (
        data_ptr,
        &cb_data_ptr->_data_ptr [cb_data_ptr->_cur_pos],
        length
     );
      
      cb_data_ptr->_cur_pos += length;
   }
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

