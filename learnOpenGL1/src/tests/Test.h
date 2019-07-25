#pragma once

#include <vector>
#include <functional>

namespace test {

	class Test {
	public:

		///functionally same as OnInit
		Test() {}	

		///functionally same as OnDestroy
		virtual ~Test() {}	

		///ideally runs at a fixed time  delta
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}

		///contains all ImGui rendering code
		virtual void OnImGuiRender() {}

	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		///contains all ImGui rendering code
		virtual void OnImGuiRender()  override;

		template<typename T>
		void RegisterTest(const std::string& name) {
			std::cout << "Registering test: " << name << '\n';
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		///current running function
		Test*& m_CurrentTest;

		///an array of test_names to functions that create said function
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}