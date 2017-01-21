#ifndef __H_C_HDR_LENS_FLARE_INCLUDED__
#define __H_C_HDR_LENS_FLARE_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
	namespace rendering {

		class CCP3DHandler;
		class CScreenQuadCB;

		class CHDRLensFlare {
			friend class CHDRManager;
		public:
			/// ctor & dtor
			CHDRLensFlare(CCP3DHandler *handler);
			~CHDRLensFlare();

			void render(irr::video::ITexture *source, CScreenQuad &screenQuad);
			void renderFinal(irr::video::ITexture *other, CScreenQuad &screenQuad);

		private:

			/// Textures
			irr::video::ITexture *LensTexture, *LensStarTexture, *LensDirtTexture;
			irr::video::ITexture *DepthTexture, *VelocityTexture;
			irr::video::ITexture *LensFlareRTT, *OtherRTT;

			/// Datas
			CCP3DHandler *Handler;
			irr::video::IVideoDriver *Driver;

			/// Material
			CScreenQuadCB *Callback, *CallbackFinal, *CallbackFinalObjectBased;
			irr::s32 MaterialType, FinalMaterialType, FinalObjectBaseMaterialType;

			/// Datas
			irr::f32 Dispersal;
			irr::f32 HaloWidth;
			irr::f32 Distortion;

			irr::f32 MotionScale;
			irr::core::matrix4 InverseViewProjection;
			irr::core::matrix4 PrevViewProjection;
			irr::core::matrix4 LensStarMatrix;

			bool FirstUpdate;
		};

	} /// End namespace rendering
} /// End namespace cp3d

#endif
