#include <irrlicht.h>

/// CP3D
// #define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
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

		class CJetance : public IEventReceiver, engine::ICP3DUpdate {
		public:
			CJetance(IrrlichtDevice *device, IEventReceiver *receiver) : CurrentTexture(0) {
				create(device, receiver);
				Engine->getEventReceiver()->addEventReceiver(this);
				Engine->getCustomUpdater()->addCustomUpdate(this);

				/// Update the application
				Engine->setDrawGUI(true);
				Engine->runEngine();
			}

			~CJetance() {
				Engine->getEventReceiver()->removeEventReceiver(this);
				Engine->getCustomUpdater()->removeCustomUpdate(this);
			}

			void OnPreUpdate() {
				if (ScreenSize != Rengine->getVideoDriver()->getScreenSize()) {
					ScreenSize = Rengine->getVideoDriver()->getScreenSize();
					ScreenRect = rect<s32>(0, 0, ScreenSize.Width, ScreenSize.Height);

					PreviewImage->setRelativePosition(rect<s32>(ScreenSize.Width / 2, 0, ScreenSize.Width, ScreenSize.Height));
					Button->setRelativePosition(rect<s32>(0, 0, ScreenSize.Width / 2, 50));
					InverseColors->setRelativePosition(rect<s32>(0, 50, ScreenSize.Width / 2, 100));
					List->setRelativePosition(rect<s32>(0, 100, ScreenSize.Width / 2, ScreenSize.Height));
				}
			}

			bool OnEvent(const SEvent &event) {
				if (event.EventType == EET_GUI_EVENT) {
					processEvent(event.GUIEvent.EventType, event.GUIEvent.Caller);
				}

				return false;
			}

		private:
			bool processEvent(const EGUI_EVENT_TYPE &type, IGUIElement *caller) {
				if (type == EGET_BUTTON_CLICKED && caller == Button) {
					OpenDialog = Rengine->getGUIEnvironment()->addFileOpenDialog(L"Select folder", true, 0, -1, false, (c8*)Rengine->getDevice()->getFileSystem()->getWorkingDirectory().c_str());
					OpenDialog->setRelativePosition(rect<s32>(0, 0, ScreenSize.Width / 2, ScreenSize.Height));

					return true;
				}
				else if (type == EGET_CHECKBOX_CHANGED && caller == InverseColors) {
					const bool checked = InverseColors->isChecked();
					Rengine->getHandler()->enableVirtualReality(checked);
				}
				else if (type == EGET_FILE_SELECTED && caller == OpenDialog) {
					io::IFileSystem *fileSystem = Rengine->getDevice()->getFileSystem();

					stringc fileName = OpenDialog->getFileName();
					stringc directoryName = fileSystem->getFileDir(fileName);

					io::IFileList *fileList = fileSystem->createFileList();
					List->clear();

					for (u32 i = 0; i < fileList->getFileCount(); i++) {
						io::path currentFileName = fileList->getFileName(i);
						io::path fileExtension;
						io::path extension = core::getFileNameExtension(fileExtension, currentFileName).make_lower();

						if (extension == ".jpg" || extension == ".png") {
							List->addItem(stringw(fileList->getFileName(i)).c_str());
							Files.push_back(stringw(fileList->getFileName(i)));
						}
					}

					return true;
				}
				else if (type == EGET_LISTBOX_CHANGED && caller == List) {
					if (List->getItemCount() > 0) {
						switchTexture([this](ITexture *texture) {
							SkyDome->setMaterialTexture(0, texture);
						});
					}

					return true;
				}

			return false;
		}

		void switchTexture(const std::function<void(ITexture *newTexture)> &callback) {
			stringc textureName = Files[(u32) List->getSelected()];
			ITexture *texture = Rengine->getVideoDriver()->getTexture(textureName);

			const u32 transitionTime = 1000;

			if (CurrentTexture != 0) {
				Timer1->runUntil(transitionTime, [this, callback]() {
					callback(CurrentTexture);
				});

				Timer2->runUntil(transitionTime * 2, [this]() {
					Timer2->stop();
				});

				Timer2->registerUpdater([this, transitionTime](const irr::u32 &time) {
					Rengine->getHandler()->setCurrentFade(core::abs_(1.f - (f32(time) / f32(transitionTime) * 1.f)));
				});
			}
			else {

			}

			CurrentTexture = texture;
		}

		void create(IrrlichtDevice *device, IEventReceiver *receiver) {
			IVideoDriver *driver = device->getVideoDriver();
			ISceneManager *smgr = device->getSceneManager();
			Gui = device->getGUIEnvironment();

			/// Create rendering engine
			Engine = cp3d::createEngine(device);
			Engine->getEventReceiver()->addEventReceiver(receiver);

			Rengine = Engine->getRenderingEngine();
			Rengine->createNormalMappingMaterial();

			rendering::ICP3DHandler *handler = Rengine->getHandler();

			/// Create a fps camera
			ICameraSceneNode *camera = smgr->addCameraSceneNode(0); //smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
			camera->setPosition(vector3df(0.f, 0.f, 0.f));
			device->getCursorControl()->setVisible(true);

			/// Draw skydome
			ITexture *skyTexture = driver->getTexture("Textures/test2.jpg");
			SkyDome = smgr->addSkyDomeSceneNode(skyTexture, 32, 16, 0.89999999f, 2.f, 500.f, 0, -1);
			SkyDome->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
			SkyDome->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);
			SkyDome->setScale(vector3df(-1.f, 1.f, 1.f));
			handler->addShadowToNode(SkyDome, rendering::EFT_NONE, rendering::ESM_EXCLUDE);

			/// HDR
			handler->getHDRManager()->setEnabled(false);
			handler->getHDRManager()->setBrightnessThreshold(1.f);
			handler->getHDRManager()->setGaussWidth(2.f);
			handler->getHDRManager()->setGaussianCoefficient(0.25f);
			handler->getHDRManager()->setMinimumLuminance(0.5f);
			handler->getHDRManager()->setMaximumLuminance(1e20f);
			handler->getHDRManager()->setDecreaseRate(0.2f);
			handler->getHDRManager()->setIncreaseRate(0.2f);
			handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.png"));

			handler->getHDRManager()->enableLuminance(false);
			handler->getHDRManager()->enableLensFlare(false);

			/// Finish rendering
			handler->setAmbientColor(SColor(255, 32, 32, 32));

			/// Create GUI
			IGUISkin *skin = Gui->createSkin(EGST_WINDOWS_CLASSIC);
			IGUIFont *font = Gui->getFont("GUI/fontlucida.png");
			skin->setFont(font);
			Gui->setSkin(skin);
			skin->setColor(EGDC_BUTTON_TEXT, SColor(255, 0xAA, 0xAA, 0xAA));
			skin->setColor(EGDC_3D_HIGH_LIGHT, SColor(255, 0x22, 0x22, 0x22));
			skin->setColor(EGDC_3D_FACE, SColor(255, 0x44, 0x44, 0x44));
			skin->setColor(EGDC_WINDOW, SColor(255, 0x66, 0x66, 0x66));
			skin->setColor(EGDC_EDITABLE, SColor(255, 0x66, 0x66, 0x66));
			skin->setColor(EGDC_INACTIVE_CAPTION, SColor(255, 0, 0, 0));
			skin->setColor(EGDC_3D_LIGHT, SColor(255, 32, 32, 32));

			ITexture *previewLeft = driver->getTexture("leftEye");
			PreviewImage = Gui->addImage(rect<s32>(0, 0, 0, 0)); // VR
			PreviewImage->setScaleImage(true);
			PreviewImage->setImage(previewLeft);

			Button = Gui->addButton(rect<s32>(0, 0, 0, 0), 0, -1, L"Select folder...", L"Select images folder...");
			List = Gui->addListBox(rect<s32>(0, 0, 0, 0), 0, -1, true);
			InverseColors = Gui->addCheckBox(true, rect<s32>(0, 0, 0, 0), 0, -1, L"Invert colors");

			/// Create timer
			Timer1 = engine::ICP3DTimer::create(Engine);
			Timer2 = engine::ICP3DTimer::create(Engine);
		}

		// Datas
		rendering::ICP3DRenderingEngine *Rengine;
		engine::ICP3DEngine *Engine;
		IGUIEnvironment *Gui;

		rect<s32> ScreenRect;
		dimension2du ScreenSize;

		ISceneNode *SkyDome;

		ITexture *CurrentTexture;
		array<stringw> Files;

		engine::ICP3DTimer *Timer1, *Timer2;

		// Gui
		IGUIImage *PreviewImage;
		IGUIButton *Button;
		IGUICheckBox *InverseColors;
		IGUIListBox *List;
		IGUIFileOpenDialog *OpenDialog;
	};

	void Jetage(irr::IrrlichtDevice *device, IEventReceiver *receiver) {
		CJetance *jetance = new CJetance(device, receiver);
	}

} /// End namespace test
} /// End namespace cp3d
