/*****************************************************************************

        FileRef.cpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
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

#include "ohm/sys/win32/FileRefImpl.h"
#include "ohm/sys/FileRef.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/utf8/PredStringCiLt.h"
#include "ohm/txt/utf8/PredStringCiEq.h"
#include "ohm/util/trace.h"
#include "ohm/util/TraceTools.h"

#define  NOMINMAX
#define  NOGDI
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FileRef::FileRef (const FileRef & other)
:  _impl_ptr (other._impl_ptr->clone())
{
   assert (&other != 0);
}



FileRef::~FileRef ()
{
   delete _impl_ptr;
   _impl_ptr = 0;
}



FileRef &   FileRef::operator = (const FileRef & other)
{
   assert (&other != 0);

   if (this != &other)
   {
      FileRefImpl *  impl_new_ptr = other._impl_ptr->clone();
      FileRefImpl *  impl_old_ptr = _impl_ptr;

      _impl_ptr = impl_new_ptr;
      delete impl_old_ptr;
   }

   return (*this);
}



bool  FileRef::operator == (const FileRef &other) const
{
   assert (&other != 0);

   const std::string & path_l = _impl_ptr->_path;
   const std::string & path_r = other._impl_ptr->_path;

   return (txt::utf8::PredStringCiEq () (path_l, path_r));
}



bool  FileRef::operator != (const FileRef &other) const
{
   assert (&other != 0);

   return (! (*this == other));
}



bool  FileRef::operator < (const FileRef &other) const
{
   assert (&other != 0);

   const std::string & path_l = _impl_ptr->_path;
   const std::string & path_r = other._impl_ptr->_path;

   return (txt::utf8::PredStringCiLt () (path_l, path_r));
}



bool  FileRef::is_directory () const
{

   return (_impl_ptr->_info_sptr->is_directory ());
}



bool  FileRef::is_hidden () const
{

   return (_impl_ptr->_info_sptr->is_hidden ());
}



bool  FileRef::is_read_only () const
{

   return (_impl_ptr->_info_sptr->is_read_only ());
}



std::string FileRef::get_name () const
{

   return (_impl_ptr->_info_sptr->get_name ());
}



const util::FileDescriptor &  FileRef::get_info () const
{

   return (*(_impl_ptr->_info_sptr));
}



void  FileRef::update_info () const
{
   _impl_ptr->update_info ();
}



void  FileRef::save (std::vector <archi::UByte> &opaque_data) const
{
   assert (&opaque_data);

   const size_t   len = _impl_ptr->_path.length ();
   opaque_data.resize (len);
   for (size_t pos = 0; pos < len; ++pos)
   {
      opaque_data [pos] = _impl_ptr->_path [pos];
   }
}



std::string FileRef::to_utf8_path () const
{
   _impl_ptr->update_info ();

   return (_impl_ptr->_path);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FileRef::FileRef (const FileRefImpl *new_impl_ptr)
:  _impl_ptr (new_impl_ptr->clone())
{
   assert (new_impl_ptr != 0);
}



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
