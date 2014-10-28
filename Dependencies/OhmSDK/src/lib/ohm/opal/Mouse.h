/*****************************************************************************

        Mouse.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 31774

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



#if ! defined (ohm_opal_Mouse_HEADER_INCLUDED)
#define  ohm_opal_Mouse_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"

#include <vector>



namespace ohm
{
namespace opal
{



class Event;

class Mouse
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Mouse (Event & event);
   virtual        ~Mouse ();
   
   class PointProxy;
   
   class FloatProxy
   {
   public:
                  FloatProxy (PointProxy & proxy, bool x_flag);
      
                  operator float () const;
      
   private:
      PointProxy &_proxy;
      bool        _x_flag;
   };
   
   class PointProxy
   {
   public:
                  PointProxy (Mouse & mouse, opa::Point & view_relative_point, bool affine_flag);
                  
                  operator opa::Point () const;
      bool        operator == (const opa::Point & other) const;
      bool        operator != (const opa::Point & other) const;
      float       get_x () const;
      float       get_y () const;
      
      FloatProxy  _x;
      FloatProxy  _y;
   
      void        invalidate ();

   private:
      
      friend class FloatProxy;
      friend class Mouse;
      
      void        update () const;
      
      const bool  _affine_flag;
      mutable bool
                  _invalid_flag;
      mutable opa::Point
                  _point;
      Mouse &     _mouse;
      opa::Point &_view_relative_point;
   };

   PointProxy     _location;
   PointProxy     _previous_location;
   PointProxy     _click_location;
   PointProxy     _location_delta;
   bool           _pressed_flag;
   int            _button_nbr;
   int            _click_count;
   PointProxy     _wheel_delta;
   
   opa::Point     _view_relative_location;
   opa::Point     _view_relative_previous_location;
   opa::Point     _view_relative_click_location;
   opa::Point     _view_relative_location_delta;
   opa::Point     _view_relative_wheel_delta;
   
   // system
   void           invalidate ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   Event &        _event;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Mouse &other);
   bool           operator != (const Mouse &other);

}; // class Mouse



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

opa::Point  operator + (const Mouse::PointProxy & l_op, const Mouse::PointProxy & r_op);
opa::Point  operator - (const Mouse::PointProxy & l_op, const Mouse::PointProxy & r_op);



}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Mouse_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
