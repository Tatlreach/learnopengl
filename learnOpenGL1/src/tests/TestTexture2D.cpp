#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_TranslationA(200.0f, 200.0f, 0.0f), m_TranslationB(400.0f, 200.0f, 0.0f)
	{

		float positions[] = {
			-50.0f,	-50.0f,	0.0f, 0.0f,    // left bottom
			-50.0f,	 50.0f,	0.0f, 1.0f, // left top
			 50.0f,	-50.0f,	1.0f, 0.0f,    // right bottom
			 50.0f,	 50.0f,	1.0f, 1.0f    // right top
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 1
		};

		//setting how opengl blends alpha pixels
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();


		//VertexBuffer(datal, size)        //auto binds the vb
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));


		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		/*
		-     GLCall(glEnableVertexAttribArray(0));
		-     //links buffer with VAO
		-     GLCall(glVertexAttribPointer(0,    //starting index of specified attribute
		-         2,                        //number of components in ea. vertex
		-         GL_FLOAT,            //Attribute Type
		-         GL_FALSE,            //normalized
		-         sizeof(float) * 2,    //stride, total size of vertex
		-         0));                    //offset for the attribute WITHIN the vertex e.g. (const void*)
		*/

		//define index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		//    glBindBuffer(GL_ARRAY_BUFFER, 0);    //bind to empty buffer
			//4x4 matrix
				//creates orthographic matrix,   



		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		//requires bound shader
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		m_Texture = std::make_unique<Texture>("res/textures/clover.png");
		//bind slot needs to match set uniform slot
		m_Texture->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);    //0 has to match bound slot for texture
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO,*m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);	//only diff vs the code above
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}


	void TestTexture2D::OnImGuiRender()
	{


		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}