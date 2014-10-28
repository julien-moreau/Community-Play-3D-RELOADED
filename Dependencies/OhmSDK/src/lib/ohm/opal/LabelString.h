/*****************************************************************************

        LabelString.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50133

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



#if ! defined (ohm_opal_LabelString_HEADER_INCLUDED)
#define  ohm_opal_LabelString_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opal/Font.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Cache.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/txt/unicode/StringU.h"

#include <string>



namespace ohm
{
namespace opal
{



class LabelString
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  LabelString ();
                  LabelString (const Scripture & scripture);
                  LabelString (const std::string & font_name, float font_size, const std::string & str, const Color & color);
                  LabelString (const std::string & font_name, float font_size, const txt::unicode::StringU & ustr, const Color & color);
                  LabelString (const Font & font, const std::string & str, const Color & color);
                  LabelString (const Font & font, const txt::unicode::StringU & ustr, const Color & color);
   virtual        ~LabelString ();

   void           begin_edit ();
   void           end_edit ();
   
   Scripture &    use_scripture ();
   
   const opa::Rect &
                  get_typographic_bounds () const;
   const opa::Rect &
                  get_image_bounds () const;
   
   void           draw (GraphicContextInterface & context) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           to_ustr (txt::unicode::StringU & ustr, const std::string & utf8) const;
   void           update () const;
   
   Scripture      _scripture;
   bool           _editing_flag;

   std::auto_ptr <Typesetter>
                  _typesetter_aptr;
   mutable bool   _typesetter_need_layout_flag;
   
   mutable opa::Rect
                  _typographic_bounds;
   mutable opa::Rect
                  _image_bounds;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const LabelString &other);
   bool           operator != (const LabelString &other);

}; // class LabelString



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_LabelString_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
