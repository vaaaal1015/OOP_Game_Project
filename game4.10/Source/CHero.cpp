#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameMap.h"
#include "CHero.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CHero::CHero()
	{
		Initialize();
	}

	int CHero::GetX1()
	{
		return x;
	}

	int CHero::GetY1()
	{
		return y;
	}

	int CHero::GetX2()
	{
		return x + animation.Width();
	}

	int CHero::GetY2()
	{
		return y + animation.Height();
	}

	void CHero::Initialize()
	{
		//const int X_POS = 280;
		//const int Y_POS = 320;
		const int X_POS = 0;
		const int Y_POS = 0;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;

		/*
		const int INITIAL_VELOCITY = 20;	// 初始上升速度
		const int FLOOR = 320;				// 地板座標
		floor = FLOOR;
		y = FLOOR - 1;				// y座標比地板高1點(站在地板上)
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		*/
	}

	void CHero::LoadBitmap()
	{
		animation.AddBitmap(IDB_ERASER1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER2, RGB(255, 255, 255));
	}

	void CHero::OnMove(gameMap *mymap)
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		mymap->SetSXSY(x, y);
		if (isMovingLeft)
		{
			x -= STEP_SIZE;
			mymap->SetSXSY(x, y);
		}
		if (isMovingRight)
		{
			x += STEP_SIZE;
			mymap->SetSXSY(x, y);
		}
		//if (isMovingUp && y == (floor - 1))
		if (isMovingUp)
		{
			//rising = true;
			y -= STEP_SIZE;
			mymap->SetSXSY(x, y);
		}
		if (isMovingDown)
		{
			y += STEP_SIZE;
			mymap->SetSXSY(x, y);
		}
		/*
		if (rising) {			// 上升狀態
			if (velocity > 0) {
				y -= velocity;	// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;		// 受重力影響，下次的上升速度降低
			}
			else {
				rising = false; // 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;	// 下降的初速(velocity)為1
			}
		}
		else {				// 下降狀態
			if (y < floor - 1) {  // 當y座標還沒碰到地板
				y += velocity;	// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;		// 受重力影響，下次的下降速度增加
			}
			else {
				y = floor - 1;  // 當y座標低於地板，更正為地板上
				//rising = true;	// 探底反彈，下次改為上升
				velocity = initial_velocity; // 重設上升初始速度
			}
		}
		*/

		//mymap->SetSXSY(x, y);
	}

	void CHero::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CHero::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CHero::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CHero::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CHero::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CHero::OnShow(gameMap *mymap)
	{
		animation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
		animation.OnShow();
	}
}