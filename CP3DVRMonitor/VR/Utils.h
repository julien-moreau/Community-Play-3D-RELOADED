#ifndef __H_VR_UTILS_INCLUDED__
#define __H_VR_UTILS_INCLUDED__

#include <irrlicht.h>
#include <openvr.h>

namespace cp3d {
namespace engine {

	class Utils {
	public:
		// Returns an irrlicht matrix from a 4x4 VR matrix
		static irr::core::matrix4 getMatrix(const vr::HmdMatrix44_t &mat) {
			irr::core::matrix4 final;
			final(0, 0) = mat.m[0][0];
			final(0, 1) = mat.m[1][0];
			final(0, 2) = mat.m[2][0];
			final(0, 3) = mat.m[3][0];
			final(1, 0) = mat.m[0][1];
			final(1, 1) = mat.m[1][1];
			final(1, 2) = mat.m[2][1];
			final(1, 3) = mat.m[3][1];
			final(2, 0) = mat.m[0][2];
			final(2, 1) = mat.m[1][2];
			final(2, 2) = mat.m[2][2];
			final(2, 3) = mat.m[3][2];
			final(3, 0) = mat.m[0][3];
			final(3, 1) = mat.m[1][3];
			final(3, 2) = mat.m[2][3];
			final(3, 3) = mat.m[3][3];

			return final;
		}

		// Returns an irrlicht matrix from a 3x4 VR matrix
		static irr::core::matrix4 getMatrix(const vr::HmdMatrix34_t &mat) {
			irr::core::matrix4 final;
			final(0, 0) = mat.m[0][0];
			final(0, 1) = mat.m[1][0];
			final(0, 2) = mat.m[2][0];
			final(0, 3) = 0.f;
			final(1, 0) = mat.m[0][1];
			final(1, 1) = mat.m[1][1];
			final(1, 2) = mat.m[2][1];
			final(1, 3) = 0.f;
			final(2, 0) = mat.m[0][2];
			final(2, 1) = mat.m[1][2];
			final(2, 2) = mat.m[2][2];
			final(2, 3) = 0.f;
			final(3, 0) = mat.m[0][3];
			final(3, 1) = mat.m[1][3];
			final(3, 2) = mat.m[2][3],
				final(3, 3) = 1.0f;

			return final;
		}
	};

} /// End namespace engine
} /// End namespace cp3d

#endif
