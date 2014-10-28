/*****************************************************************************

        Categ.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20605

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



#if ! defined (ohm_txt_unicode_Categ_HEADER_INCLUDED)
#define  ohm_txt_unicode_Categ_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/unicode/CategClass.h"



namespace ohm
{
namespace txt
{
namespace unicode
{



enum Categ
{
   Categ_BITS  = 4,  // Private use, reserved

   Categ_LETTER_UPPERCASE           = CategClass_LETTER      << Categ_BITS,
   Categ_LETTER_LOWERCASE,
   Categ_LETTER_TITLECASE,
   Categ_LETTER_MODIFIER,
   Categ_LETTER_OTHER,

   Categ_MARK_NONSPACING            = CategClass_MARK        << Categ_BITS,
   Categ_MARK_SPACING_COMBINING,
   Categ_MARK_ENCLOSING,

   Categ_NUMBER_DECIMAL_DIGIT       = CategClass_NUMBER      << Categ_BITS,
   Categ_NUMBER_LETTER,
   Categ_NUMBER_OTHER,

   Categ_PUNCTUATION_CONNECTOR      = CategClass_PUNCTUATION << Categ_BITS,
   Categ_PUNCTUATION_DASH,
   Categ_PUNCTUATION_OPEN,
   Categ_PUNCTUATION_CLOSE,
   Categ_PUNCTUATION_INITIAL_QUOTE,
   Categ_PUNCTUATION_FINAL_QUOTE,
   Categ_PUNCTUATION_OTHER,

   Categ_SYMBOL_MATH                = CategClass_SYMBOL      << Categ_BITS,
   Categ_SYMBOL_CURRENCY,
   Categ_SYMBOL_MODIFIER,
   Categ_SYMBOL_OTHER,

   Categ_SEPARATOR_SPACE            = CategClass_SEPARATOR   << Categ_BITS,
   Categ_SEPARATOR_LINE,
   Categ_SEPARATOR_PARAGRAPH,

   Categ_OTHER_CONTROL              = CategClass_OTHER       << Categ_BITS,
   Categ_OTHER_FORMAT,
   Categ_OTHER_SURROGATE,
   Categ_OTHER_PRIVATE_USE,
   Categ_OTHER_NOT_ASSIGNED

}; // enum Categ



ohm_archi_FORCEINLINE CategClass Categ_conv_to_class (Categ categ);



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



#include "ohm/txt/unicode/Categ.hpp"



#endif   // ohm_txt_unicode_Categ_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
