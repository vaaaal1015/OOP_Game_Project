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
		if ((x + animation.Width() >= x2) && (x <= x2) && (y + animation.Height() >= y1) && (y <= y2))//A.x+A.W >= B.x �G Sprite-A ���k��ɤj�󵥩� Sprite-B ������ɡC
		{																							  //A.x <= B.x+B.W �G Sprite-A ������ɤp�󵥩� Sprite-B ���k��ɡC
			return heroAttackDamage;																  //A.y+A.H >= B.y �G Sprite-A ���U��ɤj�󵥩� Sprite-B ���W��ɡC
		}																							  //A.y <= B.y+B.H �G Sprite-A ���W��ɤp�󵥩� Sprite-B ���U��ɡC
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
		animation.SetDelayCount(3);
		animation1.SetDelayCount(3);
		sword.SetDelayCount(1);
		swordAttack.SetDelayCount(1);
		HeroAttackMovement.SetDelayCount(3);
		sword1.SetDelayCount(1);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(5);
		moveLeftAnimation.SetDelayCount(3);
		heroHP = 100;			//�D���w�]��q��100
		heroAttackDamage = 5;	//�D���w�]�����O��5
		const int INITIAL_VELOCITY = 15;	// ��l�W�ɳt��
		const int FLOOR = 100;				// �a�O�y��
		floor = FLOOR;
		//y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
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
		//sword.AddBitmap(IDB_sword_6, RGB(255, 255, 255));
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
		
		swordAttack.AddBitmap(IDB_SWORDATTACK_1,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_2,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_3,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_4,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_5,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_6,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_7,RGB(255, 255, 255));
	}

	void CHero::OnMove(gameMap *mymap)
	{
		const int STEP_SIZE = 10;
		animation.OnMove();
		animation1.OnMove();
		sword.OnMove();
		sword1.OnMove();
		HeroAttackMovement.OnMove();
		swordAttack.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		jumpAnimation.OnMove();
		if (isMovingLeft)
		{
			setHeroDirection("left");   //����V����
			if (mymap->isSpace(GetX1(), GetY1()) && mymap->isSpace(GetX1(), GetY2() - 20)) // ��x�y���٨S�I����
				x -= STEP_SIZE;
		}
		if (isMovingRight)
		{
			setHeroDirection("right");   //����V�k��
			if (mymap->isSpace(GetX2(), GetY1()) && mymap->isSpace(GetX2(), GetY2() - 20)) // ��y�y���٨S�I����
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
			rising = true;	//�אּ�W�ɪ��A
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
		if (rising) {			// �W�ɪ��A
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

	void CHero::OnShow(gameMap *mymap)
	{
		//animation.SetTopLeft(mymap->ScreenX(x - GetWidth() / 2), mymap->ScreenY(y - GetHeight() / 2));
		if (isMovingRight)   //�V�k��
		{
			moveRightAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			moveRightAnimation.OnShow();
		}
		else if (isMovingLeft)  //�V����
		{
			moveLeftAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			moveLeftAnimation.OnShow();
		}
		else if (isMovingUp)   //���D
		{
			jumpAnimation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			jumpAnimation.OnShow();
		}
		else if (isAttacking)
		{
			HeroAttackMovement.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
			HeroAttackMovement.OnShow();
		}
		else
		{
			if (faceDirection == "right")   //�R��V�k��
			{
				animation.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
				animation.OnShow();
			}
			else							//�R��V����
			{
				animation1.SetTopLeft(mymap->ScreenX(x), mymap->ScreenY(y));
				animation1.OnShow();
			}
			
		}
		//�B�zsword��ܦ�m
		if (faceDirection == "right")		//�V�k��
		{
			if (isAttacking)
			{
				swordAttack.SetTopLeft(mymap->ScreenX(x - 40), mymap->ScreenY(y));
				swordAttack.OnShow();
			}
			else
			{
				sword.SetTopLeft(mymap->ScreenX(x - 75), mymap->ScreenY(y + 10));
				sword.OnShow();
			}
			
		}
		else								//�V����
		{
			sword1.SetTopLeft(mymap->ScreenX(x + 37), mymap->ScreenY(y + 10));
			sword1.OnShow();
		}
		
	}

	void CHero::SetHeroHP(int inputHP)
	{
		heroHP = inputHP;
	}



}