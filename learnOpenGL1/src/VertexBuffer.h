#pragma once

class VertexBuffer {
	private:
		unsigned int m_RendererID;
	public:
		/// Generates buffers. Binds GL_ARRAY_BUFFER. Writes the buffers.
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
};