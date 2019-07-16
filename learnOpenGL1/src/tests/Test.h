#pragma once

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
}