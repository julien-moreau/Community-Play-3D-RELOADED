/*****************************************************************************

        RndGenMT19937.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18971

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

#include "ohm/math/RndGenMT19937.h"

#include <cassert>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



RndGenMT19937::RndGenMT19937 ()
:/*   _mt ()
,*/   _mti (N + 1)
{
   // Nothing
}



// Initializes mt[N] with a seed
void  RndGenMT19937::init (unsigned long s)
{
   _mt [0] = s & 0xFFFFFFFFUL;
   for (_mti = 1; _mti < N; ++_mti)
   {
      // See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
      // In the previous versions, MSBs of the seed affect
      // only MSBs of the array _mt [].
      // 2002/01/09 modified by Makoto Matsumoto
      _mt [_mti] = (1812433253UL * (_mt [_mti-1] ^ (_mt [_mti-1] >> 30)) + _mti); 

      // for >32 bit machines
      _mt [_mti] &= 0xFFFFFFFFUL;
   }
}



// Initialize by an array with array-length
// init_key is the array for initializing keys
// key_length is its length
// slight change for C++, 2004/2/26
void  RndGenMT19937::init (unsigned long init_key [], int key_length)
{
   assert (init_key != 0);
   assert (key_length > 0);

   init (19650218UL);

   int            i = 1;
   int            j = 0;
   int            k = (N > key_length ? N : key_length);
   for ( ; k; --k)
   {
      _mt [i] = (_mt [i] ^ ((_mt [i-1] ^ (_mt [i-1] >> 30)) * 1664525UL)) + init_key [j] + j; // Non linear
      _mt [i] &= 0xFFFFFFFFUL;   // for >32 bit machines

      ++ i;
      ++ j;
      if (i >= N)
      {
         _mt [0] = _mt [N-1];
         i = 1;
      }
      if (j >= key_length)
      {
         j = 0;
      }
   }

   for (k = N - 1; k; --k)
   {
      _mt [i] = (_mt [i] ^ ((_mt [i-1] ^ (_mt [i-1] >> 30)) * 1566083941UL)) - i; // Non linear
      _mt [i] &= 0xFFFFFFFFUL;   // for >32 bit machines

      ++ i;
      if (i >= N)
      {
         _mt [0] = _mt [N-1];
         i = 1;
      }
   }

   _mt [0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array
}



// Generates a random number on [0, 0xFFFFFFFF]-interval
unsigned long  RndGenMT19937::gen_int32 ()
{
   const unsigned long  MATRIX_A   = 0x9908B0DFUL; // Constant vector a
   const unsigned long  UPPER_MASK = 0x80000000UL; // Most significant w-r bits
   const unsigned long  LOWER_MASK = 0x7FFFFFFFUL; // Least significant r bits

   // mag01[x] = x * MATRIX_A  for x=0,1
   static const unsigned long mag01 [2] = { 0x0UL, MATRIX_A };

   // Generate N words at one time
   if (_mti >= N)
   {
      int            kk;

      // If init_genrand() has not been called,
      if (_mti == N+1)
      {
         // A default initial seed is used
         init (5489UL);
      }

      for (kk = 0; kk < N - M ; ++kk)
      {
         const unsigned long  y =
            (_mt [kk] & UPPER_MASK) | (_mt [kk+1] & LOWER_MASK);
         _mt [kk] = _mt [kk+M] ^ (y >> 1) ^ mag01 [y & 0x1UL];
      }

      for ( ; kk < N - 1; ++kk)
      {
         const unsigned long  y =
            (_mt [kk] & UPPER_MASK) | (_mt [kk+1] & LOWER_MASK);
         _mt [kk] = _mt [kk + (M-N)] ^ (y >> 1) ^ mag01 [y & 0x1UL];
      }
   
      const unsigned long  y =
         (_mt [N-1] & UPPER_MASK) | (_mt [0] & LOWER_MASK);
      _mt [N-1] = _mt [M-1] ^ (y >> 1) ^ mag01 [y & 0x1UL];

      _mti = 0;
   }

   unsigned long  y = _mt [_mti];
   ++ _mti;

   // Tempering
   y ^= (y >> 11);
   y ^= (y <<  7) & 0x9D2C5680UL;
   y ^= (y << 15) & 0xEFC60000UL;
   y ^= (y >> 18);

   return (y);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace math
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
