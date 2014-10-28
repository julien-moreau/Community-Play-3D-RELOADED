/*****************************************************************************

        PngWriter.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52476

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

#include "ohm/opal/private/PngWriter.h"
#include "ohm/archi/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/opal/bitmap.h"

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
Name: write_png
==============================================================================
*/

bool  PngWriter::write_png (std::vector <archi::UByte> & data, const Bitmap & bitmap)
{
   assert (&bitmap != 0);
   assert (&data != 0);

   png_structp png_ptr = 0;
   png_infop   info_ptr = 0;
   init_png (png_ptr, info_ptr);
   
   png_set_compression_level (png_ptr, Z_BEST_COMPRESSION);
   
   png_set_IHDR (
      png_ptr, info_ptr,
      bitmap.get_width (), bitmap.get_height (), 8, PNG_COLOR_TYPE_RGBA,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
   );
   
   // init via custom write fnc
   
   WriteCbData write_cb_data;
   write_cb_data._data_ptr = &data; 

   png_set_write_fn (
      png_ptr,
      (void *) &write_cb_data,   // user data
      write_cb,                  // user write fn
      0                          // user flush fn
   );
   
   std::vector <png_bytep> row_pointer_ptr_arr;

   row_pointer_ptr_arr.resize (bitmap.get_height ());

   for (size_t i = 0 ; i < bitmap.get_height () ; ++i)
   {
      const PixArgb * pix_ptr = &bitmap (0, i);
      
      PixArgb * nc_pix_ptr = const_cast <PixArgb *> (pix_ptr);
      
      row_pointer_ptr_arr [i] = reinterpret_cast <png_bytep> (nc_pix_ptr);
   }
   
   png_set_rows (png_ptr, info_ptr, &row_pointer_ptr_arr [0]);
   
   // png_set_swap_alpha (png_ptr);
   
   png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);
   
   png_write_end (png_ptr, info_ptr);

   restore_png (png_ptr, info_ptr);
   
   return true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*
==============================================================================
Name: init_png
==============================================================================
*/

void PngWriter::init_png (png_structp & png_ptr, png_infop & info_ptr)
{
   png_ptr = png_create_write_struct (
      PNG_LIBPNG_VER_STRING,
      0, // user error ptr
      0, // user error fn
      0  // user warning fn
   );
   
   if (png_ptr == 0)
   {
      throw std::runtime_error (
         "PngWriter::init_png : png_create_write_struct fails"
      );
   }
   
   info_ptr = png_create_info_struct (png_ptr);
   
   if (info_ptr == 0)
   {
      png_destroy_write_struct (&png_ptr, 0);
      throw std::runtime_error (
         "PngWriter::init_png : png_create_write_struct fails"
      );
   }
   else if (setjmp (png_jmpbuf (png_ptr)))
   {
      png_destroy_write_struct (&png_ptr, &info_ptr);
      
      throw std::runtime_error (
         "PngWriter::init_png : setjmp fails"
      );
   }  
}



/*
==============================================================================
Name: restore_png
==============================================================================
*/

void PngWriter::restore_png (png_structp & png_ptr, png_infop & info_ptr)
{
   png_destroy_write_struct (&png_ptr, &info_ptr);
}



/*
==============================================================================
Name: write_cb
==============================================================================
*/

void PNGAPI PngWriter::write_cb (png_structp png_ptr, png_bytep data_ptr, png_size_t length)
{
   WriteCbData * cb_data_ptr
      = reinterpret_cast <WriteCbData *> (png_get_io_ptr (png_ptr));
   assert (cb_data_ptr != 0);
   assert (cb_data_ptr->_data_ptr != 0);
   
   cb_data_ptr->_data_ptr->insert (
      cb_data_ptr->_data_ptr->end (),
      data_ptr, data_ptr + length
   );
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

