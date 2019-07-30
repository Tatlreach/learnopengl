#pragma once


#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>


namespace test {
	
	class TestTexture2D : public Test
	{
	public:
		//not virtual as we won't be extending these classes
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr <VertexArray> m_VAO;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <VertexBuffer> m_VertexBuffer;
		std::unique_ptr <Shader> m_Shader;
		std::unique_ptr <Texture> m_Texture;

		glm::mat4 m_Proj;	///when multiplied, converts it to a space between -1 & 1

		///camera transformations are actually reverse, because we're acting on the world not the camera
		glm::mat4 m_View;	///creates an identity matrix (of 1s), translates x -100.0f
		glm::vec3 m_TranslationA, m_TranslationB;
	};
}