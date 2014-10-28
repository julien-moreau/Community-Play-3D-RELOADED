/*****************************************************************************

        Mouse.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Mouse.h"
#include "ohm/opal/Event.h"
#include "ohm/opal/Frame.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : FloatProxy::ctor
==============================================================================
*/

Mouse::FloatProxy::FloatProxy (PointProxy & proxy, bool x_flag)
:  _proxy (proxy)
,  _x_flag (x_flag)
{
}



/*
==============================================================================
Name : FloatProxy::operator float
==============================================================================
*/

Mouse::FloatProxy::operator float () const
{
   _proxy.update ();
   
   return _x_flag ? _proxy.get_x () : _proxy.get_y ();
}



/*
==============================================================================
Name : PointProxy::ctor
==============================================================================
*/

Mouse::PointProxy::PointProxy (Mouse & mouse, opa::Point & view_relative_point, bool affine_flag)
:  _x (*this, true)
,  _y (*this, false)

,  _affine_flag (affine_flag)
,  _invalid_flag (false)
,  _mouse (mouse)
,  _view_relative_point (view_relative_point)
{
}



/*
==============================================================================
Name : PointProxy::operator Point
==============================================================================
*/

Mouse::PointProxy::operator opa::Point () const
{
   if (_invalid_flag)
   {
      update ();
   }
   
   return _point;
}



/*
==============================================================================
Name : PointProxy::operator ==
==============================================================================
*/

bool  Mouse::PointProxy::operator == (const opa::Point & other) const
{
   opa::Point pt = this->operator opa::Point ();
   
   return (pt == other);
}



/*
==============================================================================
Name : PointProxy::operator !=
==============================================================================
*/

bool  Mouse::PointProxy::operator != (const opa::Point & other) const
{
   opa::Point pt = this->operator opa::Point ();
   
   return (pt != other);
}



/*
==============================================================================
Name : PointProxy::get_x
==============================================================================
*/

float Mouse::PointProxy::get_x () const
{
   return this->operator opa::Point ()._x;
}



/*
==============================================================================
Name : PointProxy::get_y
==============================================================================
*/

float Mouse::PointProxy::get_y () const
{
   return this->operator opa::Point ()._y;
}



/*
==============================================================================
Name : PointProxy::invalidate
==============================================================================
*/

void  Mouse::PointProxy::invalidate ()
{
   _invalid_flag = true;
}


/*
==============================================================================
Name : PointProxy::update
==============================================================================
*/

void  Mouse::PointProxy::update () const
{
   Event & event = _mouse._event;
   const opa::AffineTransform & affine_transform = event.get_affine_transform ();
   
   if (_affine_flag)
   {
      _point = _view_relative_point;

      affine_transform.apply_inverse (_point);
   }
   else
   {
      opa::Size size (_view_relative_point._x, _view_relative_point._y);
      
      affine_transform.apply_inverse (size);
      
      _point._x = size._width;
      _point._y = size._height;
   }
   
   _invalid_flag = false;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Mouse::Mouse (Event & event)
:  _location (*this, _view_relative_location, true)
,  _previous_location (*this, _view_relative_previous_location, true)
,  _click_location (*this, _view_relative_click_location, true)
,  _location_delta (*this, _view_relative_location_delta, false)
,  _pressed_flag (false)
,  _button_nbr (0)
,  _click_count (0)
,  _wheel_delta (*this, _view_relative_wheel_delta, false)

,  _view_relative_location (opa::Point::_zero)
,  _view_relative_previous_location (opa::Point::_zero)
,  _view_relative_click_location (opa::Point::_zero)
,  _view_relative_location_delta (opa::Point::_zero)
,  _view_relative_wheel_delta (opa::Point::_zero)

,  _event (event)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Mouse::~Mouse ()
{
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  Mouse::invalidate ()
{
   _location.invalidate ();
   _previous_location.invalidate ();
   _click_location.invalidate ();
   _location_delta.invalidate ();
   _wheel_delta.invalidate ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

opa::Point  operator + (const Mouse::PointProxy & l_op, const Mouse::PointProxy & r_op)
{
   opa::Point ret_val (l_op._x, l_op._y);
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

opa::Point  operator - (const Mouse::PointProxy & l_op, const Mouse::PointProxy & r_op)
{
   opa::Point ret_val (l_op._x, l_op._y);
   ret_val -= r_op;
   
   return ret_val;
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
