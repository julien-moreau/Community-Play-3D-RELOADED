/*****************************************************************************

        ArgFloat.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30475

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



#if ! defined (ohm_txt_out_ArgFloat_HEADER_INCLUDED)
#define  ohm_txt_out_ArgFloat_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/math/FloatMultiPrecPrint.h"
#include "ohm/txt/out/ArgNumBase.h"



namespace ohm
{
namespace txt
{
namespace out
{



class ArgFloat
:  public ArgNumBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  long double FloatType;

                  ArgFloat (DevInterface &output, const FmtInfo &fmt);
   virtual        ~ArgFloat () {}

   void           print (const FloatType &val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // ArgNumBase
   virtual long   get_nbr_displayed_chars () const;
   virtual void   print_number_sign ();
   virtual void   print_digit_sequence (long higher_pos, long nbr_digits);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  math::FloatMultiPrecPrint <4> FloatPrinter;
   typedef  FloatPrinter::FloatType BigFloat;

   struct Mantissa
   {
      long           _int_part_size;      // Only absolute value, without dot, ideal
      long           _dec_part_size;      // Without decimal dot, limited by format specifications
      int            _sign_val;           // -1, 0 or 1
      int            _sign_size;          // 0 or 1
      int            _dot_size;           // 0 or 1
      char           _sign_char;          // ' ', '-' or '+'
   };

   struct Exponent
   {
      long           _value;
      long           _size;               // Without sign
      int            _sign_size;          // 0 or 1
      char           _character;          // 'e' or 'E'
      char           _sign_char;
   };

   int            get_exp () const;
   void           evaluate_format_and_size (const FloatType &val);
   void           find_display_format ();
   void           format_exponent ();
   long           format_mantissa_integer_part ();
   long           get_decimal_trail_length ();
   void           print_value ();
   void           print_exponent ();
   int            compute_dig_at (int dig_exp);
   int            compute_dig_at (int dig_exp, bool &significant_flag);

   FloatPrinter   _printer;               // Only if non-zero
   Mantissa       _mnt_fmt;
   Exponent       _exp_fmt;
   int            _base;
   int            _pos_prn;               // Exponent of the next digit to be printed.
   bool           _infinite_flag;
   bool           _scientific_flag;
   bool           _zero_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ArgFloat ();
                  ArgFloat (const ArgFloat &other);
   ArgFloat &     operator = (const ArgFloat &other);
   bool           operator == (const ArgFloat &other);
   bool           operator != (const ArgFloat &other);

}; // class ArgFloat



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/ArgFloat.hpp"



#endif   // ohm_txt_out_ArgFloat_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
