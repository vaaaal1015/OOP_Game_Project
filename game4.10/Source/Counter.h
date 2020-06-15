#pragma once

#ifndef _COUNTER_H_
#define _COUNTER_H_

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

#endif // !_COUNTER_H_