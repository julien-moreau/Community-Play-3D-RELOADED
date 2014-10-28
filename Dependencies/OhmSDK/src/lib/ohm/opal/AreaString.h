/*****************************************************************************

        AreaString.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58260

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



#if ! defined (ohm_opal_AreaString_HEADER_INCLUDED)
#define  ohm_opal_AreaString_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Typesetter.h"

#include <string>



namespace ohm
{
namespace opal
{



class AreaString
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  AreaString ();
                  AreaString (const Scripture & scripture);
   virtual        ~AreaString ();
   
   void           set_bounds (const opa::Size & size);
   const opa::Size &
                  get_bounds () const;
   void           enable_auto_height (bool auto_height_flag = true);

   void           begin_edit ();
   void           end_edit ();
   
   Scripture &    use_scripture ();
   
   float          get_height ();
   const opa::Rect &
                  get_image_bounds () const;
   
   void           draw_selection (GraphicContextInterface & context, size_t first_pos, size_t second_pos, const opal::Color & color) const;
   bool           draw_text (GraphicContextInterface & context) const;
   void           draw_caret (GraphicContextInterface & context, size_t first_pos, size_t second_pos, const opal::Color & color) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           update () const;
   
   mutable opa::Size
                  _size;
   bool           _auto_height_flag;
   
   Scripture      _scripture;
   bool           _editing_flag;

   std::auto_ptr <Typesetter>
                  _typesetter_aptr;
   mutable bool   _typesetter_need_layout_flag;
   mutable bool   _text_cropped_flag;

   mutable opa::Rect
                  _image_bounds;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const AreaString &other);
   bool           operator != (const AreaString &other);

}; // class AreaString



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_AreaString_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
