#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
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

	CHero::~CHero()
	{
		for (vector<gameMap*>::iterator i = maps.begin(); i != maps.end(); i++) delete (*i);
		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++) delete (*i);
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
			CurrentHP -= objectDamage;
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

		const int INITIAL_VELOCITY = 15;	// ��l�W�ɳt��
		const int FLOOR = 100;				// �a�O�y��
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;

		animation.SetDelayCount(3);
		animation1.SetDelayCount(3);
		sword.SetDelayCount(3);
		swordAttack.SetDelayCount(3);
		swordAttack1.SetDelayCount(3);
		HeroAttackMovement.SetDelayCount(3);
		HeroAttackMovement1.SetDelayCount(3);
		sword1.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(4);
		jumpAnimation1.SetDelayCount(4);
		moveLeftAnimation.SetDelayCount(3);
		SetAttackDelayCount = AttackDelayCount = 15;

		FullHP = 100;						// �D���w�]��q��100
		CurrentHP = FullHP;
		heroAttackDamage = 5;				// �D���w�]�����O��5
		AttackRange = 100;					// �D�������d��
		
		maps.push_back(new gameMap("level_1.txt"));
		maps.push_back(new gameMap("level_2.txt"));
		SetMap(0);

		LifeBarRed.push_back(new CMovingBitmap);
		LifeBarRed.push_back(new CMovingBitmap);
		LifeBarRed.push_back(new CMovingBitmap);
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

		jumpAnimation.AddBitmap(IDB_HEROJUMP_3, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_4, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_5, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_6, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_7, RGB(255, 255, 255));

		jumpAnimation1.AddBitmap(IDB_HEROJUMP_0_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_1_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_2_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_3_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_4_1, RGB(255, 255, 255));

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

		LifeBarHead.LoadBitmap(IDB_LIFEBARHEAD, RGB(255, 255, 255));
		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++) (*i)->LoadBitmap(IDB_LIFEBAR, RGB(255, 255, 255));
		for (vector<gameMap*>::iterator i = maps.begin(); i != maps.end(); i++) (*i)->LoadBitmap();
	}

	int CHero::GetHeroFullHP()
	{
		return FullHP;
	}

	int CHero::GetHeroCurrentHP()
	{
		return CurrentHP;
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
		jumpAnimation1.OnMove();
		if(AttackDelayCount !=0) AttackDelayCount--;

		if (isMovingLeft)
		{
			setHeroDirection("left");   //����V����
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2()-10)) // ��x�y���٨S�I����
				x -= STEP_SIZE;
		}
		if (isMovingRight)
		{
			setHeroDirection("right");   //����V�k��
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2()-10)) // ��y�y���٨S�I����
				x += STEP_SIZE;
		}
		if (isMovingUp && y == (floor))
		{
			rising = true;						// �אּ�W�ɪ��A
		}
		if (rising) {							// �W�ɪ��A
			if (velocity > 0) {
				y -= velocity;					// ���t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				velocity--;						// �����O�v�T�A�U�����W�ɳt�׭��C
				if (!currentMap->isSpace(GetX1(), GetY1()) || !currentMap->isSpace(GetX2(), GetY1()))  // ��x�y�иI��Ѫ�O
				{
					rising = false;
					velocity = 1;
				}
			}
			else {
				rising = false;					// ���t�� <= 0�A�W�ɲפ�A�U���אּ�U��
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
		
		currentMap->SetSXSY(GetCenterX() - SIZE_X / 2, GetCenterY() - SIZE_Y / 2);
		currentMap->OnMove();
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
		if (isMovingUp == false && flag == true)
		{
			jumpAnimation.Reset();
			jumpAnimation1.Reset();
			isMovingUp = true;
		}
	}

	void CHero::SetHeroAttack(bool flag)
	{
		if (AttackDelayCount == 0 && flag == true) {
			swordAttack1.Reset();
			swordAttack.Reset();
			HeroAttackMovement.Reset();
			HeroAttackMovement1.Reset();
			isAttacking = true;
			AttackDelayCount = SetAttackDelayCount;
			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;   //���⤣���䨫�����
		}
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
		LifeBarHead.SetTopLeft(currentMap->ScreenX(x-290), currentMap->ScreenY(y-205));
		LifeBarHead.ShowBitmap();
		int xMove = currentMap->ScreenX(x - 270);
		int yMove = currentMap->ScreenY(y - 205);
		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin() ; i != LifeBarRed.end() ; i++)
		{
			(*i)->SetTopLeft(xMove, yMove);
			(*i)->ShowBitmap();
			xMove++;
		}
		if (isMovingUp)	//���D
		{
			if (faceDirection == "right")
			{
				jumpAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				jumpAnimation.OnShow();
				if (jumpAnimation.IsFinalBitmap())
				{
					isMovingUp = false;
					jumpAnimation.Reset();
				}
			}
			else
			{
				jumpAnimation1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				jumpAnimation1.OnShow();
				if (jumpAnimation1.IsFinalBitmap())
				{
					isMovingUp = false;
					jumpAnimation1.Reset();
				}
			}
			
		}
		else if (isMovingRight)		// �V�k��
		{
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
		}
		else if (isMovingLeft)	// �V����
		{
			moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveLeftAnimation.OnShow();
		}
		else if (isAttacking)
		{
			if (faceDirection == "right")
			{
				HeroAttackMovement.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement.OnShow();
				swordAttack.SetTopLeft(currentMap->ScreenX(x - 40), currentMap->ScreenY(y));
				swordAttack.OnShow();
			}
			else
			{
				HeroAttackMovement1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement1.OnShow();
				swordAttack1.SetTopLeft(currentMap->ScreenX(x - 75), currentMap->ScreenY(y));
				swordAttack1.OnShow();
			}
		}
		else
		{
			if (faceDirection == "right")   // �R��V�k��
			{
				animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				animation.OnShow();
			}
			else							// �R��V����
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
			}
			else
			{
				swordAttack1.SetTopLeft(currentMap->ScreenX(x - 75), currentMap->ScreenY(y));
				swordAttack1.OnShow();
			}

			if (swordAttack.IsFinalBitmap() || swordAttack1.IsFinalBitmap())
			{
				isAttacking = false;
			}
		}
		else
		{
			if (faceDirection == "right")   // �R��V�k��
			{
				sword.SetTopLeft(currentMap->ScreenX(x - 85), currentMap->ScreenY(y + 10));
				sword.OnShow();
			}
			else							// �R��V����
			{
				sword1.SetTopLeft(currentMap->ScreenX(x + 17), currentMap->ScreenY(y + 10));
				sword1.OnShow();
			}
		}
	}

	void CHero::SetHeroHP(int inputHP)
	{
		FullHP = inputHP;
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

		const int INITIAL_VELOCITY = 15;	// ��l�W�ɳt��
		const int FLOOR = 100;				// �a�O�y��
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}
}