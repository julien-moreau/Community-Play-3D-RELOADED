/*****************************************************************************

        def.h
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 70209

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



#if ! defined (ohm_lang_def_HEADER_INCLUDED)
#define  ohm_lang_def_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace lang
{



// Check a constant expression to make the compiler fail if false.
// name is a string containing only alpha+num+underscore, free of double quote.
// Requires a ";" at the end.
#define  ohm_lang_CHECK_CST(name, cond)   \
   typedef int ohm_lang_CheckCst_##name##_##__LINE__ [(cond) ? 1 : -1]

// Combines assert() and error code propagation.
// ret_val must be an assignable expression.
// Requires a ";" at the end.
#define  ohm_lang_ASSERT_E(ret_val, err_code, cond)   \
   if ((ret_val) != Err_OK) ; else {   \
      assert (cond); \
      if (cond) ; else { (ret_val) = (err_code); } \
   } while (false)

#define  ohm_lang_CRASH()  do { (*((int *) (0)) = 0); } while (false)

// Macros for compile-time evaluation
#define  ohm_lang_IS_POW_2(val)  (((val) & ((val) - 1)) == 0)
#define  ohm_lang_IS_MULTIPLE(val, mult)  ((val) % (mult) == 0)



}  // namespace lang
}  // namespace ohm



//#include  "ohm/lang/def.hpp"



#endif   // ohm_lang_def_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
