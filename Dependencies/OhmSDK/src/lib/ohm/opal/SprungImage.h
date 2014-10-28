/*****************************************************************************

        SprungImage.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 59357

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



#if ! defined (ohm_opal_SprungImage_HEADER_INCLUDED)
#define  ohm_opal_SprungImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Image.h"
#include "ohm/opal/SprungMode.h"
#include "ohm/opal/FilterMode.h"
#include "ohm/opal/private/Engine.h"

#include <vector>



namespace ohm
{
namespace opa
{
class Rect;
}



namespace opal
{



class Bitmap;

class SprungImage
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Position
   {
                  Position_FIRST = 0,
                  Position_MIDDLE,
                  Position_LAST,
                  
                  NBR_Position_TOTAL
   };
                  SprungImage ();
                  SprungImage (const SprungImage & other);
                  SprungImage (GraphicContextInterface & gc, const Bitmap & bitmap, const opa::Rect & rect, SprungMode sprung_mode_x = SprungMode_DISTEND, SprungMode sprung_mode_y = SprungMode_DISTEND, FilterMode filter_mode = FilterMode_LINEAR);
   SprungImage &  operator = (const SprungImage & other);
   virtual        ~SprungImage ();
   
   bool           empty () const;
   size_t         get_width () const;
   size_t         get_height () const;

   size_t         get_col_width (Position position) const;
   size_t         get_row_height (Position position) const;
   
   void           get_cols_width (size_t & first, size_t & middle, size_t & last, const opa::Rect & rect) const;
   void           get_rows_height (size_t & first, size_t & middle, size_t & last, const opa::Rect & rect) const;
   
   const Image &  operator () (Position col, Position row) const;
   Image &        operator () (Position col, Position row);
   bool           operator == (const SprungImage & other) const;
   bool           operator != (const SprungImage & other) const;

   void           retain ();
   void           release ();
   void           release_check_total ();
   
   void           hole_center ();
   
   Engine *       _eng_ptr;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum
   {
                  NBR_IMAGE = 9,
   };
   
   size_t         _col_width [NBR_Position_TOTAL];
   size_t         _row_height [NBR_Position_TOTAL];
   Image          _image_arr [NBR_IMAGE];



};


}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/SprungImage.hpp"



#endif   // ohm_opal_SprungImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
