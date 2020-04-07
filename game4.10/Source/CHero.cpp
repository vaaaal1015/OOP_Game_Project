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
	
	int CHero::HitEnemyAndReternDamage(int x1, int x2, int y1, int y2)
	{
		if ((GetX2() + 100 >= x1) && (x2 -100 >= GetX1()) && (GetY2() >= y1) && (y2 >= GetY1())) 
		{
			return heroAttackDamage;
		}
		return 0;
	}

	void CHero::beHit(int objectX, int objectY, int objectDamage)
	{
		if (((x <= objectX)) && (objectX <= x + animation.Width()) && ((y <= objectY) && (y + animation.Height())))
		{
			heroHP -= objectDamage;
		}

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

		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = false;

		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;

		animation.SetDelayCount(3);
		animation1.SetDelayCount(3);
		sword.SetDelayCount(1);
		swordAttack.SetDelayCount(1);
		swordAttack1.SetDelayCount(1);
		HeroAttackMovement.SetDelayCount(3);
		HeroAttackMovement1.SetDelayCount(3);
		sword1.SetDelayCount(1);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(5);
		moveLeftAnimation.SetDelayCount(3);

		heroHP = 100;						// 主角預設血量為100
		heroAttackDamage = 5;				// 主角預設攻擊力為5
		AttackRange = 100;					// 主角攻擊範圍
		
		maps.push_back(new gameMap("level_1.txt"));
		maps.push_back(new gameMap("level_2.txt"));
		SetMap(0);
	}

	void CHero::LoadBitmap()
	{
		animation.AddBitmap(IDB_HeroNoMove_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_4, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_5, RGB(255, 255, 255));

		animation1.AddBitmap(IDB_HERONOMOVELEFT_1, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_2, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_3, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_4, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_5, RGB(255, 255, 255));

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

		jumpAnimation.AddBitmap(IDB_HEROJUMP_4, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_3, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_2, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_1, RGB(255, 255, 255));

		sword.AddBitmap(IDB_sword_1, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_2, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_3, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_4, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_5, RGB(255, 255, 255));

		sword.AddBitmap(IDB_sword_7, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_1_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_2_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_3_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_4_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_5_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_7_1, RGB(255, 255, 255));
		
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_1, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_2, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_3, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_4, RGB(255, 255, 255));
		
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_1_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_2_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_3_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_4_1, RGB(255, 255, 255));

		swordAttack.AddBitmap(IDB_SWORDATTACK_2,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_3,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_4,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_5,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_6,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_7,RGB(255, 255, 255));

		swordAttack1.AddBitmap(IDB_SWORDATTACK_3_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_4_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_5_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_6_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_7_1, RGB(255, 255, 255));

		for (vector<gameMap*>::iterator i = maps.begin(); i != maps.end(); i++) (*i)->LoadBitmap();
	}

	void CHero::OnMove()
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		animation1.OnMove();
		sword.OnMove();
		sword1.OnMove();
		HeroAttackMovement.OnMove();
		HeroAttackMovement1.OnMove();
		swordAttack.OnMove();
		swordAttack1.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		jumpAnimation.OnMove();
		if (isMovingLeft)
		{
			setHeroDirection("left");   //角色向左看
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2()-10)) // 當x座標還沒碰到牆
				x -= STEP_SIZE;
		}
		if (isMovingRight)
		{
			setHeroDirection("right");   //角色向右看
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2()-10)) // 當y座標還沒碰到牆
				x += STEP_SIZE;
		}
		if (isMovingUp && y == (floor))
		{
			rising = true;						// 改為上升狀態
		}
		if (rising) {							// 上升狀態
			if (velocity > 0) {
				y -= velocity;					// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;						// 受重力影響，下次的上升速度降低
				if (!currentMap->isSpace(GetX1(), GetY1()) || !currentMap->isSpace(GetX2(), GetY1()))  // 當x座標碰到天花板
				{
					rising = false;
					velocity = 1;
				}
			}
			else {
				rising = false;					// 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;					// 下降的初速(velocity)為1
			}
		}
		else {									// 下降狀態
			if (currentMap->isSpace(GetX1(), GetY2()) && currentMap->isSpace(GetX2(), GetY2())) {  // 當y座標還沒碰到地板
				y += velocity;					// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;						// 受重力影響，下次的下降速度增加
			}
			else {
				floor = currentMap->GetBlockY(GetY2()) - GetHeight();
				y = floor;					// 當y座標低於地板，更正為地板上
				velocity = initial_velocity;	// 重設上升初始速度
			}
		}
		
		currentMap->SetSXSY(GetCenterX() - SIZE_X / 2, GetCenterY() - SIZE_Y / 2);
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

	void CHero::SetHeroAttack(bool flag)
	{
		isAttacking = flag;
	}

	void CHero::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CHero::setHeroDirection(string direction)
	{
		faceDirection = direction;
	}

	void CHero::OnShow()
	{
		currentMap->OnShow();

		if (isMovingRight)		// 向右走
		{
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();

		}
		else if (isMovingLeft)	// 向左走
		{
			moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveLeftAnimation.OnShow();
		}
		else if (isMovingUp)	//跳躍
		{
			jumpAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			jumpAnimation.OnShow();
		}
		else if (isAttacking)
		{
			if (faceDirection == "right")
			{
				HeroAttackMovement.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement.OnShow();
			}
			else
			{
				HeroAttackMovement1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement1.OnShow();
			}
		}
		else
		{
			if (faceDirection == "right")   // 靜止向右看
			{
				animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				animation.OnShow();
			}
			else							// 靜止向左看
			{
				animation1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				animation1.OnShow();
			}
		}

		if (isAttacking)
		{
			if (faceDirection == "right")
			{
				swordAttack.SetTopLeft(currentMap->ScreenX(x - 40), currentMap->ScreenY(y));
				swordAttack.OnShow();
				if (swordAttack.IsFinalBitmap()) SetHeroAttack(false);
			}
			else
			{
				swordAttack1.SetTopLeft(currentMap->ScreenX(x - 75), currentMap->ScreenY(y));
				swordAttack1.OnShow();
				if (swordAttack1.IsFinalBitmap()) SetHeroAttack(false);
			}
		}
		else
		{
			if (faceDirection == "right")   // 靜止向右看
			{
				sword.SetTopLeft(currentMap->ScreenX(x - 75), currentMap->ScreenY(y + 10));
				sword.OnShow();
			}
			else							// 靜止向左看
			{
				sword1.SetTopLeft(currentMap->ScreenX(x + 37), currentMap->ScreenY(y + 10));
				sword1.OnShow();
			}
		}
	}

	void CHero::SetHeroHP(int inputHP)
	{
		heroHP = inputHP;
	}

	void CHero::SetMap(int index)
	{
		GAME_ASSERT((int)maps.size() > index, "CHero: SetMap input index over range");
		currentMap = maps[index];

		const int X_POS = 0;
		const int Y_POS = 0;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = false;

		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}
}