/*****************************************************************************

        Animator.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58553

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



#if ! defined (ohm_opak_Animator_HEADER_INCLUDED)
#define  ohm_opak_Animator_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/ClockListener.h"

#include "ohm/util/SingletonBase.h"

#include <map>



namespace ohm
{
namespace opak
{



class AnimatorListener;

class Animator
:  public util::SingletonBase <Animator>
,  public opal::ClockListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Animator ();
   
   void           init ();
   void           restore ();
   
   static double  get_fast_delay ();
   static double  get_moderate_delay ();

   void           insert (AnimatorListener & listener);
   void           erase (AnimatorListener & listener);
   
   // ClockListener
   virtual void   init (double timestamp);
   virtual void   tick (double timestamp);

   double         get_time () const;
   
   int            _nbr_skip_frame;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  Animator ();
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (Animator)
   
   typedef std::map <AnimatorListener *, bool> ListenerMap;
   
   ListenerMap    _listener_map;
   
   bool           _time_inited_flag;
   double         _time_s;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Animator (const Animator &other);
   Animator &     operator = (const Animator &other);
   bool           operator == (const Animator &other);
   bool           operator != (const Animator &other);

}; // class Animator



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/private/Animator.hpp"



#endif   // ohm_opak_Animator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
