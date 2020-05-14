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
		const int INITIAL_VELOCITY = 15;		// ��l�W�ɳt��
		const int FLOOR = 100;					// �a�O�y��
		isMovingRight = true;
		rising = false;

		animation.SetDelayCount(2);
		moveRightAnimation.SetDelayCount(2);
		DeadAnimation.SetDelayCount(2);
		AttackLeftAnimation.SetDelayCount(2);
		AttackRightAnimation.SetDelayCount(2);
		moveingStep = 20;
		enemyHP = 100;							//�ĤH�w�]�ͩR��
		enemyAttackDamage = 10;					//�ĤH�w�]�����O
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 40;
		velocity = initial_velocity;
		state = STAND_LEFT;
	}
	
	CEnemy_sunFlower::~CEnemy_sunFlower()
	{
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			delete *i;
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
		vector<bullet_sunFlower*>::iterator iter = allBullet.begin();
		while (iter != allBullet.end())
		{
			if (((*iter)->GetX2() >= hero["x1"]) && (hero["x2"] >= (*iter)->GetX1()) && ((*iter)->GetY2() >= hero["y1"]) && (hero["y2"] >= (*iter)->GetY1()))
			{
				*heroHP -= enemyAttackDamage;
				delete *iter;
				iter = allBullet.erase(iter);
			}
			else
				iter++;
		}

		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			//*heroHP -= enemyAttackDamage;
		}
	}

	ACTION CEnemy_sunFlower::DetectHero(ACTION state)
	{
		if(attackDelayCount <= 0)
		{
			if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1() - 130) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 130 >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
			if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1() - 230) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				return MOVE_LEFT;
			}
			if ((GetX2() + 230 >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				return MOVE_RIGHT;
			}
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}
		
		return STAND_LEFT;
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

		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_0, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_1, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_2, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_3, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
	}

	void CEnemy_sunFlower::OnMove()
	{
		const int STEP_SIZE = 2;

		animation.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		AttackLeftAnimation.OnMove();
		AttackRightAnimation.OnMove();

		vector<bullet_sunFlower*>::iterator iter = allBullet.begin();
		while (iter != allBullet.end())
		{
			if ((*iter)->isDelet())
			{
				delete *iter;
				iter = allBullet.erase(iter);
			}
			else
				iter++;
		}

		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			(*i)->OnMove();

		if (attackDelayCount > 0) attackDelayCount--;

		state = DetectHero(state);

		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // ��y�y���٨S�I����
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // ��y�y���٨S�I����
				x += STEP_SIZE;
		}

		if (state == ATTACK_LEFT && AttackLeftAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX1(), GetY1() + 40, -3));
			allBullet.back()->LoadBitmap();
		}

		if (state == ATTACK_RIGHT && AttackRightAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX2(), GetY1() + 40, 3));
			allBullet.back()->LoadBitmap();
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
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
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			(*i)->OnShow();

		switch (state)
		{
		case STAND_LEFT:
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
			break;
		case STAND_RIGHT:
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
			break;
		case MOVE_LEFT:
			moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveLeftAnimation.OnShow();
			break;
		case ATTACK_LEFT:
			if(AttackLeftAnimation.IsFinalBitmap()) attackDelayCount = attackDelay;

			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (AttackRightAnimation.IsFinalBitmap())attackDelayCount = attackDelay;

			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			break;
		case DEAD:
			if (enemyHP <= 0)
			{
				if (!DeadAnimation.IsFinalBitmap())
				{
					DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
					DeadAnimation.OnShow();
				}
			}
		}

	}

	bool CEnemy_sunFlower::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Statue: Enemy Statue class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Statue::CEnemy_Statue(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// ��l�W�ɳt��
		const int FLOOR = 100;					// �a�O�y��
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

	/////////////////////////////////////////////////////////////////////////////
	// bullet: bullet base class
	/////////////////////////////////////////////////////////////////////////////
	bullet::bullet(gameMap* point,int nx, int ny, int step)
	{
		x = nx;
		y = ny;
		currentMap = point;
		distance = 0;
		STEP_SIZE = step;
		animation.SetDelayCount(2);
	}

	bullet::~bullet() {}

	int bullet::GetX1()
	{
		return x;
	}

	int bullet::GetY1()
	{
		return y;
	}

	int bullet::GetX2()
	{
		return x + animation.Width();
	}

	int bullet::GetY2()
	{
		return y + animation.Height();
	}

	bool bullet::isDelet()
	{
		if (distance > 500)
		{
			return true;
		}

		if (!currentMap->isSpace(GetX1(), GetY1()))  // ��x�y�иI����
		{
			return true;
		}

		return false;
	}

	void bullet::OnMove()
	{
		animation.OnMove();

		x += STEP_SIZE;

		if (STEP_SIZE > 0)
		{
			distance += STEP_SIZE;
		}
		else
		{
			distance -= STEP_SIZE;
		}
	}
	void bullet::OnShow()
	{
		animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		animation.OnShow();
	}

	/////////////////////////////////////////////////////////////////////////////
	// bullet_sunFlower : bullet_sunFlower class
	/////////////////////////////////////////////////////////////////////////////
	bullet_sunFlower::bullet_sunFlower(gameMap* point, int nx, int ny, int step) : bullet(point, nx, ny, step) {}

	bullet_sunFlower::~bullet_sunFlower() {}

	void bullet_sunFlower::LoadBitmap()
	{
		animation.AddBitmap(IDB_SUNFLOWERBULLET_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_2, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_3, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_4, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_5, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_6, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_7, RGB(63, 72, 204));
	}
}