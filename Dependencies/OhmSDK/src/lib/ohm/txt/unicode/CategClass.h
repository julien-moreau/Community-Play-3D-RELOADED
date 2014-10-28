/*****************************************************************************

        CategClass.h
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



#if ! defined (ohm_txt_unicode_CategClass_HEADER_INCLUDED)
#define  ohm_txt_unicode_CategClass_HEADER_INCLUDED

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



enum CategClass
{

   CategClass_LETTER = 0,
   CategClass_MARK,
   CategClass_NUMBER,
   CategClass_PUNCTUATION,
   CategClass_SYMBOL,
   CategClass_SEPARATOR,
   CategClass_OTHER,

   CategClass_NBR_ELT

}; // enum CategClass



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/unicode/CategClass.hpp"



#endif   // ohm_txt_unicode_CategClass_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
