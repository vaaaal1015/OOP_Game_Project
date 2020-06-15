#include "stdafx.h"
#include "Counter.h"

namespace game_framework {

	Counter::Counter()
	{
	}

	void Counter::OnMove()
	{
		if (counter > 0) counter--;
	}

	void Counter::Reset()
	{
		counter = counterDelay;
	}

	void Counter::SetDelayCount(int delay)
	{
		if (delay >= 0)
			counterDelay = delay;
	}

	bool Counter::isFinish()
	{
		if (counter == 0)
			return true;
		else
			return false;
	}

}