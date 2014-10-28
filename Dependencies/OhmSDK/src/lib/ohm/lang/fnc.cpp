/*****************************************************************************

        fnc.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 61457

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/CodeTxt.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace lang
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Returns 0 if the code is not known.
const char *   get_code_txt (long code, const lang::CodeTxt * const code_meta_list [])
{
   assert (code_meta_list != 0);

   const char *   txt_0 = 0;

   bool           cont_flag = true;
   int            list_index = 0;
   do
   {
      const lang::CodeTxt *   code_list_ptr = code_meta_list [list_index];
      if (code_list_ptr == 0)
      {
         cont_flag = false;
      }

      else
      {
         txt_0 = get_code_txt (code, code_list_ptr);
         cont_flag = (txt_0 == 0);
         ++ list_index;
      }
   }
   while (cont_flag);

   return (txt_0);
}



const char *   get_code_txt (long code, const CodeTxt code_list [])
{
   assert (code_list != 0);

   const char *   txt_0 = 0;

   int            code_index = 0;
   bool           code_cont_flag = true;
   do
   {
      const lang::CodeTxt &   pack = code_list [code_index];

      if (pack._txt_0 == 0)
      {
         code_cont_flag = false;
      }

      else if (pack._code == code)
      {
         txt_0 = pack._txt_0;
         code_cont_flag = false;
      }

      else
      {
         ++ code_index;
      }
   }
   while (code_cont_flag);

   return (txt_0);
}



// Returns an empty string if the code is not known.
const char *   get_code_txt_nn (long code, const lang::CodeTxt * const code_meta_list [])
{
   const char *   txt_0 = get_code_txt (code, code_meta_list);
   if (txt_0 == 0)
   {
      txt_0 = "";
   }

   return (txt_0);
}



const char *   get_code_txt_nn (long code, const CodeTxt code_list [])
{
   const char *   txt_0 = get_code_txt (code, code_list);
   if (txt_0 == 0)
   {
      txt_0 = "";
   }

   return (txt_0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace lang
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
