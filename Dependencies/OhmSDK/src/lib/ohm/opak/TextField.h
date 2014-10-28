/*****************************************************************************

        TextField.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36388

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



#if ! defined (ohm_opak_TextField_HEADER_INCLUDED)
#define  ohm_opak_TextField_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeScripture.h"
#include "ohm/opak/base/TypeBool.h"
#include "ohm/opak/base/TypeSize.h"
#include "ohm/opak/base/TypeInt.h"
#include "ohm/opak/base/TypeColor.h"
#include "ohm/opal/AreaString.h"
#include "ohm/opal/Cache.h"



namespace ohm
{
namespace opak
{



class TextField
:  public virtual Primitive
,  public Sizable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TextField ();
   
   // Video Thread
   virtual void   display (GraphicContext & context);

   TypeScripture <TextField>
                  _scripture;
   TypeInt <TextField>
                  _caret_first;
   TypeInt <TextField>
                  _caret_second;
   TypeColor <TextField>
                  _caret_color;
   TypeColor <TextField>
                  _selection_color;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~TextField ();
   
   // Sizable
   virtual void   do_size_changed ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           scripture_changed ();
   void           caret_position_changed ();
   void           caret_color_changed ();
   void           selection_color_changed ();
   
   void           update_bbox ();
   
   opal::AreaString
                  _area_string;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TextField (const TextField &other);
   TextField &    operator = (const TextField &other);
   bool           operator == (const TextField &other);
   bool           operator != (const TextField &other);

}; // class TextField



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/TextField.hpp"



#endif   // ohm_opak_TextField_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
