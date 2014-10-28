/*****************************************************************************

        FpsDisplay.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54753

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



#if ! defined (ohm_opak_FpsDisplay_HEADER_INCLUDED)
#define  ohm_opak_FpsDisplay_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeColor.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/private/ClockListener.h"
#include "ohm/opal/LabelString.h"



namespace ohm
{
namespace opak
{



class FpsDisplay
:  public virtual Primitive
,  public opal::ClockListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FpsDisplay ();
   
   // Video Thread
   virtual void   init_display ();
   virtual void   restore_display ();
   virtual void   display (GraphicContext & context);

   // ClockListener in Video Thread
   virtual void   init (double timestamp);
   virtual void   tick (double timestamp);
   
   opak::TypeColor <FpsDisplay>
                  _color;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~FpsDisplay ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           color_changed ();
   
   double         _last_time;
   int            _hundred;
   int            _ten;
   int            _one;
   
   std::vector <opal::Image>
                  _image_arr;
   std::vector <opa::Point>
                  _offset_arr;
   float          _num_width;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FpsDisplay (const FpsDisplay &other);
   FpsDisplay &   operator = (const FpsDisplay &other);
   bool           operator == (const FpsDisplay &other);
   bool           operator != (const FpsDisplay &other);

}; // class FpsDisplay



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/FpsDisplay.hpp"



#endif   // ohm_opak_FpsDisplay_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
