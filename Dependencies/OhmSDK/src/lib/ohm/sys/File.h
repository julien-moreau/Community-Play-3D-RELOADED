/*****************************************************************************

        File.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 49137

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



#if ! defined (ohm_sys_File_HEADER_INCLUDED)
#define  ohm_sys_File_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/sys/FileRefSPtr.h"



namespace ohm
{
namespace sys
{



class UtcTime;
class FileImpl;

class File
{
   friend class FileImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Flags
   {
      Flags_READ   = 0x01, 
      Flags_WRITE  = 0x02, 
      Flags_CREATE = 0x04, 
      Flags_ERASE  = 0x08
   };

   enum ExistTypeFlag
   {
      ExistTypeFlag_FILE = 0x01,
      ExistTypeFlag_DIR  = 0x02
   };

                  File ();
   virtual        ~File ();

   int            open (const char *pathname_0, long flags);
   int            close ();
   bool           is_open () const;

   int            read (archi::UByte data_ptr [], long len);
   int            read (char data_ptr [], long len);
   int            write (const archi::UByte data_ptr [], long len);
   int            write (const char data_ptr [], long len);

   int            get_pos (archi::Int64 &pos) const;
   int            seek (archi::Int64 pos);

   int            get_length (archi::Int64 &len) const;
   int            set_length (archi::Int64 len);

   int            get_fileref (FileRefSPtr &fref_sptr) const;
   long           get_flags () const;

   int            set_creation_date (const sys::UtcTime &date_cre);
   int            set_modification_date (const sys::UtcTime &date_mod);

   int            get_creation_date (sys::UtcTime &date_cre) const;
   int            get_modification_date (sys::UtcTime &date_mod) const;

   static bool    exist (const char *pathname_0, int req_type_mask = ExistTypeFlag_FILE);

   static long    make_flags (bool read_flag, bool write_flag, bool create_flag, bool erase_flag);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   FileImpl *     _impl_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  File (const File &other);
   File &         operator = (const File &other);
   bool           operator == (const File &other);
   bool           operator != (const File &other);

}; // class File



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/File.hpp"



#endif   // ohm_sys_File_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
