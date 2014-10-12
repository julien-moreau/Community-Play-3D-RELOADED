#ifndef __H_SSHADOW_LIGHT_INCLUDED__
#define __H_SSHADOW_LIGHT_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

struct SShadowLight
{

	SShadowLight(const irr::u32 shadowMapResolution,
				 const irr::core::vector3df& position, 
				 const irr::core::vector3df& target,
				 irr::video::SColorf lightColour = irr::video::SColor(0xffffffff), 
				 irr::f32 nearValue = 10.0, irr::f32 farValue = 100.0,
				 irr::f32 fov = 90.0 * irr::core::DEGTORAD64, bool directional = false)
				 :	pos(position), tar(target), farPlane(directional ? 1.0f : farValue), diffuseColour(lightColour), 
					mapRes(shadowMapResolution)
	{
		nearValue = nearValue <= 0.0f ? 0.1f : nearValue;

		updateViewMatrix();
		
		if(directional)
			projMat.buildProjectionMatrixOrthoLH(fov, fov, nearValue, farValue);
		else
			projMat.buildProjectionMatrixPerspectiveFovLH(fov, 1.0f, nearValue, farValue);
	}

	/// Sets the light's position.
	void setPosition(const irr::core::vector3df& position) {
		pos = position;
		updateViewMatrix();
	}

	/// Sets the light's target.
	void setTarget(const irr::core::vector3df& target) {
		tar = target;
		updateViewMatrix();
	}

	/// Gets the light's position.
	const irr::core::vector3df& getPosition() const {
		return pos;
	}

	/// Gets the light's target.
	const irr::core::vector3df& getTarget()  const {
		return tar;
	}

	/// Sets the light's view matrix.
	void setViewMatrix(const irr::core::matrix4& matrix) {
		viewMat = matrix;
		irr::core::matrix4 vInverse;
		viewMat.getInverse(vInverse);
		pos = vInverse.getTranslation();
	}

	/// Sets the light's projection matrix.
	void setProjectionMatrix(const irr::core::matrix4& matrix) {
		projMat = matrix;
	}

	/// Gets the light's view matrix.
	irr::core::matrix4& getViewMatrix() {
		return viewMat;
	}

	/// Gets the light's projection matrix.
	irr::core::matrix4& getProjectionMatrix() {
		return projMat;
	}

	/// Gets the light's far value.
	irr::f32 getFarValue() const {
		return farPlane;
	}

	/// Gets the light's color.
	const irr::video::SColorf& getLightColor() const {
		return diffuseColour;
	}

	/// Sets the light's color.
	void setLightColor(const irr::video::SColorf& lightColour) {
		diffuseColour = lightColour;
	}

	/// Sets the shadow map resolution for this light.
	void setShadowMapResolution(const irr::u32 shadowMapResolution) {
		mapRes = shadowMapResolution;
	}

	/// Gets the shadow map resolution for this light.
	const irr::u32 getShadowMapResolution() const {
		return mapRes;
	}

private:

	void updateViewMatrix() {
		viewMat.buildCameraLookAtMatrixLH(pos, tar,
			(pos - tar).dotProduct(irr::core::vector3df(1.0f, 0.0f, 1.0f)) == 0.0f ?
			irr::core::vector3df(0.0f, 0.0f, 1.0f) : irr::core::vector3df(0.0f, 1.0f, 0.0f)); 
	}

	irr::video::SColorf diffuseColour;
	irr::core::vector3df pos, tar;
	irr::f32 farPlane;
	irr::core::matrix4 viewMat, projMat;
	irr::u32 mapRes;
};

} /// End namespace rendering
} /// End namsepace cp3d

#endif
