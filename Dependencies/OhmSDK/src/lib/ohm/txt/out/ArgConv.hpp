/*****************************************************************************

        ArgConv.hpp
        Copyright (c) 2006 Ohm Force

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



#if defined (ohm_txt_out_ArgConv_CURRENT_CODEHEADER)
   #error Recursive inclusion of ArgConv code header.
#endif
#define  ohm_txt_out_ArgConv_CURRENT_CODEHEADER

#if ! defined (ohm_txt_out_ArgConv_CODEHEADER_INCLUDED)
#define  ohm_txt_out_ArgConv_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace txt
{
namespace out
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
T  ArgConv <T>::conv (const T &x)
{
   assert (&x != 0);

   return (x);
}



template <>
const char *   ArgConv <std::string>::conv (const std::string &x)
{
   assert (&x != 0);

   return (x.c_str ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace out
}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_out_ArgConv_CODEHEADER_INCLUDED

#undef ohm_txt_out_ArgConv_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
