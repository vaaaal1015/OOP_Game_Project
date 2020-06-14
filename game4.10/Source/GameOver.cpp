#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "GameOver.h"

namespace game_framework {
	GameOver::GameOver()
	{
		Ghost.SetDelayCount(4);
	}

	GameOver::~GameOver()
	{
	}

	void GameOver::LoadBitmap()
	{
		Ghost.AddBitmap(IDB_GHOST_1);
		Ghost.AddBitmap(IDB_GHOST_2);
		Ghost.AddBitmap(IDB_GHOST_3);
		Ghost.AddBitmap(IDB_GHOST_4);
		Ghost.AddBitmap(IDB_GHOST_5);
		Ghost.AddBitmap(IDB_GHOST_6);
		Ghost.AddBitmap(IDB_GHOST_7);
		Ghost.AddBitmap(IDB_GHOST_8);
		Ghost.AddBitmap(IDB_GHOST_9);
		Ghost.AddBitmap(IDB_GHOST_10);
		Ghost.AddBitmap(IDB_GHOST_11);
		Ghost.AddBitmap(IDB_GHOST_12);
		Ghost.AddBitmap(IDB_GHOST_13);
		Ghost.AddBitmap(IDB_GHOST_14);
		Ghost.AddBitmap(IDB_GHOST_15);
		Ghost.AddBitmap(IDB_GHOST_16);
		Ghost.AddBitmap(IDB_GHOST_17);
		Ghost.AddBitmap(IDB_GHOST_18);
		Ghost.AddBitmap(IDB_GHOST_19);
		Ghost.AddBitmap(IDB_GHOST_20);

		pressEnter.LoadBitmap(IDB_WORD_PRESS_ENTER);
	}

	void GameOver::OnMove()
	{
		Ghost.OnMove();
	}

	void  GameOver::OnShow()
	{

		Ghost.SetTopLeft((SIZE_X - Ghost.Width()) / 2, (SIZE_Y - Ghost.Height()) / 2);
		Ghost.OnShow();

		pressEnter.SetTopLeft((SIZE_X - pressEnter.Width()) / 2, (SIZE_Y - 100));
		pressEnter.ShowBitmap();
	}
}