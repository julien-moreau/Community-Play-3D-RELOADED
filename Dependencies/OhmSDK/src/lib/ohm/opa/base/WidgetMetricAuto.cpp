/*****************************************************************************

        WidgetMetricAuto.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57589

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

#include "ohm/opa/base/WidgetMetricAuto.h"
#include "ohm/opa/base/WidgetBase.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMetricAuto::WidgetMetricAuto (WidgetBase & widget)
:  _widget (widget)
,  _type (Type_AUTO)
,  _val (0.f)
,  _need_update_flag (true)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetMetricAuto::~WidgetMetricAuto ()
{
}



/*
==============================================================================
Name : set
Descripition :
   Set the parameter to 'val' and explicitely exit other modes.
==============================================================================
*/

void  WidgetMetricAuto::set (float val)
{
   if ((_val != val) || (_type != Type_FIXED))
   {
      _type = Type_FIXED;
      _val = val;

      _need_update_flag = false;
      
      _widget.invalidate_layout ();
   }
}



/*
==============================================================================
Name : set_auto
Descripition :
   Set the parameter to auto mode.
==============================================================================
*/

void  WidgetMetricAuto::set_auto ()
{
   if (_type != Type_AUTO)
   {
      _type = Type_AUTO;

      _need_update_flag = true;
      
      _widget.invalidate_layout ();
   }
}



/*
==============================================================================
Name : set_ext
Descripition :
   Set the parameter to external mode. That is the value of width is not
   handled by 'this'. This is used when a widget metric is dependent on a
   parent metric.
   In that case this object maintains the minimal metric.
==============================================================================
*/

void  WidgetMetricAuto::set_ext ()
{
   if (_type != Type_EXT)
   {
      _type = Type_EXT;

      _widget.invalidate_layout ();
   }
}



/*
==============================================================================
Name : get
==============================================================================
*/

float WidgetMetricAuto::get () const
{
   if (_type != Type_FIXED)
   {
      assert (!_need_update_flag);
   }
   
   return _val;
}



/*
==============================================================================
Name : is_auto
==============================================================================
*/

bool  WidgetMetricAuto::is_auto () const
{
   return _type == Type_AUTO;
}



/*
==============================================================================
Name : is_ext
==============================================================================
*/

bool  WidgetMetricAuto::is_ext () const
{
   return _type == Type_EXT;
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  WidgetMetricAuto::invalidate ()
{
   if (_type != Type_FIXED)
   {
      _need_update_flag = true;
   }
}



/*
==============================================================================
Name : need_update
==============================================================================
*/

bool  WidgetMetricAuto::need_update () const
{
   return (_type != Type_FIXED) && _need_update_flag;
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  WidgetMetricAuto::update (float val)
{
   assert (need_update ());
   
   _val = val;
   
   _need_update_flag = false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
