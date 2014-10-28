/*****************************************************************************

        Taxi.h
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



#if ! defined (model_Taxi_HEADER_INCLUDED)
#define  model_Taxi_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/SpacialObject.h"
#include "model/Range.h"

#include "ohm/flip/Enum.h"



namespace model
{



class Taxi
:  public SpacialObject
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Color
   {
                  Color_UNDEFINED = -1,
                  
                  Color_WHITE = 0,
                  Color_BLACK,
                  Color_YELLOW,
                  Color_BLUE,
                  Color_GREEN,
                  
                  Color_NBR_ELT
   };
   
   static void    declare ();

                  Taxi (ohm::flip::DocumentBase & document);
   virtual        ~Taxi ();
   
   void           ext_set_width (ohm::archi::Float64 width);
   bool           width_changed () const;
   ohm::archi::Float64
                  get_width () const;
   
   static const char *
                  get_color_name_0 (Color color);
   void           ext_set_color (Color color);
   bool           color_changed () const;
   Color          get_color () const;

   void           intersect_width_delta_range (RangeFloat64 & range);
   
   static const ohm::archi::Float64
                  _min_width;
   static const ohm::archi::Float64
                  _max_width;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   ohm::flip::Float64
                  _width;
   ohm::flip::Enum <Color, Color_WHITE, Color_NBR_ELT>
                  _color;

   ohm::flip::TxSessionGuard
                  _tx_session_guard;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Taxi ();
                  Taxi (const Taxi &other);
   Taxi &   operator = (const Taxi &other);
   bool           operator == (const Taxi &other);
   bool           operator != (const Taxi &other);

}; // class Taxi



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "model/Taxi.hpp"



#endif   // model_Taxi_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
