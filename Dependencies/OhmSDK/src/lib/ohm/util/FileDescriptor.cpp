/*****************************************************************************

        FileDescriptor.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18235

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

#include "ohm/util/FileDescriptor.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FileDescriptor::FileDescriptor (const std::string &name, const sys::UtcTime &creation_date, const sys::UtcTime &modification_date, archi::Int64 length, long flags)
:  _name (name)
,  _creation_date (creation_date)
,  _modification_date (modification_date)
,  _length (length)
,  _directory_flag ((flags & Flags_DIR) != 0)
,  _read_only_flag ((flags & Flags_RO ) != 0)
,  _hidden_flag ((flags & Flags_HIDDEN) != 0)
,  _system_flag ((flags & Flags_SYSTEM) != 0)
{
   assert (creation_date.is_valid ());
   assert (modification_date.is_valid ());
   assert (_name.length () > 0);
   assert (_length >= 0);
   assert (_directory_flag == false || _length == 0);
}



FileDescriptor::FileDescriptor (const FileDescriptor &other)
:  _name (other._name)
,  _creation_date (other._creation_date)
,  _modification_date (other._modification_date)
,  _length (other._length)
,  _directory_flag (other._directory_flag)
,  _read_only_flag (other._read_only_flag)
,  _hidden_flag (other._hidden_flag)
,  _system_flag (other._read_only_flag)
{
   // Nothing
}



std::string FileDescriptor::get_name () const
{
   return (_name);
}



const sys::UtcTime & FileDescriptor::get_creation_date () const
{
   return (_creation_date);
}



const sys::UtcTime & FileDescriptor::get_modification_date () const
{
   return (_modification_date);
}



archi::Int64   FileDescriptor::get_length () const
{
   return (_length);
}



bool  FileDescriptor::is_directory () const
{
   return (_directory_flag);
}



bool  FileDescriptor::is_read_only () const
{
   return (_read_only_flag);
}



bool  FileDescriptor::is_hidden () const
{
   return (_hidden_flag);
}



bool  FileDescriptor::is_system () const
{
   return (_system_flag);
}



long  FileDescriptor::make_flags (bool dir_flag, bool ro_flag, bool hidden_flag, bool system_flag)
{
   long           flags = 0;

   flags |= dir_flag    ? Flags_DIR    : 0;
   flags |= ro_flag     ? Flags_RO     : 0;
   flags |= hidden_flag ? Flags_HIDDEN : 0;
   flags |= system_flag ? Flags_SYSTEM : 0;

   return (flags);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
