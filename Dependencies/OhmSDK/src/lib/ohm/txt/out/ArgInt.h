/*****************************************************************************

        ArgInt.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20570

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



#if ! defined (ohm_txt_out_ArgInt_HEADER_INCLUDED)
#define  ohm_txt_out_ArgInt_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/mem/Array.h"
#include "ohm/txt/out/ArgNumBase.h"

#include <climits>



namespace ohm
{
namespace txt
{
namespace out
{



class ArgInt
:  public ArgNumBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  archi::Int64   IntType;

                  ArgInt (DevInterface &output, const FmtInfo &fmt);
   virtual        ~ArgInt () {}

   void           print (const IntType &val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // ArgNumBase
   virtual long   get_nbr_displayed_chars () const;
   virtual void   print_number_sign ();
   virtual void   print_digit_sequence (long higher_pos, long nbr_digits);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         BIT_DEPTH   = sizeof (IntType) * CHAR_BIT };

   typedef  mem::Array <int, BIT_DEPTH>   DigitArr;

   void           compute_digits ();
   void           evaluate_format_and_size (const IntType &val);
   void           print_value ();

   static int     log_int (const IntType &val, int base);

   long           _size;               // Only absolute value, ideal
   int            _sign_val;           // -1, 0 or 1
   int            _sign_size;          // 0 or 1
   char           _sign_char;          // ' ', '-' or '+'

   IntType        _val;
   int            _base;
   DigitArr       _digit_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ArgInt ();
                  ArgInt (const ArgInt &other);
   ArgInt &       operator = (const ArgInt &other);
   bool           operator == (const ArgInt &other);
   bool           operator != (const ArgInt &other);

}; // class ArgInt



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/ArgInt.hpp"



#endif   // ohm_txt_out_ArgInt_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
