/*****************************************************************************

        Bitmap.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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



#if ! defined (ohm_opal_Bitmap_HEADER_INCLUDED)
#define  ohm_opal_Bitmap_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/PixArgb.h"



namespace ohm
{
namespace opal
{


class Color;
class ColorLab;
class ColorProcessorInterface;

class Bitmap
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Bitmap ();
                  Bitmap (const Bitmap &other);
   virtual        ~Bitmap ();

   Bitmap &       operator = (const Bitmap &other);
   
   PixArgb &      operator [] (size_t idx);
   const PixArgb &operator [] (size_t idx) const;
   PixArgb &      operator () (size_t x, size_t y);
   const PixArgb &operator () (size_t x, size_t y) const;
   
   void           resize (size_t width, size_t height);
   void           swap (Bitmap & other);
   void           premultiply ();
   
   size_t         size () const;
   size_t         get_width () const;
   size_t         get_height () const;
   bool           is_fully_transparent () const;
   
   PixArgb *      data ();
   const PixArgb *data () const;
   
   void           clear_rect (size_t x, size_t y, size_t width, size_t height);
   void           colorize_premultiply (const Color & color);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           release ();
   
   size_t         _width;
   size_t         _height;
   PixArgb *      _data_ptr;
   bool           _release_data_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Bitmap &other);
   bool           operator != (const Bitmap &other);

}; // class Bitmap



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Bitmap_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
