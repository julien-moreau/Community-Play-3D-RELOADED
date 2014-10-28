/*****************************************************************************

        Direct3dState.h
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



#if ! defined (ohm_opal_Direct3dState_HEADER_INCLUDED)
#define  ohm_opal_Direct3dState_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN

#include "ohm/archi/types.h"
#include "ohm/opal/directx/Direct3dCaps.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>



namespace ohm
{
namespace opal
{



class Direct3dState
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Direct3dState ();
   virtual        ~Direct3dState ();

   void           init (::IDirect3DDevice9 * device_ptr);
   void           restore ();

   bool           is_inited () const;

   int            reset_to_default ();

   void           set_blend_mode_normal ();
   void           set_blend_mode_multiply ();
   void           set_blend_mode_screen ();
   void           set_blend_mode_replace ();
   void           set_blend_mode_add ();
   void           set_blend_mode_max ();
   void           set_blend_mode_min ();

   void           set_vertex_mode_image ();
   void           set_vertex_mode_color ();

   void           set_sampler_filter_mode_nearest ();
   void           set_sampler_filter_mode_linear ();

   void           set_sampler_address_u_mode_wrap ();
   void           set_sampler_address_u_mode_clamp ();
   void           set_sampler_address_v_mode_wrap ();
   void           set_sampler_address_v_mode_clamp ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum BlendEquation
   {
                  BlendEquation_NOT_DEFINED = 0,
                  BlendEquation_ADD,
                  BlendEquation_MAX,
                  BlendEquation_MIN,
   };
   
   enum BlendFunc
   {
                  BlendFunc_NOT_DEFINED = 0,
                  BlendFunc_ONE_ONE_MINUS_arg_ALPHA,
                  BlendFunc_DST_COLOR_ONE_MINUS_arg_ALPHA,
                  BlendFunc_ONE_MINUS_DST_COLOR_ONE,
                  BlendFunc_ONE_ZERO,
                  BlendFunc_ONE_ONE,
   };
   
   enum VertexOp
   {
                  VertexOp_NOT_DEFINED = 0,
                  VertexOp_MODULATE,
                  VertexOp_SELECT_ARG1,
                  VertexOp_SELECT_ARG2
   };

   enum VertexArg
   {
                  VertexArg_NOT_DEFINED = 0,
                  VertexArg_TEXTURE,
                  VertexArg_DIFFUSE
   };

   enum SamplerFilterMode
   {
                  SamplerFilterMode_NOT_DEFINED = 0,
                  SamplerFilterMode_NEAREST,
                  SamplerFilterMode_LINEAR,
   };

   enum SamplerAddressMode
   {
                  SamplerAddressMode_NOT_DEFINED = 0,
                  SamplerAddressMode_WRAP,
                  SamplerAddressMode_CLAMP,
   };

   bool           set_render_state_once (::DWORD &state, ::_D3DRENDERSTATETYPE type, ::DWORD value);

   static bool    check_hresult (::HRESULT result);



   ::IDirect3DDevice9 *
                  _device_ptr;

   BlendEquation  _cur_blend_equation;
   BlendFunc      _cur_blend_func;
   VertexOp       _cur_color_vertex_op;
   VertexArg      _cur_color_vertex_arg1;
   VertexArg      _cur_color_vertex_arg2;
   VertexOp       _cur_alpha_vertex_op;
   VertexArg      _cur_alpha_vertex_arg1;
   VertexArg      _cur_alpha_vertex_arg2;
   SamplerFilterMode
                  _cur_sampler_filter_mode;
   SamplerAddressMode
                  _cur_sampler_address_u_mode;
   SamplerAddressMode
                  _cur_sampler_address_v_mode;
   ::DWORD        _render_state_src;
   ::DWORD        _render_state_dst;
   Direct3dCaps   _d3d_caps;

   bool           _inited_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Direct3dState (const Direct3dState &other);
   Direct3dState &   operator = (const Direct3dState &other);
   bool           operator == (const Direct3dState &other);
   bool           operator != (const Direct3dState &other);

}; // class Direct3dState



}  // namespace opal
}  // namespace ohm



//#include  "Direct3dState.hpp"



#endif   // ohm_opal_Direct3dState_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

