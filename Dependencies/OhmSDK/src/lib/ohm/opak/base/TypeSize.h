/*****************************************************************************

        TypeSize.h
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



#if ! defined (ohm_opak_TypeSize_HEADER_INCLUDED)
#define  ohm_opak_TypeSize_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opa/Size.h"



namespace ohm
{
namespace opak
{



template <class Target>
class TypeSize
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeSize (Target & target, Proc proc);
   virtual        ~TypeSize ();
   void           enable_stabilization ();
   
   TypeSize &     operator = (const TypeSize &other);
   TypeSize &     operator = (const opa::Size & value);
                  operator opa::Size () const;
   
   void           init (const opa::Size & value);
   void           init (float width, float height);
   
   TypeSize &     push (const opa::Size & value, double transition_time, TransitionFunction transition_function);
   TypeSize &     push_speed (const opa::Size & value, const opa::Size & abs_dv_per_sec, TransitionFunction transition_function);
   TypeSize &     loop ();
   TypeSize &     stop ();
   
   void           set_transition_time (double delay_s);
   void           set_transition_function (TransitionFunction function);
   
   // Video Thread
   void           do_stop ();
   bool           is_animating () const;
   opa::Size      get_cur_value () const;
   opa::Size      get_prev_value () const;
   
   TypeFloat <Target>
                  _width;
   TypeFloat <Target>
                  _height;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeSize ();
                  TypeSize (const TypeSize &other);
   bool           operator == (const TypeSize &other);
   bool           operator != (const TypeSize &other);

}; // class TypeSize



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeSize.hpp"



#endif   // ohm_opak_TypeSize_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
