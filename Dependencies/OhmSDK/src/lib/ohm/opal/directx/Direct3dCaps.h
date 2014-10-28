/*****************************************************************************

        Direct3dCaps.h
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62251

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



#if ! defined (ohm_opal_Direct3dCaps_HEADER_INCLUDED)
#define  ohm_opal_Direct3dCaps_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>



namespace ohm
{
namespace opal
{



class Direct3dCaps
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum TexSize
   {
                  TexSize_NPOT = 0,
                  TexSize_NPOT_RESTRICTED,
                  TexSize_NPOT_POW_2,
   };

                  Direct3dCaps ();
   virtual        ~Direct3dCaps () {}

   void           init (::IDirect3DDevice9 * device_ptr);
   void           restore ();

   bool           is_inited () const;
   
   const ::D3DCAPS9 &   
                  use_d3d_caps () const;
   
   bool           can_do_hardware_primitives ();
   bool           can_do_alpha_blend_op () const;
   TexSize        can_do_tex_size () const;
   archi::UInt32  get_max_primitive_count () const;
   archi::UInt32  get_max_vertice_count () const;

   archi::UInt16  get_pixel_shader_version ();
   archi::UByte   get_pixel_shader_version_major ();
   archi::UByte   get_pixel_shader_version_minor ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ::D3DCAPS9     _d3d_caps;
   bool           _inited_flag;


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Direct3dCaps (const Direct3dCaps &other);
   Direct3dCaps & operator = (const Direct3dCaps &other);
   bool           operator == (const Direct3dCaps &other);
   bool           operator != (const Direct3dCaps &other);

}; // class Direct3dCaps



}  // namespace opal
}  // namespace ohm



//#include  "Direct3dCaps.hpp"



#endif   // ohm_opal_Direct3dCaps_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

