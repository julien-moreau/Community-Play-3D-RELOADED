/*****************************************************************************

        FileRefOp.h
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41758

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



#if ! defined (ohm_sys_FileRefOp_HEADER_INCLUDED)
#define  ohm_sys_FileRefOp_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/sys/FileRef.h"
#include "ohm/sys/FileRefList.h"
#include "ohm/sys/FileRefSPtr.h"

#include <string>



namespace ohm
{
namespace sys
{



class FileRefOpImpl;

class FileRefOp
{
   friend class FileRefOpImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static FileRefSPtr
                     up (const FileRef &file_ref);
   static FileRefSPtr
                     up (FileRefSPtr file_ref_sptr);
   static FileRefSPtr
                     up (FileRefSPtr file_ref_sptr, int count);
   static void       ls (FileRefList &file_list, const FileRef &file_ref);
   static void       lsr (FileRefList &file_list, const FileRef &file_ref);
   static FileRefSPtr
                     mkdir (const FileRef &file_ref, const std::string &name);
   static FileRefSPtr
                     enter (const FileRef &file_ref, const std::string &name);

   static void       rm (const FileRef &file_ref);
   static void       rmr (const FileRef &file_ref);

   static FileRefSPtr
                     mv (const FileRef &file_ref, const FileRef &new_parent_ref, const std::string &name);
   static FileRefSPtr
                     cp (const FileRef &file_ref, const FileRef &new_parent_ref, const std::string &name);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static void       ls_rec (FileRefList &lst, const FileRef &file_ref, bool rec_flag);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
                  FileRefOp ();
                  ~FileRefOp ();
                  FileRefOp (const FileRefOp &other);
   FileRefOp &    operator = (const FileRefOp &other);
   bool           operator == (const FileRefOp &other);
   bool           operator != (const FileRefOp &other);

}; // class FileRefOp



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_sys_FileRefOp_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
