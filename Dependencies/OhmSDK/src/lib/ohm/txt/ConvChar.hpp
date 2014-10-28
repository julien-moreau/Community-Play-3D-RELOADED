/*****************************************************************************

        ConvChar.hpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 31091

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



#if defined (ohm_txt_ConvChar_CURRENT_CODEHEADER)
   #error Recursive inclusion of ConvChar code header.
#endif
#define  ohm_txt_ConvChar_CURRENT_CODEHEADER

#if ! defined (ohm_txt_ConvChar_CODEHEADER_INCLUDED)
#define  ohm_txt_ConvChar_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include <cstddef>



namespace ohm
{
namespace txt
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class ST, class DT>
DT ConvChar <ST, DT>::convert (ST s)
{
   return (static_cast <DT> (s));
}


#if defined (ohm_archi_SUPPORT_WCHAR_T)



wchar_t  ConvChar <char, wchar_t>::convert (char s)
{
   wchar_t        d;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN || ohm_archi_SYS == ohm_archi_SYS_BEOS || ohm_archi_SYS == ohm_archi_SYS_LINUX)

   // Support of s is already in Basic Latin + Latin-1 Supplement
   d = s & 255;
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   // convert s from MacRoman to Basic Latin + Latin-1 Supplement
   // char from 0 to 127 are Basic Latin compliant
   if (s >= 0)
   {
      d = s;
   }
   else
   {
      // 8-Bit Mac Roman (128 to 255) to Unicode table.
      static const wchar_t mac_roman_to_latin_table [] =
      {
         196,  197,  199,  201,  209,  214,  220,  225,
         224,  226,  228,  227,  229,  231,  233,  232,
         234,  235,  237,  236,  238,  239,  241,  243,
         242,  244,  246,  245,  250,  249,  251,  252,
         8224, 176,  162,  163,  167,  8226, 182,  223,
         174,  169,  8482, 180,  168,  8800, 198,  216,
         8734, 177,  8804, 8805, 165,  181,  8706, 8721,
         8719, 960,  8747, 170,  186,  937,  230,  248,
         191,  161,  172,  8730, 402,  8776, 8710, 171,
         187,  8230, 160,  192,  195,  213,  338,  339,
         8211, 8212, 8220, 8221, 8216, 8217, 247,  9674,
         255,  376,  8260, 8364, 8249, 8250, 64257,64258,
         8225, 183,  8218, 8222, 8240, 194,  202,  193,
         203,  200,  205,  206,  207,  204,  211,  212,
         63743,210,  218,  219,  217,  305,  710,  732,
         175,  728,  729,  730,  184,  733,  731,  711
      };

      // s range is [-128 ; -1]
      d = mac_roman_to_latin_table [128 + s];
   }
   
#else

   #error Unsupported operating system.
   
#endif

   return (d);
}



char  ConvChar <wchar_t, char>::convert (wchar_t s)
{
   char           d;

#if (ohm_archi_SYS == ohm_archi_SYS_WIN || ohm_archi_SYS == ohm_archi_SYS_BEOS || ohm_archi_SYS == ohm_archi_SYS_LINUX)

   if (s > 255 || s < 0)
   {
      d = ' ';
   }
   else
   {
      // Support of s is already in Basic Latin + Latin-1 Supplement
      d = static_cast <char> (s);
   }
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   // convert s from Basic Latin + Latin-1 Supplement to MacRoman
   if (s >= 0 && s <= 127)
   {
      // char from 0 to 127 are Basic Latin compliant
      d = s;
   }
   else if (s >= 128 && s <= 255)
   {
      // Unicode truncated (from 128 to 255) to Mac Roman table.
      // Non mappable code are replaced by blank space.
      static const char latin_to_mac_roman_table [] =
      {
         ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', 
         ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', 
         ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', 
         ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', 
         '\xCA', '\xC1', '\xA2', '\xA3', ' ',    '\xB4', ' ',    '\xA4',
         '\xAC', '\xA9', '\xBB', '\xC7', '\xC2', ' ',    '\xA8', '\xF8',
         '\xA1', '\xB1', ' ',    ' ',    '\xAB', '\xB5', '\xA6', '\xE1',
         '\xFC', ' ',    '\xBC', '\xC8', ' ',    ' ',    ' ',    '\xC0',
         '\xCB', '\xE7', '\xE5', '\xCC', '\x80', '\x81', '\xAE', '\x82',
         '\xE9', '\x83', '\xE6', '\xE8', '\xED', '\xEA', '\xEB', '\xEC',
         ' ',    '\x84', '\xF1', '\xEE', '\xEF', '\xCD', '\x85', ' ',
         '\xAF', '\xF4', '\xF2', '\xF3', '\x86', ' ',    ' ',    '\xA7',
         '\x88', '\x87', '\x89', '\x8B', '\x8A', '\x8C', '\xBE', '\x8D',
         '\x8F', '\x8E', '\x90', '\x91', '\x93', '\x92', '\x94', '\x95',
         ' ',    '\x96', '\x98', '\x97', '\x99', '\x9B', '\x9A', '\xD6',
         '\xBF', '\x9D', '\x9C', '\x9E', '\x9F', ' ',    ' ',    '\xD8'
      };

      // char from 128 to 255 need conversion when possible
      d = latin_to_mac_roman_table [s - 128];
   }
   else
   {
      // some unicode that map on MacRoman in the 128;255 range
      switch (s)
      {
         case 0x2020: d = 0xA0; break; // &dagger
         case 0x2022: d = 0xA5; break; // &bull
         case 0x2122: d = 0xAA; break; // &trade
         case 0x2260: d = 0xAD; break; // &ne
         case 0x221E: d = 0xB0; break; // &infin
         case 0x2264: d = 0xB2; break; // &le
         case 0x2265: d = 0xB3; break; // &ge
         case 0x2202: d = 0xB6; break; // &part
         case 0x2211: d = 0xB7; break; // &sum
         case 0x03C0: d = 0xB9; break; // &pi
         case 0x222B: d = 0xBA; break; // &int
         case 0x03A9: d = 0xBD; break; // &Omega
         case 0x221A: d = 0xC3; break; // &radic
         case 0x0192: d = 0xC4; break; // &fnof
         case 0x2248: d = 0xC5; break; // &asymp
         case 0x2206: d = 0xC6; break; // increment
         case 0x2026: d = 0xC9; break; // &hellip
         case 0x0152: d = 0xCD; break; // &OElig
         case 0x0153: d = 0xCE; break; // &oelig
         case 0x2013: d = 0xD0; break; // &ndash
         case 0x2014: d = 0xD1; break; // &mdash
         case 0x201C: d = 0xD2; break; // &ldquo
         case 0x201D: d = 0xD3; break; // &rdquo
         case 0x2018: d = 0xD4; break; // &lsquo
         case 0x2019: d = 0xD5; break; // &rsquo
         case 0x25CA: d = 0xD7; break; // &loz
         case 0x0178: d = 0xD9; break; // &Yuml
         case 0x2044: d = 0xDA; break; // &frasl
         case 0x20AC: d = 0xDB; break; // &euro
         case 0x2039: d = 0xDC; break; // &lsaquo
         case 0x203A: d = 0xDD; break; // &rsaquo
         case 0xFB01: d = 0xDE; break; // fi ligature
         case 0xFB02: d = 0xDF; break; // fl ligature
         case 0x2021: d = 0xE0; break; // &Dagger
         case 0x201A: d = 0xE2; break; // &sbquo
         case 0x201E: d = 0xE3; break; // &bdquo
         case 0x2030: d = 0xE4; break; // &permil
         case 0xF8FF: d = 0xF0; break; // Apple logo
         case 0x0131: d = 0xF5; break; // dotless i
         case 0x02C6: d = 0xF6; break; // &circ
         case 0x02DC: d = 0xF7; break; // &tilde
         case 0x02D8: d = 0xF9; break; // breve
         case 0x02D9: d = 0xFA; break; // dot above
         case 0x02DA: d = 0xFB; break; // ring above
         case 0x02DD: d = 0xFD; break; // double acute accent
         case 0x02DB: d = 0xFE; break; // ogonek
         case 0x02C7: d = 0xFF; break; // caron

         default:     d = ' ';  break; // non mappable
      }
   }
   
#else

   #error Unsupported operating system.

#endif

   return (d);
}



#endif   // ohm_archi_SUPPORT_WCHAR_T



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_ConvChar_CODEHEADER_INCLUDED

#undef ohm_txt_ConvChar_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
