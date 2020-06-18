#pragma once

#ifndef _COUNTER_H_
#define _COUNTER_H_

namespace game_framework {
	class Counter
	{
	public:
		Counter();
		~Counter() = default;
		void OnMove();						// ���ʤ���
		bool isFinish();					// �^�ǬO�_�p�Ƨ���
		void Reset();						// �]�w���s�p��
		void SetDelayCount(int delay);		// �]�w�p�Ʀ���
	private:
		int counter;						// �ثe�p�Ƽ�
		int counterDelay;					// �`�p�Ƽ�
	};
}

#endif // !_COUNTER_H_