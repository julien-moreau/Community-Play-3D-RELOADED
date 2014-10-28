/*****************************************************************************

        BidiClass.h
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



#if ! defined (ohm_txt_unicode_BidiClass_HEADER_INCLUDED)
#define  ohm_txt_unicode_BidiClass_HEADER_INCLUDED

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



enum BidiClass
{

   BidiClass_L,   // Left-to-Right
   BidiClass_LRE, // Left-to-Right Embedding
   BidiClass_LRO, // Left-to-Right Override
   BidiClass_R,   // Right-to-Left
   BidiClass_AL,  // Right-to-Left Arabic
   BidiClass_RLE, // Right-to-Left Embedding
   BidiClass_RLO, // Right-to-Left Override
   BidiClass_PDF, // Pop Directional Format
   BidiClass_EN,  // European Number
   BidiClass_ES,  // European Number Separator
   BidiClass_ET,  // European Number Terminator
   BidiClass_AN,  // Arabic Number
   BidiClass_CS,  // Common Number Separator
   BidiClass_NSM, // Non-Spacing Mark
   BidiClass_BN,  // Boundary Neutral
   BidiClass_B,   // Paragraph Separator
   BidiClass_S,   // Segment Separator
   BidiClass_WS,  // Whitespace
   BidiClass_ON,  // Other Neutrals

   BidiClass_NBR_ELT

}; // class BidiClass



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/unicode/BidiClass.hpp"



#endif   // ohm_txt_unicode_BidiClass_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
