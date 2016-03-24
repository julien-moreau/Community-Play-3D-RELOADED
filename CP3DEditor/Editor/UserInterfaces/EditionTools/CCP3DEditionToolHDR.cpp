
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../../GUIElements/CGUIManager.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "CCP3DEditionToolHDR.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolHDR::CCP3DEditionToolHDR(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Rengine = editorCore->getRenderingEngine();
	Handler = Rengine->getHandler();

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolHDR::~CCP3DEditionToolHDR() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolHDR::createInterface() {
	/// Tabs
	HDRTab = EditionTool->addTab("Post-Processes");
	
	/// Parameters
	EditionTool->setNewZone(HDRTab, "HDR");
	HDRExposure = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Exposure"));
	HDRIncrease = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Increase Rate"));
	HDRDecrease = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Decrease Rate"));
	HDRMinLuminance = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Minimum Luminance"));

	EditionTool->setNewZone(HDRTab, "Bloom");
	BloomThreshold = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Brightness Threshold"));
	BloomCoeff = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Coefficient"));
	BloomMean = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Mean"));
	BloomStandDev = EditionTool->addField(HDRTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Standard Derivation"));
}

void CCP3DEditionToolHDR::configure() {
	rendering::ICP3DHDRManager *mgr = Handler->getHDRManager();

	// Configure
	HDRExposure.TextBox->setText(stringw(mgr->getExposure()).c_str());
	HDRIncrease.TextBox->setText(stringw(mgr->getIncreaseRate()).c_str());
	HDRDecrease.TextBox->setText(stringw(mgr->getDecreaseRate()).c_str());
	HDRMinLuminance.TextBox->setText(stringw(mgr->getMinimumLuminance()).c_str());

	BloomThreshold.TextBox->setText(stringw(mgr->getBrightnessThreshold()).c_str());
	BloomCoeff.TextBox->setText(stringw(mgr->getGaussianCoefficient()).c_str());
	BloomMean.TextBox->setText(stringw(mgr->getGaussianMean()).c_str());
	BloomStandDev.TextBox->setText(stringw(mgr->getGaussianStandardDerivation()).c_str());
}

void CCP3DEditionToolHDR::apply() {
	rendering::ICP3DHDRManager *mgr = Handler->getHDRManager();

	// Apply
	mgr->setExposure(core::fast_atof(stringc(HDRExposure.TextBox->getText()).c_str()));
	mgr->setIncreaseRate(core::fast_atof(stringc(HDRIncrease.TextBox->getText()).c_str()));
	mgr->setDecreaseRate(core::fast_atof(stringc(HDRDecrease.TextBox->getText()).c_str()));
	mgr->setMinimumLuminance(core::fast_atof(stringc(HDRMinLuminance.TextBox->getText()).c_str()));

	mgr->setBrightnessThreshold(core::fast_atof(stringc(BloomThreshold.TextBox->getText()).c_str()));
	mgr->setGaussianCoefficient(core::fast_atof(stringc(BloomCoeff.TextBox->getText()).c_str()));
	mgr->setGaussianMean(core::fast_atof(stringc(BloomMean.TextBox->getText()).c_str()));
	mgr->setGaussianStandardDerivation(core::fast_atof(stringc(BloomStandDev.TextBox->getText()).c_str()));
}

bool CCP3DEditionToolHDR::OnEvent(const SEvent &event) {

	return false;
}


} /// End namespace cp3d
