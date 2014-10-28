/*****************************************************************************

        Animator.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opal/def.h"
#include "ohm/opak/private/Animator.h"
#include "ohm/opak/private/AnimatorListener.h"
#include "ohm/opal/private/Clock.h"

#if (ohm_opal_API == ohm_opal_API_OPENGL)

   #include "ohm/opal/opengl/OpenGLEngine.h"

   typedef ohm::opal::OpenGLEngine  AnimatorPlatformEngine;

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)

   #include "ohm/opal/directx/DirectXEngine.h"

   typedef ohm::opal::DirectXEngine AnimatorPlatformEngine;

#else

   #error unsupported API

#endif

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Animator::~Animator ()
{
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  Animator::init ()
{
   opal::Clock::use_instance ().add (*this, 1);
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  Animator::restore ()
{
   opal::Clock::use_instance ().remove (*this);
}



/*
==============================================================================
Name : get_fast_delay
==============================================================================
*/

double   Animator::get_fast_delay ()
{
   static const double delay = 0.2;
   
   return delay;
}



/*
==============================================================================
Name : get_moderate_delay
==============================================================================
*/

double   Animator::get_moderate_delay ()
{
   static const double delay = 0.5;
   
   return delay;
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  Animator::insert (AnimatorListener & listener)
{
#if ! defined (NDEBUG)
   ListenerMap::iterator it = _listener_map.find (&listener);
   
   if (it != _listener_map.end ())
   {
      bool resident_flag = it->second;
      
      assert (!resident_flag);
   }
#endif
   
   _listener_map [&listener] = true;
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  Animator::erase (AnimatorListener & listener)
{
#if ! defined (NDEBUG)
   ListenerMap::iterator it = _listener_map.find (&listener);
   
   assert (it != _listener_map.end ());
   
   bool resident_flag = it->second;
   assert (resident_flag);
#endif
   
   _listener_map [&listener] = false;
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  Animator::init (double timestamp)
{
   _time_s = timestamp;
   _time_inited_flag = true;
}



/*
==============================================================================
Name : tick
==============================================================================
*/

void  Animator::tick (double timestamp)
{
   assert (_time_inited_flag);
   _time_s = timestamp;
   
   // notify
   
   ListenerMap::iterator it = _listener_map.begin ();
   ListenerMap::iterator it_end = _listener_map.end ();
   
   for (; it != it_end ; ++it)
   {
      bool resident_flag = it->second;
      
      if (resident_flag)
      {
         AnimatorListener * listener_ptr = it->first;
         
         listener_ptr->tick (timestamp);
      }
   }
   
   // cleanup
   
   it = _listener_map.begin ();
   
   for (; it != it_end ;)
   {
      ListenerMap::iterator it_next = it;
      ++it_next;
      
      bool resident_flag = it->second;
      
      if (!resident_flag)
      {
         _listener_map.erase (it);
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name : get_time
==============================================================================
*/

double   Animator::get_time () const
{
   assert (_time_inited_flag);
   
   return _time_s;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Animator::Animator ()
:  _listener_map ()

,  _time_inited_flag (false)
,  _time_s (0.0)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
