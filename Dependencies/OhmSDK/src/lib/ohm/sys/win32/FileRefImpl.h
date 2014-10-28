/*****************************************************************************

        FileRefImpl.h
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 60080

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



#if ! defined (ohm_sys_FileRefImpl_HEADER_INCLUDED)
#define  ohm_sys_FileRefImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/SharedPtr.h"
#include "ohm/txt/String16.h"
#include "ohm/util/FileDescriptor.h"

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>



namespace ohm
{

namespace util
{
   class Date;
}

namespace sys
{



class FileRefImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  mem::SharedPtr <util::FileDescriptor>  InfoSPtr;

            FileRefImpl ();

   void           init_data (const std::string &fopen_path);
   void           update_info ();
   FileRefImpl *  clone () const;

   std::string    _path;
   txt::String16  _path_16;
   InfoSPtr       _info_sptr;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           init_data_path (const std::string &fopen_path);
#if 0
   void           update_info (const ::_WIN32_FIND_DATAW &data);
#else
   void           update_info (const ::_BY_HANDLE_FILE_INFORMATION &data, const std::string &path);
#endif

   static void    remove_end_delim (std::string &path);
   static int     make_absolute (std::string &path);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   FileRefImpl &  operator = (const FileRefImpl &other);
   bool           operator == (const FileRefImpl &other);
   bool           operator != (const FileRefImpl &other);

}; // class FileRefImpl



}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/win32/FileRefImpl.hpp"



#endif   // ohm_sys_FileRefImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
