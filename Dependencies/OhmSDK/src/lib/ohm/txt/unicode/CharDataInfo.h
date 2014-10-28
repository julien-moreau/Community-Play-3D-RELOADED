/*****************************************************************************

        CharDataInfo.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 21436

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



#if ! defined (ohm_txt_unicode_CharDataInfo_HEADER_INCLUDED)
#define  ohm_txt_unicode_CharDataInfo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace txt
{
namespace unicode
{



class CharDataInfo
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   inline unsigned long
                  compute_internal_sort_code1 () const;
   inline unsigned long
                  compute_internal_sort_code2 () const;

   archi::Int32   _upper;                       // UnicodeData.txt, column 12. Code point of the upper case equivalent, or 0 if none
   archi::Int32   _lower;                       // UnicodeData.txt, column 13. Code point of the lower case equivalent, or 0 if none
   archi::Int32   _title;                       // UnicodeData.txt, column 14. Code point of the title case equivalent, or 0 if none
   archi::UByte   _digit;                       // UnicodeData.txt, column 6. Decimal digit, or 0 if none
   archi::UByte   _categ;                       // UnicodeData.txt, column 2. General Category (Categ type)
   archi::UByte   _kangxi_radical_stroke_cnt;   // (Currently ignored, always 0) Unihan.txt, kRSUnicode field, first number. KangXi Radical Stroke Count
   archi::UByte   _residual_stroke_cnt;         // (Currently ignored, always 0) Unihan.txt, kRSUnicode field, second number. Residual Stroke Count
   archi::UByte   _grapheme_break_prop;         // auxiliary/GraphemeBreakProperty.txt. Information for Grapheme Cluster Break (GraphemeBreakProp type)
   archi::UByte   _line_break_prop;             // LineBreak.txt. Information for line break detection (LineBreakProp type)
   archi::UByte   _word_break_prop;             // WordBreakProperty.txt. Information for word boudnary detection (WordBreakProp type)
   archi::UByte   _bidi_class;                  // UnicodeData.txt, column 4. Bidi Class (BidiClass type)



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class CharDataInfo



static inline bool   operator < (const CharDataInfo &lhs, const CharDataInfo &rhs);



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



#include "ohm/txt/unicode/CharDataInfo.hpp"



#endif   // ohm_txt_unicode_CharDataInfo_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
