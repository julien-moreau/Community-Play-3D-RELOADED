/*****************************************************************************

        AffineTransform.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67384

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

#include "ohm/opa/AffineTransform.h"
#include "ohm/math/fnc.h"

#include <cassert>
#include <cmath>



namespace ohm
{
namespace opa
{



/*
==============================================================================
                              Model
   
   The affine transformation is modelized as :
      a b tx  x
      c d ty  y
      0 0 1   1
   
   Forward - Composition is :
      This o New
   
   Backward - Composition is :
      New-1 o This

   Hints :
      if true then the property is true
      if false then the property is not necesseraly false
   
   
==============================================================================
*/


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

AffineTransform::AffineTransform (bool backward_flag)
:  _backward_flag (backward_flag)
,  _identity_hint_flag (true)
,  _translation_hint_flag (true)
,  _scaling_translation_hint_flag (true)
,  _perpendicular_hint_flag (false)
,  _a (1.f)
,  _b (0.f)
,  _c (0.f)
,  _d (1.f)
,  _tx (0.f)
,  _ty (0.f)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AffineTransform::~AffineTransform ()
{
}



/*
==============================================================================
Name : apply
Description :
   Affine version (for points)
==============================================================================
*/

void  AffineTransform::apply (Point & pt, bool round_flag) const
{
   if (!_identity_hint_flag)
   {
      if (_backward_flag)
      {
         apply_backward (pt, round_flag);
      }
      else
      {
         apply_forward (pt, round_flag);
      }
   }
}



/*
==============================================================================
Name : apply
Description :
   Linear version (for sizes, vectors, etc.)
==============================================================================
*/

void  AffineTransform::apply (Size & sz, bool round_flag) const
{
   if (!_translation_hint_flag)
   {
      if (_backward_flag)
      {
         apply_backward (sz, round_flag);
      }
      else
      {
         apply_forward (sz, round_flag);
      }
   }
}



/*
==============================================================================
Name : apply
==============================================================================
*/

void  AffineTransform::apply (Quad & quad, bool round_flag) const
{
   opa::Point pt0 = quad [0];
   opa::Point pt1 = quad [1];
   opa::Point pt2 = quad [2];
   opa::Point pt3 = quad [3];
   
   apply (pt0, round_flag);
   apply (pt1, round_flag);
   apply (pt2, round_flag);
   apply (pt3, round_flag);
   
   quad.set (pt0, pt1, pt2, pt3);
}



/*
==============================================================================
Name : apply
==============================================================================
*/

void  AffineTransform::apply (Rect & rect, bool round_flag) const
{
#if 0
   Quad quad (rect);
   
   apply (quad, round_flag);
   
   quad.get_bounding_box (rect);

#elif 1
   opa::Point pt_0 = rect._origin;
   apply (pt_0);
   
   opa::Size sz_0 (rect._size._width, 0.f);
   apply (sz_0);

   opa::Size sz_1 (0.f, rect._size._height);
   apply (sz_1);
   
   float left = lang::min (pt_0._x, pt_0._x + sz_0._width);
   lang::min_in_place (left, left + sz_1._width);

   float right = lang::max (pt_0._x, pt_0._x + sz_0._width);
   lang::max_in_place (right, right + sz_1._width);

   float top = lang::min (pt_0._y, pt_0._y + sz_0._height);
   lang::min_in_place (top, top + sz_1._height);

   float bottom = lang::max (pt_0._y, pt_0._y + sz_0._height);
   lang::max_in_place (bottom, bottom + sz_1._height);
   
   rect._origin._x = left;
   rect._origin._y = top;
   rect._size._width = right - left;
   rect._size._height = bottom - top;

#endif
}



/*
==============================================================================
Name : apply_inverse
Description :
   Affine version (for points)
==============================================================================
*/

void  AffineTransform::apply_inverse (Point & pt, bool round_flag) const
{
   if (!_identity_hint_flag)
   {
      if (_backward_flag)
      {
         apply_forward (pt, round_flag);
      }
      else
      {
         apply_backward (pt, round_flag);
      }
   }
}



/*
==============================================================================
Name : apply_inverse
Description :
   Linear version (for sizes, vectors, etc.)
==============================================================================
*/

void  AffineTransform::apply_inverse (Size & sz, bool round_flag) const
{
   if (!_translation_hint_flag)
   {
      if (_backward_flag)
      {
         apply_forward (sz, round_flag);
      }
      else
      {
         apply_backward (sz, round_flag);
      }
   }
}



/*
==============================================================================
Name : apply_inverse
==============================================================================
*/

void  AffineTransform::apply_inverse (Quad & quad, bool round_flag) const
{
   opa::Point pt0 = quad [0];
   opa::Point pt1 = quad [1];
   opa::Point pt2 = quad [2];
   opa::Point pt3 = quad [3];
   
   apply_inverse (pt0, round_flag);
   apply_inverse (pt1, round_flag);
   apply_inverse (pt2, round_flag);
   apply_inverse (pt3, round_flag);
   
   quad.set (pt0, pt1, pt2, pt3);
}



/*
==============================================================================
Name : apply_inverse
==============================================================================
*/

void  AffineTransform::apply_inverse (Rect & rect, bool round_flag) const
{
   Quad quad (rect);
   
   apply_inverse (quad, round_flag);
   
   quad.get_bounding_box (rect);
}



/*
==============================================================================
Name : reset
Description :
   Reset to identity
==============================================================================
*/

void  AffineTransform::reset ()
{
   _identity_hint_flag = true;
   _translation_hint_flag = true;
   _scaling_translation_hint_flag = true;
   _perpendicular_hint_flag = false;

   _a = 1.f;
   _b = 0.f;
   _c = 0.f;
   _d = 1.f;
   _tx = 0.f;
   _ty = 0.f;
}



/*
==============================================================================
Name : translate
==============================================================================
*/

void  AffineTransform::translate (float tx, float ty)
{
   assert (math::is_finite (tx));
   assert (math::is_finite (ty));
   
   _identity_hint_flag = false;
   
   if (_backward_flag)
   {
      _tx -= tx;
      _ty -= ty;
   }
   else if (_translation_hint_flag)
   {
      _tx += tx;
      _ty += ty;
   }
   else if (_scaling_translation_hint_flag)
   {
      _tx += _a * tx;
      _ty += _d * ty;
   }
   else if (_perpendicular_hint_flag)
   {
      _tx += _b * ty;
      _ty += _c * tx;
   }
   else
   {
      _tx += _a * tx + _b * ty;
      _ty += _c * tx + _d * ty;
   }
}



/*
==============================================================================
Name : translate
==============================================================================
*/

void  AffineTransform::translate (float & old_tx, float & old_ty, float tx, float ty)
{
   old_tx = _tx;
   old_ty = _ty;
   
   translate (tx, ty);
}



/*
==============================================================================
Name : override_translate
==============================================================================
*/

void  AffineTransform::override_translate (float tx, float ty)
{
   _tx = tx;
   _ty = ty;
}



/*
==============================================================================
Name : scale
==============================================================================
*/

void  AffineTransform::scale (float sx, float sy)
{
   _identity_hint_flag = false;
   _translation_hint_flag = false;
   
   //#warning we should put a forbidden radius around zero
   // or the inverse calculus is going to be quite instable
   
   assert (sx != 0.f);
   assert (sy != 0.f);
   assert (math::is_finite (sx));
   assert (math::is_finite (sy));
   
   if (_backward_flag)
   {
      float inv_sx = 1.f / sx;
      float inv_sy = 1.f / sy;
      
      _a *= inv_sx;
      _b *= inv_sx;
      _c *= inv_sy;
      _d *= inv_sy;
      
      _tx *= inv_sx;
      _ty *= inv_sy;
   }
   else
   {
      _a *= sx;
      _c *= sx;
      _b *= sy;
      _d *= sy;
   }
}



/*
==============================================================================
Name : rotate
Description :
   Rotation : (clockwise ! a quarter turn transform (1,0) into (0,1) )
   cos -sin 0
   sin  cos 0
   0    0   1 

==============================================================================
*/

void  AffineTransform::rotate (float rot)
{
   assert (math::is_finite (rot));

   float rot4 = rot * 4.f;
   int rot4d = int (rot4);
   
   bool quarter_turn_flag = float (rot4d) == rot4;
   
   rot4d &= 3;
   
   if (_backward_flag)
   {
      if (quarter_turn_flag)
      {
         if (rot4d != 0)
         {
            _identity_hint_flag = false;
            
            switch (rot4d)
            {
            case 1:
               {
                  if (_scaling_translation_hint_flag || _translation_hint_flag)
                  {
                     _perpendicular_hint_flag = true;
                     _scaling_translation_hint_flag = false;
                     _translation_hint_flag = false;
                  }
                  else if (_perpendicular_hint_flag)
                  {
                     _scaling_translation_hint_flag = true;
                     _perpendicular_hint_flag = false;
                  }
                  
                  float ta = _a;
                  _a =   _c;
                  _c = - ta;
                  
                  float tb = _b;
                  _b = _d;
                  _d = - tb;
                  
                  float tx = _tx;
                  _tx = _ty;
                  _ty = - tx;
               }
               break;

            case 2:
               {
                  _a = - _a;
                  _b = - _b;
                  _c = - _c;
                  _d = - _d;
                  _tx = - _tx;
                  _ty = - _ty;
               }
               break;

            case 3:
               {
                  if (_scaling_translation_hint_flag || _translation_hint_flag)
                  {
                     _perpendicular_hint_flag = true;
                     _scaling_translation_hint_flag = false;
                     _translation_hint_flag = false;
                  }
                  else if (_perpendicular_hint_flag)
                  {
                     _scaling_translation_hint_flag = true;
                     _perpendicular_hint_flag = false;
                  }
                  
                  float ta = _a;
                  _a = - _c;
                  _c = ta;
                  
                  float tb = _b;
                  _b = - _d;
                  _d = tb;
                  
                  float tx = _tx;
                  _tx = - _ty;
                  _ty = tx;
               }
               break;
            }
         }
      }
      else
      {
         _identity_hint_flag = false;
         _translation_hint_flag = false;
         _scaling_translation_hint_flag = false;
         _perpendicular_hint_flag = false;

         float angle = rot * 3.1415926535897932f * 2.f;
         float c = cos (angle);
         float s = sin (angle);
         
         float ta = _a;
         _a =   ta * c + _c * s;
         _c = - ta * s + _c * c;
         
         float tb = _b;
         _b =   tb * c + _d * s;
         _d = - tb * s + _d * c;
         
         float tx = _tx;
         _tx =   tx * c + _ty * s;
         _ty = - tx * s + _ty * c;
      }
   }
   else
   {
      if (quarter_turn_flag)
      {
         if (rot4d != 0)
         {
            _identity_hint_flag = false;
            
            switch (rot4d)
            {
            case 1:
               {
                  if (_scaling_translation_hint_flag || _translation_hint_flag)
                  {
                     _perpendicular_hint_flag = true;
                     _scaling_translation_hint_flag = false;
                     _translation_hint_flag = false;
                  }
                  else if (_perpendicular_hint_flag)
                  {
                     _scaling_translation_hint_flag = true;
                     _perpendicular_hint_flag = false;
                  }
                  
                  float ta = _a;
                  _a = _b;
                  _b = - ta;
                  
                  float tc = _c;
                  _c =  _d;
                  _d = - tc;
               }
               break;

            case 2:
               {
                  if (_translation_hint_flag)
                  {
                     _translation_hint_flag = false;
                     _scaling_translation_hint_flag = true;
                  }
                  else if (_scaling_translation_hint_flag)
                  {
                     if ((_a == -1.f) && (_b == -1.f))
                     {
                        _translation_hint_flag = true;
                     }
                  }
                  
                  _a = - _a;
                  _b = - _b;
                  _c = - _c;
                  _d = - _d;
               }
               break;

            case 3:
               {
                  if (_scaling_translation_hint_flag || _translation_hint_flag)
                  {
                     _perpendicular_hint_flag = true;
                     _scaling_translation_hint_flag = false;
                     _translation_hint_flag = false;
                  }
                  else if (_perpendicular_hint_flag)
                  {
                     _scaling_translation_hint_flag = true;
                     _perpendicular_hint_flag = false;
                  }
                  
                  float ta = _a;
                  _a = - _b;
                  _b = ta;
                  
                  float tc = _c;
                  _c = - _d;
                  _d = tc;
               }
               break;
            }
         }
      }
      else
      {
         _identity_hint_flag = false;
         _translation_hint_flag = false;
         _scaling_translation_hint_flag = false;
         _perpendicular_hint_flag = false;

         float angle = rot * 3.1415926535897932f * 2.f;
         float c = cos (angle);
         float s = sin (angle);
         
         float ta = _a;
         _a =   ta * c + _b * s;
         _b = - ta * s + _b * c;
         
         float tc = _c;
         _c =   tc * c + _d * s;
         _d = - tc * s + _d * c;
      }
   }
}



/*
==============================================================================
Name : is_identity_hint
Description :
   Returns true if transform is identity.
   if false, the transform might be or not identity.
==============================================================================
*/

bool  AffineTransform::is_identity_hint () const
{
   return _identity_hint_flag;
}



/*
==============================================================================
Name : is_translation_hint
Description :
   Returns true if transform is a translation.
   if false, the transform might be or not a translation.
==============================================================================
*/

bool  AffineTransform::is_translation_hint () const
{
   return _translation_hint_flag;
}



/*
==============================================================================
Name : is_scaling_translation_hint
Description :
   Returns true if transform is a composition of scale and translation.
   if false, the transform might be or not a composition of scale and
   transation.
==============================================================================
*/

bool  AffineTransform::is_scaling_translation_hint () const
{
   return _scaling_translation_hint_flag;
}



/*
==============================================================================
Name : is_scaling_translation_hint
Description :
   Returns true if transform not a composition of scale and translation and
   a rotation which angle is perpendicular.
   if false, the transform might be or not a composition of scale and
   transation and rotation which angle is perpendicular.
==============================================================================
*/

bool  AffineTransform::is_perpendicular_hint () const
{
   return _perpendicular_hint_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : apply_forward
Description :
   Affine version (for points)
==============================================================================
*/

void  AffineTransform::apply_forward (Point & pt, bool round_flag) const
{
   if (_translation_hint_flag)
   {
      pt._x += _tx;
      pt._y += _ty;
   }
   else if (_scaling_translation_hint_flag)
   {
      pt._x = _a * pt._x + _tx;
      pt._y = _d * pt._y + _ty;
   }
   else if (_perpendicular_hint_flag)
   {
      float t = pt._x;
      pt._x = _b * pt._y + _tx;
      pt._y = _c * t + _ty;
   }
   else
   {
      float t = pt._x;
      pt._x = _a * t + _b * pt._y + _tx;
      pt._y = _c * t + _d * pt._y + _ty;
   }
   
   if (round_flag)
   {
      pt.nearest_integral ();
   }
}



/*
==============================================================================
Name : apply_forward
Description :
   Linear version (for sizes, vectors, etc.)
==============================================================================
*/

void  AffineTransform::apply_forward (Size & sz, bool round_flag) const
{
   if (_scaling_translation_hint_flag)
   {
      sz._width = _a * sz._width;
      sz._height = _d * sz._height;
   }
   else if (_perpendicular_hint_flag)
   {
      float t = sz._width;
      sz._width = _b * sz._height;
      sz._height = _c * t;
   }
   else
   {
      float t = sz._width;
      sz._width = _a * t + _b * sz._height;
      sz._height = _c * t + _d * sz._height;
   }
   
   if (round_flag)
   {
      sz.nearest_integral ();
   }
}



/*
==============================================================================
Name : apply_backward
Description :
   Affine version (for points)
==============================================================================
*/

void  AffineTransform::apply_backward (Point & pt, bool round_flag) const
{
   pt._x -= _tx;
   pt._y -= _ty;

   if (_translation_hint_flag)
   {
      // nothing
   }
   else if (_scaling_translation_hint_flag)
   {
      pt._x /= _a;
      pt._y /= _d;
   }
   else if (_perpendicular_hint_flag)
   {
      float t = pt._x;
      pt._x = pt._y / _c;
      pt._y = t / _b;
   }
   else
   {
      float det = _c * _b - _a * _d;
      assert (det != 0.f);
      float inv_det = 1.f / det;
      
      float t = pt._x;
      pt._x = (_b *  pt._y - _d * t) * inv_det;
      pt._y = (_c * t - _a * pt._y) * inv_det;
   }
   
   if (round_flag)
   {
      pt.nearest_integral ();
   }
}



/*
==============================================================================
Name : apply_backward
Description :
   Linear version (for sizes, vectors, etc.)
==============================================================================
*/

void  AffineTransform::apply_backward (Size & sz, bool round_flag) const
{
   if (_scaling_translation_hint_flag)
   {
      sz._width /= _a;
      sz._height /= _d;
   }
   else if (_perpendicular_hint_flag)
   {
      float t = sz._width;
      sz._width = sz._height / _c;
      sz._height = t / _b;
   }
   else
   {
      float det = _c * _b - _a * _d;
      assert (det != 0.f);
      float inv_det = 1.f / det;
      
      float t = sz._width;
      sz._width = (_b *  sz._height - _d * t) * inv_det;
      sz._height = (_c * t - _a * sz._height) * inv_det;
   }
   
   if (round_flag)
   {
      sz.nearest_integral ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
