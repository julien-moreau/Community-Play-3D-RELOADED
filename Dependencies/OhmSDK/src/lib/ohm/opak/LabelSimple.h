/*****************************************************************************

        LabelSimple.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66502

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



#if ! defined (ohm_opak_LabelSimple_HEADER_INCLUDED)
#define  ohm_opak_LabelSimple_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeColor.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypeString.h"

#include "ohm/opak/base/LabelSimpleHelper.h"



namespace ohm
{
namespace opak
{



class LabelSimple
:  public Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  LabelSimple ();
   
   // Video Thread
   virtual void   restore_display ();
   virtual void   display (GraphicContext & context);

   TypeFloat <LabelSimple>
                  _font_size;
   TypeFloat <LabelSimple>
                  _width;
   TypeString <LabelSimple>
                  _str;
   TypeColor <LabelSimple>
                  _color;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~LabelSimple ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           font_changed ();
   void           bbox_changed ();
   void           str_changed ();
   void           color_changed ();
   
   bool           is_ok ();
   
   const opal::Font *
                  _font_ptr;
   std::auto_ptr <opak::LabelSimpleHelper>
                  _lsh_aptr;
   
   bool           _update_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LabelSimple (const LabelSimple &other);
   LabelSimple &  operator = (const LabelSimple &other);
   bool           operator == (const LabelSimple &other);
   bool           operator != (const LabelSimple &other);

}; // class LabelSimple



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/LabelSimple.hpp"



#endif   // ohm_opak_LabelSimple_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
