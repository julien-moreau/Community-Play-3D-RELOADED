/*****************************************************************************

        Opacifiable.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 32023

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



#if ! defined (ohm_opak_Opacifiable_HEADER_INCLUDED)
#define  ohm_opak_Opacifiable_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeFloat.h"



namespace ohm
{
namespace opak
{



class Opacifiable
:  public virtual Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Opacifiable ();
   virtual        ~Opacifiable ();
   
   // Video Thread
   virtual bool   is_visible ();
   virtual bool   was_visible ();

   TypeFloat <Opacifiable>
                  _opacity;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   class Auto
   {
   public:
                  Auto (Opacifiable & primitive, GraphicContext & context);
                  ~Auto ();
   private:
      Opacifiable &
                  _primitive;
      GraphicContext &
                  _context;
   };
   
   virtual void   do_opacity_changed ();


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           pre_display (GraphicContext & context);
   void           post_display (GraphicContext & context);

   void           opacity_changed ();
   
   static const float
                  _min_visibility_step;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Opacifiable (const Opacifiable &other);
   Opacifiable &  operator = (const Opacifiable &other);
   bool           operator == (const Opacifiable &other);
   bool           operator != (const Opacifiable &other);

}; // class Opacifiable



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/Opacifiable.hpp"



#endif   // ohm_opak_Opacifiable_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
