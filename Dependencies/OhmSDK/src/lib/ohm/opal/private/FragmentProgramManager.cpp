/*****************************************************************************

        FragmentProgramManager.cpp
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

#include "ohm/opal/private/FragmentProgramManager.h"
#include "ohm/opal/shader/ShaderFloat4.h"
#include "ohm/opal/shader/ShaderTexture.h"
#include "ohm/txt/out/PrintNa.h"

#include <cassert>

#if (ohm_opal_API == ohm_opal_API_OPENGL)

   #include "ohm/opal/opengl/OpenGLEngine.h"

   typedef ohm::opal::OpenGLEngine  FPMPlatformEngine;

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)

   #include "ohm/opal/directx/DirectXEngine.h"

   typedef ohm::opal::DirectXEngine FPMPlatformEngine;

#else

   #error unsupported API

#endif


namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use_instance
==============================================================================
*/

FragmentProgramManager &   FragmentProgramManager::use_instance ()
{
   static FragmentProgramManager instance;
   
   return instance;
}
   


/*
==============================================================================
Name : dtor
==============================================================================
*/

FragmentProgramManager::~FragmentProgramManager ()
{
}



/*
==============================================================================
Name : get
==============================================================================
*/

archi::UInt32  FragmentProgramManager::get (FragmentProgramProc proc)
{
   /*assert (proc != 0);

   ProcProgMap::iterator it = _prog_proc_map.find (proc);
   
   if (it == _prog_proc_map.end ())
   {
      archi::UInt32 program_id = 0;
      gc.create (program_id, proc);
      
      std::pair <ProcProgMap::iterator, bool> ret = _prog_proc_map.insert (
         std::make_pair (proc, program_id)
      );
      assert (ret.second);
      
      it = ret.first;
   }
   
   return it->second;*/
   
   return 0;
}



/*
==============================================================================
Name : compile
==============================================================================
*/

const std::string &  FragmentProgramManager::compile (FragmentProgramProc proc)
{
   _cur_program_text.clear ();
   
   _cur_program_text += "!!ARBfp1.0\n";
   
   ShaderFloat4 out ("result.color");
   ShaderFloat4 color ("fragment.color");
   ShaderTexture texture ("texture[0]");
   ShaderFloat4 tex_coords ("fragment.texcoord[0]");
   ShaderFloat4 local_param ("program.local[0]");
   
   proc (out, color, texture, tex_coords, local_param);
   
   _cur_program_text += "END";
   
   return _cur_program_text;
}


/*
==============================================================================
Name : get_temporary_name
==============================================================================
*/

std::string FragmentProgramManager::get_temporary_name ()
{
   int temp_id = _temporary_id;
   ++_temporary_id;
   
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("tmp%1%").arg (temp_id);
   
   return std::string (dst_0);
}



/*
==============================================================================
Name : declare_temporary
==============================================================================
*/

void  FragmentProgramManager::declare_temporary (const std::string & vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("TEMP %1%;\n").arg (vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_tex
==============================================================================
*/

void  FragmentProgramManager::exec_tex (const std::string & dst_vn, const std::string & tex_coord_vn, const std::string & texture_name)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("TEX %1%, %2%, %3%, 2D;\n").arg (dst_vn.c_str ()).arg (tex_coord_vn.c_str ()).arg (texture_name.c_str ());
   
   // fow now we only support 2D (should do RECT also...)
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_mov
==============================================================================
*/

void  FragmentProgramManager::exec_mov (const std::string & dst_vn, const std::string & src_vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("MOV %1%, %2%;\n").arg (dst_vn.c_str ()).arg (src_vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_add
==============================================================================
*/

void  FragmentProgramManager::exec_add (const std::string & dst_vn, const std::string & src_0_vn, const std::string & src_1_vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("ADD %1%, %2%, %3%;\n")
      .arg (dst_vn.c_str ())
      .arg (src_0_vn.c_str ())
      .arg (src_1_vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_add
==============================================================================
*/

void  FragmentProgramManager::exec_add (const std::string & dst_vn, const std::string & src_vn, float scalar)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("ADD %1%, %2%, %3%;\n")
      .arg (dst_vn.c_str ())
      .arg (src_vn.c_str ())
      .arg (scalar);
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_mul
==============================================================================
*/

void  FragmentProgramManager::exec_mul (const std::string & dst_vn, const std::string & src_0_vn, const std::string & src_1_vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("MUL %1%, %2%, %3%;\n")
      .arg (dst_vn.c_str ())
      .arg (src_0_vn.c_str ())
      .arg (src_1_vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_mul
==============================================================================
*/

void  FragmentProgramManager::exec_mul (const std::string & dst_vn, const std::string & src_vn, float scalar)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("MUL %1%, %2%, %3%;\n")
      .arg (dst_vn.c_str ())
      .arg (src_vn.c_str ())
      .arg (scalar);
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_sin
==============================================================================
*/

void  FragmentProgramManager::exec_sin (const std::string & dst_vn, const std::string & src_vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("SIN %1%, %2%;\n").arg (dst_vn.c_str ()).arg (src_vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*
==============================================================================
Name : exec_cos
==============================================================================
*/

void  FragmentProgramManager::exec_cos (const std::string & dst_vn, const std::string & src_vn)
{
   char dst_0 [MAX_LEN];
   ohm::txt::out::PrintNa dst (dst_0, MAX_LEN + 1);
   dst.print ("COS %1%, %2%;\n").arg (dst_vn.c_str ()).arg (src_vn.c_str ());
   
   _cur_program_text += dst_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FragmentProgramManager::FragmentProgramManager ()
:  _temporary_id (0)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
