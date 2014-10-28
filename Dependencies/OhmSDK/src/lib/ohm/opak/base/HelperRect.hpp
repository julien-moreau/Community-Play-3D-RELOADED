/*****************************************************************************

        HelperRect.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68169

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



#if defined (ohm_opak_HelperRect_CURRENT_CODEHEADER)
   #error Recursive inclusion of HelperRect code header.
#endif
#define  ohm_opak_HelperRect_CURRENT_CODEHEADER

#if ! defined (ohm_opak_HelperRect_CODEHEADER_INCLUDED)
#define  ohm_opak_HelperRect_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use
==============================================================================
*/

HelperRect  use (const opa::Rect & rect)
{
   HelperRect ret_val (rect);
   
   return ret_val;
}



/*
==============================================================================
Name : use
==============================================================================
*/

HelperRect  use (const opa::Point & pt, const opa::Size & sz)
{
   HelperRect ret_val (opa::Rect (pt, sz));
   
   return ret_val;
}



/*
==============================================================================
Name : use
==============================================================================
*/

HelperRect  use (float x, float y, float width, float height)
{
   HelperRect ret_val (opa::Rect (x, y, width, height));
   
   return ret_val;
}



/*
==============================================================================
Name : ctor
Description :
   Set rect
==============================================================================
*/

HelperRect::HelperRect (const opa::Rect & rect)
:  _rect (rect)
,  _margin ()
,  _margined_flag (false)
,  _hint_flag (true)
,  _hint_absolute_flag (false)
{
}



/*
==============================================================================
Name : ctor
Note :
   Only used in ret val of 'use' : only copy _rect
==============================================================================
*/

HelperRect::HelperRect (const HelperRect & other)
:  _rect (other._rect)
,  _margin ()
,  _margined_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

HelperRect::~HelperRect ()
{
}



/*
==============================================================================
Name : margin
Description :
   Add *untransformed* margins (that is multiplied by context inv_px)
==============================================================================
*/

HelperRect &   HelperRect::margin (const opa::Margin & margin)
{
   _margined_flag = true;
   
   _margin += margin;
   
   return *this;
}



/*
==============================================================================
Name : margin
Description :
   Add *untransformed* margins (that is multiplied by context inv_px)
==============================================================================
*/

HelperRect &   HelperRect::margin (float left, float top, float right, float bottom)
{
   _margined_flag = true;
   
   _margin += opa::Margin (left, top, right, bottom);
   
   return *this;
}



/*
==============================================================================
Name : color
Description :
   Set color
==============================================================================
*/

HelperRect &   HelperRect::color (const opal::Color & color)
{
   _color = color;
   
   return *this;
}



/*
==============================================================================
Name : hint
==============================================================================
*/

HelperRect &   HelperRect::hint ()
{
   _hint_flag = true;
   _hint_absolute_flag = false;
   
   return *this;
}



/*
==============================================================================
Name : hint_absolute
==============================================================================
*/

HelperRect &   HelperRect::hint_absolute ()
{
   _hint_flag = true;
   _hint_absolute_flag = true;
   
   return *this;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_HelperRect_CODEHEADER_INCLUDED

#undef ohm_opak_HelperRect_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
