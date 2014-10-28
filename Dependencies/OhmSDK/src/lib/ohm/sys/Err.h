/*****************************************************************************

        Err.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64156

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



#if ! defined (ohm_sys_Err_HEADER_INCLUDED)
#define  ohm_sys_Err_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace sys
{



enum Err
{
   Err_OK = 0,

   Err_ZONE = -3000,

   Err_EXCEPTION = Err_ZONE,
   Err_GENERIC_ERROR,
   Err_CANNOT_OPEN_FILE,
   Err_CANNOT_CLOSE_FILE,
   Err_CANNOT_READ_FILE,
   Err_CANNOT_READ_PAST_FILE_END,
   Err_CANNOT_WRITE_FILE,
   Err_CANNOT_GET_FILE_POSITION,
   Err_CANNOT_SET_FILE_POSITION,
   Err_CANNOT_GET_FILE_LENGTH,
   Err_CANNOT_SET_FILE_LENGTH,
   Err_CANNOT_CONVERT_PATH_TO_FILEREF,
   Err_CANNOT_CONVERT_FSREF_TO_FILEREF,
   Err_CANNOT_CHANGE_FILE_CREATION_DATE,
   Err_CANNOT_CHANGE_FILE_MODIFICATION_DATE,
   Err_CANNOT_READ_FILE_CREATION_DATE,
   Err_CANNOT_READ_FILE_MODIFICATION_DATE,
   Err_CANNOT_CONVERT_DATE_TO_TICKS,
   Err_CANNOT_CONVERT_LOCAL_DATE_TO_UTC,
   Err_DATE_OUT_OF_RANGE,
   Err_DATE_CONVERSION_ERROR,
   Err_CANNOT_START_THREAD,
   Err_INVALID_PATH,
   Err_FILE_NOT_FOUND,
   Err_INVALID_NAME,
   Err_CANNOT_CREATE_REGISTRY_KEY,
   Err_CANNOT_OPEN_REGISTRY_KEY,
   Err_CANNOT_CLOSE_REGISTRY_KEY,
   Err_CANNOT_DELETE_REGISTRY_KEY,
   Err_CANNOT_ENUMERATE_REGISTRY_KEY,
   Err_REGISTRY_KEY_NOT_FOUND,
   Err_CANNOT_READ_REGISTRY_VALUE,
   Err_CANNOT_WRITE_REGISTRY_VALUE,
   Err_CANNOT_DELETE_REGISTRY_VALUE,
   Err_REGISTRY_VALUE_TOO_BIG,
   Err_CANNOT_LOAD_DYNAMIC_LIBRARY,
   Err_DYNAMIC_LIBRARY_MISSING_FNC,
   Err_CANNOT_CREATE_LOCAL_MESSAGE_PORT,
   Err_CANNOT_CREATE_RUN_LOOP_SOURCE,
   Err_CANNOT_CREATE_REMOTE_MESSAGE_PORT,
   Err_CANNOT_SEND_REQUEST_TO_MESSAGE_PORT,
   Err_CANNOT_CONVERT_STRING,
   Err_CANNOT_CREATE_DIR,
   Err_CANNOT_MOVE,
   Err_CANNOT_DELETE,
   Err_CANNOT_READ_ACCESS_RIGHTS,
   Err_CANNOT_WRITE_ACCESS_RIGHTS,
   Err_CANNOT_CREATE_PROCESS,
   Err_CANNOT_INITIALISE_COM,
   Err_CANNOT_CREATE_COM_OBJ,
   Err_CANNOT_SET_LINK_PARAM,
   Err_CANNOT_SAVE_LINK,
   Err_CANNOT_INITIALISE_OLE,
   Err_CANNOT_KILL_PROCESS,
   Err_CANNOT_SET_PRIORITY

}; // enum Err



}  // namespace sys
}  // namespace ohm



#endif   // ohm_sys_Err_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
