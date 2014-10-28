/*****************************************************************************

        ArgNumBase.h
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



#if ! defined (ohm_txt_out_ArgNumBase_HEADER_INCLUDED)
#define  ohm_txt_out_ArgNumBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/out/ArgBase.h"



namespace ohm
{
namespace txt
{
namespace out
{



class ArgNumBase
:  public ArgBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ArgNumBase (DevInterface &output, const FmtInfo &fmt);
   virtual        ~ArgNumBase () {}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   long           conv_nbr_digits_to_nbr_char (long nbr_digits) const;
   long           conv_nbr_char_to_nbr_digits (long nbr_chars) const;
   void           format_filler ();
   void           print_pre_fill_and_sign (long int_size);
   void           print_post_fill ();
   void           print_positive_integer (long int_size, long offset);

   virtual long   get_nbr_displayed_chars () const = 0;
   virtual void   print_number_sign () = 0;
   virtual void   print_digit_sequence (long higher_pos, long nbr_digits) = 0;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           print_pre_fill (long int_size);

   long           _pre_fill_size;
   long           _post_fill_size;
   char           _pre_fill_char;
   char           _post_fill_char;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  ArgNumBase ();
                  ArgNumBase (const ArgNumBase &other);
   ArgNumBase &   operator = (const ArgNumBase &other);
   bool           operator == (const ArgNumBase &other);
   bool           operator != (const ArgNumBase &other);

}; // class ArgNumBase



}  // namespace out
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/out/ArgNumBase.hpp"



#endif   // ohm_txt_out_ArgNumBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
