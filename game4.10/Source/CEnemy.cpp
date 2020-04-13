#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
#include "gameMap.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CEnemy::CEnemy(gameMap* pointer)
	{
		currentMap = pointer;
		Initialize();
	}

	int CEnemy::GetX1()
	{
		return x;
	}

	int CEnemy::GetY1()
	{
		return y;
	}

	int CEnemy::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy::GetHeight()
	{
		return animation.Height();
	}

	int CEnemy::GetCenterX()
	{
		return x + animation.Width() / 2;
	}

	int CEnemy::GetCenterY()
	{
		return y + animation.Height() / 2;
	}


	void CEnemy::Initialize()
	{
		const int X_POS = 600;
		const int Y_POS = 600;
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		x = X_POS;
		y = Y_POS;
		isMovingRight = true;

		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(1);

		enemyHP = 100;							//敵人預設生命值
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;

	}

	void CEnemy::LoadBitmap()
	{
		animation.AddBitmap(IDB_SUNFLOWERNOMOVE_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SUNFLOWERNOMOVE_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SUNFLOWERNOMOVE_3, RGB(255, 255, 255));

		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_5, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_6, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_7, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_8, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_9, RGB(255, 255, 255));

		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_0, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_1, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_2, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_3, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
	}

	
	void CEnemy::OnMove()
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		moveRightAnimation.OnMove();
		DeadAnimation.OnMove();
	}
	void CEnemy::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CEnemy::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CEnemy::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CEnemy::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CEnemy::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CEnemy::OnShow()
	{
		animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		animation.OnShow();
	}

}