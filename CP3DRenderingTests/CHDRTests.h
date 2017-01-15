#include <irrlicht.h>

/// CP3D
#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DCompileConfig.h>
#include <CP3DRenderingEngine.h>

namespace cp3d {
	namespace test {
		using namespace irr;
		using namespace video;
		using namespace scene;
		using namespace core;
		using namespace gui;

		class AnimatorTest: public rendering::ICP3DAnimator<rendering::ICP3DLightSceneNode *> {
		public:
			AnimatorTest(rendering::ICP3DLightSceneNode *light, const SColorf &from, const SColorf &to, const u32 &duration)
				: From(from), To(to), StartTime(-1), Duration(duration), Inverse(false)
			{
				Data = light;
				Data->setLightColor(from);
			}

			void animateNode(ISceneNode* node, u32 timeMs) {
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

				rendering::ICP3DAnimator<rendering::ICP3DLightSceneNode *>::animateNode(node, timeMs);
			}

		private:
			SColorf From, To;
			u32 StartTime, Duration;
			bool Inverse;
		};

		void HDRTest(irr::IrrlichtDevice *device) {
			IVideoDriver *driver = device->getVideoDriver();
			ISceneManager *smgr = device->getSceneManager();
			IGUIEnvironment *gui = device->getGUIEnvironment();

			/// Logging
			device->getLogger()->setLogLevel(ELL_INFORMATION);

			/// Create rendering engine
			cp3d::rendering::ICP3DRenderingEngine *cpre = cp3d::createRenderingEngine(device);
			cp3d::rendering::ICP3DHandler *handler = cpre->getHandler();

			/// Create a fps camera
			ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
			camera->setPosition(vector3df(50.f, 50.f, 0.f));
			device->getCursorControl()->setVisible(false);

			/// Create a test scene
			IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(100, 100),
				0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));

			IMeshSceneNode *planeNode = smgr->addMeshSceneNode(planeMesh);
			//ISceneNode *planeNode = smgr->addWaterSurfaceSceneNode(planeMesh, 20.f, 200.f, 0.05f);
			planeNode->setMaterialTexture(0, driver->getTexture("data/textures/spnza_bricks_a_diff.tga"));
			planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
			planeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
			planeNode->setMaterialFlag(EMF_LIGHTING, false);
			planeNode->getMaterial(0).Shininess = 0.f;
			handler->addShadowToNode(planeNode, cp3d::rendering::EFT_16PCF, cp3d::rendering::ESM_BOTH);

			/*IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
			cubeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
			cubeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
			cubeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
			cubeNode->setMaterialFlag(EMF_LIGHTING, false);
			cubeNode->getMaterial(0).Shininess = 0.f;
			handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_16PCF, cp3d::rendering::ESM_BOTH);*/

			/// Create the normal mapping material
			cpre->createNormalMappingMaterial();
			//cubeNode->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			planeNode->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);

			cp3d::rendering::ICP3DLightSceneNode *light = cpre->createLightSceneNode(true, true);
			light->setPosition(vector3df(0.f, 300.f, 0.f));
			light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));
			light->getLightData().SpecularColor = SColorf(1.f, 0.5f, 0.f, 1.f);
			light->getShadowLight()->setUseRoundSpotLight(false);
			light->getShadowLight()->setFarValue(1000.f);
			light->getShadowLight()->setShadowMapResolution(4096);

			ISceneNodeAnimator *animator = smgr->createFlyStraightAnimator(vector3df(-500.f, 300.f, -100.f), vector3df(500.f, 300.f, 100.f), 10000, true, true);
			ILightSceneNode *lightNode = *light;
			lightNode->addAnimator(animator);

			//animator = new AnimatorTest(light, SColorf(0.f, 0.f, 0.f), SColorf(1.f, 1.f, 1.f), 5000);
			//lightNode->addAnimator(animator);

			/// Skybox
			ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
				driver->getTexture("Textures/Skybox/glacier_up.png"),
				driver->getTexture("Textures/Skybox/glacier_dn.png"),
				driver->getTexture("Textures/Skybox/glacier_lf.png"),
				driver->getTexture("Textures/Skybox/glacier_rt.png"),
				driver->getTexture("Textures/Skybox/glacier_ft.png"),
				driver->getTexture("Textures/Skybox/glacier_bk.png"));

			/// Sponza
			IMesh *lvMesh = smgr->getMesh("data/lv/CAT_XMAS2015-N60040.obj");
			IMeshSceneNode *lvSceneNode = smgr->addMeshSceneNode(lvMesh, 0, -1, vector3df(0.f, 50.f, 0.f), vector3df(0.f), vector3df(1.f));
			lvSceneNode->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			handler->addShadowToNode(lvSceneNode, rendering::EFT_16PCF, rendering::ESM_BOTH);

			/*IMeshSceneNode *sponza = smgr->addMeshSceneNode(smgr->getMesh("data/sponza.obj"), 0, -1, vector3df(0.f), vector3df(0.f), vector3df(0.1f));
			sponza->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			handler->addShadowToNode(sponza, rendering::EFT_16PCF, rendering::ESM_BOTH);*/

			/// Billboard
			/*IBillboardSceneNode * bill = smgr->addBillboardSceneNode(0, dimension2df(1500.f, 1500.f), vector3df(-1000.f, 650.f, 0.f));
			bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
			bill->setMaterialFlag(EMF_LIGHTING, false);
			bill->setMaterialTexture(0, driver->getTexture("Textures/Clouds/sun.png"));*/

			cpre->getEffectsManager()->createSSAOEffect(true);

			//handler->getDepthPassManager()->addNodeToPass(skyboxNode);
			handler->getDepthPassManager()->addNodeToPass(planeNode);
			handler->getDepthPassManager()->addNodeToPass(lvSceneNode);

			/// Finish
			handler->setAmbientColor(SColor(255, 32, 32, 32));

			handler->getHDRManager()->setEnabled(true);
			handler->getHDRManager()->setBrightnessThreshold(0.8f);
			handler->getHDRManager()->setGaussWidth(2.f);
			handler->getHDRManager()->setGaussianCoefficient(0.2f);
			handler->getHDRManager()->setMinimumLuminance(0.5f);
			handler->getHDRManager()->setMaximumLuminance(1e20f);
			handler->getHDRManager()->setDecreaseRate(0.5f);
			handler->getHDRManager()->setIncreaseRate(0.2f);
			handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.jpg"));

			/// Get hdr texture
			handler->update();
			ITexture *hdrTexture = driver->getTexture("BlurVMaterial");
			IGUIImage *img = gui->addImage(rect<s32>(driver->getScreenSize().Width - 512, 0, driver->getScreenSize().Width, 512));
			img->setScaleImage(true);
			img->setImage(hdrTexture);

			/// Update the application
			while (device->run()) {
				if (!device->isWindowActive())
					continue;

				driver->beginScene(true, true, SColor(0x0));
				handler->update();

				gui->drawAll();
				driver->endScene();
			}
		}

	} /// End namespace test
} /// End namespace cp3d
