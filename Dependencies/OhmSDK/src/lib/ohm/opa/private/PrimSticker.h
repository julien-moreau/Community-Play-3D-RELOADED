/*****************************************************************************

        PrimSticker.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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



#if ! defined (ohm_opa_PrimSticker_HEADER_INCLUDED)
#define  ohm_opa_PrimSticker_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/Opacifiable.h"
#include "ohm/opak/base/TypeBool.h"
#include "ohm/opak/base/TypeColor.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypeFont.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opal/AreaString.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opa/prim/PrimWidget.h"



namespace ohm
{
namespace opa
{



class PrimSticker
:  public virtual PrimWidget
,  public opak::Sizable
//,   public opak::Opacifiable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum State
   {
      State_NORMAL = 0,
      State_ROLLOVER,
      State_CLICKED
   };
   
                  PrimSticker ();
   
   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   

   opak::TypeEnum <PrimSticker, State, State_NORMAL>
                  _state;
   opak::TypeBool <PrimSticker>
                  _signaled_flag;
   opak::TypeColor <PrimSticker>
                  _color_non_signaled;
   opak::TypeColor <PrimSticker>
                  _color_signaled;
   opak::TypeColor <PrimSticker>
                  _color_label;
   opak::TypeString <PrimSticker>
                  _label;
   opak::TypeFont <PrimSticker>
                  _font;
   opak::TypeFloat <PrimSticker>
                  _str_offset;
                  
                  
                  
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimSticker ();
   
   // opak::Sizable
   virtual void   do_size_changed ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           label_changed ();
   void           invalidate_on_change ();

   opal::AreaString
                  _area_string;
   opal::SprungImage
                  _mask;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimSticker (const PrimSticker &other);
   PrimSticker &  operator = (const PrimSticker &other);
   bool           operator == (const PrimSticker &other);
   bool           operator != (const PrimSticker &other);

}; // class PrimSticker



}  // namespace opa
}  // namespace ohm



//#include  "PrimSticker.hpp"



#endif   // ohm_opa_PrimSticker_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

