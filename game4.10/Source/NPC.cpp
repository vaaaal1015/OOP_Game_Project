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
	}

	void NPC::LoadBitmap()
	{
		
	}


	void NPC::OnMove()
	{
		animation.OnMove();
	}
	

	void NPC::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void NPC::OnShow()
	{
		
	}
}