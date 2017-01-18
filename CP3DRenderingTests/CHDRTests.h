#include <irrlicht.h>

/// CP3D
//#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
//#define CP3DR_COMPILE_ENGINE

#include <CP3DCompileConfig.h>
#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

namespace cp3d {
	namespace test {
		using namespace irr;
		using namespace video;
		using namespace scene;
		using namespace core;
		using namespace gui;

		/*class AnimatorTest: public rendering::ICP3DAnimator<rendering::ICP3DLightSceneNode *> {
		public:
			AnimatorTest(rendering::ICP3DLightSceneNode *light, const SColorf &from, const SColorf &to, const u32 &duration)
				: From(from), To(to), StartTime(-1), Duration(duration), Inverse(false)
			{
				Data = light;
				Data->setLightColor(from);
			}

			void animateNode(ISceneNode* node, u32 timeMs) {
				rendering::ICP3DAnimator<rendering::ICP3DLightSceneNode *>::animateNode(node, timeMs);

				if (StartTime == -1)
					StartTime = timeMs;

				f32 diff = f32(timeMs - StartTime) / f32(Duration);
				if (diff >= 1.f) {
					StartTime = timeMs;
					Inverse = !Inverse;
					diff = 0.f;
				}

				if (Inverse)
					Data->setLightColor(To.getInterpolated(From, diff));
				else
					Data->setLightColor(From.getInterpolated(To, diff));
			}

		private:
			SColorf From, To;
			u32 StartTime, Duration;
			bool Inverse;
		};*/

		void HDRTest(irr::IrrlichtDevice *device, IEventReceiver *receiver) {
			IVideoDriver *driver = device->getVideoDriver();
			ISceneManager *smgr = device->getSceneManager();
			IGUIEnvironment *gui = device->getGUIEnvironment();

			/// Create rendering engine
			cp3d::engine::ICP3DEngine *engine = cp3d::createEngine(device);
			engine->getEventReceiver()->addEventReceiver(receiver);

			cp3d::rendering::ICP3DRenderingEngine *cpre = engine->getRenderingEngine();
			cpre->createNormalMappingMaterial();

			cp3d::rendering::ICP3DHandler *handler = cpre->getHandler();

			/// Create a fps camera
			ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
			camera->setPosition(vector3df(50.f, 50.f, 0.f));
			device->getCursorControl()->setVisible(false);

			/// Create a light
			cp3d::rendering::ICP3DLightSceneNode *light = cpre->createLightSceneNode(true, true);
			light->setPosition(vector3df(150.f, 300.f, 0.f));
			light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));
			light->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
			light->getShadowLight()->setUseRoundSpotLight(false);
			light->getShadowLight()->setFarValue(1000.f);
			light->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
			light->getShadowLight()->setShadowMapResolution(4096);

			ISceneNodeAnimator *animator = smgr->createFlyStraightAnimator(vector3df(-250.f, 200.f, -100.f), vector3df(250.f, 200.f, 100.f), 10000, true, true);

			ILightSceneNode *lightNode = *light;
			lightNode->addAnimator(animator);

			/// Skybox
			ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
				driver->getTexture("Textures/Skybox/glacier_up.png"),
				driver->getTexture("Textures/Skybox/glacier_dn.png"),
				driver->getTexture("Textures/Skybox/glacier_lf.png"),
				driver->getTexture("Textures/Skybox/glacier_rt.png"),
				driver->getTexture("Textures/Skybox/glacier_ft.png"),
				driver->getTexture("Textures/Skybox/glacier_bk.png"));

			/// Sponza
			IMeshSceneNode *sponza = smgr->addMeshSceneNode(smgr->getMesh("data/chinese.obj"), 0, -1, vector3df(0.f), vector3df(0.f), vector3df(3.5f));
			sponza->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			handler->addShadowToNode(sponza, rendering::EFT_16PCF, rendering::ESM_BOTH);

			/// Batman
			IAnimatedMeshSceneNode *batman = smgr->addAnimatedMeshSceneNode(smgr->getMesh("data/batman.x"), 0, -1, vector3df(10.f, -19.f, -30.f), vector3df(0.f), vector3df(0.3f));
			batman->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
			batman->setFrameLoop(0, batman->getEndFrame());
			batman->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			batman->setMaterialTexture(0, driver->getTexture("data/batman/IOS_Batman_Diff.jpg"));
			batman->setMaterialTexture(1, driver->getTexture("data/batman/IOS_Batman_Norm.jpg"));
			batman->setMaterialTexture(2, driver->getTexture("data/batman/IOS_Batman_Spec.jpg"));
			handler->addShadowToNode(batman, rendering::EFT_16PCF, rendering::ESM_BOTH);

			/// Clouds
			engine->getSceneNodeCreator()->createCloudNode(vector2df(0.008f, 0.0f), driver->getTexture("Textures/Clouds/cloud01.png"), 1.f, 0.5f, 0.1f, -0.05f);
			engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud02.png"), 0.4f, 0.05f, -0.1f, 0.5f);
			engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud03.png"), 0.035f, 0.f, -0.15f, 0.4f);

			/// SSAO and Depth
			cpre->getEffectsManager()->createSSAOEffect(true);

			handler->getDepthPassManager()->addNodeToPass(sponza);
			handler->getDepthPassManager()->addNodeToPass(batman);

			/// HDR
			handler->getHDRManager()->setEnabled(true);
			handler->getHDRManager()->setBrightnessThreshold(0.8f);
			handler->getHDRManager()->setGaussWidth(2.f);
			handler->getHDRManager()->setGaussianCoefficient(0.25f);
			handler->getHDRManager()->setMinimumLuminance(0.5f);
			handler->getHDRManager()->setMaximumLuminance(1e20f);
			handler->getHDRManager()->setDecreaseRate(0.5f);
			handler->getHDRManager()->setIncreaseRate(0.2f);
			handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.jpg"));

			/// Finish
			handler->setAmbientColor(SColor(255, 32, 32, 32));

			/// Get hdr texture
			handler->update();

			ITexture *hdrTexture = driver->getTexture("CP3DHandler_SM_4096");
			IGUIImage *img = gui->addImage(rect<s32>(driver->getScreenSize().Width - 512, 0, driver->getScreenSize().Width, 512));
			img->setScaleImage(true);
			img->setImage(hdrTexture);

			/// Update the application
			engine->setDrawGUI(false);
			engine->runEngine();
		}

	} /// End namespace test
} /// End namespace cp3d
