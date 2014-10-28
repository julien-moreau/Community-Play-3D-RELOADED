/*****************************************************************************

        fnc_win32.h
        Copyright (c) 2011 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 69022

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



#if ! defined (ohm_sys_win32_fnc_win32_HEADER_INCLUDED)
#define  ohm_sys_win32_fnc_win32_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <string>



namespace ohm
{
namespace sys
{
namespace win32
{


typedef  void (*CbThreadOp) (void *user_data_ptr, void *thread_hnd, unsigned long thread_id);

std::string  get_csidl_path (int csidl);
int   execute_process (const std::string &path, const std::string &cur_dir, int flags, bool inherit_flag, bool init_wait_flag, bool load_feedback_flag);
bool  find_process (unsigned long &pid, const std::string &path_substring);
int   kill_process_by_name (const char *name_0, double timeout_s = 0);
int   iterate_on_threads (unsigned long proc_id, bool cur_thread_flag, CbThreadOp callback_ptr, void *user_data_ptr, long access_flags);
int   create_shortcut (const std::string &path_link, const std::string &path_real, const std::string &work_dir, const std::string &arguments);



}  // namespace win32
}  // namespace sys
}  // namespace ohm



//#include  "ohm/sys/win32/fnc_win32.hpp"



#endif   // ohm_sys_win32_fnc_win32_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
