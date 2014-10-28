/*****************************************************************************

        AffineTransform.h
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



#if ! defined (ohm_opa_AffineTransform_HEADER_INCLUDED)
#define  ohm_opa_AffineTransform_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"
#include "ohm/opa/Quad.h"



namespace ohm
{
namespace opa
{



class AffineTransform
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  AffineTransform (bool backward_flag);
   virtual        ~AffineTransform ();
   
   void           apply (Point & pt, bool round_flag = false) const;
   void           apply (Size & sz, bool round_flag = false) const;
   void           apply (Quad & quad, bool round_flag = false) const;
   void           apply (Rect & rect, bool round_flag = false) const;
   
   void           apply_inverse (Point & pt, bool round_flag = false) const;
   void           apply_inverse (Size & sz, bool round_flag = false) const;
   void           apply_inverse (Quad & quad, bool round_flag = false) const;
   void           apply_inverse (Rect & rect, bool round_flag = false) const;
   
   void           reset ();
   void           translate (float tx, float ty);
   void           translate (float & old_tx, float & old_ty, float tx, float ty);
   void           override_translate (float tx, float ty);
   void           scale (float sx, float sy);
   void           rotate (float rot);
   
   bool           is_identity_hint () const;
   bool           is_translation_hint () const;
   bool           is_scaling_translation_hint () const;
   bool           is_perpendicular_hint () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           apply_forward (Point & pt, bool round_flag) const;
   void           apply_forward (Size & sz, bool round_flag) const;

   void           apply_backward (Point & pt, bool round_flag) const;
   void           apply_backward (Size & sz, bool round_flag) const;
   
   bool           _backward_flag;
   bool           _identity_hint_flag;
   bool           _translation_hint_flag;
   bool           _scaling_translation_hint_flag;
   bool           _perpendicular_hint_flag;
   float          _a;
   float          _b;
   float          _c;
   float          _d;
   float          _tx;
   float          _ty;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  AffineTransform ();
   bool           operator == (const AffineTransform &other);
   bool           operator != (const AffineTransform &other);

}; // class AffineTransform



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/AffineTransform.hpp"



#endif   // ohm_opa_AffineTransform_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
