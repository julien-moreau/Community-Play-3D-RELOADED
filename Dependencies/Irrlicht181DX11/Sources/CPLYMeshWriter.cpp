// Copyright (C) 2008-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_PLY_WRITER_

#include "CPLYMeshWriter.h"
#include "os.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "IWriteFile.h"

namespace irr
{
namespace scene
{

CPLYMeshWriter::CPLYMeshWriter()
{
	#ifdef _DEBUG
	setDebugName("CPLYMeshWriter");
	#endif
}


//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CPLYMeshWriter::getType() const
{
	return EMWT_PLY;
}

//! writes a mesh
bool CPLYMeshWriter::writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags)
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

	// write PLY header
	core::stringc header =
        	"ply\n"
		"format ascii 1.0\n"
		"comment Irrlicht Engine ";
	header +=  IRRLICHT_SDK_VERSION;

	// get vertex and triangle counts
	u32 VertexCount   = 0;
	u32 TriangleCount = 0;

	for (u32 i=0; i < mesh->getMeshBufferCount(); ++i)
	{
		VertexCount   += mesh->getMeshBuffer(i)->getVertexBuffer()->getVertexCount();
		TriangleCount += mesh->getMeshBuffer(i)->getIndexBuffer()->getIndexCount() / 3;
	}

	// vertex definition
	header += "\nelement vertex ";
	header += VertexCount;

	header += "\n"
		"property float x\n"
		"property float y\n"
		"property float z\n"
		"property float nx\n"
		"property float ny\n"
		"property float nz\n";
	// todo: writer flags for extended (r,g,b,u,v) and non-standard (alpha,u1,uv,tx,ty,tz) properties
	//	"property uchar red\n"
	//	"property uchar green\n"
	//	"property uchar blue\n"
	//	"property uchar alpha\n"
	//	"property float u\n"
	//	"property float v\n";
	//	"property float u1\n
	//	"property float v1\n"
	//	"property float tx\n"
	//	"property float ty\n"
	//	"property float tz\n"

	// face definition

	header += "element face ";
	header += TriangleCount;
	header += "\n"
		"property list uchar int vertex_indices\n"
		"end_header\n";

	// write header
	file->write(header.c_str(), header.size());

	// write vertices

	c8 outLine[1024];

	for (u32 i=0; i < mesh->getMeshBufferCount(); ++i)
	{
		scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
		for (u32 j=0; j < mb->getVertexBuffer()->getVertexCount(); ++j)
		{
			core::vector3df pos;
			core::vector3df n;
//			const core::vector2df& tc  = mb->getTCoords(j);

			u8 *buf  = (u8*)mb->getVertexBuffer()->getVertices();
			switch(mb->getVertexBuffer()->getVertexSize())
			{
			case sizeof(video::S3DVertex):
				{
					video::S3DVertex *Vertices  = (video::S3DVertex*)mb->getVertexBuffer()->getVertices();
					pos = Vertices[j].Pos;
					n = Vertices[j].Normal;
					buf += sizeof(video::S3DVertex)*j;
				}
				break;
			case sizeof(video::S3DVertex2TCoords):
				{
					video::S3DVertex2TCoords *Vertices  = (video::S3DVertex2TCoords*)mb->getVertexBuffer()->getVertices();
					pos = Vertices[j].Pos;
					n = Vertices[j].Normal;
					buf += sizeof(video::S3DVertex2TCoords)*j;
				}
				break;
			case sizeof(video::S3DVertexTangents):
				{
					video::S3DVertexTangents *Vertices  = (video::S3DVertexTangents*)mb->getVertexBuffer()->getVertices();
					pos = Vertices[j].Pos;
					n = Vertices[j].Normal;
					buf += sizeof(video::S3DVertexTangents)*j;
				}
				break;
			}
//			video::SColor &col = ( (video::S3DVertex*)buf )->Color;

			// x y z nx ny nz red green blue alpha u v [u1 v1 | tx ty tz]\n
			snprintf(outLine, 1024,
				"%f %f %f %f %f %f\n",// %u %u %u %u %f %f\n",
				pos.X, pos.Z, pos.Y, // Y and Z are flipped
				n.X, n.Z, n.Y);
				/*col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha(),
				tc.X, tc.Y);*/

			// write the line
			file->write(outLine, strlen(outLine));
		}
	}

	// index of the first vertex in the current mesh buffer
	u32 StartOffset = 0;

	// write triangles
	for (u32 i=0; i < mesh->getMeshBufferCount(); ++i)
	{
		scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
		for (u32 j=0; j < mb->getIndexBuffer()->getIndexCount(); j+=3)
		{
			// y and z are flipped so triangles are reversed
			u32 a=StartOffset;
			u32 b=StartOffset;
			u32 c=StartOffset;

			a += mb->getIndexBuffer()->getIndex(j+0);
			c += mb->getIndexBuffer()->getIndex(j+1);
			b += mb->getIndexBuffer()->getIndex(j+2);

			// count a b c\n
			snprintf(outLine, 1024, "3 %u %u %u\n", a, b, c);
			// write the line
			file->write(outLine, strlen(outLine));
		}

		// increment offset
		StartOffset += mb->getVertexBuffer()->getVertexCount();
	}

	// all done!
	return true;
}

} // end namespace
} // end namespace

#endif // _IRR_COMPILE_WITH_PLY_WRITER_

