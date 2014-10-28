/*****************************************************************************

        PngLoader.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 39021

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



#if ! defined (ohm_opal_PngLoader_HEADER_INCLUDED)
#define  ohm_opal_PngLoader_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "libpng/png.h"
#include "ohm/archi/types.h"

#include <string>
#include <vector>



namespace ohm
{
namespace opal
{


class Bitmap;

class PngLoader
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    read_png (Bitmap & bitmap, const archi::UByte * data_ptr, size_t data_len);
   static bool    read_png (Bitmap & bitmap, const std::vector <archi::UByte> & data);
   static bool    read_png (Bitmap & bitmap, const std::string & path);
   
   
   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class ReadCbData
   {
   public:
      archi::UByte *
                  _data_ptr;
      long        _data_size;
      long        _cur_pos;
   };
   
   static void    init_png (png_structp & png_ptr, png_infop & info_ptr);
   static void    restore_png (png_structp & png_ptr, png_infop & info_ptr);
   static bool    process (Bitmap & bitmap, png_structp & png_ptr, png_infop & info_ptr);
   
   static void PNGAPI
                  read_cb (png_structp png_ptr, png_bytep data_ptr, png_size_t length);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PngLoader ();
                  PngLoader (const PngLoader &other);
   PngLoader &    operator = (const PngLoader &other);
   bool           operator == (const PngLoader &other) const;
   bool           operator != (const PngLoader &other) const;

}; // class PngLoader



}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/PngLoader.hpp"



#endif   // ohm_opal_PngLoader_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

