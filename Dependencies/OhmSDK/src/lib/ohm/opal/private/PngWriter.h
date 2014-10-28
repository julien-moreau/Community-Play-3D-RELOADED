/*****************************************************************************

        PngWriter.h
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



#if ! defined (ohm_opal_PngWriter_HEADER_INCLUDED)
#define  ohm_opal_PngWriter_HEADER_INCLUDED

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

class PngWriter
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    write_png (std::vector <archi::UByte> & data, const Bitmap & bitmap);
   
   
   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class WriteCbData
   {
   public:
      std::vector <archi::UByte> *
                  _data_ptr;
   };
   
   static void    init_png (png_structp & png_ptr, png_infop & info_ptr);
   static void    restore_png (png_structp & png_ptr, png_infop & info_ptr);
   
   static void PNGAPI
                  write_cb (png_structp png_ptr, png_bytep data_ptr, png_size_t length);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PngWriter ();
                  PngWriter (const PngWriter &other);
   PngWriter &    operator = (const PngWriter &other);
   bool           operator == (const PngWriter &other) const;
   bool           operator != (const PngWriter &other) const;

}; // class PngWriter



}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/PngWriter.hpp"



#endif   // ohm_opal_PngWriter_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

