#pragma once

class VertexBuffer {
	private:
		unsigned int m_RendererID;
	public:
		/// Generates buffer. Binds GL_ARRAY_BUFFER. Writes data to buffer.
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
};