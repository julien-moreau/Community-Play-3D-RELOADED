/*****************************************************************************

        TypePoint.h
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



#if ! defined (ohm_opak_TypePoint_HEADER_INCLUDED)
#define  ohm_opak_TypePoint_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opa/Point.h"



namespace ohm
{
namespace opak
{



template <class Target>
class TypePoint
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypePoint (Target & target, Proc proc);
                  TypePoint (Target & target, Proc proc, Proc prev_proc);
   virtual        ~TypePoint ();
   void           enable_stabilization ();
   
   TypePoint &    operator = (const TypePoint &other);
   TypePoint &    operator = (const opa::Point & value);
                  operator opa::Point () const;

   void           init (const opa::Point & value);
   void           init (float x, float y);
   
   TypePoint &    push (const opa::Point & value, double transition_time, TransitionFunction transition_function);
   TypePoint &    push_speed (const opa::Point & value, const opa::Point & abs_dv_per_sec, TransitionFunction transition_function);
   TypePoint &    loop ();
   TypePoint &    stop ();
   
#if ! defined (ohm_opak_TypeKeyframable_RMU)
   void           set_transition_time (double delay_s);
   void           set_transition_function (TransitionFunction function);
#endif
   
   // Video Thread
   void           do_stop ();
   bool           is_animating () const;
   virtual opa::Point
                  get_cur_value () const;
   opa::Point     get_prev_value () const;
   
   TypeFloat <Target>
                  _x;
   TypeFloat <Target>
                  _y;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypePoint ();
                  TypePoint (const TypePoint &other);
   bool           operator == (const TypePoint &other);
   bool           operator != (const TypePoint &other);

}; // class TypePoint



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypePoint.hpp"



#endif   // ohm_opak_TypePoint_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
