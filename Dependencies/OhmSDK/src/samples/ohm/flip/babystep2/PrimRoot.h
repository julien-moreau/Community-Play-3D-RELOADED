/*****************************************************************************

        PrimRoot.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70401

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



#if ! defined (PrimRoot_HEADER_INCLUDED)
#define  PrimRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeColor.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypePoint.h"



class PrimRoot
// inherit the capability to embed other Primitives
:  public virtual ohm::opak::PrimitiveContainer
// inherit the capability to have a concept of size
,  public virtual ohm::opak::Sizable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimRoot ();
   
   // Video Thread
   virtual void   display (ohm::opak::GraphicContext & context);
   
   ohm::opak::TypeColor <PrimRoot>
                  _color;
   ohm::opak::TypeFloat <PrimRoot>
                  _alpha;
   ohm::opak::TypePoint <PrimRoot>
                  _point;
   ohm::opak::TypeFloat <PrimRoot>
                  _sprung;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~PrimRoot ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           prepare_display (ohm::opak::GraphicContext & context);
   void           display_rect (ohm::opak::GraphicContext & context);
   void           display_image (ohm::opak::GraphicContext & context);
   void           display_sprung_image (ohm::opak::GraphicContext & context);

   void           color_changed ();
   void           something_changed ();
   
   ohm::opal::Image
                  _image;
   ohm::opal::SprungImage
                  _sprung_image;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimRoot (const PrimRoot &other);
   PrimRoot &     operator = (const PrimRoot &other);
   bool           operator == (const PrimRoot &other);
   bool           operator != (const PrimRoot &other);

}; // class PrimRoot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // PrimRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
