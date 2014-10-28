/*****************************************************************************

        fnc.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 32209

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



#if defined (ohm_lang_fnc_CURRENT_CODEHEADER)
   #error Recursive inclusion of fnc code header.
#endif
#define  ohm_lang_fnc_CURRENT_CODEHEADER

#if ! defined (ohm_lang_fnc_CODEHEADER_INCLUDED)
#define  ohm_lang_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace lang
{



/*==========================================================================*/
/*      Name: max                                                           */
/*      Description: Returns the maximum of the two values.                 */
/*      Input parameters:                                                   */
/*        - a : first operand.                                              */
/*        - b : second operand.                                             */
/*      Return: The maximum value.                                          */
/*      Throws: Nothing for POD, depends on operators redefinitions for     */
/*              classes.                                                    */
/*==========================================================================*/

template <class T>
T  max (T a, T b)
{
   return ((a < b) ? b : a);
}



template <class T>
void  max_in_place (T &a, T b)
{
   if (a < b)
   {
      a = b;
   }
}



/*==========================================================================*/
/*      Name: min                                                           */
/*      Description: Returns the minimum of the two values.                 */
/*      Input parameters:                                                   */
/*        - a : first operand.                                              */
/*        - b : second operand.                                             */
/*      Return: The minimum value.                                          */
/*      Throws: Nothing for POD, depends on operators redefinitions for     */
/*              classes.                                                    */
/*==========================================================================*/

template <class T>
T  min (T a, T b)
{
   return ((a < b) ? a : b);
}



template <class T>
void  min_in_place (T &a, T b)
{
   if (b < a)
   {
      a = b;
   }
}



/*==========================================================================*/
/*      Name: limit                                                         */
/*      Description: Returns a value bounded in a given range.              */
/*      Input parameters:                                                   */
/*        - a : Value to bound.                                             */
/*        - b : Lower bound.                                                */
/*        - c : Upper bound. Should be greater or equal to b.               */
/*      Return: The boudned value.                                          */
/*      Throws: Nothing for POD, depends on operators redefinitions for     */
/*              classes.                                                    */
/*==========================================================================*/

template <class T>
T  limit (T a, T b, T c)
{
   assert (! (c < b));

   return (max (min (a, c), b));
}



template <class T>
void  limit_in_place (T &a, T b, T c)
{
   assert (! (c < b));

   if (a < b)
   {
      a = b;
   }
   else if (c < a)
   {
      a = c;
   }
}



template <class T>
void  sort_2_elt (T &mi, T &ma, T a, T b)
{
   if (a < b)
   {
      mi = a;
      ma = b;
   }
   else
   {
      mi = b;
      ma = a;
   }
}



/*==========================================================================*/
/*      Name: abs                                                           */
/*      Description: Return the absolute value. Shouldn't be used for       */
/*                   floating point values because of intrinsic functions   */
/*                   automatically used by compiler with fabs().            */
/*      Input parameters:                                                   */
/*        - a : Value whose absolute value has to be calculated.            */
/*      Returns: Absolute value.                                            */
/*      Throws: Nothing for POD, depends on operators redefinitions for     */
/*              classes.                                                    */
/*==========================================================================*/

template <class T>
T  abs (T x)
{
   return ((x < T (0)) ? -x : x);
}



/*==========================================================================*/
/*      Name: sgn                                                           */
/*      Description: Returns 0, -1 or 1 depending on the sign of the input. */
/*      Input parameters:                                                   */
/*        - x : Input number.                                               */
/*      Returns:  0 for null number,                                        */
/*               +1 for positive number,                                    */
/*               -1 for negative number.                                    */
/*      Throws: Nothing for POD, depends on operators redefinitions for     */
/*              classes.                                                    */
/*==========================================================================*/

template <class T>
int   sgn (T x)
{
   return (  (T (0) < x)
           ? 1
           : (  (x < T (0))
              ? -1
              : 0));
}



/*
==============================================================================
Name : cmp
Returns :
   0 if (a < b) is false and (b < a) is false (does not necessary imply a == b)
   -1 if (a < b) is true
   +1 if (b < a) is true
Throws :
   Nothing for POD, depends on operator < redefinition for classes.
==============================================================================
*/

template <class T>
int   cmp (const T & a, const T & b)
{
   return (
      (a < b)
         ? -1
         : (b < a
            ? 1
            : 0)
   );
}



}  // namespace lang
}  // namespace ohm



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_lang_fnc_CODEHEADER_INCLUDED

#undef ohm_lang_fnc_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
