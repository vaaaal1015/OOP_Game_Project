#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameMap.h"
#include "CHero.h"
#include "CEnemy.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CEnemy::CEnemy()
	{
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

	bool CEnemy::isHit(int objectX, int objectY)
	{
		if (((x <= objectX)) && (objectX <= x + animation.Width()) && ((y <= objectY) && (y + animation.Height())))
		{
			return true;
		}
		return false;
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

	int CEnemy::GetHP()
	{
		return enemyHP;
	}

	void CEnemy::isHitByHero(CHero *hero)
	{
		int damage = hero->HitEnemyAndReternDamage(GetX1(), GetX2(), GetY1(), GetY2());
		if (damage > 0)
		{
			enemyHP -= damage;
		}
	}

	void CEnemy::Initialize()
	{
		const int X_POS = 600;
		const int Y_POS = 600;
		x = X_POS;
		y = Y_POS;
		isMovingRight = true;
		animation.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		enemyHP = 100;			//�ĤH�w�]�ͩR��
		enemyAttackDamage = 10;	//�ĤH�w�]�����O
		const int INITIAL_VELOCITY = 15;	// ��l�W�ɳt��
		const int FLOOR = 100;				// �a�O�y��
		floor = FLOOR;
		//y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
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
	}

	
	void CEnemy::OnMove(gameMap *mymap, CHero *hero)
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		moveRightAnimation.OnMove();
		/*if (isMovingLeft)
		{
			if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2() - 20)) // ��x�y���٨S�I����
				x -= STEP_SIZE;
		}
		*/
		if (isMovingRight)
		{
			if (mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2() - 20)) // ��y�y���٨S�I����
				x += STEP_SIZE;
		}
		/*
		if (isMovingUp && y == (floor - 1))
		{
			//if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2()) && mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2()))
			rising = true;	//�אּ�W�ɪ��A
		}
		if (isMovingDown)
		{

		}*/


		/*if (rising) {			// �W�ɪ��A
			if (velocity > 0) {
				y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
				if (!mymap->isSpace(GetX1() + 10, GetY1() - 1) || !mymap->isSpace(GetX2() - 10, GetY1() - 1))  // ��x�y�иI��Ѫ�O
					velocity = -1;
			}
			else {
				rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
				velocity = 1;	// �U������t(velocity)��1
			}
		}
		else {				// �U�����A
			//if (y < floor - 1)
			//if (mymap->isSpace(x, y - 1))
			if (mymap->isSpace(GetX1() + 10, GetY2() + 1) && mymap->isSpace(GetX2() - 10, GetY2() + 1)) {  // ��y�y���٨S�I��a�O
				y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
				velocity++;		// �����O�v�T�A�U�����U���t�׼W�[
				floor = GetY2() - GetHeight();		//�]�wy�y�Ь��a�O
			}
			else {
				y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
				//rising = true;	// �����ϼu�A�U���אּ�W��
				velocity = initial_velocity; // ���]�W�ɪ�l�t��
			}
		}*/

		//mymap->SetSXSY(GetCenterX() - SIZE_X / 2, GetCenterY() - SIZE_Y / 2);

		if(hero->isAttacking) isHitByHero(hero);

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

	void CEnemy::OnShow(gameMap *mymap)
	{
		//animation.SetTopLeft(mymap->ScreenX(x - GetWidth() / 2), mymap->ScreenY(y - GetHeight() / 2));
		if (isMovingRight)
		{
			moveRightAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			moveRightAnimation.OnShow();

		}
		/*
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
		*/
		else
		{
			animation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			animation.OnShow();
		}

	}
}