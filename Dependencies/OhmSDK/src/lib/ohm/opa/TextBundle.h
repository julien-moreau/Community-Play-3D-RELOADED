/*****************************************************************************

        TextBundle.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36953

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



#if ! defined (ohm_opa_TextBundle_HEADER_INCLUDED)
#define  ohm_opa_TextBundle_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Bundle.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opa/Rect.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/TextField.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opal/TextFrame.h"



namespace ohm
{
namespace opa
{



class TextBundle
:  public Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TextBundle (Bundle & parent_bundle, size_t bundle_slot_nbr = 0);
   virtual        ~TextBundle ();

   void           set_origin (const Point & point);
   void           set_bounds (const Size & size);

   void           push_state_font (const opal::Font & font);
   void           clear_state_font ();
   void           set_state_color (const opal::Color & color);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   std::auto_ptr <opal::TextFrame>
                  _frame_aptr;
   opak::AutoPrimPtr <opak::TextField>
                  _text_field_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TextBundle ();
                  TextBundle (const TextBundle &other);
   TextBundle &      operator = (const TextBundle &other);
   bool           operator == (const TextBundle &other);
   bool           operator != (const TextBundle &other);

}; // class TextBundle



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opa_TextBundle_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
