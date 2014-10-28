/*****************************************************************************

        PrimTaxi.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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



#if ! defined (client_PrimTaxi_HEADER_INCLUDED)
#define  client_PrimTaxi_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/Aspect.h"

#include "model/Taxi.h"

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeEnum.h"



namespace client
{



class PrimTaxi
:  public virtual ohm::opak::Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimTaxi ();
   
   // Video Thread
   virtual void   display (ohm::opak::GraphicContext & context);
   
   ohm::opak::TypeEnum <PrimTaxi, model::Taxi::Color, model::Taxi::Color_WHITE>
                  _color;
   ohm::opak::TypeFloat <PrimTaxi>
                  _width;
   ohm::opak::TypeBool <PrimTaxi>
                  _selected_flag;
   ohm::opak::TypeBool <PrimTaxi>
                  _ext_selected_flag;
   ohm::opak::TypeBool <PrimTaxi>
                  _conflict_flag;
   ohm::opak::TypeFloat <PrimTaxi>
                  _highlight_flag;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~PrimTaxi ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           something_changed ();
   void           width_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimTaxi (const PrimTaxi &other);
   PrimTaxi &  operator = (const PrimTaxi &other);
   bool           operator == (const PrimTaxi &other);
   bool           operator != (const PrimTaxi &other);

}; // class PrimTaxi



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/PrimTaxi.hpp"



#endif   // client_PrimTaxi_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
