/*****************************************************************************

        DirectXEngine.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70238

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

#include "ohm/archi/def.h"
#include "ohm/lang/def.h"
#include "ohm/math/def.h"
#include "ohm/opa/Path.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/Cache.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/def.h"
#include "ohm/opal/directx/Direct3d.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/opal/directx/FVFVertex.h"
#include "ohm/opal/private/FragmentProgramManager.h"
#include "ohm/sys/ConvString.h"
#include "ohm/util/trace.h"

#include <set>
#include <cassert>
#include <cmath>

#define  ohm_opal_DirectXEngine_DEBUG_FLAG

#if defined (ohm_opal_DirectXEngine_DEBUG_FLAG)
   #define eng_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define eng_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define eng_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define eng_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define eng_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define eng_TRACE(msg_0)
   #define eng_TRACE1(fmt_0, arg1)
   #define eng_TRACE2(fmt_0, arg1, arg2)
   #define eng_TRACE3(fmt_0, arg1, arg2, arg3)
   #define eng_TRACE4(fmt_0, arg1, arg2, arg3, arg4)

#endif



namespace ohm
{
namespace opal
{



#if (ohm_opal_API == ohm_opal_API_DIRECTX)

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

DirectXEngine::DirectXEngine (::HWND hwnd)
:  _need_reset_flag (false)
,  _thread_id (sys::ThreadMgr::get_current_thread_id ())

,  _device_ptr (0)
,  _present_params ()
,  _hwnd (0)
,  _hdc (0)
,  _hwnd_surface_ptr  (0)

,  _cur_translation (opa::Point::_zero)
,  _cur_scissor_rect (opa::Rect::_zero)
,  _cur_view_size ()
,  _cur_surface_ptr (0)
,  _cur_fragment_program_enabled_flag (false)

,  _strategy ()
,  _capacities ()
,  _device_state ()
,  _cache_manager ()
,  _video_memory_size (0)

,  _d3d_pool_default_cnt (0)
{
   assert (_hwnd == 0);
   assert (_hdc == 0);
   assert (_device_ptr == 0);
   assert (_hwnd_surface_ptr == 0);

   {
      _hwnd = hwnd;
      _hdc = ::GetDC (_hwnd);

      assert (_hwnd != 0);
      assert (_hdc != 0);

      ::RECT rect;
      ::GetWindowRect (hwnd, &rect);
      
      // CreateDevice would fail with a zero sized window
      assert (rect.right != rect.left);
      assert (rect.bottom != rect.top);
   }

   {
      // log some information about the capabilities of the driver
      ::IDirect3D9 * direct_3d_ptr = Direct3d::use_instance ().get ();

      ::D3DCAPS9                 d3d_caps;
      ::D3DADAPTER_IDENTIFIER9   d3d_adapt;
      
      ::HRESULT result = D3D_OK;
      
      if (result == D3D_OK)
      {
         direct_3d_ptr->GetDeviceCaps (
            D3DADAPTER_DEFAULT, 
            D3DDEVTYPE_HAL, 
            &d3d_caps
         );

         if (result != D3D_OK)
         {
            ohm_util_TRACE_ERROR ("DirectXEngine::ctor: () : GetDeviceCaps failed");
         }
      }

      if (result == D3D_OK)
      {
         result = direct_3d_ptr->GetAdapterIdentifier (
            d3d_caps.AdapterOrdinal,
            0,
            &d3d_adapt
         );

         if (result != D3D_OK)
         {
            ohm_util_TRACE_ERROR ("DirectXEngine::ctor: () : GetAdapterIdentifier failed");
         }
      }

      if (result == D3D_OK)
      {
         std::string    str;

         ohm_util_TRACE_INFO ("--- Graphic adapter ---");

         sys::ConvString::conv_native_string_to_utf8 (str, d3d_adapt.Driver);
         ohm_util_TRACE_INFO1 ("Driver        : %1%", str.c_str ());

         sys::ConvString::conv_native_string_to_utf8 (str, d3d_adapt.Description);
         ohm_util_TRACE_INFO1 ("Description   : %1%", str.c_str ());

         sys::ConvString::conv_native_string_to_utf8 (str, d3d_adapt.DeviceName);
         ohm_util_TRACE_INFO1 ("DeviceName    : %1%", str.c_str ());

         ohm_util_TRACE_INFO2 (
            "DriverVersion : 0x%1m8Xrw0%%2m8Xrw0%",
            d3d_adapt.DriverVersion.HighPart,
            d3d_adapt.DriverVersion.LowPart
         );
         ohm_util_TRACE_INFO1 ("VendorId      : 0x%1m8Xrw0%", d3d_adapt.VendorId);
         ohm_util_TRACE_INFO1 ("DeviceId      : 0x%1m8Xrw0%", d3d_adapt.DeviceId);
         ohm_util_TRACE_INFO1 ("SubSysId      : 0x%1m8Xrw0%", d3d_adapt.SubSysId);
         ohm_util_TRACE_INFO1 ("Revision      : 0x%1m8Xrw0%", d3d_adapt.Revision);
      }
   }
   
   // Important remarks about hardware vertex processing:
   // http://software.intel.com/en-us/articles/better-device-creation-for-hardware-acceleration/
   {
      ::DWORD     behavior_flags_base = 0;

      // was inited in VideoThreadImpl
      ::IDirect3D9 * direct_3d_ptr = Direct3d::use_instance ().get ();

      const int      nbr_tries = 3;
      static const ::DWORD loc_flags_arr [nbr_tries] =
      {
         D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
         D3DCREATE_MIXED_VERTEXPROCESSING,
         D3DCREATE_SOFTWARE_VERTEXPROCESSING
      };

      ::HRESULT         result = S_FALSE; // Anything different of D3D_OK
      for (int try_cnt = 0; try_cnt < nbr_tries && result != D3D_OK; ++try_cnt)
      {
         _device_ptr = 0;
         init_present_params (_present_params, hwnd);

         const ::DWORD     behavior_flags =
            behavior_flags_base | loc_flags_arr [try_cnt];

         ohm_util_TRACE_INFO1 (
            "ctor: Trying to initialise the device with flags 0x%1X%...",
            behavior_flags
         );

         result = direct_3d_ptr->CreateDevice (
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            behavior_flags,
            &_present_params,
            &_device_ptr
         );
         
         if (result != D3D_OK && _device_ptr != 0)
         {
            _device_ptr->Release ();
            _device_ptr = 0;
            ohm_util_TRACE_ERROR2 (
               "ctor: CreateDevice() attempts #%1% failed, returning 0x%2X%.",
               try_cnt,
               static_cast <unsigned long> (result)
            );
         }
      }

      if (result == D3D_OK)
      {
         ohm_util_TRACE_INFO ("ctor: Device successfully created.");
      }
      else
      {
         ohm_util_TRACE_ERROR1 (
            "ctor: All CreateDevice() attempts failed, returning 0x%1X%.",
            static_cast <unsigned long> (result)
         );

         ::MessageBoxW (
            _hwnd,
            L"Error: your graphics card is not supported.\n"
            L"\n"
            L"It is most likely quite ancient or lacks important\n"
            L"3-D features for some reason.\n",
            L"Error",
            MB_OK | MB_ICONSTOP
         );

         throw std::runtime_error (
            "ohm::opal::DirectXEngine::ctor: CreateDevice() failed."
         );
      }

      assert (result == D3D_OK);
      assert (_device_ptr != 0);
   }
   
   {
      HRESULT result = _device_ptr->GetRenderTarget (0, &_hwnd_surface_ptr);
      assert (result == D3D_OK);
      assert (_hwnd_surface_ptr != 0);
   }

   _capacities.init (_device_ptr);
   assert (_capacities.is_inited ());
   assert (_capacities.can_do_hardware_primitives ());
   assert (_capacities.can_do_alpha_blend_op ());

   build_strategy ();
   _strategy._use_double_buffer_flag = false;
   
   _device_state.init (_device_ptr);
   assert (_device_state.is_inited ());

   reset_device_state ();

   _need_reset_flag = false;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DirectXEngine::~DirectXEngine ()
{
   try
   {
      assert (_hwnd_surface_ptr != 0);
      assert (_device_ptr != 0);
      assert (_hdc != 0);
      assert (_hwnd != 0);
      
      if (_hwnd_surface_ptr != 0)
      {
         _hwnd_surface_ptr->Release ();
         _hwnd_surface_ptr = 0;
      }

      if (_device_ptr != 0)
      {
         _device_ptr->Release ();
         _device_ptr = 0;
      }

      if (_hdc != 0)
      {
         ::ReleaseDC (_hwnd, _hdc);
         _hdc = 0;
      }
   
      _device_state.restore ();

      _hwnd = 0;  
      assert (_hdc == 0);
      assert (_hwnd == 0);
      assert (_device_ptr == 0);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : check_thread_id
==============================================================================
*/

void  DirectXEngine::check_thread_id () const
{
   sys::ThreadMgr::IdType cur_thread_id
      = sys::ThreadMgr::get_current_thread_id ();
   
   assert (cur_thread_id == _thread_id);
}



/*
==============================================================================
Name : get_video_memory_size
==============================================================================
*/

size_t   DirectXEngine::get_video_memory_size () const
{
   return _video_memory_size;
}



/*
==============================================================================
Name : use_cache_manager
==============================================================================
*/

CacheManager & DirectXEngine::use_cache_manager ()
{
   return _cache_manager;
}



/*
==============================================================================
Name : hwnd_size_changed
==============================================================================
*/

void  DirectXEngine::hwnd_size_changed (int width, int height)
{
   assert (_device_ptr != 0);

   if (width > 0 && height > 0)
   {
      _present_params.BackBufferWidth = width;
      _present_params.BackBufferHeight = height;

      if (!_need_reset_flag)
      {
         D3DSURFACE_DESC surf_desc;
         HRESULT result = _hwnd_surface_ptr->GetDesc (&surf_desc);
         assert (result == D3D_OK);
         
         bool changed_flag
            = (surf_desc.Height != _present_params.BackBufferHeight)
            || (surf_desc.Width != _present_params.BackBufferWidth);
         
         if (changed_flag)
         {
            lost_device ();
            
            // will be 0x0 when minimized, some driver do not
            // support a 0x0 surface
            bool ok_flag = (width > 0) && (height > 0);
            
            if (ok_flag)
            {
               reset (true);
            }
         }
      }
   }
}



/*
==============================================================================
Name : notify_surface_released
==============================================================================
*/

void  DirectXEngine::notify_surface_released (IDirect3DSurface9 * surface_ptr)
{  
   // since '_cur_surface_ptr' is implicitely used as an id for
   // state change detection, when a surface is no longer available,
   // we need to reset it, to ensure that the diff system works even
   // if the exact same surface pointer is emited from the device
   
   if (_cur_surface_ptr == surface_ptr)
   {
      _cur_surface_ptr = 0;
   }
}



/*
==============================================================================
Name : update_context
==============================================================================
*/

void  DirectXEngine::update_context (::IDirect3DSurface9 * surface_ptr, const opa::Size & size, const GraphicState & graphic_state)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);
   
   if (surface_ptr == 0)
   {
      surface_ptr = _hwnd_surface_ptr;
      assert (surface_ptr != 0);
   }
   
#if !defined (NDEBUG)
   {
      D3DSURFACE_DESC desc;

      HRESULT result = surface_ptr->GetDesc (&desc);
      assert (result == D3D_OK);

      assert (desc.Width == UINT (size._width));
      assert (desc.Height == UINT (size._height));
   }
#endif

   if (_cur_surface_ptr != surface_ptr)
   {
      _cur_surface_ptr = surface_ptr;
      
      HRESULT result = _device_ptr->SetRenderTarget (0, surface_ptr);
      assert (result == D3D_OK);
   }
   
   /*
   this is the default view port dimension which implicitely
   sets the 'SetRenderTarget' call above

   D3DVIEWPORT9 vp;
   vp.X      = 0;
   vp.Y      = 0;
   vp.Width  = DWORD (size._width);
   vp.Height = DWORD (size._height);
   vp.MinZ   = 0.0f;
   vp.MaxZ   = 1.0f;

   HRESULT result = _device_ptr->SetViewport (&vp);
   assert (result == D3D_OK);
   */

   _cur_view_size = size;
   _cur_translation = graphic_state._total_translation;

   switch (graphic_state._blend_mode)
   {
   case BlendMode_NORMAL:
      _device_state.set_blend_mode_normal ();
      break;

   case BlendMode_MULTIPLY:
      _device_state.set_blend_mode_multiply ();
      break;

   case BlendMode_SCREEN:
      _device_state.set_blend_mode_screen ();
      break;

   case BlendMode_REPLACE:
      _device_state.set_blend_mode_replace ();
      break;

   case BlendMode_ADD:
      _device_state.set_blend_mode_add ();
      break;

   case BlendMode_MAX:
      _device_state.set_blend_mode_max ();
      break;

   case BlendMode_MIN:
      _device_state.set_blend_mode_min ();
      break;
   
   default:
      assert (false);
      break;
   }
   
   opa::Rect clip_rect = graphic_state._global_clip_rect;
   assert (!clip_rect.is_empty ());
   
   // compensate small calculation imprecisions
   clip_rect.round ();

   opa::Rect view_rect (opa::Point::_zero, _cur_view_size);
   
   assert ((clip_rect | view_rect) == view_rect);
   
   // scissor
   {
      //eng_TRACE4 ("scissor %1% %2% %3% %4%", clip_rect._origin._x, clip_rect._origin._y, clip_rect._size._width, clip_rect._size._height);

      RECT scissor_rect;
      scissor_rect.top = LONG (clip_rect._origin._y);
      scissor_rect.bottom = LONG (scissor_rect.top  + clip_rect._size._height);
      scissor_rect.left = LONG (clip_rect._origin._x);
      scissor_rect.right = LONG (scissor_rect.left + clip_rect._size._width);

      HRESULT result = _device_ptr->SetScissorRect (&scissor_rect);
      assert (result == D3D_OK);
      
      _cur_scissor_rect = clip_rect;
   }
   
   bool fragment_program_enabled_flag = (graphic_state._fragment_program_proc != 0);
   
   if (_cur_fragment_program_enabled_flag != fragment_program_enabled_flag)
   {
      _cur_fragment_program_enabled_flag = fragment_program_enabled_flag;

      if (fragment_program_enabled_flag)
      {
//       manager the pixel shader

//       _device_ptr->SetPixelShader (...);
      }
      else
      {
         _device_ptr->SetPixelShader (0);
      }
   }
}



/*
==============================================================================
Name : update_context
Note :
   Do not draw if returns false, but still call end_scene ()
==============================================================================
*/

bool  DirectXEngine::begin_scene ()
{
   assert (_device_ptr != 0);
   
   bool ret_val = !_need_reset_flag;

   if (ret_val)
   {
      ::HRESULT result = _device_ptr->BeginScene ();
      assert (result == D3D_OK);
   }

   return ret_val;
}



/*
==============================================================================
Name : end_scene
==============================================================================
*/

void  DirectXEngine::end_scene ()
{
   assert (_device_ptr != 0);
   
   if (!_need_reset_flag)
   {
      ::HRESULT result = _device_ptr->EndScene ();
      assert (result == D3D_OK);
   }
}



/*
==============================================================================
Name : present
==============================================================================
*/

void  DirectXEngine::present ()
{
   assert (_device_ptr != 0);
   
   if (!_need_reset_flag)
   {
      // see IDirect3DSwapChain9
      // see D3DPRESENT_INTERVAL_IMMEDIATE
      
      // or may be :
      // EndScene for each view
      // WaitForVerticalBlank WaitForVBlank (IDirect3DDevice9Ex only since vista)
      // GetRasterStatus to check if we are InVBlank
      // raise the priority of the thread to real time
      // timeBeginPeriod / Sleep
      // Present immediate for all views
      
      ::HRESULT result = _device_ptr->Present (0, 0, 0, 0);

      if (result == D3DERR_DEVICELOST)
      {
         lost_device ();
         _need_reset_flag = true;
      }
      else
      {
         assert (result == D3D_OK);
      }
   }

   if (_need_reset_flag)
   {
      reset (false);
   }
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  DirectXEngine::fill (const opa::Path & path, const Color & color)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);

   // for the -0.5 offset see 
   // http://msdn.microsoft.com/en-us/library/bb219690(VS.85).aspx
   // and http://msdn.microsoft.com/en-us/library/bb147314(VS.85).aspx

   const float tx = _cur_translation._x - 0.5f;
   const float ty = _cur_translation._y - 0.5f;

   const archi::UByte a8 = archi::UByte (color._a * 255.f);
   const archi::UByte r8 = archi::UByte (color._a * color._r * 255.f);
   const archi::UByte g8 = archi::UByte (color._a * color._g * 255.f);
   const archi::UByte b8 = archi::UByte (color._a * color._b * 255.f);

   const ::D3DCOLOR  d3d_color = D3DCOLOR_ARGB (a8, r8, g8, b8);
   const std::vector <opa::Point> & 
                     pt_arr = path.get_polygon ();
   
   const size_t      buf_len = pt_arr.size () * sizeof (ColorVertex);

   _color_vertex_data.resize (buf_len);
   ColorVertex * v_arr = reinterpret_cast <ColorVertex *> (& _color_vertex_data [0]);

   for (size_t i = 0 ; i < pt_arr.size () ; ++i)
   {
      const opa::Point & pt = pt_arr [i];
      
      v_arr [i].x = pt._x + tx;
      v_arr [i].y = pt._y + ty;
      v_arr [i].z = 0.f;
      v_arr [i].rhw = 1.f;
      v_arr [i].color = d3d_color;
   }
   
   ::HRESULT result = _device_ptr->SetFVF (ColorVertex::_fvf_flag);
   assert (result == D3D_OK);

   _device_state.set_vertex_mode_color ();

   result = _device_ptr->DrawPrimitiveUP (D3DPT_TRIANGLEFAN, pt_arr.size () - 2, v_arr, sizeof (ColorVertex));
   assert (result == D3D_OK);
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  DirectXEngine::fill (const opa::Rect & rect, const Color & color)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);

   const float tx = _cur_translation._x - 0.5f;
   const float ty = _cur_translation._y - 0.5f;

   const archi::UByte a8 = archi::UByte (color._a * 255.f);
   const archi::UByte r8 = archi::UByte (color._a * color._r * 255.f);
   const archi::UByte g8 = archi::UByte (color._a * color._g * 255.f);
   const archi::UByte b8 = archi::UByte (color._a * color._b * 255.f);

   const ::D3DCOLOR  d3d_color = D3DCOLOR_ARGB (a8, r8, g8, b8);
      
   _color_vertex_data.resize (4 * sizeof (ColorVertex));
   ColorVertex * v_arr = reinterpret_cast <ColorVertex *> (& _color_vertex_data [0]);
   
   v_arr [0].x = rect._origin._x + tx;
   v_arr [0].y = rect._origin._y + ty;
   v_arr [0].z = 0.f;
   v_arr [0].rhw = 1.f;
   v_arr [0].color = d3d_color;

   v_arr [1].x = rect._origin._x + rect._size._width + tx;
   v_arr [1].y = rect._origin._y + ty;
   v_arr [1].z = 0.f;
   v_arr [1].rhw = 1.f;
   v_arr [1].color = d3d_color;

   v_arr [2].x = rect._origin._x + rect._size._width + tx;
   v_arr [2].y = rect._origin._y + rect._size._height + ty;
   v_arr [2].z = 0.f;
   v_arr [2].rhw = 1.f;
   v_arr [2].color = d3d_color;

   v_arr [3].x = rect._origin._x + tx;
   v_arr [3].y = rect._origin._y + rect._size._height + ty;
   v_arr [3].z = 0.f;
   v_arr [3].rhw = 1.f;
   v_arr [3].color = d3d_color;
   
   ::HRESULT result = _device_ptr->SetFVF (ColorVertex::_fvf_flag);
   assert (result == D3D_OK);
   
   _device_state.set_vertex_mode_color ();

   result = _device_ptr->DrawPrimitiveUP (D3DPT_TRIANGLEFAN, 2, v_arr, sizeof (ColorVertex));
   assert (result == D3D_OK);
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  DirectXEngine::fill (const opa::Quad & vertices, const Color & color)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);

   const float tx = _cur_translation._x - 0.5f;
   const float ty = _cur_translation._y - 0.5f;

   const archi::UByte a8 = archi::UByte (color._a * 255.f);
   const archi::UByte r8 = archi::UByte (color._a * color._r * 255.f);
   const archi::UByte g8 = archi::UByte (color._a * color._g * 255.f);
   const archi::UByte b8 = archi::UByte (color._a * color._b * 255.f);

   const  ::D3DCOLOR d3d_color = D3DCOLOR_ARGB (a8, r8, g8, b8);
      
   _color_vertex_data.resize (4 * sizeof (ColorVertex));
   ColorVertex * v_arr = reinterpret_cast <ColorVertex *> (& _color_vertex_data [0]);
   
   for (int v_cnt = 0; v_cnt < 4; ++ v_cnt)
   {
      opa::Point vertice = vertices [v_cnt];

      v_arr [v_cnt].x = vertice._x + tx;
      v_arr [v_cnt].y = vertice._y + ty;
      v_arr [v_cnt].z = 0.f;
      v_arr [v_cnt].rhw = 1.f;
      v_arr [v_cnt].color = d3d_color;    
   }

   ::HRESULT result = _device_ptr->SetFVF (ColorVertex::_fvf_flag);
   assert (result == D3D_OK);
   
   _device_state.set_vertex_mode_color ();

   result = _device_ptr->DrawPrimitiveUP (D3DPT_TRIANGLEFAN, 2, v_arr, sizeof (ColorVertex));
   assert (result == D3D_OK);
}



/*
==============================================================================
Name : stroke
==============================================================================
*/

void  DirectXEngine::stroke (const opa::Path & path, const Color & color)
{
   // not used anymore, must be removed from the interface
   assert (false);
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  DirectXEngine::draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);

   if (image._data_ref != 0)
   {
      assert (image._width > 0);
      assert (image._height > 0);

      Image::DataRef data_ref = image._data_ref;
      
      assert (_device_ptr != 0);
            
      const float tx = _cur_translation._x - 0.5f;
      const float ty = _cur_translation._y - 0.5f;

      const archi::UByte a8 = archi::UByte (alpha * 255.f);
      const ::D3DCOLOR  d3d_color = D3DCOLOR_ARGB (a8, a8, a8, a8);

      _texture_vertex_data.resize (4 * sizeof (TextureVertex));
      TextureVertex * v_arr = reinterpret_cast <TextureVertex *> (& _texture_vertex_data [0]);

      v_arr [0].x = vertices [0]._x + tx;
      v_arr [0].y = vertices [0]._y + ty;
      v_arr [0].z = 0.f;
      v_arr [0].rhw = 1.f;
      v_arr [0].color = d3d_color;
      v_arr [0].tu = tex_coords [0]._x;
      v_arr [0].tv = tex_coords [0]._y;

      v_arr [1].x = vertices [1]._x + tx;
      v_arr [1].y = vertices [1]._y + ty;
      v_arr [1].z = 0.f;
      v_arr [1].rhw = 1.f;
      v_arr [1].color = d3d_color;
      v_arr [1].tu = tex_coords [1]._x;
      v_arr [1].tv = tex_coords [1]._y;

      v_arr [2].x = vertices [2]._x + tx;
      v_arr [2].y = vertices [2]._y + ty;
      v_arr [2].z = 0.f;
      v_arr [2].rhw = 1.f;
      v_arr [2].color = d3d_color;
      v_arr [2].tu = tex_coords [2]._x;
      v_arr [2].tv = tex_coords [2]._y;

      v_arr [3].x = vertices [3]._x + tx;
      v_arr [3].y = vertices [3]._y + ty;
      v_arr [3].z = 0.f;
      v_arr [3].rhw = 1.f;
      v_arr [3].color = d3d_color;
      v_arr [3].tu = tex_coords [3]._x;
      v_arr [3].tv = tex_coords [3]._y;
      
      ::HRESULT result = _device_ptr->SetTexture (0, data_ref->_texture_ptr);
      assert (result == D3D_OK);
      
      _device_state.set_vertex_mode_image ();
      
      result = _device_ptr->SetFVF (TextureVertex::_fvf_flag);
      assert (result == D3D_OK);
      
      if (image._filter_mode == FilterMode_LINEAR)
      {
         _device_state.set_sampler_filter_mode_linear ();
      }
      else
      {
         _device_state.set_sampler_filter_mode_nearest ();
      }
      
      if (image._sprung_mode_x == SprungMode_REPEAT)
      {
         _device_state.set_sampler_address_u_mode_wrap ();
      }
      else
      {
         _device_state.set_sampler_address_u_mode_clamp ();
      }
      
      if (image._sprung_mode_y == SprungMode_REPEAT)
      {
         _device_state.set_sampler_address_v_mode_wrap ();
      }
      else
      {
         _device_state.set_sampler_address_v_mode_clamp ();
      }

      result = _device_ptr->DrawPrimitiveUP (D3DPT_TRIANGLEFAN, 2, v_arr, sizeof (TextureVertex));
      assert (result == D3D_OK);

      result = _device_ptr->SetTexture (0, 0);
      assert (result == D3D_OK);
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  DirectXEngine::draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);

   if (mask._data_ref != 0)
   {
      assert (mask._width > 0);
      assert (mask._height > 0);

      Image::DataRef data_ref = mask._data_ref;

      assert (_device_ptr != 0);

      const float tx = _cur_translation._x - 0.5f;
      const float ty = _cur_translation._y - 0.5f;

      archi::UByte a8 = archi::UByte (color._a * 255.f);
      archi::UByte r8 = archi::UByte (color._a * color._r * 255.f);
      archi::UByte g8 = archi::UByte (color._a * color._g * 255.f);
      archi::UByte b8 = archi::UByte (color._a * color._b * 255.f);

      ::D3DCOLOR d3d_color = D3DCOLOR_ARGB (a8, r8, g8, b8);
      
      _texture_vertex_data.resize (4 * sizeof (TextureVertex));
      TextureVertex * v_arr = reinterpret_cast <TextureVertex *> (& _texture_vertex_data [0]);

      v_arr [0].x = vertices [0]._x + tx;
      v_arr [0].y = vertices [0]._y + ty;
      v_arr [0].z = 0.f;
      v_arr [0].rhw = 1.f;
      v_arr [0].color = d3d_color;
      v_arr [0].tu = tex_coords [0]._x;
      v_arr [0].tv = tex_coords [0]._y;

      v_arr [1].x = vertices [1]._x + tx;
      v_arr [1].y = vertices [1]._y + ty;
      v_arr [1].z = 0.f;
      v_arr [1].rhw = 1.f;
      v_arr [1].color = d3d_color;
      v_arr [1].tu = tex_coords [1]._x;
      v_arr [1].tv = tex_coords [1]._y;

      v_arr [2].x = vertices [2]._x + tx;
      v_arr [2].y = vertices [2]._y + ty;
      v_arr [2].z = 0.f;
      v_arr [2].rhw = 1.f;
      v_arr [2].color = d3d_color;
      v_arr [2].tu = tex_coords [2]._x;
      v_arr [2].tv = tex_coords [2]._y;

      v_arr [3].x = vertices [3]._x + tx;
      v_arr [3].y = vertices [3]._y + ty;
      v_arr [3].z = 0.f;
      v_arr [3].rhw = 1.f;
      v_arr [3].color = d3d_color;
      v_arr [3].tu = tex_coords [3]._x;
      v_arr [3].tv = tex_coords [3]._y;

      ::HRESULT result = _device_ptr->SetTexture (0, data_ref->_texture_ptr);
      assert (result == D3D_OK);

      _device_state.set_vertex_mode_image ();
      
      result = _device_ptr->SetFVF (TextureVertex::_fvf_flag);
      assert (result == D3D_OK);

      if (mask._filter_mode == FilterMode_LINEAR)
      {
         _device_state.set_sampler_filter_mode_linear ();
      }
      else
      {
         _device_state.set_sampler_filter_mode_nearest ();
      }
      
      if (mask._sprung_mode_x == SprungMode_REPEAT)
      {
         _device_state.set_sampler_address_u_mode_wrap ();
      }
      else
      {
         _device_state.set_sampler_address_u_mode_clamp ();
      }
      
      if (mask._sprung_mode_y == SprungMode_REPEAT)
      {
         _device_state.set_sampler_address_v_mode_wrap ();
      }
      else
      {
         _device_state.set_sampler_address_v_mode_clamp ();
      }

      result = _device_ptr->DrawPrimitiveUP (D3DPT_TRIANGLEFAN, 2, v_arr, sizeof (TextureVertex));
      assert (result == D3D_OK);

      result = _device_ptr->SetTexture (0, 0);
      assert (result == D3D_OK);
   }
}



/*
==============================================================================
Name : prepare
==============================================================================
*/

void  DirectXEngine::prepare (Image & image, size_t width, size_t height)
{
   image._width = width;
   image._height = height;
   
   if (_strategy._support_npot_flag)
   {
      image._tex_width = width;
      image._tex_height = height;
   }
   else
   {
      image._tex_width = get_next_pot (width);
      image._tex_height = get_next_pot (height);
   }
   
   image._scanline_width = ((image._tex_width + 31) / 32) * 32;
// image._scanline_width = image._tex_width;
}



/*
==============================================================================
Name : create
Note :
   buf_ptr is an array of size 'image._scan_line_width * image._height'
==============================================================================
*/

void  DirectXEngine::create (Image & image, const PixArgb * buf_ptr, const Image::Conf & conf, bool buf_owner_flag)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);
   
   image._eng_ptr = this;
   
   assert (image._data_ref == 0);
   ::HRESULT result = D3D_OK;
   
   if (image._width > 0 && image._height > 0)
   {
      image._data_ref = new Image::Data;
      image._data_ref->_texture_ptr = 0;
      image._data_ref->_retain_cnt = 1;
      image._data_ref->_buf_ptr = buf_ptr;
      image._data_ref->_buf_owner_flag = buf_owner_flag;

      make_tex_props (image, conf);

      assert (_device_ptr != 0);
      
      create (image, 0);      
      restore (image, buf_ptr);
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  DirectXEngine::release (Image & image)
{
   if (image._data_ref->_buf_ptr == 0)
   {
      --_d3d_pool_default_cnt;
      
      //ohm_util_TRACE_DEBUG1 ("D3DPOOL_DEFAULT - %1%", _d3d_pool_default_cnt);
   }
   
   ULONG ref_cnt = image._data_ref->_texture_ptr->Release ();
   assert (ref_cnt == 0);
   image._data_ref->_texture_ptr = 0;
}



/*
==============================================================================
Name : save
Description :
   Save the texture surface content to 'image' pixel buffer.
   Used from the cache system.
==============================================================================
*/

void  DirectXEngine::save (Image & image)
{
   assert (image._data_ref->_buf_owner_flag == false);
   
   IDirect3DTexture9 * texture_ptr = image._data_ref->_texture_ptr;
   assert (texture_ptr != 0);
   
   assert (image._data_ref->_buf_ptr == 0);
   
   PixArgb * buf_ptr = new PixArgb [image._scanline_width * image._height];
   image._data_ref->_buf_ptr = buf_ptr;
   image._data_ref->_buf_owner_flag = true;
   
   ::IDirect3DSurface9 * surface_ptr = 0;
   ::HRESULT result = texture_ptr->GetSurfaceLevel (0, &surface_ptr);
   assert (result == D3D_OK);
   assert (surface_ptr != 0);
   
   D3DLOCKED_RECT locked_rect;
   result = surface_ptr->LockRect (
      &locked_rect,
      0, // entire surface
      0  // default mode
   );
   assert (result == D3D_OK);
   
   archi::UByte * data_ptr = reinterpret_cast <archi::UByte *> (locked_rect.pBits);
   
   for (size_t y = 0 ; y < image._height ; ++y)
   {
      archi::UByte * pix_ptr = &data_ptr [y * locked_rect.Pitch];
      
      memcpy (&buf_ptr [y * image._scanline_width], pix_ptr, image._width * 4);
   }
   
   result = surface_ptr->UnlockRect ();
   assert (result == D3D_OK);
   
   ULONG ref_cnt = surface_ptr->Release ();
   surface_ptr = 0;
}



/*
==============================================================================
Name : restore
Note :
   Used in 2 case :
   - from 'create' to create an image from a pixel buffer
   - from cache, to restore a cache from a pixel buffer
==============================================================================
*/

void  DirectXEngine::restore (Image & image, const PixArgb * buf_ptr)
{
   IDirect3DTexture9 * texture_ptr = image._data_ref->_texture_ptr;
   assert (texture_ptr != 0);
   
   ::IDirect3DSurface9 * surface_ptr = 0;
   ::HRESULT result = texture_ptr->GetSurfaceLevel (0, &surface_ptr);
   assert (result == D3D_OK);
   assert (surface_ptr != 0);
   
   D3DLOCKED_RECT locked_rect;
   result = surface_ptr->LockRect (
      &locked_rect,
      0, // entire surface
      0  // default mode
   );
   assert (result == D3D_OK);
   
   archi::UByte * data_ptr = reinterpret_cast <archi::UByte *> (locked_rect.pBits);
   
   for (size_t y = 0 ; y < image._height ; ++y)
   {
      archi::UByte * pix_ptr = &data_ptr [y * locked_rect.Pitch];
      
      memcpy (pix_ptr, &buf_ptr [y * image._scanline_width], image._width * 4);
   }
   
   result = surface_ptr->UnlockRect ();
   assert (result == D3D_OK);
   
   ULONG ref_cnt = surface_ptr->Release ();
   surface_ptr = 0;
}



/*
==============================================================================
Name : create
Note :
   Cache version
==============================================================================
*/

void  DirectXEngine::create (Image & image, const Image::Conf & conf)
{
   assert (_device_ptr != 0);
   assert (!_need_reset_flag);
   assert (image._data_ref == 0);
   
   image._eng_ptr = this;
   
   ::HRESULT result = D3D_OK;
   
   if (image._width > 0 && image._height > 0)
   {
      image._data_ref = new Image::Data;
      image._data_ref->_texture_ptr = 0;
      image._data_ref->_retain_cnt = 1;
      image._data_ref->_buf_ptr = 0;
      image._data_ref->_buf_owner_flag = false;

      make_tex_props (image, conf);

      assert (_device_ptr != 0);
      
      create (image, D3DUSAGE_RENDERTARGET);
      
      ++_d3d_pool_default_cnt;
      
      //ohm_util_TRACE_DEBUG1 ("D3DPOOL_DEFAULT + %1%", _d3d_pool_default_cnt);
   }
}



/*
==============================================================================
Name : create
Note :
   Cache version
==============================================================================
*/

void  DirectXEngine::create (Image & image, ::DWORD usage)
{
   IDirect3DTexture9 * & texture_ptr = image._data_ref->_texture_ptr;
   assert (texture_ptr == 0);
      
   ::HRESULT result = _device_ptr->CreateTexture (
      image._tex_width,
      image._tex_height,
      1, // levels
      usage,
      D3DFMT_A8R8G8B8,
      (usage == D3DUSAGE_RENDERTARGET) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED,
      &texture_ptr,
      0  // reserved
   );
   assert (result == D3D_OK);
   assert (texture_ptr != 0);
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  DirectXEngine::create (archi::UInt32 & program_id, FragmentProgramProc fragment_program_proc)
{
   const std::string & program_text
      = FragmentProgramManager::use_instance ().compile (fragment_program_proc);

   // use ::D3DXCompileShader, then ::IDirect3DDevice9::CreatePixelShader
#if 0
   ohm_util_TRACE_DEBUG1 ("%1%", program_text.c_str ());
#endif
}



/*
==============================================================================
Name : debug_fill
==============================================================================
*/

void  DirectXEngine::debug_fill (const opa::Rect & rect)
{
   assert (_device_ptr != 0);
   
   if (_cur_surface_ptr == _hwnd_surface_ptr)
   {
      opa::Rect trect = rect;
      trect -= _cur_translation;
      
      trect.round ();
      
      _device_ptr->SetRenderState (D3DRS_SCISSORTESTENABLE, FALSE);
      
      fill (trect, Color (1.f, 1.f, 0.f, 1.f));

      _device_ptr->SetRenderState (D3DRS_SCISSORTESTENABLE, TRUE);
   }
}



/*
==============================================================================
Name : debug_flash
==============================================================================
*/

void  DirectXEngine::debug_flash ()
{
   assert (_device_ptr != 0);
   
   if (_cur_surface_ptr == _hwnd_surface_ptr)
   {
      end_scene ();
      
      bool ok_flag = begin_scene ();
      assert (ok_flag);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_d3d_device
==============================================================================
*/

::IDirect3DDevice9 * DirectXEngine::get_d3d_device ()
{
   assert (_device_ptr != 0);

   return (_device_ptr);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_tex_props
==============================================================================
*/

void  DirectXEngine::make_tex_props (Image & image, const Image::Conf & conf)
{
   Image::Data * data_ref = image._data_ref;
   assert (data_ref != 0);
   
   image._sprung_mode_x = conf._sprung_mode_x;
   image._sprung_mode_y = conf._sprung_mode_y;
   image._filter_mode = conf._filter_mode;
   
   if (
      (conf._sprung_mode_x == SprungMode_REPEAT)
      || (conf._sprung_mode_y == SprungMode_REPEAT)
      )
   {
      // some warnings for the graphist
      
      if ((conf._sprung_mode_x == SprungMode_REPEAT) && (image._width != image._tex_width) && !_strategy._support_npot_repeat_flag)
      {
         if (image._name_0 != 0)
         {
            ohm_util_TRACE_DEBUG1 (
               "Repeat X requested for '%1%', but width is not a power of two",
               image._name_0
            );
         }
         else
         {
            ohm_util_TRACE_DEBUG (
               "Repeat X requested for an unnamed image, but width is not a power of two"
            );
         }
      }
      
      if ((conf._sprung_mode_y == SprungMode_REPEAT) && (image._height != image._tex_height) && !_strategy._support_npot_repeat_flag)
      {
         if (image._name_0 != 0)
         {
            ohm_util_TRACE_DEBUG1 (
               "Repeat Y requested for '%1%', but height is not a power of two",
               image._name_0
            );
         }
         else
         {
            ohm_util_TRACE_DEBUG (
               "Repeat Y requested for an unnamed image, but height is not a power of two"
            );
         }
      }

      // configuration
      if (_strategy._support_npot_repeat_flag)
      {
         image._tex_s = 1.f;
         image._tex_t = 1.f;
         image._software_wrap_repeat_x_flag = false;
         image._software_wrap_repeat_y_flag = false;
      }
      else  
      {
         if (
            (conf._sprung_mode_x == SprungMode_REPEAT)
            && (image._width == image._tex_width)
            )
         {
            image._tex_s = 1.f;
            image._software_wrap_repeat_x_flag = false;
         }
         else
         {
            image._tex_s = float (image._width) / float (image._tex_width);
            image._software_wrap_repeat_x_flag = true;
         }
         
         if (
            (conf._sprung_mode_y == SprungMode_REPEAT)
            && (image._height == image._tex_height)
            )
         {  
            image._tex_t = 1.f;
            image._software_wrap_repeat_y_flag = false;
         }
         else
         {
            image._tex_t = float (image._height) / float (image._tex_height);
            image._software_wrap_repeat_y_flag = true;
         }
      }
   }
   else
   {
      image._software_wrap_repeat_x_flag = false;
      image._software_wrap_repeat_y_flag = false;
      
      if (_strategy._support_npot_flag)
      {
         image._tex_s = 1.f;
         image._tex_t = 1.f;
      }
      else
      {
         image._tex_s = float (image._width) / float (image._tex_width);
         image._tex_t = float (image._height) / float (image._tex_height);
      }
   }
}



/*
==============================================================================
Name : get_next_pot
==============================================================================
*/

size_t   DirectXEngine::get_next_pot (size_t x)
{
#if (ohm_archi_WORD_SIZE == 64)

   ohm_lang_CHECK_CST (size_t_is_64_bits, sizeof (size_t) == 8);
   
   --x;
   x = (x >> 1) | x;
   x = (x >> 2) | x;
   x = (x >> 4) | x;
   x = (x >> 8) | x;
   x = (x >> 16) | x;
   x = (x >> 32) | x;
   ++x;

#elif (ohm_archi_WORD_SIZE == 32)

   ohm_lang_CHECK_CST (size_t_is_32_bits, sizeof (size_t) == 4);
   
   --x;
   x = (x >> 1) | x;
   x = (x >> 2) | x;
   x = (x >> 4) | x;
   x = (x >> 8) | x;
   x = (x >> 16) | x;
   ++x;

#else
   #error architecture not supported
#endif
   
   return x;
}



/*
==============================================================================
Name : build_strategy
Description :
   Check the capacilities of the OpenGL Driver to issue different strategies
   dependant on the hardware we are running to.
==============================================================================
*/

void  DirectXEngine::build_strategy ()
{
   assert (_device_ptr != 0);
   
   // texture strategy : based on advertised device caps
   {
      // fall back case : always support POT
      _strategy._support_npot_flag = false;
      _strategy._support_npot_repeat_flag = false;
      

      switch (_capacities.can_do_tex_size ())
      {
      case Direct3dCaps::TexSize_NPOT:
         _strategy._support_npot_flag = true;
         _strategy._support_npot_repeat_flag = true;
         break;

      case Direct3dCaps::TexSize_NPOT_RESTRICTED:
         _strategy._support_npot_flag = true;
         break;
      }
   }

   // texture strategy : specific ATI problems
/* if (renderer.find ("ATI") == 0)
   {
      size_t num_start = renderer.find_first_of ("01234566789");
      size_t num_stop = renderer.find_first_not_of ("01234566789", num_start);
      
      std::string numbers;
         
      if (num_start != std::string::npos)
      {
         if (num_stop != std::string::npos)
         {
            numbers = renderer.substr (num_start, num_stop - num_start);
         }
         else
         {
            numbers = renderer.substr (num_start);
         }
      }
      else
      {
         // huh. ignore.
         ohm_util_TRACE_DEBUG1 ("'%1%' : problem detecting card serie", renderer.c_str ());
      }
      
      std::set <std::string> npot_repeat_pb_set;
      npot_repeat_pb_set.insert ("1800");
      npot_repeat_pb_set.insert ("1900");
      npot_repeat_pb_set.insert ("1600");
      npot_repeat_pb_set.insert ("1300");
      npot_repeat_pb_set.insert ("1000");
      npot_repeat_pb_set.insert ("9600");
      npot_repeat_pb_set.insert ("9550");
      npot_repeat_pb_set.insert ("9700");
      npot_repeat_pb_set.insert ("9800");
      npot_repeat_pb_set.insert ("600");
      npot_repeat_pb_set.insert ("800");
      
      if (npot_repeat_pb_set.find (numbers) != npot_repeat_pb_set.end ())
      {
         // driver advertised that NPOT mode is supported, but actually
         // GL_REPEAT is not supported with NPOT textures. This even
         // make the FBO completeness test to fail.
         
         _strategy._support_npot_repeat_flag = false;
      }
   }
*/ 
   // texture strategy : test fallback
   {
#if 0
      ohm_util_TRACE_WARNING ("Strategy test fallback.");
      _strategy._support_npot_flag = false;
      _strategy._support_npot_repeat_flag = false;
#endif
   }
   
   // sanity checks
   if (_strategy._support_npot_repeat_flag)
   {
      assert (_strategy._support_npot_flag);
   }

   _strategy._use_double_buffer_flag = false;
   
   ohm_util_TRACE_DEBUG ("DirectX strategy :");
   
   if (_strategy._support_npot_flag)
      ohm_util_TRACE_DEBUG ("  Support NPOT");

   if (_strategy._support_npot_repeat_flag)
      ohm_util_TRACE_DEBUG ("  Support NPOT for REPEAT");
   
   
   // memory
   
   _video_memory_size = _device_ptr->GetAvailableTextureMem ();
}



/*
==============================================================================
Name : lost_device
==============================================================================
*/

void DirectXEngine::lost_device ()
{
   assert (_device_ptr != 0);
   
   eng_TRACE ("DirectXEngine: device lost");
   
   ::HRESULT result = S_OK;
   
   ULONG ref_cnt = _hwnd_surface_ptr->Release ();
   assert (ref_cnt == 0);
   _hwnd_surface_ptr = 0;
   
   _cache_manager.save_all ();

   _need_reset_flag = true;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void DirectXEngine::reset (bool force_flag)
{
   assert (_device_ptr != 0);
   assert (_hwnd_surface_ptr == 0);
   assert (_need_reset_flag);

   eng_TRACE ("DirectXEngine: reset");
   
   ::HRESULT result = _device_ptr->TestCooperativeLevel ();

   if ((result == D3DERR_DEVICENOTRESET) || force_flag)
   {
      eng_TRACE ("DirectXEngine:   D3DERR_DEVICENOTRESET");
   
      result = _device_ptr->Reset (&_present_params);
      
      if (result == D3D_OK)
      {
         eng_TRACE ("DirectXEngine:     OK");

         _need_reset_flag = false;

         result = _device_ptr->GetRenderTarget (0, &_hwnd_surface_ptr);

         assert (result == D3D_OK);
         assert (_hwnd_surface_ptr != 0);
         
         _cache_manager.restore_all ();
         
         reset_device_state ();
      }
      else
      {
         eng_TRACE1 ("DirectXEngine:     Reset returned %1m8rw0X%", archi::UInt32 (result));
      }
   }
   else if (result == D3DERR_DEVICELOST)
   {
      // nothing, keep the device not reset
      
      eng_TRACE ("DirectXEngine:   D3DERR_DEVICELOST");
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : reset_device_state
==============================================================================
*/

void DirectXEngine::reset_device_state ()
{
   _device_state.reset_to_default ();
}



/*
==============================================================================
Name : init_present_params
==============================================================================
*/

void DirectXEngine::init_present_params (::D3DPRESENT_PARAMETERS & present_params, ::HWND hwnd)
{
   ::D3DPRESENT_PARAMETERS & dpp = present_params;
   
   ::ZeroMemory (&dpp, sizeof (dpp));
   
   dpp.Windowed = TRUE;
   dpp.hDeviceWindow = hwnd;
   
   // will be replaced with a valid format (normally X8R8G8B8)
   // after the CreateDevice call
   dpp.BackBufferFormat = D3DFMT_UNKNOWN;
   
   // if the 3 following values are zero, they will be updated
   // with right values after the CreateDevice call
   dpp.BackBufferCount = 1;
   dpp.BackBufferWidth = 0;
   dpp.BackBufferHeight = 0;
   
   dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
   dpp.MultiSampleQuality = 0;
   dpp.SwapEffect = D3DSWAPEFFECT_COPY;
   
// dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

   // see 'end_scene'
   dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
}



#endif   // (ohm_opal_API == ohm_opal_API_DIRECTX)


/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
