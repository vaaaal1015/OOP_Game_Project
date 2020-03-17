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
		const int INITIAL_VELOCITY = 20;	// ��l�W�ɳt��
		const int FLOOR = 320;				// �a�O�y��
		floor = FLOOR;
		y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
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
		if (rising) {			// �W�ɪ��A
			if (velocity > 0) {
				y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
			}
			else {
				rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
				velocity = 1;	// �U������t(velocity)��1
			}
		}
		else {				// �U�����A
			if (y < floor - 1) {  // ��y�y���٨S�I��a�O
				y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
				velocity++;		// �����O�v�T�A�U�����U���t�׼W�[
			}
			else {
				y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
				//rising = true;	// �����ϼu�A�U���אּ�W��
				velocity = initial_velocity; // ���]�W�ɪ�l�t��
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