
#include "stdafx.h"
#include "CCP3DGeometryCreator.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace cp3d {
namespace engine {

auto CCP3DGeometryCreatorAddVertex = [](CMeshBuffer<S3DVertex> *buffer, S3DVertex v, const u32 &indice) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	buffer->getVertexBuffer(indice)->addVertex(&v);
	#else
	buffer->Vertices.push_back(v);
	#endif
};
auto CCP3DGeometryCreatorAddIndex = [](CMeshBuffer<S3DVertex> * buffer, const u32 &index) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	buffer->getIndexBuffer()->addIndex(index);
	#else
	buffer->Indices.push_back(index);
	#endif
};

auto CCP3DGeometryCreatorSetUsedVertices = [](CMeshBuffer<S3DVertex> *buffer, const u32 &used) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	buffer->getVertexBuffer(0)->set_used(used);
	#else
	buffer->Vertices.set_used(used);
	#endif
};
auto CCP3DGeometryCreatorSetUsedIndices = [](CMeshBuffer<S3DVertex> *buffer, const u32 &used) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	buffer->getIndexBuffer()->set_used(used);
	#else
	buffer->Indices.set_used(used);
	#endif
};

auto CCP3DGeometryCreatorGetVertices = [](IMeshBuffer *buffer) -> S3DVertex* {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	return (S3DVertex *)buffer->getVertexBuffer(0)->getVertices();
	#else
	return (S3DVertex *)buffer->getVertices();
	#endif
};
auto CCP3DGeometryCreatorGetVertex = [](CMeshBuffer<S3DVertex> *buffer, const u32 &indice) -> S3DVertex& {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	return *(S3DVertex *)buffer->getVertexBuffer(0)->getVertex(indice);
	#else
	return buffer->Vertices[indice];
	#endif
};
auto CCP3DGeometryCreatorSetIndex = [](CMeshBuffer<S3DVertex> *buffer, const u32 &indice, const u32& value) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	buffer->getIndexBuffer()->setIndex(indice, value);
	#else
	buffer->Indices[indice] = value;
	#endif
};

auto CCP3DGeometryCreatorGetVertexCount = [](IMeshBuffer *buffer) -> const u32 {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	return buffer->getVertexBuffer(0)->getVertexCount();
	#else
	return buffer->getVertexCount();
	#endif
};

CCP3DGeometryCreator::CCP3DGeometryCreator(ISceneManager *smgr) : Smgr(smgr)
{

}

CCP3DGeometryCreator::~CCP3DGeometryCreator() {

}

IMesh *CCP3DGeometryCreator::createRingMesh(f32 radius, f32 width, u32 sections, SColor color) {
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	IVertexDescriptor *descriptor = Smgr->getVideoDriver()->getVertexDescriptor(0);
	CMeshBuffer<S3DVertex>* mb = new CMeshBuffer<S3DVertex>(descriptor);
	#else
	CMeshBuffer<S3DVertex>* mb = new CMeshBuffer<S3DVertex>();
	#endif

	CCP3DGeometryCreatorSetUsedVertices(mb, sections * 2 + 2);
	CCP3DGeometryCreatorSetUsedIndices(mb, sections * 6);

    // radians per segment
    f32 segRadians = (core::PI * 2.f) / f32(sections);
    f32 segTCoord = 1.f / f32(sections);

    // position vertices
    for (u32 i=0; i < sections; ++i)
    {
        const vector2df outer = vector2df((f32)sin(segRadians*f32(i)), cos(segRadians*f32(i))) * radius;
        const vector2df inner = vector2df((f32)sin(segRadians*f32(i)), cos(segRadians*f32(i))) * (radius-width);

		S3DVertex &v1 = CCP3DGeometryCreatorGetVertex(mb, i * 2 + 0);
		S3DVertex &v2 = CCP3DGeometryCreatorGetVertex(mb, i * 2 + 1);

        v1.Color = color;
        v1.Normal.set(0.f, 1.f, 0.f);
        v1.Pos.set(outer.X, 0.f, outer.Y);
        v1.TCoords.set(1.f, segTCoord * f32(i));

        v2.Color = color;
        v2.Normal.set(0.f, 1.f, 0.f);
        v2.Pos.set(inner.X, 0.f,inner.Y);
        v2.TCoords.set(0.f, segTCoord * f32(i));
    }

    // dupe vertices at the end
	CCP3DGeometryCreatorGetVertex(mb, sections * 2 + 0) = CCP3DGeometryCreatorGetVertex(mb, 0);
	CCP3DGeometryCreatorGetVertex(mb, sections * 2 + 0).TCoords.set(1.f, 1.f);
	CCP3DGeometryCreatorGetVertex(mb, sections * 2 + 1) = CCP3DGeometryCreatorGetVertex(mb, 1);
	CCP3DGeometryCreatorGetVertex(mb, sections * 2 + 1).TCoords.set(0.f, 1.f);

    // stitch indices
    u32 v=0;
    for (u32 i=0; i < sections * 6; i+=6, v+=2)
    {
		CCP3DGeometryCreatorSetIndex(mb, i + 0, v + 0);
		CCP3DGeometryCreatorSetIndex(mb, i + 1, v + 2);
		CCP3DGeometryCreatorSetIndex(mb, i + 2, v + 3);
		CCP3DGeometryCreatorSetIndex(mb, i + 3, v + 0);
		CCP3DGeometryCreatorSetIndex(mb, i + 4, v + 3);
		CCP3DGeometryCreatorSetIndex(mb, i + 5, v + 1);
    }

    mb->recalculateBoundingBox();

	SMesh *mesh = new SMesh();
    mesh->addMeshBuffer(mb);
    mesh->recalculateBoundingBox();
    mb->drop();

	return mesh;
}

IMesh *CCP3DGeometryCreator::createMassMesh(f32 size, f32 length, SColor color) {
	/// Create meshes
	IMesh *cubeMesh = Smgr->getGeometryCreator()->createCubeMesh(vector3df(size));
	IMesh *lengthMesh = Smgr->getGeometryCreator()->createCubeMesh(vector3df(size / 4.f, length, size / 4.f));

	/// set new pos of the mass
	IMeshBuffer *mb = cubeMesh->getMeshBuffer(0); /// Only 1 mesh buffer for cubes
	S3DVertex *vertices = CCP3DGeometryCreatorGetVertices(mb);

	for (u32 i=0; i < CCP3DGeometryCreatorGetVertexCount(mb); i++) {
		vertices[i].Pos.Y += length;
		vertices[i].Color = color;
	}
	mb->recalculateBoundingBox();
	mb->setDirty();

	mb = lengthMesh->getMeshBuffer(0);
	vertices = CCP3DGeometryCreatorGetVertices(mb);
	for (u32 i=0; i < CCP3DGeometryCreatorGetVertexCount(mb); i++) {
		vertices[i].Pos.Y += length / 2.f;
		vertices[i].Color = color;
	}
	mb->recalculateBoundingBox();
	mb->setDirty();

	((SMesh *)cubeMesh)->addMeshBuffer(lengthMesh->getMeshBuffer(0));
	((SMesh *)cubeMesh)->recalculateBoundingBox();

	/// Configure final mesh
	lengthMesh->drop();
	cubeMesh->setDirty();

	return cubeMesh;
}

irr::scene::IMesh *CCP3DGeometryCreator::createTorusMesh(irr::f32 diameter, irr::f32 thickness, irr::u32 tessellation, SColor color) {
#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	IVertexDescriptor *descriptor = Smgr->getVideoDriver()->getVertexDescriptor(0);
	CMeshBuffer<S3DVertex> *mb = new CMeshBuffer<S3DVertex>(descriptor);
#else
	CMeshBuffer<S3DVertex> *mb = new CMeshBuffer<S3DVertex>();
#endif

	auto TransformCoordinates = [&](vector3df vector, matrix4 transformation) -> vector3df {
		f32 rx = (vector.X * transformation.pointer()[0]) + (vector.Y * transformation.pointer()[4]) + (vector.Z * transformation.pointer()[8]) + transformation.pointer()[12];
        f32 ry = (vector.X * transformation.pointer()[1]) + (vector.Y * transformation.pointer()[5]) + (vector.Z * transformation.pointer()[9]) + transformation.pointer()[13];
        f32 rz = (vector.X * transformation.pointer()[2]) + (vector.Y * transformation.pointer()[6]) + (vector.Z * transformation.pointer()[10]) + transformation.pointer()[14];
        f32 rw = (vector.X * transformation.pointer()[3]) + (vector.Y * transformation.pointer()[7]) + (vector.Z * transformation.pointer()[11]) + transformation.pointer()[15];

		return vector3df(rx / rw, ry / rw, rz / rw);
	};

	auto TransformNormal = [&](vector3df vector, matrix4 transformation) -> vector3df {
		return vector3df((vector.X * transformation.pointer()[0]) + (vector.Y * transformation.pointer()[4]) + (vector.Z * transformation.pointer()[8]),
						 (vector.X * transformation.pointer()[1]) + (vector.Y * transformation.pointer()[5]) + (vector.Z * transformation.pointer()[9]),
						 (vector.X * transformation.pointer()[2]) + (vector.Y * transformation.pointer()[6]) + (vector.Z * transformation.pointer()[10]));
	};

	u32 stride = tessellation + 1;
	
	for (u32 i=0; i <= tessellation; i++) {
		f32 u = f32(i) / f32(tessellation);

		f32 outerAngle = i * core::PI * 2.f / tessellation - core::PI / 2.f;

		matrix4 transform;
		//transform.setTranslation(vector3df(diameter / 2.f, 0.f, 0.f)).rotateVect(vector3df(0.f, outerAngle, 0.f));

		for (u32 j=0; j <= tessellation; j++) {
			f32 v = 1.f - f32(j) / f32(tessellation);

			f32 innerAngle = j * core::PI * 2.f / tessellation + core::PI;
            f32 dx = cos(innerAngle);
            f32 dy = sin(innerAngle);

			vector3df normal(dx, dy, 0);
			vector3df position = normal * (thickness / 2.f);
			vector2df textureCoordinate(u, v);

			position = TransformCoordinates(position, transform);
            normal = TransformNormal(normal, transform);

			CCP3DGeometryCreatorAddVertex(mb, S3DVertex(position, normal, color, vector2df(u, v)), 0);

			u32 nextI = (i + 1) % stride;
            u32 nextJ = (j + 1) % stride;

			CCP3DGeometryCreatorAddIndex(mb, i * stride + j);
			CCP3DGeometryCreatorAddIndex(mb, i * stride + nextJ);
			CCP3DGeometryCreatorAddIndex(mb, i * stride + j);

			CCP3DGeometryCreatorAddIndex(mb, i * stride + nextJ);
			CCP3DGeometryCreatorAddIndex(mb, i * stride + nextJ);
			CCP3DGeometryCreatorAddIndex(mb, i * stride + j);

		}

	}

	mb->recalculateBoundingBox();

	SMesh *mesh = new SMesh();
    mesh->addMeshBuffer(mb);
    mesh->recalculateBoundingBox();
    mb->drop();

	return mesh;
}

} /// End namespace engine
} /// End namespace cp3d
