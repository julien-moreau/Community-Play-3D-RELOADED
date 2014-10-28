/*****************************************************************************

        FileDescriptor.h
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



#if ! defined (ohm_util_FileDescriptor_HEADER_INCLUDED)
#define  ohm_util_FileDescriptor_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/sys/UtcTime.h"

#include <string>
#include <ctime>



namespace ohm
{
namespace util
{



class FileDescriptor
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Flags
   {
      Flags_DIR    = 0x01,
      Flags_RO     = 0x02,
      Flags_HIDDEN = 0x04,
      Flags_SYSTEM = 0x08
   };
   
                  FileDescriptor (const std::string &name, const sys::UtcTime &creation_date, const sys::UtcTime &modification_date, archi::Int64 length, long flags);
                  FileDescriptor (const FileDescriptor &other);
   virtual        ~FileDescriptor () {}

   std::string    get_name () const;
   const sys::UtcTime &
                  get_creation_date () const;
   const sys::UtcTime &
                  get_modification_date () const;
   archi::Int64   get_length () const;
   bool           is_directory () const;
   bool           is_read_only () const;
   bool           is_hidden () const;
   bool           is_system () const;

   static long    make_flags (bool dir_flag, bool ro_flag, bool hidden_flag, bool system_flag);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::string    _name;
   sys::UtcTime   _creation_date;
   sys::UtcTime   _modification_date;
   archi::Int64   _length;
   bool           _directory_flag;
   bool           _read_only_flag;
   bool           _hidden_flag;
   bool           _system_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FileDescriptor ();
   bool           operator == (const FileDescriptor &other);
   bool           operator != (const FileDescriptor &other);

}; // class FileDescriptor



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_util_FileDescriptor_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
