#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameMap.h"
#include "CHero.h"

#include <fstream>


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
	
	bool CHero::isHit(int objectX, int objectY)
	{
		if (((x <= objectX)) && (objectX <= x + animation.Width()) && ((y <= objectY) && (y + animation.Height())))
		{
			return true;
		}
		return false;
	}

	int CHero::GetWidth()
	{
		return animation.Width();
	}

	int CHero::GetHeight()
	{
		return animation.Height();
	}

	int CHero::GetCenterX()
	{
		return x + animation.Width() / 2;
	}

	int CHero::GetCenterY()
	{
		return y + animation.Height() / 2;
	}

	void CHero::Initialize()
	{
		const int X_POS = 0;
		const int Y_POS = 0;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		//y = FLOOR - 1;				// y座標比地板高1點(站在地板上)
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		
	}

	void CHero::LoadBitmap()
	{
		animation.AddBitmap(IDB_HeroNoMove_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_4, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_5, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_5, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_5, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_1, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_2, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_3, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_4, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_5, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_6, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_7, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_8, RGB(255, 255, 255));

	}

	void CHero::OnMove(gameMap *mymap)
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		jumpAnimation.OnMove();
		if (isMovingLeft)
		{
			if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2() - 20)) // 當x座標還沒碰到牆
				x -= STEP_SIZE;
		}
		if (isMovingRight)
		{
			if (mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2() - 20)) // 當y座標還沒碰到牆
				x += STEP_SIZE;
		}
		if (isMovingUp && y == (floor - 1))
		{
			/*
			int flag = 0;
			for (int i = 0; i < 480; i+=10)
				for (int j = 0; j < 640; j+=10)
					if (!mymap->isSpace(i, j)) flag = 1;
			*/
			//if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2()) && mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2()))
			rising = true;	//改為上升狀態
			/*
			if (!mymap->isSpace(x, GetY2() + 1))
				floor = GetY2()-1;
			*/
			//CreatTxt(mymap);
			//y -= STEP_SIZE;
		}
		if (isMovingDown)
		{
			//if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2()) && mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2()))
			//y += STEP_SIZE;
		}
		
		
		if (rising) {			// 上升狀態
			if (velocity > 0) {
				y -= velocity;	// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;		// 受重力影響，下次的上升速度降低
				if (!mymap->isSpace(GetX1() + 10, GetY1() - 1) || !mymap->isSpace(GetX2() - 10, GetY1() - 1))  // 當x座標碰到天花板
					velocity = -1;
			}
			else {
				rising = false; // 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;	// 下降的初速(velocity)為1
			}
		}
		else {				// 下降狀態
			//if (y < floor - 1)
			//if (mymap->isSpace(x, y - 1))
			if (mymap->isSpace(GetX1() + 10, GetY2() + 1) && mymap->isSpace(GetX2() - 10, GetY2() + 1)) {  // 當y座標還沒碰到地板
				y += velocity;	// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;		// 受重力影響，下次的下降速度增加
				floor = GetY2() - GetHeight();		//設定y座標為地板
			}
			else {
				y = floor - 1;  // 當y座標低於地板，更正為地板上
				//rising = true;	// 探底反彈，下次改為上升
				velocity = initial_velocity; // 重設上升初始速度
			}
		}
		
		mymap->SetSXSY(GetCenterX() - SIZE_X / 2, GetCenterY() - SIZE_Y / 2);
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
		//animation.SetTopLeft(mymap->ScreenX(x - GetWidth() / 2), mymap->ScreenY(y - GetHeight() / 2));
		if (isMovingRight)
		{
			moveRightAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			moveRightAnimation.OnShow();
		}
		else if (isMovingLeft)
		{
			moveLeftAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			moveLeftAnimation.OnShow();
		}
		else if (isMovingUp)
		{
			jumpAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			jumpAnimation.OnShow();
		}
		else
		{
			animation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			animation.OnShow();
		}
	}

	void CHero::CreatTxt(gameMap *mymap)//建立txt檔案
	{
		//fstream file;      //宣告fstream物件
		fstream file2;
		//file.open("testMap.txt", ios::out | ios::trunc);
		file2.open("testBool.txt", ios::out | ios::trunc);

		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入

		for (int i = 0; i < 960; i++) {
			for (int j = 0; j < 1280; j++) {
				//file << map[i][j];		//將str寫入檔案
				if (mymap->isSpace(j, i)) file2 << '0';
				else file2 << 'X';
			}
			//file << endl;
			file2 << endl;
		}

		//file.close();       //關閉檔案
		file2.close();
	}
}