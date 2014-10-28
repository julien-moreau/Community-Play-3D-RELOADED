/*****************************************************************************

        RandomGenerator.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 62640

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

#include "ohm/flip/private/RandomGenerator.h"

#if ! defined (ohm_flip_SERVER)
   #include "ohm/sys/CpuTimer.h"
#endif

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use_instance
==============================================================================
*/

RandomGenerator & RandomGenerator::use_instance ()
{
   static RandomGenerator instance;
   
   return instance;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

RandomGenerator::~RandomGenerator ()
{
}



/*
==============================================================================
Name : get
==============================================================================
*/

archi::UInt32 RandomGenerator::get ()
{
   archi::UInt32 ret_val = _rnd_gen_mt19937.gen_int32 ();
   
   return ret_val;
}



/*
==============================================================================
Name : get
==============================================================================
*/

void RandomGenerator::get (archi::UByte * buf_ptr, size_t buf_size)
{
   if (buf_size > 0)
   {
      for (size_t i = 0 ; i < buf_size ; ++i)
      {
         archi::UInt32 r = _rnd_gen_mt19937.gen_int32 ();
         
         buf_ptr [i] = r;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

RandomGenerator::RandomGenerator ()
{
#if defined (ohm_flip_SERVER)
   _rnd_gen_mt19937.init (0);

#else
   sys::CpuTimer cpu_timer;
   _rnd_gen_mt19937.init (static_cast <unsigned long> (
      cpu_timer.read_clock ()
   ));
#endif
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
