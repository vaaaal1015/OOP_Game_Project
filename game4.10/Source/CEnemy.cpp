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

	void CEnemy::GetAttack(int x1, int x2, int y1, int y2, int damage)
	{
		//if ((GetX2() + 100 >= x1) && (x2 - 100 >= GetX1()) && (GetY2() >= y1) && (y2 >= GetY1()))
		if ((GetX2() >= x1) && (x2 >= GetX1()) && (GetY2() >= y1) && (y2 >= GetY1()))
		{
			enemyHP -= damage;
		}
	}

	void CEnemy::Initialize()
	{
		const int X_POS = 600;
		const int Y_POS = 0;
		const int INITIAL_VELOCITY = 15;		// ��l�W�ɳt��
		const int FLOOR = 100;					// �a�O�y��
		x = X_POS;
		y = Y_POS;
		isMovingRight = true;

		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		moveingStep = 10;
		enemyHP = 100;							//�ĤH�w�]�ͩR��
		enemyAttackDamage = 10;					//�ĤH�w�]�����O
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
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
	}

	
	void CEnemy::OnMove()
	{
		const int STEP_SIZE = 0;
		animation.OnMove();
		moveRightAnimation.OnMove();
		if(enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();

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