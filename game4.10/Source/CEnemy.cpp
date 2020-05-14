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
	// CEnemy: Enemy base class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy::CEnemy(gameMap* pointer, int x, int y) : currentMap(pointer), x(x), y(y) {};
	CEnemy::~CEnemy() {};

	void CEnemy::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		hero["x1"] = x1;
		hero["x2"] = x2;
		hero["y1"] = y1;
		hero["y2"] = y2;
	}

	void CEnemy::SetEnemyXY(int SetX, int SetY)
	{
		x = SetX;
		y = SetY;
	}

	void CEnemy::SetHeroAttackRange(int x1, int x2, int y1, int y2)
	{
		heroAttackRange["x1"] = x1;
		heroAttackRange["x2"] = x2;
		heroAttackRange["y1"] = y1;
		heroAttackRange["y2"] = y2;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_sunFlower: Enemy sunFlower class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_sunFlower::CEnemy_sunFlower(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;

		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(5);
		AttackRightAnimation.SetDelayCount(5);
		Bullet.SetDelayCount(4);
		DeadAnimation.SetDelayCount(3);
		moveingStep = 10;
		enemyHP = 100;							//敵人預設生命值
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}
	
	CEnemy_sunFlower::~CEnemy_sunFlower() {}

	int CEnemy_sunFlower::GetX1()
	{
		return x;
	}

	int CEnemy_sunFlower::GetY1()
	{
		return y;
	}

	int CEnemy_sunFlower::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_sunFlower::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_sunFlower::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_sunFlower::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_sunFlower::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()))
		{
			enemyHP -= damage;
		}
	}

	void CEnemy_sunFlower::AttackByEnemy(int *heroHP)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	string CEnemy_sunFlower::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_sunFlower::LoadBitmap()
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

		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_0, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_5, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_6, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_7, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_8, RGB(255, 255, 255));

		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_0, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_1, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_2, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_3, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_4, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_5, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_6, RGB(255, 255, 255));

		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_0, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_1, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_2, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_3, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_4, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_5, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_6, RGB(255, 255, 255));

		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_0, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_1, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_2, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_3, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_4, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_5, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_6, RGB(63, 72, 204));
		Bullet.AddBitmap(IDB_SUNFLOWERBULLET_7, RGB(63, 72, 204));

		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_0, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_1, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_2, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_3, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
	}

	void CEnemy_sunFlower::OnMove()
	{
		const int STEP_SIZE = 0;

		animation.OnMove();
		moveRightAnimation.OnMove();
		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();

		/*
		moveingStepCount--;
		if (moveingStepCount < 0)
		{
			moveingStepCount = moveingStep;
			isMovingRight = !isMovingRight;
		}
		*/

		if (isMovingRight)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當y座標還沒碰到牆
				x += STEP_SIZE;
		}
		else
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10)) // 當y座標還沒碰到牆
				x -= STEP_SIZE;
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
	}

	void CEnemy_sunFlower::OnShow()
	{
		if (enemyHP <= 0)
		{
			if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}
		}
		else if (isMovingRight)
		{
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();

		}
		else
		{
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
		}
	}

	bool CEnemy_sunFlower::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}


	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Cactus: Enemy Cactus class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Cactus::CEnemy_Cactus(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

	CEnemy_Cactus::~CEnemy_Cactus() {}

	int CEnemy_Cactus::GetX1()
	{
		return x;
	}

	int CEnemy_Cactus::GetY1()
	{
		return y;
	}

	int CEnemy_Cactus::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_Cactus::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_Cactus::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_Cactus::GetHeight()
	{
		return animation.Height();
	}

	string CEnemy_Cactus::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Cactus::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()))
		{
			enemyHP -= damage;
		}
	}


	void CEnemy_Cactus::LoadBitmap()
	{
		animation.AddBitmap(IDB_CACTUSNOMOVE_0, RGB(255, 127, 39));
		animation.AddBitmap(IDB_CACTUSNOMOVE_1, RGB(255, 127, 39));
		animation.AddBitmap(IDB_CACTUSNOMOVE_2, RGB(255, 127, 39));

		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_0, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_1, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_2, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_3, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_4, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_5, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_6, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_7, RGB(255, 127, 39));

		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_0, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_1, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_2, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_3, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_4, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_5, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_6, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_7, RGB(255, 127, 39));
	}

	void CEnemy_Cactus::OnMove()
	{
		const int STEP_SIZE = 0;

		animation.OnMove();
		AttackAnimation.OnMove();
		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();
		/*
		moveingStepCount--;
		if (moveingStepCount < 0)
		{
			moveingStepCount = moveingStep;
			isMovingRight = !isMovingRight;
		}
		*/


	}

	void CEnemy_Cactus::OnShow()
	{
		if (enemyHP <= 0)
		{
			if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}
		}
		/*else if (isAttacking)
		{
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();

		}*/
		else
		{
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
		}
	}

	void CEnemy_Cactus::AttackByEnemy(int *heroHP)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	bool CEnemy_Cactus::isDead()
	{
		if (enemyHP <= 0) return true;
		else return false;
	}


	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Statue: Enemy Statue class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Statue::CEnemy_Statue(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

	CEnemy_Statue::~CEnemy_Statue() {}

	int CEnemy_Statue::GetX1()
	{
		return x;
	}

	int CEnemy_Statue::GetY1()
	{
		return y;
	}

	int CEnemy_Statue::GetX2()
	{
		return x + Statue.Width();
	}

	int CEnemy_Statue::GetY2()
	{
		return y + Statue.Height();
	}

	int CEnemy_Statue::GetWidth()
	{
		return Statue.Width();
	}

	int CEnemy_Statue::GetHeight()
	{
		return Statue.Height();
	}

	string CEnemy_Statue::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Statue::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()))
		{
			enemyHP -= damage;
		}
	}


	void CEnemy_Statue::LoadBitmap()
	{
		Statue.LoadBitmap(IDB_STATUE, RGB(255, 0, 0));
		Statue_Broken.LoadBitmap(IDB_STATUE_BROKEN, RGB(255, 0, 0));
	}

	void CEnemy_Statue::OnMove()
	{
		const int STEP_SIZE = 0;

		/*animation.OnMove();
		moveRightAnimation.OnMove();
		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();*/

		/*
		moveingStepCount--;
		if (moveingStepCount < 0)
		{
			moveingStepCount = moveingStep;
			isMovingRight = !isMovingRight;
		}
		*/

		
	}

	void CEnemy_Statue::OnShow()
	{
		if (enemyHP <= 0)
		{
			Statue_Broken.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			Statue_Broken.ShowBitmap();
			/*if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}*/
		}
		else
		{
			Statue.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			Statue.ShowBitmap();
		}
	}

	void CEnemy_Statue::AttackByEnemy(int *heroHP)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	bool CEnemy_Statue::isDead()
	{
		if (enemyHP <= 0) return true;
		else return false;
	}
}