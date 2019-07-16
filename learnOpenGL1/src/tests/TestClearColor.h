#pragma once


#include "Test.h"



namespace test {
	
	class TestClearColor : public Test
	{
	public:
		//not virtual as we won't be extending these classes
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];

	};
}