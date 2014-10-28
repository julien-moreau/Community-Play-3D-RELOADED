/*****************************************************************************

        CharData.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 31296

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



#if ! defined (ohm_txt_unicode_CharData_HEADER_INCLUDED)
#define  ohm_txt_unicode_CharData_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/txt/unicode/BidiClass.h"
#include "ohm/txt/unicode/Categ.h"
#include "ohm/txt/unicode/Char.h"
#include "ohm/txt/unicode/CharDataInfo.h"
#include "ohm/txt/unicode/GraphemeBreakProp.h"
#include "ohm/txt/unicode/LineBreakAction.h"
#include "ohm/txt/unicode/LineBreakProp.h"
#include "ohm/txt/unicode/WordBreakProp.h"
#include "ohm/txt/unicode/WordBreakPropTableNext.h"
#include "ohm/txt/unicode/WordBreakPropTablePrev.h"



namespace ohm
{
namespace txt
{
namespace unicode
{



class CharData
{

   friend class CharDataBuilder;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  size_t   FindCplxBreakFnc (LineBreakAction action_arr [], const LineBreakProp prop_arr [], LineBreakProp cls, size_t len);

   enum {         MAX_NBR_CODE_POINTS  = 0x110000  };

   static Categ   get_categ (Char c);
   static Char    to_upper (Char c);
   static Char    to_lower (Char c);
   static Char    to_title (Char c);
   static GraphemeBreakProp
                  get_grapheme_break_prop (Char c);
   static LineBreakProp
                  get_line_break_prop (Char c);
   static WordBreakProp
                  get_word_break_prop (Char c);
   static BidiClass
                  get_bidi_class (Char c);

   static bool    is_grapheme_cluster_boundary (Char c_prev, Char c_next);
   static size_t  find_line_breaks (LineBreakAction action_arr [], const LineBreakProp prop_arr [], size_t len, FindCplxBreakFnc &find_complex_line_breaks = CharData::find_complex_line_breaks_default);
   static void    find_word_breaks (archi::UByte action_arr [], const WordBreakProp prop_arr [], size_t len);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         PAGE_SIZE_L2   = 8   };
   enum {         DIRECT_L2      = 15  }; // Bit must fit in an archi::UInt16

   static const CharDataInfo &
                  use_info (Char c);
   static size_t  find_complex_line_breaks_default (LineBreakAction action_arr [], const LineBreakProp prop_arr [], LineBreakProp cls, size_t len);

   static const LineBreakAction
                  _table_break_line [LineBreakProp_NBR_ELT_TABLE] [LineBreakProp_NBR_ELT_TABLE];

   // Maximum size: 0x8000 elements
   // First element always describes an unassigned character.
   static const CharDataInfo
                  _info_unique_list [];

   // List of indexes in _info_unique_list
   // Maximum size: 0x110000 elements (more likely something like 0x30000)
   static const archi::Int16
                  _index_info_list [];

   // Input is (character code point) >> PAGE_SIZE_L2
   // Bit DIRECT_L2 clear: Index >> PAGE_SIZE_L2 for blocks in _index_info_list
   // Bit DIRECT_L2 set  : Index in _info_unique_list (for uniform blocks)
   // Maximum size: 0x1100 elements
   static const archi::UInt16
                  _index_page_list [MAX_NBR_CODE_POINTS >> PAGE_SIZE_L2];

   static const bool
                  _table_break_grapheme [GraphemeBreakProp_NBR_ELT] [GraphemeBreakProp_NBR_ELT];

   static const bool
                  _table_break_word [WordBreakPropTablePrev_NBR_ELT] [WordBreakPropTableNext_NBR_ELT];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CharData ();
                  CharData (const CharData &other);
   CharData &     operator = (const CharData &other);
   bool           operator == (const CharData &other);
   bool           operator != (const CharData &other);

}; // class CharData



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/unicode/CharData.hpp"



#endif   // ohm_txt_unicode_CharData_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
