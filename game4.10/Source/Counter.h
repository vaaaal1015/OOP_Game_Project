#pragma once
namespace game_framework {
	class Counter
	{
	public:
		Counter();
		~Counter() = default;
		void OnMove();
		bool isFinish();
		void Reset();
		void SetDelayCount(int delay);
	private:
		int counter;
		int counterDelay;
	};
}

