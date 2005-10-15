// OGLVertexStream.hpp
// KlayGE OpenGL顶点数据流类 头文件
// Ver 2.8.0
// 版权所有(C) 龚敏敏, 2003-2005
// Homepage: http://klayge.sourceforge.net
//
// 2.8.0
// 增加了CopyToMemory (2005.7.24)
// 只支持vbo (2005.7.31)
//
// 2.7.0
// 支持vertex_buffer_object (2005.6.19)
//
// 2.0.4
// 初次建立 (2004.4.3)
//
// 修改记录
//////////////////////////////////////////////////////////////////////////////////

#ifndef _OGLVERTEXSTREAM_HPP
#define _OGLVERTEXSTREAM_HPP

#include <vector>
#include <glloader/glloader.h>

#include <KlayGE/VertexBuffer.hpp>

#ifdef KLAYGE_DEBUG
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL_d.lib")
#else
	#pragma comment(lib, "KlayGE_RenderEngine_OpenGL.lib")
#endif

namespace KlayGE
{
	class OGLVertexStream : public VertexStream
	{
	public:
		OGLVertexStream(vertex_elements_type const & vertex_elems, bool staticStream);
		~OGLVertexStream();

		bool IsStatic() const
		{
			return static_stream_;
		}
		uint32_t NumVertices() const
		{
			return numVertices_;
		}

		void Assign(void const * src, uint32_t numVertices);
		void CopyToMemory(void* data);

		void Active();

		GLuint OGLvbo() const
		{
			return vb_;
		}

	protected:
		uint32_t numVertices_;

		bool static_stream_;

		GLuint vb_;
	};
}

#endif			// _OGLVERTEXSTREAM_HPP
