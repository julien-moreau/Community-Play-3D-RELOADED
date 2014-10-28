/*****************************************************************************

        LogLine.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 19207

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/fnc.h"
#include "ohm/util/LogLine.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*==========================================================================*/
/*      Name: ctor                                                          */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

LogLine::LogLine ()
:  _date (-1)
,  _ord (-1)
,  _line_nbr (-1)
{
   _scope_0 [0] = '\0';
   _msg_0 [0] = '\0';
}



LogLine::LogLine (const LogLine &other)
:  _date (other._date)
,  _ord (other._ord)
,  _line_nbr (other._line_nbr)
{
   txt::copy_string (_scope_0, other._scope_0, MAX_STR_SIZE);
   txt::copy_string (_msg_0, other._msg_0, MAX_STR_SIZE);
}



LogLine &   LogLine::operator = (const LogLine &other)
{
   set_log (
      other._ord,
      other._date,
      other._msg_0,
      other._scope_0,
      other._line_nbr
   );

   return (*this);
}



/*==========================================================================*/
/*      Name: set_log                                                       */
/*      Description: Fill the line with log information                     */
/*      Input parameters:                                                   */
/*        - ord : Ordinal number. >= 0.                                     */
/*        - date : Timestamp in seconds (unspecified time reference).       */
/*        - msg_0 : Log message. Extra-characters are truncated.            */
/*        - scope_0 : Scope/file information indicating where the log was   */
/*                    generated. Extra-characters are truncated.            */
/*        - line_nbr : Line number in the file. <= 0 means "undefined".     */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

void  LogLine::set_log (long ord, double date, const char *msg_0, const char *scope_0, long line_nbr)
{
   assert (ord >= 0);
   assert (scope_0 != 0);
   assert (msg_0 != 0);

   _ord = ord;
   _date = date;
   _line_nbr = line_nbr;
   txt::copy_string (_scope_0, scope_0, MAX_STR_SIZE);
   txt::copy_string (_msg_0, msg_0, MAX_STR_SIZE);
}



/*==========================================================================*/
/*      Name: get_ord                                                       */
/*      Description: Get the ordinal number of the line.                    */
/*      Returns: Number, >= 0                                               */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

long  LogLine::get_ord () const
{
   return (_ord);
}



/*==========================================================================*/
/*      Name: get_date                                                      */
/*      Description: Return the line timestamp.                             */
/*      Returns: Line timestamp in seconds. Time reference is unspecified.  */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

double   LogLine::get_date () const
{
   return (_date);
}



/*==========================================================================*/
/*      Name: get_msg                                                       */
/*      Description: Return the line message.                               */
/*      Returns: Pointer on the line message. Lifetime : object.            */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

const char *   LogLine::get_msg () const
{
   return (_msg_0);
}



/*==========================================================================*/
/*      Name: get_scope                                                     */
/*      Description: Return scope/file information                          */
/*      Returns: Pointer on the scope/file information.  Lifetime : object. */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

const char *   LogLine::get_scope () const
{
   return (_scope_0);
}



/*==========================================================================*/
/*      Name: get_line_nbr                                                  */
/*      Description: Return log line number in the file, if available.      */
/*      Returns: Line number. <= 0 means "undefined".                       */
/*      Throws: Nothing                                                     */
/*==========================================================================*/

long  LogLine::get_line_nbr () const
{
   return (_line_nbr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
