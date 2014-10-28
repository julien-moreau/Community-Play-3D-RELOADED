/*****************************************************************************

        Direct3dState.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 63325

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/Direct3dState.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Direct3dState::Direct3dState ()
:  _inited_flag (false)
,  _device_ptr (0)
,  _cur_blend_equation (BlendEquation_NOT_DEFINED)
,  _cur_blend_func (BlendFunc_NOT_DEFINED)
,  _cur_color_vertex_op (VertexOp_NOT_DEFINED)
,  _cur_color_vertex_arg1 (VertexArg_NOT_DEFINED)
,  _cur_color_vertex_arg2 (VertexArg_NOT_DEFINED)
,  _cur_alpha_vertex_op (VertexOp_NOT_DEFINED)
,  _cur_alpha_vertex_arg1 (VertexArg_NOT_DEFINED)
,  _cur_alpha_vertex_arg2 (VertexArg_NOT_DEFINED)
,  _cur_sampler_filter_mode (SamplerFilterMode_NOT_DEFINED)
,  _cur_sampler_address_u_mode (SamplerAddressMode_NOT_DEFINED)
,  _cur_sampler_address_v_mode (SamplerAddressMode_NOT_DEFINED)
,  _render_state_src (D3DBLEND_FORCE_DWORD)  // Unknown value
,  _render_state_dst (D3DBLEND_FORCE_DWORD)  // Unknown value
,  _d3d_caps ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Direct3dState::~Direct3dState ()
{
   assert (!_inited_flag);
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  Direct3dState::init (::IDirect3DDevice9 * device_ptr)
{
   assert (!_inited_flag);
   assert (_device_ptr == 0);
   assert (device_ptr != 0);

   _device_ptr = device_ptr;
   _device_ptr->AddRef ();

   _d3d_caps.init (_device_ptr);

   _cur_blend_equation = BlendEquation_NOT_DEFINED;
   _cur_blend_func = BlendFunc_NOT_DEFINED;
   _cur_color_vertex_op = VertexOp_NOT_DEFINED;
   _cur_color_vertex_arg1 = VertexArg_NOT_DEFINED;
   _cur_color_vertex_arg2 = VertexArg_NOT_DEFINED;
   _cur_alpha_vertex_op = VertexOp_NOT_DEFINED;
   _cur_alpha_vertex_arg1 = VertexArg_NOT_DEFINED;
   _cur_alpha_vertex_arg2 = VertexArg_NOT_DEFINED;
   _cur_sampler_filter_mode = SamplerFilterMode_NOT_DEFINED;
   _cur_sampler_address_u_mode = SamplerAddressMode_NOT_DEFINED;
   _cur_sampler_address_v_mode = SamplerAddressMode_NOT_DEFINED;

   _inited_flag = true;
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  Direct3dState::restore ()
{
   assert (_inited_flag);
   assert (_device_ptr != 0);

   _d3d_caps.restore ();

   _device_ptr->Release ();
   _device_ptr = 0;
   _inited_flag = false;
}



/*
==============================================================================
Name : is_inited
==============================================================================
*/

bool Direct3dState::is_inited () const
{
   return _inited_flag;
}



/*
==============================================================================
Name : reset_to_default
==============================================================================
*/

int   Direct3dState::reset_to_default ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);
   
   bool d3d_ok_flag = true;

   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_FOGENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_DITHERENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_ALPHATESTENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_STENCILENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_ZENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE));
   
   // we will enable it only when needed
   // in particular the window frame buffer does not need it
   // d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_SCISSORTESTENABLE, FALSE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_SCISSORTESTENABLE, TRUE));
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, FALSE));
   
   // prevent back-face culling
   d3d_ok_flag &= check_hresult (_device_ptr->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE));

   _cur_blend_equation = BlendEquation_NOT_DEFINED;
   _cur_blend_func = BlendFunc_NOT_DEFINED;
   _cur_color_vertex_op = VertexOp_NOT_DEFINED;
   _cur_color_vertex_arg1 = VertexArg_NOT_DEFINED;
   _cur_color_vertex_arg2 = VertexArg_NOT_DEFINED;
   _cur_alpha_vertex_op = VertexOp_NOT_DEFINED;
   _cur_alpha_vertex_arg1 = VertexArg_NOT_DEFINED;
   _cur_alpha_vertex_arg2 = VertexArg_NOT_DEFINED;
   _cur_sampler_filter_mode = SamplerFilterMode_NOT_DEFINED;
   _cur_sampler_address_u_mode = SamplerAddressMode_NOT_DEFINED;
   _cur_sampler_address_v_mode = SamplerAddressMode_NOT_DEFINED;
   _render_state_src = D3DBLEND_FORCE_DWORD;
   _render_state_dst = D3DBLEND_FORCE_DWORD;

   set_blend_mode_normal ();
   set_vertex_mode_image ();
   set_sampler_filter_mode_nearest ();
   set_sampler_address_u_mode_clamp ();
   set_sampler_address_v_mode_clamp ();

   d3d_ok_flag &= check_hresult (_device_ptr->Clear (0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB (255, 255, 255, 255), 0, 0));
   
   {
      ::D3DXMATRIX id;
      id._11 = id._22 = id._33 = id._44 = 1.0f; 
      id._12 = id._13 = id._14 = id._41 = 0.0f; 
      id._21 = id._23 = id._24 = id._42 = 0.0f; 
      id._31 = id._32 = id._34 = id._43 = 0.0f; 
      
      d3d_ok_flag &= check_hresult (_device_ptr->SetTransform (D3DTS_VIEW, &id));
   }
   

   return (0);
}



/*
==============================================================================
Name : set_blend_mode_normal
==============================================================================
*/

void  Direct3dState::set_blend_mode_normal ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_blend_equation != BlendEquation_ADD)
   {
      check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_ADD));

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ONE_MINUS_arg_ALPHA)
   {
      set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_ONE);
      set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

      _cur_blend_func = BlendFunc_ONE_ONE_MINUS_arg_ALPHA;
   }
}



/*
==============================================================================
Name : set_blend_mode_multiply
==============================================================================
*/

void  Direct3dState::set_blend_mode_multiply ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_blend_equation != BlendEquation_ADD)
   {
      check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_ADD));

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_DST_COLOR_ONE_MINUS_arg_ALPHA)
   {
      set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
      set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

      _cur_blend_func = BlendFunc_DST_COLOR_ONE_MINUS_arg_ALPHA;
   }
}



/*
==============================================================================
Name : set_blend_mode_screen
==============================================================================
*/

void  Direct3dState::set_blend_mode_screen ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_blend_equation != BlendEquation_ADD)
   {
      check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_ADD));

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_MINUS_DST_COLOR_ONE)
   {
      set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR);
      set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_ONE);

      _cur_blend_func = BlendFunc_ONE_MINUS_DST_COLOR_ONE;
   }
}



/*
==============================================================================
Name : set_blend_mode_replace
==============================================================================
*/

void  Direct3dState::set_blend_mode_replace ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_blend_equation != BlendEquation_ADD)
   {
      check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_ADD));

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ZERO)
   {
      set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_ONE);
      set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_ZERO);

      _cur_blend_func = BlendFunc_ONE_ZERO;
   }
}



/*
==============================================================================
Name : set_blend_mode_add
==============================================================================
*/

void  Direct3dState::set_blend_mode_add ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_blend_equation != BlendEquation_ADD)
   {
      check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_ADD));

      _cur_blend_equation = BlendEquation_ADD;
   }
   
   if (_cur_blend_func != BlendFunc_ONE_ONE)
   {
      set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_ONE);
      set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_ONE);

      _cur_blend_func = BlendFunc_ONE_ONE;
   }
}



/*
==============================================================================
Name : set_blend_mode_max
==============================================================================
*/

void  Direct3dState::set_blend_mode_max ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_d3d_caps.can_do_alpha_blend_op ())
   {
      if (_cur_blend_equation != BlendEquation_MAX)
      {
         check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_MAX));

         _cur_blend_equation = BlendEquation_MAX;
      }
      
      if (_cur_blend_func != BlendFunc_ONE_ONE)
      {
         set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_ONE);
         set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_ONE);

         _cur_blend_func = BlendFunc_ONE_ONE;
      }
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_blend_mode_min
==============================================================================
*/

void  Direct3dState::set_blend_mode_min ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_d3d_caps.can_do_alpha_blend_op ())
   {
      if (_cur_blend_equation != BlendEquation_MIN)
      {
         check_hresult (_device_ptr->SetRenderState (D3DRS_BLENDOP, D3DBLENDOP_MIN));

         _cur_blend_equation = BlendEquation_MIN;
      }
      
      if (_cur_blend_func != BlendFunc_ONE_ONE)
      {
         set_render_state_once (_render_state_src, D3DRS_SRCBLEND,  D3DBLEND_ONE);
         set_render_state_once (_render_state_dst, D3DRS_DESTBLEND, D3DBLEND_ONE);

         _cur_blend_func = BlendFunc_ONE_ONE;
      }
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_vertex_mode_image
==============================================================================
*/

void  Direct3dState::set_vertex_mode_image ()
{
   if (_cur_color_vertex_op != VertexOp_MODULATE)
   {
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE));
      _cur_color_vertex_op = VertexOp_MODULATE;
   }

   if (_cur_color_vertex_arg1 != VertexArg_TEXTURE)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE));
      _cur_color_vertex_arg1 = VertexArg_TEXTURE;
   }

   if (_cur_color_vertex_arg2 != VertexArg_DIFFUSE)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE));
      _cur_color_vertex_arg2 = VertexArg_DIFFUSE;
   }

   if (_cur_alpha_vertex_op != VertexOp_MODULATE)
   {
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE));
      _cur_alpha_vertex_op = VertexOp_MODULATE;
   }

   if (_cur_alpha_vertex_arg1 != VertexArg_TEXTURE)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE));
      _cur_alpha_vertex_arg1 = VertexArg_TEXTURE;
   }


   if (_cur_alpha_vertex_arg2 != VertexArg_DIFFUSE)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE));
      _cur_alpha_vertex_arg2 = VertexArg_DIFFUSE;
   }
}



/*
==============================================================================
Name : set_vertex_mode_color
==============================================================================
*/

void  Direct3dState::set_vertex_mode_color ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_color_vertex_op != VertexOp_SELECT_ARG1)
   {
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_SELECTARG1));
      _cur_color_vertex_op = VertexOp_SELECT_ARG1;
   }

   if (_cur_color_vertex_arg1 != VertexArg_DIFFUSE)
   {
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_DIFFUSE));
      _cur_color_vertex_arg1 = VertexArg_DIFFUSE;
   }

   if (_cur_alpha_vertex_op != VertexOp_SELECT_ARG1)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1));
      _cur_alpha_vertex_op = VertexOp_SELECT_ARG1;
   }


   if (_cur_alpha_vertex_arg1 != VertexArg_DIFFUSE)
   {  
      check_hresult (_device_ptr->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE));
      _cur_alpha_vertex_arg1 = VertexArg_DIFFUSE;
   }
}



/*
==============================================================================
Name : set_sampler_filter_mode_nearest
==============================================================================
*/

void  Direct3dState::set_sampler_filter_mode_nearest ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_filter_mode != SamplerFilterMode_NEAREST)
   {
      _cur_sampler_filter_mode = SamplerFilterMode_NEAREST;
      _device_ptr->SetSamplerState (0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      _device_ptr->SetSamplerState (0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
   }
}



/*
==============================================================================
Name : set_sampler_filter_mode_linear
==============================================================================
*/

void  Direct3dState::set_sampler_filter_mode_linear ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_filter_mode != SamplerFilterMode_LINEAR)
   {
      _cur_sampler_filter_mode = SamplerFilterMode_LINEAR;
      _device_ptr->SetSamplerState (0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      _device_ptr->SetSamplerState (0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
   }
}



/*
==============================================================================
Name : set_sampler_address_u_mode_wrap
==============================================================================
*/

void  Direct3dState::set_sampler_address_u_mode_wrap ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_address_u_mode != SamplerAddressMode_WRAP)
   {
      _cur_sampler_address_u_mode = SamplerAddressMode_WRAP;
      _device_ptr->SetSamplerState (0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
   }
}



/*
==============================================================================
Name : set_sampler_address_u_mode_clamp
==============================================================================
*/

void  Direct3dState::set_sampler_address_u_mode_clamp ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_address_u_mode != SamplerAddressMode_CLAMP)
   {
      _cur_sampler_address_u_mode = SamplerAddressMode_CLAMP;
      _device_ptr->SetSamplerState (0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
   }
}



/*
==============================================================================
Name : set_sampler_address_v_mode_wrap
==============================================================================
*/

void  Direct3dState::set_sampler_address_v_mode_wrap ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_address_v_mode != SamplerAddressMode_WRAP)
   {
      _cur_sampler_address_v_mode = SamplerAddressMode_WRAP;
      _device_ptr->SetSamplerState (0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
   }
}



/*
==============================================================================
Name : set_sampler_address_v_mode_clamp
==============================================================================
*/

void  Direct3dState::set_sampler_address_v_mode_clamp ()
{
   assert (_device_ptr != 0);
   assert (_inited_flag);

   if (_cur_sampler_address_v_mode != SamplerAddressMode_CLAMP)
   {
      _cur_sampler_address_v_mode = SamplerAddressMode_CLAMP;
      _device_ptr->SetSamplerState (0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  Direct3dState::set_render_state_once (::DWORD &state, ::_D3DRENDERSTATETYPE type, ::DWORD value)
{
   bool           ok_flag = true;

   if (state != value)
   {
      ok_flag = check_hresult (_device_ptr->SetRenderState (type, value));
      if (ok_flag)
      {
         state = value;
      }
   }

   return (ok_flag);
}



/*
==============================================================================
Name : check_hresult
==============================================================================
*/

bool  Direct3dState::check_hresult (::HRESULT result)
{
   assert (result == D3D_OK);

   return (result == D3D_OK);
}



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

