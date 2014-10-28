/*****************************************************************************

        LineBreakProp.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 21248

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



#if ! defined (ohm_txt_unicode_LineBreakProp_HEADER_INCLUDED)
#define  ohm_txt_unicode_LineBreakProp_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace txt
{
namespace unicode
{



enum LineBreakProp
{

   LineBreakProp_OP = 0,   // open
   LineBreakProp_CL, // close
   LineBreakProp_QU, // quotation
   LineBreakProp_GL, // glue
   LineBreakProp_NS, // no-start
   LineBreakProp_EX, // exclamation/interrogation
   LineBreakProp_SY, // Syntax (slash)
   LineBreakProp_IS, // infix (numeric) separator
   LineBreakProp_PR, // prefix
   LineBreakProp_PO, // postfix
   LineBreakProp_NU, // numeric
   LineBreakProp_AL, // alphabetic
   LineBreakProp_ID, // ideograph (atomic)
   LineBreakProp_IN, // inseparable
   LineBreakProp_HY, // hyphen
   LineBreakProp_BA, // break after
   LineBreakProp_BB, // break before
   LineBreakProp_B2, // break both
   LineBreakProp_ZW, // ZW space
   LineBreakProp_CM, // combining mark
   LineBreakProp_WJ, // word joiner

   // used for Korean Syllable Block pair table
   LineBreakProp_H2, // Hamgul 2 Jamo Syllable
   LineBreakProp_H3, // Hangul 3 Jamo Syllable
   LineBreakProp_JL, // Jamo leading consonant
   LineBreakProp_JV, // Jamo vowel
   LineBreakProp_JT, // Jamo trailing consonant

   LineBreakProp_NBR_ELT_TABLE,

   // these are not handled in the pair tables
   LineBreakProp_SA = LineBreakProp_NBR_ELT_TABLE, // South (East) Asian
   LineBreakProp_SP, // space
   LineBreakProp_PS, // paragraph and line separators
   LineBreakProp_BK, // hard break (newline)
   LineBreakProp_CR, // carriage return
   LineBreakProp_LF, // line feed
   LineBreakProp_NL, // next line
   LineBreakProp_CB, // contingent break opportunity
   LineBreakProp_SG, // surrogate
   LineBreakProp_AI, // ambiguous
   LineBreakProp_XX, // unknown

   LineBreakProp_NBR_ELT

}; // enum LineBreakProp



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/unicode/LineBreakProp.hpp"



#endif   // ohm_txt_unicode_LineBreakProp_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
