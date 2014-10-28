/*****************************************************************************

        RndGenMT19937.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 17555

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



#if ! defined (ohm_math_RndGenMT19937_HEADER_INCLUDED)
#define  ohm_math_RndGenMT19937_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace math
{



class RndGenMT19937
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  RndGenMT19937 ();
   virtual        ~RndGenMT19937 () {}

   void           init (unsigned long s);
   void           init (unsigned long init_key [], int key_length);

   unsigned long  gen_int32 ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         N  = 624 };
   enum {         M  = 397 };

   unsigned long  _mt [N];    // The array for the state vector
   int            _mti;       // _mti == N+1 means _mt [N] is not initialized



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  RndGenMT19937 (const RndGenMT19937 &other);
   RndGenMT19937& operator = (const RndGenMT19937 &other);
   bool           operator == (const RndGenMT19937 &other);
   bool           operator != (const RndGenMT19937 &other);

}; // class RndGenMT19937



}  // namespace math
}  // namespace ohm



//#include  "ohm/math/RndGenMT19937.hpp"



#endif   // ohm_math_RndGenMT19937_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
