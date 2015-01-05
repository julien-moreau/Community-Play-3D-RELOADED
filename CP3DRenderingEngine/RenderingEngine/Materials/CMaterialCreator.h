#ifndef __H_C_MATERIAL_CREATOR_INCLUDED__
#define __H_C_MATERIAL_CREATOR_INCLUDED__

#include <irrlicht.h>
#include <ICP3DMaterialCreator.h>

namespace cp3d {
namespace rendering {

class CShaderPreprocessor;

class CMaterialCreator : public ICP3DMaterialCreator {

public:

	CMaterialCreator(irr::video::IVideoDriver *driver);
	~CMaterialCreator();
	/// Custom
	irr::s32 createMaterialFromFiles(const irr::core::stringc &vertexFilename, const irr::core::stringc &pixelFilename,
									 irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
									 irr::video::IShaderConstantSetCallBack *callback = 0);
	irr::s32 createMaterialFromStrings(const irr::core::stringc &vertexShader, const irr::core::stringc &pixelShader,
									   irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
									   irr::video::IShaderConstantSetCallBack *callback = 0);
	/// Depth
	irr::s32 createCustomDepthMaterialFromFiles(const irr::core::stringc &vertexFilename, const irr::core::stringc &pixelFilename,
												irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
												irr::video::IShaderConstantSetCallBack *callback = 0);
	irr::s32 createCustomDepthMaterialFromStrings(const irr::core::stringc &vertexShader, const irr::core::stringc &pixelShader,
												  irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
												  irr::video::IShaderConstantSetCallBack *callback = 0);
	/// Shadows
	irr::s32 createCustomShadowsMaterialFromFile(const irr::core::stringc &vertexFilename,
												 irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
												 irr::video::IShaderConstantSetCallBack *callback = 0);
	irr::s32 createCustomShadowsMaterialFromString(const irr::core::stringc &vertexShader,
												   irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
												   irr::video::IShaderConstantSetCallBack *callback = 0);

	void addDefine(const irr::core::stringc define, const irr::core::stringc value);
	void removeDefine(const irr::core::stringc define);
	void clearDefines();

private:
	irr::video::IVideoDriver *Driver;

	CShaderPreprocessor *Spp;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
