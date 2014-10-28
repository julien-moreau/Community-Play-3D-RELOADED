/*****************************************************************************

        FileRef.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59820

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

#include "ohm/sys/FileRef.h"
#include "ohm/sys/macho/FileRefImpl.h"
#include "ohm/sys/FileRefFactory.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
/*
==============================================================================
Name : ctor
==============================================================================
*/

FileRef::FileRef (const FileRefImpl* new_impl_ptr)
:  _impl_ptr (0)
{
   _impl_ptr = new_impl_ptr->clone ();
}

/*
==============================================================================
Name : ctor
==============================================================================
*/

FileRef::FileRef (const FileRef & other)
:  _impl_ptr (0)
{
   _impl_ptr = other._impl_ptr->clone ();
}


/*
==============================================================================
Name : operator =
==============================================================================
*/

FileRef &   FileRef::operator = (const FileRef & other)
{
   delete _impl_ptr;
   _impl_ptr = other._impl_ptr->clone ();
   
   return *this;
}



/*
==============================================================================
Name : operator <
Description :
   Lexicographical order
==============================================================================
*/

bool  FileRef::operator < (const FileRef & other) const
{  
   std::vector <archi::UByte> l_data;
   std::vector <archi::UByte> r_data;
   
   save (l_data);
   other.save (r_data);
   
   bool ret_val_flag = l_data < r_data;
   
   return ret_val_flag;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  FileRef::operator == (const FileRef & other) const
{  
   std::vector <archi::UByte> l_data;
   std::vector <archi::UByte> r_data;
   
   save (l_data);
   other.save (r_data);
   
   bool ret_val_flag = l_data == r_data;
   
   return ret_val_flag;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  FileRef::operator != (const FileRef & other) const
{
   return ! operator == (other);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FileRef::~FileRef ()
{
   delete _impl_ptr;
   _impl_ptr = 0;
}



/*
==============================================================================
Name : is_directory
==============================================================================
*/

bool  FileRef::is_directory () const
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->is_directory ();
}



/*
==============================================================================
Name : is_hidden
==============================================================================
*/

bool  FileRef::is_hidden () const
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->is_hidden ();
}



/*
==============================================================================
Name : is_read_only
==============================================================================
*/

bool  FileRef::is_read_only () const
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->is_read_only ();
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

std::string FileRef::get_name () const
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->get_name ();
}



/*
==============================================================================
Name : get_info
==============================================================================
*/

const util::FileDescriptor &  FileRef::get_info () const
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->get_info ();
}



/*
==============================================================================
Name : update_info
==============================================================================
*/

void  FileRef::update_info () const
{
   _impl_ptr->update_info ();
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  FileRef::save (std::vector <archi::UByte> & opaque_data) const
{
   _impl_ptr->save (opaque_data);
}



/*
==============================================================================
Name : to_fopen
==============================================================================
*/

std::string FileRef::to_utf8_path () const
{
   return _impl_ptr->to_fopen ();
}



/*
==============================================================================
Name : get_fs_ref
==============================================================================
*/

const ::FSRef &   FileRef::get_fs_ref () const
{
   return _impl_ptr->get_fs_ref ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
