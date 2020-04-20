#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "NPC.h"
#include "CEnemy.h"
#include "gameMap.h"


namespace game_framework {
	NPC::NPC(gameMap* pointer)
	{
		currentMap = pointer;
		Initialize();
	}

	int NPC::GetX1()
	{
		return x;
	}

	int NPC::GetY1()
	{
		return y;
	}

	int NPC::GetX2()
	{
		return x + animation.Width();
	}

	int NPC::GetY2()
	{
		return y + animation.Height();
	}

	int NPC::GetWidth()
	{
		return animation.Width();
	}

	int NPC::GetHeight()
	{
		return animation.Height();
	}

	void NPC::Initialize()
	{
		const int X_POS = 600;
		const int Y_POS = 0;
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		x = X_POS;
		y = Y_POS;
		animation.SetDelayCount(3);
		InteractionBarFlag = 0;
		InteractionBar.SetDelayCount(3);
	}

	void NPC::LoadBitmap()
	{
		animation.AddBitmap(IDB_OLD_MAN_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_OLD_MAN_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_OLD_MAN_3, RGB(255, 255, 255));

		InteractionBar.AddBitmap(IDB_TALK_BAR_1, RGB(255, 255, 255));
		InteractionBar.AddBitmap(IDB_TALK_BAR_2, RGB(255, 255, 255));
		InteractionBar.AddBitmap(IDB_TALK_BAR_3, RGB(255, 255, 255));
		InteractionBar.AddBitmap(IDB_TALK_BAR_4, RGB(255, 255, 255));
		
	}


	void NPC::OnMove()
	{
		animation.OnMove();
		InteractionBar.OnMove();
	}
	

	void NPC::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void NPC::OnShow()
	{
		
		animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		animation.OnShow();
		if (InteractionBarFlag == 1)
		{
			InteractionBar.SetTopLeft(currentMap->ScreenX(x + 80), currentMap->ScreenY(y));
			InteractionBar.OnShow();
		}
	}

	bool NPC::TouchedByHero(int x1, int x2, int y1, int y2)
	{
		// TRACE("%d,%d,%d,%d\n", x1, x2, y1, y2);
		if ((GetX2() >= x1) && (x2 >= GetX1()) && (GetY2() >= y1) && (y2 >= GetY1()))

		{
			InteractionBarFlag = 1;
			return true;
		}
		InteractionBarFlag = 0;
		return false;
	}
}