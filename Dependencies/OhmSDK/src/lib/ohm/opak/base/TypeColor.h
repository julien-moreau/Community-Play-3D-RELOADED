/*****************************************************************************

        TypeColor.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 34885

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



#if ! defined (ohm_opak_TypeColor_HEADER_INCLUDED)
#define  ohm_opak_TypeColor_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opal/Color.h"



namespace ohm
{
namespace opak
{



template <class Target>
class TypeColor
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeColor (Target & target, Proc proc);
   virtual        ~TypeColor ();
   void           enable_stabilization ();
   
   TypeColor &    operator = (const TypeColor &other);
   TypeColor &    operator = (const opal::Color & value);
                  operator opal::Color () const;
   
   void           init (const opal::Color & value);
   void           init (float a, float r, float g, float b);
   
   TypeColor &    push (const opal::Color & value, double transition_time, TransitionFunction transition_function);
   TypeColor &    loop ();
   TypeColor &    stop ();
   
   void           set_transition_time (double delay_s);
   void           set_transition_function (TransitionFunction function);
   
   // Video Thread
   void           do_stop ();
   bool           is_animating () const;
   opal::Color    get_cur_value () const;
   opal::Color    get_prev_value () const;
   
   TypeFloat <Target>   
                  _a;
   TypeFloat <Target>   
                  _r;
   TypeFloat <Target>
                  _g;
   TypeFloat <Target>
                  _b;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeColor ();
                  TypeColor (const TypeColor &other);
   bool           operator == (const TypeColor &other);
   bool           operator != (const TypeColor &other);

}; // class TypeColor



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeColor.hpp"



#endif   // ohm_opak_TypeColor_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
