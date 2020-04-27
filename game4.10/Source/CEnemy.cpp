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
	void CEnemy::LoadBitmap() {};
	void CEnemy::OnMove() {};
	void CEnemy::OnShow() {};
	void  CEnemy::GetAttack(const int damage) {};
	void  CEnemy::AttackByEnemy(int* heroHP) {};

	void CEnemy::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		hero["x1"] = x1;
		hero["x2"] = x2;
		hero["y1"] = y1;
		hero["y2"] = y2;
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
		const int INITIAL_VELOCITY = 15;		// ��l�W�ɳt��
		const int FLOOR = 100;					// �a�O�y��
		isMovingRight = true;
		rising = false;

		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		moveingStep = 10;
		enemyHP = 100;							//�ĤH�w�]�ͩR��
		enemyAttackDamage = 10;					//�ĤH�w�]�����O
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

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
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // ��y�y���٨S�I����
				x += STEP_SIZE;
		}
		else
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10)) // ��y�y���٨S�I����
				x -= STEP_SIZE;
		}

		if (rising) {							// �W�ɪ��A
			if (velocity > 0) {
				y -= velocity;					// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				velocity--;						// �����O�v�T�A�U�����W�ɳt�׭��C
				if (!currentMap->isSpace(GetX1(), GetY1()) || !currentMap->isSpace(GetX2(), GetY1()))  // ��x�y�иI��Ѫ�O
				{
					rising = false;
					velocity = 1;
				}
			}
			else {
				rising = false;					// ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
				velocity = 1;					// �U������t(velocity)��1
			}
		}
		else {									// �U�����A
			if (currentMap->isSpace(GetX1(), GetY2()) && currentMap->isSpace(GetX2(), GetY2())) {  // ��y�y���٨S�I��a�O
				y += velocity;					// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
				velocity++;						// �����O�v�T�A�U�����U���t�׼W�[
			}
			else {
				floor = currentMap->GetBlockY(GetY2()) - GetHeight();
				y = floor;					// ��y�y�ЧC��a�O�A�󥿬��a�O�W
				velocity = initial_velocity;	// ���]�W�ɪ�l�t��
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
}