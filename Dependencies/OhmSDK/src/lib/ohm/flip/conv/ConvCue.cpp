/*****************************************************************************

        ConvCue.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70008

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

#include "ohm/flip/conv/ConvCue.h"
#include "ohm/flip/private/BasicTypesName.h"

#include "ohm/txt/out/Print.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ConvCue::ConvCue ()
:  ConvObject ()
{
   _class_name = get_class_name ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ConvCue::~ConvCue ()
{
}



/*
==============================================================================
Name : collect
Description :
   collect writable objects :
   - BasicTypes are not writable
   - Blobs are writable iff they are non empty
   - All other objets are writable
==============================================================================
*/

void  ConvCue::collect (std::list <ConvObject *> & obj_ptr_arr)
{
   // "BasicTypes are not writable"
   
   // nothing
}



/*
==============================================================================
Name : write_xml
==============================================================================
*/

void  ConvCue::write_xml (ContextXml & ctx) const
{
   txt::out::Print printer;
   
   ctx.print_begin (printer, *this);
   printer.cat ("/>");
   ctx.add_line (printer);
}



/*
==============================================================================
Name: get_class_name
==============================================================================
*/

const char * ConvCue::get_class_name ()
{
   return BasicTypesName::_cue_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_write_member
==============================================================================
*/

void  ConvCue::do_write_member (DataStreamOutput & dst) const
{
   DocumentFormatter::write_member_cue (dst);
}



/*
==============================================================================
Name : do_write_member_container
==============================================================================
*/

void  ConvCue::do_write_member_container (DataStreamOutput & dst) const
{
   // not supported
   
   assert (false);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
