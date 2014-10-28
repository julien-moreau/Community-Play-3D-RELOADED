/*****************************************************************************

        TypeMargin.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54606

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



#if ! defined (ohm_opak_TypeMargin_HEADER_INCLUDED)
#define  ohm_opak_TypeMargin_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opa/Margin.h"



namespace ohm
{
namespace opak
{



template <class Target>
class TypeMargin
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeMargin (Target & target, Proc proc);
   virtual        ~TypeMargin ();
   void           enable_stabilization ();
   
   TypeMargin &   operator = (const TypeMargin &other);
   TypeMargin &   operator = (const opa::Margin & value);
                  operator opa::Margin () const;
   
   void           init (const opa::Margin & value);
   void           init (float left, float top, float right, float bottom);
   
   TypeMargin &   push (const opa::Margin & value, double transition_time, TransitionFunction transition_function);
   TypeMargin &   loop ();
   TypeMargin &   stop ();
   
   void           set_transition_time (double delay_s);
   void           set_transition_function (TransitionFunction function);
   
   // Video Thread
   void           do_stop ();
   bool           is_animating () const;
   opa::Margin    get_cur_value () const;
   opa::Margin    get_prev_value () const;
   
   TypeFloat <Target>   
                  _left;
   TypeFloat <Target>   
                  _top;
   TypeFloat <Target>
                  _right;
   TypeFloat <Target>
                  _bottom;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeMargin ();
                  TypeMargin (const TypeMargin &other);
   bool           operator == (const TypeMargin &other);
   bool           operator != (const TypeMargin &other);

}; // class TypeMargin



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeMargin.hpp"



#endif   // ohm_opak_TypeMargin_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
