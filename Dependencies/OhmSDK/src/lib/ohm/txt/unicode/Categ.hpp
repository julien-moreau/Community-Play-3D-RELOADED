/*****************************************************************************

        Categ.hpp
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



#if defined (ohm_txt_unicode_Categ_CURRENT_CODEHEADER)
   #error Recursive inclusion of Categ code header.
#endif
#define  ohm_txt_unicode_Categ_CURRENT_CODEHEADER

#if ! defined (ohm_txt_unicode_Categ_CODEHEADER_INCLUDED)
#define  ohm_txt_unicode_Categ_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace txt
{
namespace unicode
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CategClass  Categ_conv_to_class (Categ categ)
{
   assert (categ >= 0);

   const CategClass  cc = static_cast <CategClass> (categ >> Categ_BITS);
   assert (cc >= 0);
   assert (cc < CategClass_NBR_ELT);

   return (cc);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_unicode_Categ_CODEHEADER_INCLUDED

#undef ohm_txt_unicode_Categ_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
