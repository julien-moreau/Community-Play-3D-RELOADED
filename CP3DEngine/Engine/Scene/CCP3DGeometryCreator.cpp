
#include "stdafx.h"
#include "CCP3DGeometryCreator.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace cp3d {
namespace engine {

CCP3DGeometryCreator::CCP3DGeometryCreator(ISceneManager *smgr) : Smgr(smgr)
{

}

CCP3DGeometryCreator::~CCP3DGeometryCreator() {

}

IMesh *CCP3DGeometryCreator::createRingMesh(f32 radius, f32 width, u32 sections, SColor color) {
	CMeshBuffer<S3DVertex>* mb = new CMeshBuffer<S3DVertex>();
    mb->Vertices.set_used(sections * 2 + 2); // 2 new verts per section, plus dupe at the start/end
    mb->Indices.set_used(sections * 6); // two triangles per section

    // radians per segment
    f32 segRadians = (core::PI * 2.f) / f32(sections);
    f32 segTCoord = 1.f / f32(sections);

    // position vertices
    for (u32 i=0; i < sections; ++i)
    {
        const vector2df outer = vector2df(sin(segRadians*f32(i)), cos(segRadians*f32(i))) * radius;
        const vector2df inner = vector2df(sin(segRadians*f32(i)), cos(segRadians*f32(i))) * (radius-width);

        S3DVertex &v1 = mb->Vertices[i * 2 + 0];
        S3DVertex &v2 = mb->Vertices[i * 2 + 1];
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
    mb->Vertices[sections * 2 + 0] = mb->Vertices[0];
    mb->Vertices[sections * 2 + 0].TCoords.set(1.f, 1.f);
    mb->Vertices[sections * 2 + 1] = mb->Vertices[1];
    mb->Vertices[sections * 2 + 1].TCoords.set(0.f, 1.f);

    // stitch indices
    u32 v=0;
    for (u32 i=0; i < sections * 6; i+=6, v+=2)
    {
        mb->Indices[i + 0] = (v + 0);
        mb->Indices[i + 1] = (v + 2);
        mb->Indices[i + 2] = (v + 3);
        mb->Indices[i + 3] = (v + 0);
        mb->Indices[i + 4] = (v + 3);
        mb->Indices[i + 5] = (v + 1);
    }

    mb->recalculateBoundingBox();

	SMesh *mesh = new SMesh();
    mesh->addMeshBuffer(mb);
    mesh->recalculateBoundingBox();
    mb->drop();

	return mesh;
}

irr::scene::IMesh *CCP3DGeometryCreator::createTorusMesh(irr::f32 diameter, irr::f32 thickness, irr::u32 tessellation, SColor color) {
	CMeshBuffer<S3DVertex> *mb = new CMeshBuffer<S3DVertex>();

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
		transform.setTranslation(vector3df(diameter / 2.f, 0.f, 0.f)).rotateVect(vector3df(0.f, outerAngle, 0.f));

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

			mb->Vertices.push_back(S3DVertex(position, normal, color, vector2df(u, v)));

			u32 nextI = (i + 1) % stride;
            u32 nextJ = (j + 1) % stride;

			mb->Indices.push_back(i * stride + j);
			mb->Indices.push_back(i * stride + nextJ);
			mb->Indices.push_back(i * stride + j);

			mb->Indices.push_back(i * stride + nextJ);
			mb->Indices.push_back(i * stride + nextJ);
			mb->Indices.push_back(i * stride + j);
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
