/*****************************************************************************

        FileRef.h
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



#if ! defined (ohm_sys_FileRef_HEADER_INCLUDED)
#define  ohm_sys_FileRef_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"

#include <string>
#include <vector>



namespace ohm
{

namespace util
{
   class FileDescriptor;
}

namespace sys
{



class FileRefImpl;

class FileRef
{
   friend class FileRefImpl;
   friend class FileRefOp;
   friend class FileRefFactory;
   friend class File;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  std::vector <archi::UByte> Storage;
   
                  FileRef (const FileRef &other);
   virtual        ~FileRef ();
   
   FileRef &      operator = (const FileRef & other);

   bool           operator < (const FileRef & other) const;
   bool           operator == (const FileRef & other) const;
   bool           operator != (const FileRef & other) const;
   
   bool           is_directory () const;
   bool           is_hidden () const;
   bool           is_read_only () const;
   std::string    get_name () const;
   const util::FileDescriptor &
                  get_info () const;
   void           update_info () const;

   void           save (Storage &opaque_data) const;
   std::string    to_utf8_path () const;

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #if (ohm_archi_ARCH == ohm_archi_ARCH_MACHO)
   const ::FSRef &
                  get_fs_ref () const;
                  
   #endif // ohm_archi_ARCH
#endif // ohm_archi_SYS
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FileRef (const FileRefImpl *impl_ptr);

   FileRefImpl *  _impl_ptr;  



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:


}; // class FileRef



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_FileRef_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
