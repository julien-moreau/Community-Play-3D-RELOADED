/*****************************************************************************

        FragmentProgramManager.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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



#if ! defined (ohm_opal_FragmentProgramManager_HEADER_INCLUDED)
#define  ohm_opal_FragmentProgramManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"
#include "ohm/opal/def.h"
#include "ohm/opal/private/FragmentProgramProc.h"

#include <map>
#include <string>



namespace ohm
{
namespace opal
{



class GraphicContextInterface;

class FragmentProgramManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static FragmentProgramManager &
                  use_instance ();
   
   virtual        ~FragmentProgramManager ();
   
   archi::UInt32  get (FragmentProgramProc proc);
   
   const std::string &
                  compile (FragmentProgramProc proc);

   // System Internal
   
   std::string    get_temporary_name ();
   
   void           declare_temporary (const std::string & vn);
   
   void           exec_tex (const std::string & dst_vn, const std::string & tex_coord_vn, const std::string & texture_name);
   void           exec_mov (const std::string & dst_vn, const std::string & src_vn);
   void           exec_add (const std::string & dst_vn, const std::string & src_0_vn, const std::string & src_1_vn);
   void           exec_add (const std::string & dst_vn, const std::string & src_vn, float scalar);
   void           exec_mul (const std::string & dst_vn, const std::string & src_0_vn, const std::string & src_1_vn);
   void           exec_mul (const std::string & dst_vn, const std::string & src_vn, float scalar);
   void           exec_sin (const std::string & dst_vn, const std::string & src_vn);
   void           exec_cos (const std::string & dst_vn, const std::string & src_vn);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  FragmentProgramManager ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         MAX_LEN = 256 };
   
   typedef std::map <FragmentProgramProc, archi::UInt32> ProcProgMap;
   
   ProcProgMap    _prog_proc_map;
   std::string    _cur_program_text;
   
   int            _temporary_id;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FragmentProgramManager (const FragmentProgramManager &other);
   FragmentProgramManager &         operator = (const FragmentProgramManager &other);
   bool           operator == (const FragmentProgramManager &other);
   bool           operator != (const FragmentProgramManager &other);

}; // class FragmentProgramManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_FragmentProgramManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
