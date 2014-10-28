/*****************************************************************************

        TypeRect.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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



#if ! defined (ohm_opak_TypeRect_HEADER_INCLUDED)
#define  ohm_opak_TypeRect_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/TypePoint.h"
#include "ohm/opak/base/TypeSize.h"
#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"



namespace ohm
{
namespace opak
{



template <class Target>
class TypeRect
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (Target::*Proc)();
   
                  TypeRect (Target & target, Proc proc);
                  TypeRect (Target & target, Proc proc, Proc prev_proc);
   virtual        ~TypeRect ();
   void           enable_stabilization ();
   
   TypeRect &     operator = (const TypeRect &other);
   TypeRect &     operator = (const opa::Rect & value);
                  operator opa::Rect () const;
   
   void           init (const opa::Rect & value);
   void           init (const opa::Point & origin, const opa::Size & size);
   void           init (float x, float y, float w, float h);
   
   TypeRect &     push (const opa::Rect & value, double transition_time, TransitionFunction transition_function);
   TypeRect &     push_speed (const opa::Rect & value, const opa::Rect & abs_dv_per_sec, TransitionFunction transition_function);
   TypeRect &     loop ();
   TypeRect &     stop ();
   
   void           set_transition_time (double time_s);
   void           set_transition_function (TransitionFunction function);
   
   // Video Thread
   void           do_stop_transition ();
   bool           is_animating () const;
   opa::Rect      get_cur_value () const;
   opa::Rect      get_prev_value () const;
   
   TypePoint <Target>
                  _origin;
   TypeSize <Target>
                  _size;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TypeRect ();
                  TypeRect (const TypeRect &other);
   bool           operator == (const TypeRect &other);
   bool           operator != (const TypeRect &other);

}; // class TypeRect



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/TypeRect.hpp"



#endif   // ohm_opak_TypeRect_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
