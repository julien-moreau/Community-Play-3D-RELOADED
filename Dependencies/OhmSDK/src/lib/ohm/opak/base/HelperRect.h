/*****************************************************************************

        HelperRect.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68169

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



#if ! defined (ohm_opak_HelperRect_HEADER_INCLUDED)
#define  ohm_opak_HelperRect_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/Color.h"



namespace ohm
{
namespace opak
{



class GraphicContext;

// DON'T INHERIT FROM THIS CLASS

class HelperRect
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         HelperRect (const opa::Rect & rect);
   inline         HelperRect (const HelperRect &other);
   inline         ~HelperRect ();   // NOT VIRTUAL
   
   inline HelperRect &
                  margin (const opa::Margin & margin);
   inline HelperRect &
                  margin (float left, float top, float right, float bottom);
   inline HelperRect &
                  color (const opal::Color & color);

   inline HelperRect &
                  hint ();
   inline HelperRect &
                  hint_absolute ();

   void           display (opak::GraphicContext & context);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   opa::Rect      _rect;
   opa::Margin    _margin;
   opal::Color    _color;
   bool           _margined_flag;
   bool           _hint_flag;
   bool           _hint_absolute_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HelperRect ();
   HelperRect &   operator = (const HelperRect &other);
   bool           operator == (const HelperRect &other);
   bool           operator != (const HelperRect &other);

}; // class HelperRect



inline HelperRect use (const opa::Rect & rect);
inline HelperRect use (const opa::Point & pt, const opa::Size & sz);
inline HelperRect use (float x, float y, float width, float height);



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/HelperRect.hpp"



#endif   // ohm_opak_HelperRect_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
