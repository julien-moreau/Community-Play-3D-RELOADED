/*****************************************************************************

        Direct3dCaps.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/directx/Direct3dCaps.h"
#include "ohm/sys/ConvString.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/util/trace.h"

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

Direct3dCaps::Direct3dCaps ()
:  _inited_flag (false)
{
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  Direct3dCaps::init (::IDirect3DDevice9 * device_ptr)
{
   assert (device_ptr != 0);
   ::HRESULT result = device_ptr->GetDeviceCaps (&_d3d_caps);
   
   if (result == D3D_OK)
   {
      _inited_flag = true;

      ohm_util_TRACE_INFO ("init(): succeeded.");

      static int     listed_adapter = -1;

      if (listed_adapter != _d3d_caps.AdapterOrdinal)
      {
         listed_adapter = _d3d_caps.AdapterOrdinal;

         // Device information
         ::IDirect3D9 *       d3d_ptr = 0;
         if (device_ptr->GetDirect3D (&d3d_ptr) == D3D_OK)
         {
            ::D3DADAPTER_IDENTIFIER9   d3d_adapt;
            const ::HRESULT   res = d3d_ptr->GetAdapterIdentifier (
               _d3d_caps.AdapterOrdinal,
               0,
               &d3d_adapt
            );

            if (res == D3D_OK)
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

            d3d_ptr->Release ();
            d3d_ptr = 0;
         }

         // Capability list
         ohm_util_TRACE_INFO ("--- Capability list ---");

#define  ohm_opal_directx_LOG_CAPS_D(x)    \
   ohm_util_TRACE_INFO2 ("%1m35k%: %2%", #x, _d3d_caps.x);

#define  ohm_opal_directx_LOG_CAPS_H(x)    \
   ohm_util_TRACE_INFO2 ("%1m35k%: 0x%2m8Xrw0%", #x, _d3d_caps.x);

#define  ohm_opal_directx_LOG_BITS(x, y) \
   if (((_d3d_caps.x) & (y)) != 0)       \
   {                                     \
      ohm_util_TRACE_INFO1 ("  %1m45k%", #y);  \
   }

         ohm_opal_directx_LOG_CAPS_D (DeviceType                       );
         ohm_opal_directx_LOG_CAPS_D (AdapterOrdinal                   );
         ohm_opal_directx_LOG_CAPS_H (Caps                             );
         ohm_opal_directx_LOG_CAPS_H (Caps2                            );
         ohm_opal_directx_LOG_CAPS_H (Caps3                            );
         ohm_opal_directx_LOG_CAPS_H (PresentationIntervals            );
         ohm_opal_directx_LOG_CAPS_H (CursorCaps                       );
         ohm_opal_directx_LOG_CAPS_H (DevCaps                          );

         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_EXECUTESYSTEMMEMORY    );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_EXECUTEVIDEOMEMORY     );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_TLVERTEXSYSTEMMEMORY   );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_TLVERTEXVIDEOMEMORY    );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_TEXTURESYSTEMMEMORY    );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_TEXTUREVIDEOMEMORY     );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_DRAWPRIMTLVERTEX       );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_CANRENDERAFTERFLIP     );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_TEXTURENONLOCALVIDMEM  );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_DRAWPRIMITIVES2        );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_SEPARATETEXTUREMEMORIES);
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_DRAWPRIMITIVES2EX      );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_HWTRANSFORMANDLIGHT    );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_CANBLTSYSTONONLOCAL    );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_HWRASTERIZATION        );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_PUREDEVICE             );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_QUINTICRTPATCHES       );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_RTPATCHES              );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_RTPATCHHANDLEZERO      );
         ohm_opal_directx_LOG_BITS (DevCaps, D3DDEVCAPS_NPATCHES               );

         ohm_opal_directx_LOG_CAPS_H (PrimitiveMiscCaps                );

         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_DITHER             );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_ZTEST              );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_FOGVERTEX          );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_FOGTABLE           );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_MIPMAPLODBIAS      );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_ZBUFFERLESSHSR     );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_FOGRANGE           );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_ANISOTROPY         );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_WBUFFER            );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_WFOG               );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_ZFOG               );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_COLORPERSPECTIVE   );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_SCISSORTEST        );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS);
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_DEPTHBIAS          );
         ohm_opal_directx_LOG_BITS (RasterCaps, D3DPRASTERCAPS_MULTISAMPLE_TOGGLE );

         ohm_opal_directx_LOG_CAPS_H (ZCmpCaps                         );
         ohm_opal_directx_LOG_CAPS_H (SrcBlendCaps                     );

         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_ZERO           );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_ONE            );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_SRCCOLOR       );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_INVSRCCOLOR    );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_SRCALPHA       );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_INVSRCALPHA    );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_DESTALPHA      );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_INVDESTALPHA   );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_DESTCOLOR      );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_INVDESTCOLOR   );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_SRCALPHASAT    );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_BOTHSRCALPHA   );
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_BOTHINVSRCALPHA);
         ohm_opal_directx_LOG_BITS (SrcBlendCaps, D3DPBLENDCAPS_BLENDFACTOR    );

         ohm_opal_directx_LOG_CAPS_H (DestBlendCaps                    );

         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_ZERO           );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_ONE            );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_SRCCOLOR       );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_INVSRCCOLOR    );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_SRCALPHA       );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_INVSRCALPHA    );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_DESTALPHA      );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_INVDESTALPHA   );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_DESTCOLOR      );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_INVDESTCOLOR   );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_SRCALPHASAT    );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_BOTHSRCALPHA   );
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_BOTHINVSRCALPHA);
         ohm_opal_directx_LOG_BITS (DestBlendCaps, D3DPBLENDCAPS_BLENDFACTOR    );

         ohm_opal_directx_LOG_CAPS_H (AlphaCmpCaps                     );
         ohm_opal_directx_LOG_CAPS_H (ShadeCaps                        );
         ohm_opal_directx_LOG_CAPS_H (TextureCaps                      );

         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_PERSPECTIVE             );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_POW2                    );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_ALPHA                   );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_SQUAREONLY              );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_TEXREPEATNOTSCALEDBYSIZE);
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_ALPHAPALETTE            );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_NONPOW2CONDITIONAL      );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_PROJECTED               );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_CUBEMAP                 );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_VOLUMEMAP               );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_MIPMAP                  );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_MIPVOLUMEMAP            );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_MIPCUBEMAP              );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_CUBEMAP_POW2            );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_VOLUMEMAP_POW2          );
         ohm_opal_directx_LOG_BITS (TextureCaps, D3DPTEXTURECAPS_NOPROJECTEDBUMPENV      );

         ohm_opal_directx_LOG_CAPS_H (TextureFilterCaps                );
         ohm_opal_directx_LOG_CAPS_H (CubeTextureFilterCaps            );
         ohm_opal_directx_LOG_CAPS_H (VolumeTextureFilterCaps          );
         ohm_opal_directx_LOG_CAPS_H (TextureAddressCaps               );
         ohm_opal_directx_LOG_CAPS_H (VolumeTextureAddressCaps         );
         ohm_opal_directx_LOG_CAPS_H (LineCaps                         );

         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_TEXTURE  );
         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_ZTEST    );
         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_BLEND    );
         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_ALPHACMP );
         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_FOG      );
         ohm_opal_directx_LOG_BITS (LineCaps, D3DLINECAPS_ANTIALIAS);

         ohm_opal_directx_LOG_CAPS_D (MaxTextureWidth                  );
         ohm_opal_directx_LOG_CAPS_D (MaxTextureHeight                 );
         ohm_opal_directx_LOG_CAPS_D (MaxVolumeExtent                  );
         ohm_opal_directx_LOG_CAPS_D (MaxTextureRepeat                 );
         ohm_opal_directx_LOG_CAPS_D (MaxTextureAspectRatio            );
         ohm_opal_directx_LOG_CAPS_D (MaxAnisotropy                    );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexW                       );
         ohm_opal_directx_LOG_CAPS_D (GuardBandLeft                    );
         ohm_opal_directx_LOG_CAPS_D (GuardBandTop                     );
         ohm_opal_directx_LOG_CAPS_D (GuardBandRight                   );
         ohm_opal_directx_LOG_CAPS_D (GuardBandBottom                  );
         ohm_opal_directx_LOG_CAPS_D (ExtentsAdjust                    );
         ohm_opal_directx_LOG_CAPS_H (StencilCaps                      );
         ohm_opal_directx_LOG_CAPS_H (FVFCaps                          );
         ohm_opal_directx_LOG_CAPS_H (TextureOpCaps                    );
         ohm_opal_directx_LOG_CAPS_D (MaxTextureBlendStages            );
         ohm_opal_directx_LOG_CAPS_D (MaxSimultaneousTextures          );
         ohm_opal_directx_LOG_CAPS_H (VertexProcessingCaps             );
         ohm_opal_directx_LOG_CAPS_D (MaxActiveLights                  );
         ohm_opal_directx_LOG_CAPS_D (MaxUserClipPlanes                );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexBlendMatrices           );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexBlendMatrixIndex        );
         ohm_opal_directx_LOG_CAPS_D (MaxPointSize                     );
         ohm_opal_directx_LOG_CAPS_D (MaxPrimitiveCount                );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexIndex                   );
         ohm_opal_directx_LOG_CAPS_D (MaxStreams                       );
         ohm_opal_directx_LOG_CAPS_D (MaxStreamStride                  );
         ohm_opal_directx_LOG_CAPS_H (VertexShaderVersion              );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexShaderConst             );
         ohm_opal_directx_LOG_CAPS_H (PixelShaderVersion               );
         ohm_opal_directx_LOG_CAPS_D (PixelShader1xMaxValue            );
         ohm_opal_directx_LOG_CAPS_H (DevCaps2                         );
         ohm_opal_directx_LOG_CAPS_D (MasterAdapterOrdinal             );
         ohm_opal_directx_LOG_CAPS_D (AdapterOrdinalInGroup            );
         ohm_opal_directx_LOG_CAPS_D (NumberOfAdaptersInGroup          );
         ohm_opal_directx_LOG_CAPS_H (DeclTypes                        );
         ohm_opal_directx_LOG_CAPS_D (NumSimultaneousRTs               );
         ohm_opal_directx_LOG_CAPS_H (StretchRectFilterCaps            );

         ohm_opal_directx_LOG_CAPS_D (VS20Caps.Caps                    );
         ohm_opal_directx_LOG_CAPS_D (VS20Caps.DynamicFlowControlDepth );
         ohm_opal_directx_LOG_CAPS_D (VS20Caps.NumTemps                );
         ohm_opal_directx_LOG_CAPS_D (VS20Caps.StaticFlowControlDepth  );

         ohm_opal_directx_LOG_CAPS_D (PS20Caps.Caps                    );
         ohm_opal_directx_LOG_CAPS_D (PS20Caps.DynamicFlowControlDepth );
         ohm_opal_directx_LOG_CAPS_D (PS20Caps.NumTemps                );
         ohm_opal_directx_LOG_CAPS_D (PS20Caps.StaticFlowControlDepth  );
         ohm_opal_directx_LOG_CAPS_D (PS20Caps.NumInstructionSlots     );

         ohm_opal_directx_LOG_CAPS_H (VertexTextureFilterCaps          );
         ohm_opal_directx_LOG_CAPS_D (MaxVShaderInstructionsExecuted   );
         ohm_opal_directx_LOG_CAPS_D (MaxPShaderInstructionsExecuted   );
         ohm_opal_directx_LOG_CAPS_D (MaxVertexShader30InstructionSlots);
         ohm_opal_directx_LOG_CAPS_D (MaxPixelShader30InstructionSlots );

#undef   ohm_opal_directx_LOG_CAPS_D
#undef   ohm_opal_directx_LOG_CAPS_H
#undef   ohm_opal_directx_LOG_BITS
      }

   }
   else
   {
      ohm_util_TRACE_ERROR1 ("init(): failed. Returned %1%.", result);
      assert (false);
   }
}



void  Direct3dCaps::restore ()
{
   // Nothing
}



/*
==============================================================================
Name : is_inited
==============================================================================
*/

bool Direct3dCaps::is_inited () const
{
   return _inited_flag;
}



/*
==============================================================================
Name : use_d3d_caps
==============================================================================
*/

const ::D3DCAPS9 & Direct3dCaps::use_d3d_caps () const
{
   assert (is_inited ());
   
   return _d3d_caps;
}



/*
==============================================================================
Name : can_do_hardware_primitives
==============================================================================
*/

bool  Direct3dCaps::can_do_hardware_primitives ()
{
   assert (is_inited ());
   
   bool ret_val = false;

   if (is_inited ())
   {
      ret_val = ((_d3d_caps.DevCaps & D3DDEVCAPS_DRAWPRIMTLVERTEX) != 0);
   }

   return ret_val;
}



/*
==============================================================================
Name: Direct3dCaps::can_do_alpha_blend_op
Description:
   Device supports alpha-blending operations other than D3DBLENDOP_ADD
   See IDirect3dDevice9::SetRenderState / D3DRS_BLENDOP
==============================================================================
*/

bool Direct3dCaps::can_do_alpha_blend_op () const
{
   assert (is_inited ());
   
   bool ret_val = false;
   
   if (is_inited ())
   {
      ret_val = ((_d3d_caps.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) != 0);
   }
   
   return ret_val;
}



/*
==============================================================================
Name: Direct3dCaps::can_do_tex_size_npot_unrestricted
Description: 
   Returns the texture size type the driver handles
==============================================================================
*/

Direct3dCaps::TexSize Direct3dCaps::can_do_tex_size () const
{
   assert (is_inited ());
   
   TexSize ret_val = TexSize_NPOT_POW_2;
   
   if (is_inited ())
   {
      const DWORD test_npot_cond_bits= D3DPTEXTURECAPS_NONPOW2CONDITIONAL | D3DPTEXTURECAPS_POW2;
      const DWORD test_result = _d3d_caps.TextureCaps & test_npot_cond_bits;
   
      if (test_result == test_npot_cond_bits)
      {
         // conditional support is provided for 2D textures with 
         // dimensions that are not powers of two
         ret_val = TexSize_NPOT_RESTRICTED;
      }
      else if (test_result == 0)
      {
         // unconditional support is provided for 2D textures with 
         // dimensions that are not powers of two
         ret_val = TexSize_NPOT;
      }
      else
      {
         // support is provided for 2D textures with 
         // dimensions that are powers of two only
         assert (test_result == D3DPTEXTURECAPS_POW2);
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name: Direct3dCaps::get_max_primitive_count
Description:
   Maximum number of primitives for each 
   IDirect3DDevice9::DrawPrimitive call. There are two cases:
    - If MaxPrimitiveCount is not equal to 0xffff, you can draw at most 
      MaxPrimitiveCount primitives with each draw call.
    - However, if MaxPrimitiveCount equals 0xffff, you can still draw at most
      MaxPrimitiveCount primitive, but you may also use no more than 
      MaxPrimitiveCount unique vertices (since each primitive can potentially 
      use three different vertices).
==============================================================================
*/

archi::UInt32 Direct3dCaps::get_max_primitive_count () const
{
   assert (is_inited ());
   
   archi::UInt32 ret_val = 0;

   if (is_inited ())
   {
      ret_val = _d3d_caps.MaxPrimitiveCount;
   }

   return ret_val;
}



/*
==============================================================================
Name: Direct3dCaps::get_max_vertice_count
Description:
   Maximum number of vertices for each DrawPrimitive call
   See 'get_max_primitive_count'
==============================================================================
*/

archi::UInt32 Direct3dCaps::get_max_vertice_count () const
{
   assert (is_inited ());
   
   archi::UInt32 ret_val = 0;

   if (is_inited ())
   {
      archi::UInt32 prim_count = get_max_primitive_count ();
      
      if (prim_count < 0xffffL)
      {
         ret_val = 3 * prim_count;
      }
      else
      {
         ret_val = 0xffffL;
      }
   }

   return ret_val;
}



/*
==============================================================================
Name : get_pixel_shader_version
==============================================================================
*/

archi::UInt16  Direct3dCaps::get_pixel_shader_version ()
{
   assert (is_inited ());
   
   archi::UInt16 ret_val = 0;

   if (is_inited ())
   {
      ret_val = archi::UInt16 (_d3d_caps.PixelShaderVersion & 0xFFFF);
   }

   return ret_val;

}



/*
==============================================================================
Name : get_pixel_shader_version_major
==============================================================================
*/

archi::UByte   Direct3dCaps::get_pixel_shader_version_major ()
{
   assert (is_inited ());
   
   archi::UByte ret_val = 0;

   if (is_inited ())
   {
      ret_val = archi::UByte ((_d3d_caps.PixelShaderVersion >> 8) & 0xFF);
   }

   return ret_val;
}



/*
==============================================================================
Name : get_pixel_shader_version_minor
==============================================================================
*/

archi::UByte   Direct3dCaps::get_pixel_shader_version_minor ()
{
   assert (is_inited ());
   
   archi::UByte ret_val = 0;

   if (is_inited ())
   {
      ret_val = archi::UByte (_d3d_caps.PixelShaderVersion & 0xFF);
   }

   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

