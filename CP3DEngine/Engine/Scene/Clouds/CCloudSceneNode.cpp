
#include "stdafx.h"

#include "CCloudSceneNode.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace cp3d {
namespace engine {

CCloudSceneNode::CCloudSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
	: ISceneNode(parent, mgr, id)
{
	#ifdef _DEBUG
	setDebugName("CCloudSceneNode");
	#endif

	setAutomaticCulling(EAC_OFF);
	
	BBox.MaxEdge.set(0,0,0);
	BBox.MinEdge.set(0,0,0);

	Material.Wireframe = false;
	Material.Lighting = false;
    if (mgr->getVideoDriver()->getDriverType() != EDT_OPENGL)
        Material.ZBuffer = ECFN_NEVER;
	Material.ZWriteEnable = false;

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	//if (mgr->getVideoDriver()->getDriverType() == EDT_DIRECT3D11)
	//	Material.ZBuffer = ECFN_ALWAYS;
	#endif

	Material.TextureLayer[0].TextureWrapU = ETC_REPEAT;
	Material.TextureLayer[0].TextureWrapV = ETC_REPEAT;

	Material.MaterialType = EMT_ONETEXTURE_BLEND;
	Material.MaterialTypeParam = pack_textureBlendFunc(
		EBF_SRC_ALPHA, 
		EBF_ONE_MINUS_SRC_ALPHA,
		EMFN_MODULATE_1X, 
		EAS_TEXTURE | EAS_VERTEX_COLOR);

	LastUpdate = 0;

	TextureScale = 1.0f;
	Material.getTextureMatrix(0).setTextureScale(TextureScale, TextureScale);

	InnerRadius = 1.5f;
	OuterRadius = 2.0f;
	
	CenterHeight = 0.5f;
	InnerHeight = 0.1f;
	OuterHeight = 0.0f;

	CenterColor = SColor(220,220,220,220);
	InnerColor = SColor(180,220,220,220);
	OuterColor = SColor(0,220,220,220);

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	InnerBuffer = new CMeshBuffer<S3DVertex>(mgr->getVideoDriver()->getVertexDescriptor(0), EIT_16BIT);
	OuterBuffer = new CMeshBuffer<S3DVertex>(mgr->getVideoDriver()->getVertexDescriptor(0), EIT_16BIT);
	#endif

	createCloudLayer();
}

void CCloudSceneNode::createCloudLayer() {
	vector2d<f32> edge(OuterRadius, 0.f);
	f32 factor = InnerRadius / OuterRadius;
		
	InnerVertices[0] = S3DVertex(0, CenterHeight, 0, 0.f, -1.f, 0.f, CenterColor, 0, 0);
	
	for (s32 i=0; i<CLOUDSUBDIV; i++) {
		edge = edge.rotateBy(360.f/(f32)CLOUDSUBDIV, vector2d<f32>(0.f, 0.f));
		InnerVertices[i+1] = S3DVertex(edge.X*factor, InnerHeight, edge.Y*factor, 0.f, -1.f, 0.f, InnerColor, edge.X*factor, edge.Y*factor);
		OuterVertices[i] = InnerVertices[i+1];
		OuterVertices[i+CLOUDSUBDIV] = S3DVertex(edge.X, OuterHeight, edge.Y, 0.f, -1.f, 0.f, OuterColor, edge.X, edge.Y);
	}
	
	for (s32 i=0; i<CLOUDSUBDIV+1; i++) {
		InnerIndices[i] = i;
	}
	InnerIndices[CLOUDSUBDIV+1] = 1;

	for (s32 i=0; i<CLOUDSUBDIV; i++) {
		OuterIndices[i*2] = i;
		OuterIndices[i*2+1] = i+CLOUDSUBDIV;
	}
	OuterIndices[CLOUDSUBDIV*2] = 0;
	OuterIndices[CLOUDSUBDIV*2+1] = CLOUDSUBDIV;

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	resetD3D11Buffers();
	#endif
}

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
void CCloudSceneNode::resetD3D11Buffers() {
	// Inner
	InnerBuffer->getVertexBuffer(0)->clear();
	for (u32 i = 0; i < CLOUDSUBDIV + 1; i++)
		InnerBuffer->getVertexBuffer(0)->addVertex(&InnerVertices[i]);

	InnerBuffer->getIndexBuffer()->clear();
	for (u32 i = 0; i < CLOUDSUBDIV + 2; i++)
		InnerBuffer->getIndexBuffer()->addIndex(InnerIndices[i]);

	// Outer
	OuterBuffer->getVertexBuffer(0)->clear();
	for (u32 i = 0; i < 2 * CLOUDSUBDIV; i++)
		OuterBuffer->getVertexBuffer(0)->addVertex(&OuterVertices[i]);

	OuterBuffer->getIndexBuffer()->clear();
	for (u32 i = 0; i < 2 * CLOUDSUBDIV + 2; i++)
		OuterBuffer->getIndexBuffer()->addIndex(OuterIndices[i]);

	// Finish
	InnerBuffer->setDirty();
	OuterBuffer->setDirty();
}
#endif

void CCloudSceneNode::setTranslation(const vector2d<f32>& translation) {
	Translation = translation; 
}

void CCloudSceneNode::setTextureScale(f32 textureScale) {
	TextureScale = textureScale; 

	Material.getTextureMatrix(0).setTextureScale(TextureScale, TextureScale);
}

void CCloudSceneNode::setCloudRadius(f32 innerRadius, f32 outerRadius) {
	InnerRadius = innerRadius;
	OuterRadius = outerRadius;

	createCloudLayer();
}

void CCloudSceneNode::setCloudHeight(f32 centerHeight, f32 innerHeight, f32 outerHeight) {
	CenterHeight = centerHeight;
	InnerHeight = innerHeight;
	OuterHeight = outerHeight;

	InnerVertices[0].Pos.Y = CenterHeight;
	
	for (s32 i=0; i<CLOUDSUBDIV; i++) {
		InnerVertices[i+1].Pos.Y = InnerHeight;
		OuterVertices[i].Pos.Y = InnerHeight;
		OuterVertices[i+CLOUDSUBDIV].Pos.Y = OuterHeight;
	}

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	resetD3D11Buffers();
	#endif
}

void CCloudSceneNode::setCloudColor(const SColor& centerColor, const SColor& innerColor, const SColor& outerColor) {
	CenterColor = centerColor;
	InnerColor = innerColor;
	OuterColor = outerColor;

	InnerVertices[0].Color = CenterColor;
	
	for (s32 i=0; i<CLOUDSUBDIV; i++) {
		InnerVertices[i+1].Color = InnerColor;
		OuterVertices[i].Color = InnerColor;
		OuterVertices[i+CLOUDSUBDIV].Color = OuterColor;
	}

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	resetD3D11Buffers();
	#endif
}

void CCloudSceneNode::OnRegisterSceneNode() {
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);
	
	ISceneNode::OnRegisterSceneNode();
}

void CCloudSceneNode::OnAnimate(u32 timeMs) {
	ISceneNode::OnAnimate(timeMs);

	u32 time = timeMs-LastUpdate;

	CurrentTranslation += Translation*(f32)time*0.001f;

	Material.getTextureMatrix(0).setTextureTranslate(CurrentTranslation.X, CurrentTranslation.Y);

	LastUpdate = timeMs;
}

void CCloudSceneNode::render() {
	IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	if (!camera->isOrthogonal()) {
		matrix4 translate(AbsoluteTransformation);
		translate.setTranslation(SceneManager->getActiveCamera()->getAbsolutePosition());

		const f32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
		matrix4 scale;
		scale.setScale(vector3df(viewDistance, viewDistance, viewDistance));

		driver->setTransform(ETS_WORLD, translate * scale);

		driver->setMaterial(Material);

		#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
		driver->drawIndexedTriangleFan(true, InnerBuffer->getVertexBuffer(0), true, InnerBuffer->getIndexBuffer(), CLOUDSUBDIV);
		driver->drawVertexPrimitiveList(true, OuterBuffer->getVertexBuffer(0), true, OuterBuffer->getIndexBuffer(), 2 * CLOUDSUBDIV, EPT_TRIANGLE_STRIP);
		#else
		driver->drawIndexedTriangleFan(InnerVertices, CLOUDSUBDIV+1, InnerIndices, CLOUDSUBDIV);
		driver->drawVertexPrimitiveList(OuterVertices, 2*CLOUDSUBDIV, OuterIndices, 2*CLOUDSUBDIV, EVT_STANDARD, EPT_TRIANGLE_STRIP, EIT_16BIT);
		#endif

		if (DebugDataVisible & EDS_MESH_WIRE_OVERLAY) {
			SMaterial m;
			m.Wireframe = true;
			m.Lighting = false;
			m.AntiAliasing = 0;
			driver->setMaterial(m);

			#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
			driver->drawIndexedTriangleFan(false, InnerBuffer->getVertexBuffer(0), false, InnerBuffer->getIndexBuffer(), CLOUDSUBDIV);
			driver->drawVertexPrimitiveList(false, InnerBuffer->getVertexBuffer(0), false, OuterBuffer->getIndexBuffer(), 2 * CLOUDSUBDIV, EPT_TRIANGLE_STRIP);
			#else
			driver->drawIndexedTriangleFan(InnerVertices, CLOUDSUBDIV+1, InnerIndices, CLOUDSUBDIV);
			driver->drawVertexPrimitiveList(OuterVertices, 2*CLOUDSUBDIV, OuterIndices, 2*CLOUDSUBDIV, EVT_STANDARD, EPT_TRIANGLE_STRIP, EIT_16BIT);
			#endif
		}
	}
}

const aabbox3d<f32>& CCloudSceneNode::getBoundingBox() const {
	return BBox;
}

ESCENE_NODE_TYPE CCloudSceneNode::getType() const {
	return ESNT_UNKNOWN;
}

u32 CCloudSceneNode::getMaterialCount() const {
	return 1;
}

SMaterial& CCloudSceneNode::getMaterial(u32 i) {
	return Material;
}	

void CCloudSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const {
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("InnerRadius", InnerRadius);
	out->addFloat("OuterRadius", OuterRadius);
	
	out->addFloat("CenterHeight", CenterHeight);
	out->addFloat("InnerHeight", InnerHeight);
	out->addFloat("OuterHeight", OuterHeight);
	
	out->addColor("CenterColor", CenterColor);
	out->addColor("InnerColor", InnerColor);
	out->addColor("OuterColor", OuterColor);
	
	out->addFloat("TranslationX", Translation.X);
	out->addFloat("TranslationY", Translation.Y);

	out->addFloat("CurrentTranslationX", CurrentTranslation.X);
	out->addFloat("CurrentTranslationY", CurrentTranslation.Y);

	out->addFloat("TextureScale", TextureScale);
}

void CCloudSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
	ISceneNode::deserializeAttributes(in, options);
	
	InnerRadius = in->getAttributeAsFloat("InnerRadius");
	OuterRadius = in->getAttributeAsFloat("OuterRadius");
	
	CenterHeight = in->getAttributeAsFloat("CenterHeight");
	InnerHeight = in->getAttributeAsFloat("InnerHeight");
	OuterHeight = in->getAttributeAsFloat("OuterHeight");
	
	CenterColor = in->getAttributeAsColor("CenterColor");
	InnerColor = in->getAttributeAsColor("InnerColor");
	OuterColor = in->getAttributeAsColor("OuterColor");

	Translation.X = in->getAttributeAsFloat("TranslationX");
	Translation.Y = in->getAttributeAsFloat("TranslationY");
	
	CurrentTranslation.X = in->getAttributeAsFloat("CurrentTranslationX");
	CurrentTranslation.Y = in->getAttributeAsFloat("CurrentTranslationY");


	TextureScale = in->getAttributeAsFloat("TextureScale");


	createCloudLayer();
}

ISceneNode* CCloudSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager) {

	if (!newParent) 
		newParent = Parent;

	if (!newManager) 
		newManager = SceneManager;

	CCloudSceneNode* nb = new CCloudSceneNode(newParent, newManager, ID);

	nb->cloneMembers(this, newManager);

	nb->Material = Material;

	nb->CurrentTranslation = CurrentTranslation;
	nb->setTranslation(Translation); 
	nb->setTextureScale(TextureScale); 
	nb->setCloudColor(CenterColor, InnerColor, OuterColor);
	nb->setCloudHeight(CenterHeight, InnerHeight, OuterHeight);
	nb->setCloudRadius(InnerRadius, OuterRadius);
	
	nb->drop();

	return nb;
}

} // end namespace engine
} // end namespace cp3d
