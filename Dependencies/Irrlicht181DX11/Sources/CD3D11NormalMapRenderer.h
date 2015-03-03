// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_D3D11_NORMAL_MAPMATERIAL_RENDERER_H_INCLUDED__
#define __C_D3D11_NORMAL_MAPMATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_WINDOWS_

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#include "CD3D11MaterialRenderer.h"
#include "IShaderConstantSetCallBack.h"

namespace irr
{
namespace video
{

//! Renderer for normal maps
class CD3D11NormalMapRenderer : public CD3D11MaterialRenderer, IShaderConstantSetCallBack
{
public:

	CD3D11NormalMapRenderer(ID3D11Device* device, video::IVideoDriver* driver, CD3D11CallBridge* bridgeCalls, s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);
	virtual ~CD3D11NormalMapRenderer();

	virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);

	virtual void OnSetConstants( IMaterialRendererServices* services, s32 userData );

	//! Returns the render capability of the material.
	virtual s32 getRenderCapability() const;

	virtual void OnSetMaterial( const SMaterial& material );	

private:
	s32 cbPerFrameId;

	struct SCbPerFrame
	{
		core::matrix4 g_mWorld;
		core::matrix4 g_mWorldViewProj;
		SColorf	g_lightColor1;
		SColorf	g_lightColor2;
		core::vector3df g_lightPos1;
		core::vector3df g_lightPos2;
	} cbPerFrame;
};

} // end namespace video
} // end namespace irr

#endif
#endif
#endif

