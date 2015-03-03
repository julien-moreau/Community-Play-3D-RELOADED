// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_STL_WRITER_

#include "CSTLMeshWriter.h"
#include "os.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "IAttributes.h"
#include "ISceneManager.h"
#include "IMeshCache.h"
#include "IWriteFile.h"

namespace irr
{
namespace scene
{

CSTLMeshWriter::CSTLMeshWriter(scene::ISceneManager* smgr)
	: SceneManager(smgr)
{
	#ifdef _DEBUG
	setDebugName("CSTLMeshWriter");
	#endif

	if (SceneManager)
		SceneManager->grab();
}


CSTLMeshWriter::~CSTLMeshWriter()
{
	if (SceneManager)
		SceneManager->drop();
}


//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CSTLMeshWriter::getType() const
{
	return EMWT_STL;
}


//! writes a mesh
bool CSTLMeshWriter::writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags)
{
	if (!file || !mesh)
		return false;

	for(u32 i = 0; i < mesh->getMeshBufferCount(); ++i)
	{
		u32 Size = mesh->getMeshBuffer(i)->getVertexBuffer()->getVertexSize();

		if(Size != sizeof(video::S3DVertex) && Size != sizeof(video::S3DVertex2TCoords) && Size != sizeof(video::S3DVertexTangents))
			return false;
	}

	os::Printer::log("Writing mesh", file->getFileName());

	if (flags & scene::EMWF_WRITE_COMPRESSED)
		return writeMeshBinary(file, mesh, flags);
	else
		return writeMeshASCII(file, mesh, flags);
}


bool CSTLMeshWriter::writeMeshBinary(io::IWriteFile* file, scene::IMesh* mesh, s32 flags)
{
	// write STL MESH header

	file->write("binary ",7);
	const core::stringc name(SceneManager->getMeshCache()->getMeshName(mesh));
	const s32 sizeleft = 73-name.size(); // 80 byte header
	if (sizeleft<0)
		file->write(name.c_str(),73);
	else
	{
		char* buf = new char[80];
		memset(buf, 0, 80);
		file->write(name.c_str(),name.size());
		file->write(buf,sizeleft);
		delete [] buf;
	}
	u32 facenum = 0;
	for (u32 j=0; j<mesh->getMeshBufferCount(); ++j)
		facenum += mesh->getMeshBuffer(j)->getIndexBuffer()->getIndexCount()/3;
	file->write(&facenum,4);

	// write mesh buffers

	for (u32 i=0; i<mesh->getMeshBufferCount(); ++i)
	{
		IMeshBuffer* buffer = mesh->getMeshBuffer(i);
		if (buffer)
		{
			const u32 indexCount = buffer->getIndexBuffer()->getIndexCount();
			const u16 attributes = 0;

			video::IVertexAttribute* attribute = buffer->getVertexDescriptor()->getAttributeBySemantic(video::EVAS_POSITION);

			if(!attribute)
				continue;

			u8* Vertices = static_cast<u8*>(buffer->getVertexBuffer()->getVertices());
			Vertices += attribute->getOffset();

			for (u32 j=0; j<indexCount; j+=3)
			{
				core::vector3df* position0 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j));
				core::vector3df* position1 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j+1));
				core::vector3df* position2 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j+2));

				const core::vector3df& v1 = *position0;
				const core::vector3df& v2 = *position1;
				const core::vector3df& v3 = *position2;
				const core::plane3df tmpplane(v1,v2,v3);
				file->write(&tmpplane.Normal, 12);
				file->write(&v1, 12);
				file->write(&v2, 12);
				file->write(&v3, 12);
				file->write(&attributes, 2);
			}
		}
	}
	return true;
}


bool CSTLMeshWriter::writeMeshASCII(io::IWriteFile* file, scene::IMesh* mesh, s32 flags)
{
	// write STL MESH header

	file->write("solid ",6);
	const core::stringc name(SceneManager->getMeshCache()->getMeshName(mesh));
	file->write(name.c_str(),name.size());
	file->write("\n\n",2);

	// write mesh buffers

	for (u32 i=0; i<mesh->getMeshBufferCount(); ++i)
	{
		IMeshBuffer* buffer = mesh->getMeshBuffer(i);
		if (buffer)
		{
			video::IVertexAttribute* attribute = buffer->getVertexDescriptor()->getAttributeBySemantic(video::EVAS_POSITION);

			if(!attribute)
				continue;

			u8* Vertices = static_cast<u8*>(buffer->getVertexBuffer()->getVertices());
			Vertices += attribute->getOffset();

			const u32 indexCount = buffer->getIndexBuffer()->getIndexCount();
			for (u32 j=0; j<indexCount; j+=3)
			{
				core::vector3df* position0 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j));
				core::vector3df* position1 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j+1));
				core::vector3df* position2 = (core::vector3df*)(Vertices + buffer->getVertexBuffer()->getVertexSize() * buffer->getIndexBuffer()->getIndex(j+2));

				writeFace(file, *position0, *position1, *position2);
			}
			file->write("\n",1);
		}
	}

	file->write("endsolid ",9);
	file->write(name.c_str(),name.size());

	return true;
}


void CSTLMeshWriter::getVectorAsStringLine(const core::vector3df& v, core::stringc& s) const
{
	s = core::stringc(v.X);
	s += " ";
	s += core::stringc(v.Y);
	s += " ";
	s += core::stringc(v.Z);
	s += "\n";
}


void CSTLMeshWriter::writeFace(io::IWriteFile* file,
		const core::vector3df& v1,
		const core::vector3df& v2,
		const core::vector3df& v3)
{
	core::stringc tmp;
	file->write("facet normal ",13);
	getVectorAsStringLine(core::plane3df(v1,v2,v3).Normal, tmp);
	file->write(tmp.c_str(),tmp.size());
	file->write("  outer loop\n",13);
	file->write("    vertex ",11);
	getVectorAsStringLine(v1, tmp);
	file->write(tmp.c_str(),tmp.size());
	file->write("    vertex ",11);
	getVectorAsStringLine(v2, tmp);
	file->write(tmp.c_str(),tmp.size());
	file->write("    vertex ",11);
	getVectorAsStringLine(v3, tmp);
	file->write(tmp.c_str(),tmp.size());
	file->write("  endloop\n",10);
	file->write("endfacet\n",9);
}


} // end namespace
} // end namespace

#endif

