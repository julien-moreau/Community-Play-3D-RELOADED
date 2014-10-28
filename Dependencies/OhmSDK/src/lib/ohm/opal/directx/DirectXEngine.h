/*****************************************************************************

        DirectXEngine.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66813

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



#if ! defined (ohm_opal_DirectXEngine_HEADER_INCLUDED)
#define  ohm_opal_DirectXEngine_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/Direct3dState.h"
#include "ohm/opal/directx/Direct3dCaps.h"
#include "ohm/opal/directx/CacheManager.h"
#include "ohm/opal/private/GraphicContextBase.h"
#include "ohm/opal/private/FragmentProgramProc.h"
#include "ohm/sys/ThreadMgr.h"

#include <stack>

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>



namespace ohm
{
namespace opa
{
class Path;
class Quad;
}

namespace opal
{



class DirectXEngine
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   
                  DirectXEngine (::HWND hwnd);
   virtual        ~DirectXEngine ();
   
   void           check_thread_id () const;
   size_t         get_video_memory_size () const;
   
   CacheManager & use_cache_manager ();

   void           hwnd_size_changed (int width, int height);
   void           notify_surface_released (IDirect3DSurface9 * surface_ptr);
   void           update_context (IDirect3DSurface9 * surface_ptr, const opa::Size & size, const GraphicState & graphic_state);

   bool           begin_scene ();
   void           end_scene ();
   void           present ();
   
   void           fill (const opa::Path & path, const Color & color);
   void           fill (const opa::Rect & rect, const Color & color);
   void           fill (const opa::Quad & vertices, const Color & color);

   void           stroke (const opa::Path & path, const Color & color);
   
   void           draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha);
   void           draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color);
   
   void           prepare (Image & image, size_t width, size_t height);
   void           create (Image & image, const PixArgb * buf_ptr, const Image::Conf & conf, bool buf_owner_flag);
   void           release (Image & image);
   
   void           save (Image & image);
   void           restore (Image & image, const PixArgb * buf_ptr);

   void           create (Image & image, const Image::Conf & conf);
   void           create (Image & image, ::DWORD usage);

   void           create (archi::UInt32 & program_id, FragmentProgramProc fragment_program_proc);
   
   // debug
   void           debug_fill (const opa::Rect & rect);
   void           debug_flash ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   friend class ImageLoaderImpl;
   friend class CacheImpl;
                  
   ::IDirect3DDevice9 *
                  get_d3d_device ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum State
   {
                  State_CREATED = 0,
                  State_D3D_INITED,
                  State_DEVICE_INITED,
                  State_DEVICE_READY
   };

   class Strategy
   {
   public:
      bool        _support_npot_flag;
      bool        _support_npot_repeat_flag;
      bool        _use_double_buffer_flag;
   };
   
   typedef std::vector <archi::UByte>     DataBuffer;



   void           make_tex_props (Image & image, const Image::Conf & conf);

   void           build_strategy ();
   
   void           lost_device ();
   void           reset (bool force_flag);
   void           reset_device_state ();

   void           init_present_params (::D3DPRESENT_PARAMETERS & present_params, ::HWND hwnd);
   void           init_multisample_present_params (::D3DPRESENT_PARAMETERS & present_params);

   static size_t  get_next_pot (size_t x);

   bool           _need_reset_flag;
   sys::ThreadMgr::IdType
                  _thread_id;
   
   ::IDirect3DDevice9 *
                  _device_ptr;
   ::D3DPRESENT_PARAMETERS
                  _present_params;
   ::HWND         _hwnd;
   ::HDC          _hdc;
   ::IDirect3DSurface9 *
                  _hwnd_surface_ptr;
   
   // internal state
   opa::Point     _cur_translation;
   opa::Rect      _cur_scissor_rect;
   opa::Size      _cur_view_size;
   ::IDirect3DSurface9 *
                  _cur_surface_ptr;
   bool           _cur_fragment_program_enabled_flag;

   DataBuffer     _color_vertex_data;
   DataBuffer     _texture_vertex_data;

   Strategy       _strategy;
   Direct3dCaps   _capacities;
   Direct3dState  _device_state;
   CacheManager   _cache_manager;
   size_t         _video_memory_size;
   
   // debug
   int            _d3d_pool_default_cnt;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  DirectXEngine ();
                  DirectXEngine (const DirectXEngine &other);
   DirectXEngine &operator = (const DirectXEngine &other);
   bool           operator == (const DirectXEngine &other);
   bool           operator != (const DirectXEngine &other);

}; // class DirectXEngine



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_DirectXEngine_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
