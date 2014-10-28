/*****************************************************************************

        CharDataInfo.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 28012

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



#if defined (ohm_txt_unicode_CharDataInfo_CURRENT_CODEHEADER)
   #error Recursive inclusion of CharDataInfo code header.
#endif
#define  ohm_txt_unicode_CharDataInfo_CURRENT_CODEHEADER

#if ! defined (ohm_txt_unicode_CharDataInfo_CODEHEADER_INCLUDED)
#define  ohm_txt_unicode_CharDataInfo_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace txt
{
namespace unicode
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



unsigned long  CharDataInfo::compute_internal_sort_code1 () const
{
   const unsigned long  sort_code =
        (static_cast <unsigned long> (_categ                    ) << 24)
      + (static_cast <unsigned long> (_kangxi_radical_stroke_cnt) << 16)
      + (static_cast <unsigned long> (_residual_stroke_cnt      ) <<  8)
      +                               _digit;

   return (sort_code);
}



unsigned long  CharDataInfo::compute_internal_sort_code2 () const
{
   const unsigned long  sort_code =
        (static_cast <unsigned long> (_bidi_class         ) << 24)
      + (static_cast <unsigned long> (_grapheme_break_prop) << 16)
      + (static_cast <unsigned long> (_line_break_prop    ) <<  8)
      +                               _word_break_prop;

   return (sort_code);
}



bool  operator < (const CharDataInfo &lhs, const CharDataInfo &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   const unsigned long  sc1_lhs = lhs.compute_internal_sort_code1 ();
   const unsigned long  sc1_rhs = rhs.compute_internal_sort_code1 ();
   if (sc1_lhs < sc1_rhs)
   {
      return (true);
   }
   else if (sc1_lhs == sc1_rhs)
   {
      if (lhs._upper < rhs._upper)
      {
         return (true);
      }
      else if (lhs._upper == rhs._upper)
      {
         if (lhs._lower < rhs._lower)
         {
            return (true);
         }
         else if (lhs._lower == rhs._lower)
         {
            if (lhs._title < rhs._title)
            {
               return (true);
            }
            else if (lhs._title == rhs._title)
            {
               const unsigned long  sc2_lhs = lhs.compute_internal_sort_code2 ();
               const unsigned long  sc2_rhs = rhs.compute_internal_sort_code2 ();
               if (sc2_lhs < sc2_rhs)
               {
                  return (true);
               }
            }
         }
      }
   }

   return (false);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_unicode_CharDataInfo_CODEHEADER_INCLUDED

#undef ohm_txt_unicode_CharDataInfo_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
