#pragma once

#ifndef _COUNTER_H_
#define _COUNTER_H_

namespace game_framework {
	class Counter
	{
	public:
		Counter();
		~Counter() = default;
		void OnMove();						// 簿笆じ
		bool isFinish();					// 肚琌璸计Ч拨
		void Reset();						// 砞﹚穝璸计
		void SetDelayCount(int delay);		// 砞﹚璸计Ω计
	private:
		int counter;						// ヘ玡璸计计
		int counterDelay;					// 羆璸计计
	};
}

#endif // !_COUNTER_H_