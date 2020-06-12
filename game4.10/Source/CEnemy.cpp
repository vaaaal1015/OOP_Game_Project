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
	CEnemy::~CEnemy() 
	{
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete (*i);
	};

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
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(2);
		AttackRightAnimation.SetDelayCount(2);
		HitAnimation.SetDelayCount(2);
		GetHitAnimation.SetDelayCount(3);
		enemyHP = 150;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 70;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}
	
	CEnemy_sunFlower::~CEnemy_sunFlower()
	{
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++) delete *i;
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
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
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			GetHitAnimation.Reset();
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			state = GET_HIT;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_sunFlower::AttackByEnemy(int *heroHP, bool *Poison)
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
	}

	CEnemy_Action CEnemy_sunFlower::DetectHero(CEnemy_Action state)
	{
		if(attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth()/2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}
		
		if (state == GET_HIT)
		{
			return GET_HIT;
		}

		return STAND_LEFT;
	}

	string CEnemy_sunFlower::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_sunFlower::LoadBitmap()
	{
		animation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_7, RGB(255, 255, 255));
		//animation.AddBitmap(IDB_SUNFLOWERNOMOVE_3, RGB(255, 255, 255));

		animationLeft.AddBitmap(IDB_SUNFLOWERLEFTWALK_2, RGB(255, 255, 255));
		animationLeft.AddBitmap(IDB_SUNFLOWERLEFTWALK_7, RGB(255, 255, 255));
		//animationLeft.AddBitmap(IDB_SUNFLOWERNOMOVELEFT_2, RGB(255, 255, 255));


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

		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_0, RGB(255, 255, 255));
		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_1, RGB(255, 255, 255));
		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_2, RGB(255, 255, 255));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));

		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_sunFlower::OnMove()
	{
		const int STEP_SIZE = 2;

		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		AttackLeftAnimation.OnMove();
		AttackRightAnimation.OnMove();
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();
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
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(),GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}

		if (state == ATTACK_LEFT && AttackLeftAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX1(), GetY1() + 40, -5));
			allBullet.back()->LoadBitmap();
		}

		if (state == ATTACK_RIGHT && AttackRightAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX2(), GetY1() + 40, 5));
			allBullet.back()->LoadBitmap();
		}

		if (state == GET_HIT && !GetHitAnimation.IsFinalBitmap() && !HitAnimation.IsFinalBitmap())
		{
			state = GET_HIT;
			GetHitAnimation.OnMove();
			HitAnimation.OnMove();
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
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
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			(*i)->OnShow();
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
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
		case GET_HIT:
			GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			GetHitAnimation.OnShow();
			if (!HitAnimation.IsFinalBitmap())
			{
				HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HitAnimation.OnShow();
			}
			
			//if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
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
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_sunFlower::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_sunFlower::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{
				
				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Cloud: Enemy Cloud class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Cloud::CEnemy_Cloud(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(4);
		AttackRightAnimation.SetDelayCount(4);
		LightningCloud.SetDelayCount(3);
		TrackLightningCloud1.SetDelayCount(3);
		HitAnimation.SetDelayCount(2);
		enemyHP = 1500;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 30;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 150;
		HeroXTrackCounter = SetHeroXTrackCounter;
		TrackLightningDelayCount = SetTrackLightningDelayCount;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
		for (int i = 0; i < 4; i++) HeroXArray[i] = 0;          //初始化array
	}

	CEnemy_Cloud::~CEnemy_Cloud()
	{
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
	}

	int CEnemy_Cloud::GetX1()
	{
		return x;
	}

	int CEnemy_Cloud::GetY1()
	{
		return y;
	}

	int CEnemy_Cloud::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_Cloud::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_Cloud::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_Cloud::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_Cloud::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount == 0)
		{
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_Cloud::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if ((GetX2() + 150 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1() - 100) && AttackFlag)
		{
			*heroHP -= enemyAttackDamage;
		}
		if ((HeroXArray[ShowLightningCloudNumber] + 100 >= hero["x1"]) && (hero["x2"] >= HeroXArray[ShowLightningCloudNumber]) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1() - 100) && LightningStrike)
		{

			*heroHP -= enemyAttackDamage;
		}
	}

	CEnemy_Action CEnemy_Cloud::DetectHero(CEnemy_Action state)
	{
		if (attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth() / 2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
				{
					AttackLeftAnimation.Reset();
					LightningCloud.Reset();
				}
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
				{
					AttackRightAnimation.Reset();
					LightningCloud.Reset();
				}
					
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}

		if (state == GET_HIT)
		{
			return GET_HIT;
		}

		return STAND_LEFT;
	}

	string CEnemy_Cloud::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Cloud::LoadBitmap()
	{
		animation.AddBitmap(IDB_CLOUDBOSS_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_2, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_3, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_4, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_5, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_6, RGB(63, 72, 204));
		animation.AddBitmap(IDB_CLOUDBOSS_7, RGB(63, 72, 204));

		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_0, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_1, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_2, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_3, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_4, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_5, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_6, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_CLOUDBOSS_LEFT_7, RGB(63, 72, 204));


		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_0, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_1, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_2, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_3, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_4, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_5, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_CLOUDWALKRIGHT_6, RGB(63, 72, 204));

		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_0, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_1, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_2, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_3, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_4, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_5, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_CLOUDWALKLEFT_6, RGB(63, 72, 204));

		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_1, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_2, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_3, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_4, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_5, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_6, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_7, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_8, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_CLOUDYATTACKRIGHT_9, RGB(63, 72, 204));

		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_1, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_2, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_3, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_4, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_5, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_6, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_7, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_8, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_CLOUDYATTACKLEFT_9, RGB(63, 72, 204));

		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_2, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_3, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_4, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_CLOUDYDEAD_5, RGB(63, 72, 204));

		LightningCloud.AddBitmap(IDB_LIGHTNING_0, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_1, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_2, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_3, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_4, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_5, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_6, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_8, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_9, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_10, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_11, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_12, RGB(63, 72, 204));
		LightningCloud.AddBitmap(IDB_LIGHTNING_13, RGB(63, 72, 204));

		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_0, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_1, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_2, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_3, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_4, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_5, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_6, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_8, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_9, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_10, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_11, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_12, RGB(63, 72, 204));
		TrackLightningCloud1.AddBitmap(IDB_LIGHTNING_13, RGB(63, 72, 204));



		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));

		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_Cloud::OnMove()
	{
		const int STEP_SIZE = 2;
		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		AttackLeftAnimation.OnMove();
		AttackRightAnimation.OnMove();
		TrackLightningCloud1.OnMove();
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();
		if (attackDelayCount > 0) attackDelayCount--;
		if (TrackLightningDelayCount > 0 && !LightningActivated)		//追蹤閃電計數部分
		{
			//ResetTrackLightningCloudAnimation();
			TrackLightningDelayCount--;
			
			if (TrackLightningDelayCount % 20 == 0)			//每40個畫面寫入陣列
			{
				SetHeroXArray(HeroXCounter, hero["x1"]);		//Array[HeroCounter] = hero[x1]  //紀錄英雄位置
				//TrackLightningCloud1.Reset();
				//OnMoveTrackLightningCloud(HeroXCounter);
				HeroXCounter++;						//changeArrayNumber
			}
			if (TrackLightningDelayCount == 0)
			{
				LightningActivated = true;
				TrackLightningDelayCount = SetTrackLightningDelayCount;
				HeroXCounter = 0;
			}
		}
		else if (LightningActivated)
		{
			if (LightningStrikeDelayCount > 0) LightningStrikeDelayCount--;
		}
		if (LightningCloud.IsFinalBitmap() || LightningCloud.GetCurrentBitmapNumber()==0)			//無法取消攻擊動作
		{
			AttackAudio_1 = false;
			state = DetectHero(state);
		}
		
		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}
		if (state == ATTACK_LEFT || state == ATTACK_RIGHT)
		{
			LightningCloud.OnMove();
			if (LightningCloud.GetCurrentBitmapNumber() >= 7 && !AttackAudio_1)
			{
				CAudio::Instance()->Play(16, false);
				AttackAudio_1 = true;
			}
			if (LightningCloud.GetCurrentBitmapNumber() == 7)
			{
				AttackFlag = true;
			}
			else
			{
				AttackFlag = false;
			}
		}
		
		if (state == GET_HIT && !HitAnimation.IsFinalBitmap())
		{
			state = GET_HIT;
			HitAnimation.OnMove();
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
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

	void CEnemy_Cloud::OnShow()
	{

		if (state != DEAD && LightningActivated && LightningStrikeDelayCount == 0)
		{
			ShowTrackLightningCloud(ShowLightningCloudNumber);
		}
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
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
			if (LightningCloud.GetCurrentBitmapNumber() == 7) attackDelayCount = attackDelay;
			LightningCloud.SetTopLeft(currentMap->ScreenX(x - 200), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x - 100), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x + 100), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x + 200), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (LightningCloud.GetCurrentBitmapNumber() == 7)attackDelayCount = attackDelay;
			LightningCloud.SetTopLeft(currentMap->ScreenX(x - 200), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x - 100), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x + 100), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
			LightningCloud.SetTopLeft(currentMap->ScreenX(x + 200), currentMap->ScreenY(y - 72));
			LightningCloud.OnShow();
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
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_Cloud::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_Cloud::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{
				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
	}

	void CEnemy_Cloud::SetHeroXArray(int ArrayNumber, int ArrayValue)
	{
		HeroXArray[ArrayNumber] = ArrayValue;
	}
	
	void CEnemy_Cloud::ResetTrackLightningCloudAnimation(int CloudNumber)
	{
		switch (CloudNumber)
		{
		case 0:
			TrackLightningCloud1.Reset();
			break;
		default:
			break;
		}
	}

	void CEnemy_Cloud::ShowTrackLightningCloud(int CloudNumber)
	{
		if (!TrackLightningCloud1.IsFinalBitmap())
		{
			if (TrackLightningCloud1.GetCurrentBitmapNumber() >= 7 && !AttackAudio_2)
			{
				CAudio::Instance()->Play(16, false);
				AttackAudio_2 = true;
			}
			if (TrackLightningCloud1.GetCurrentBitmapNumber()==7)
			{
				LightningStrike = true;
			}
			else
			{
				LightningStrike = false;
			}
			TrackLightningCloud1.SetTopLeft(currentMap->ScreenX(HeroXArray[CloudNumber] - 50), currentMap->ScreenY(y - 72));
			TrackLightningCloud1.OnShow();
		}
		else
		{
			AttackAudio_2 = false;
			if (ShowLightningCloudNumber <= 2)
			{
				ShowLightningCloudNumber += 1;
				LightningStrikeDelayCount = 10;
			}
			else
			{
				ShowLightningCloudNumber = 0;
				LightningActivated = false;
			}
			
		}
	}

	void CEnemy_Cloud::OnMoveTrackLightningCloud(int CloudNumber)
	{
		if (CloudNumber > 0)
		{
			TrackLightningCloud1.OnMove();
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_GasRobot: Enemy class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_GasRobot::CEnemy_GasRobot(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		GasRobotFireRightAnimation.SetDelayCount(1);
		GasRobotFireLeftAnimation.SetDelayCount(1);
		GasAnimation.SetDelayCount(2);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(2);
		AttackRightAnimation.SetDelayCount(2);
		CountDownNumber.SetDelayCount(10);
		HitAnimation.SetDelayCount(2);
		enemyHP = 150;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 150;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}

	CEnemy_GasRobot::~CEnemy_GasRobot()
	{
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
	}

	int CEnemy_GasRobot::GetX1()
	{
		return x;
	}

	int CEnemy_GasRobot::GetY1()
	{
		return y;
	}

	int CEnemy_GasRobot::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_GasRobot::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_GasRobot::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_GasRobot::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_GasRobot::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount == 0)
		{
			CAudio::Instance()->Play(13, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			state = GET_HIT;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_GasRobot::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if ((GetX2() + 50 >= hero["x1"]) && (hero["x2"] >= GetX1() - 50) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
		{
			*Poison = true;
		}
	}

	CEnemy_Action CEnemy_GasRobot::DetectHero(CEnemy_Action state)
	{
		if (attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth() / 2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}

		if (state == GET_HIT)
		{
			return GET_HIT;
		}

		return STAND_LEFT;
	}

	string CEnemy_GasRobot::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_GasRobot::LoadBitmap()
	{
		animation.AddBitmap(IDB_GASROBOTSTANDRIGHT_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_GASROBOTSTANDRIGHT_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_GASROBOTSTANDRIGHT_2, RGB(63, 72, 204));
		animation.AddBitmap(IDB_GASROBOTSTANDRIGHT_3, RGB(63, 72, 204));

		animationLeft.AddBitmap(IDB_GASROBOTSTANDLEFT_0, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_GASROBOTSTANDLEFT_1, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_GASROBOTSTANDLEFT_2, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_GASROBOTSTANDLEFT_3, RGB(63, 72, 204));


		moveRightAnimation.AddBitmap(IDB_GASROBOTSTANDRIGHT_0, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_GASROBOTSTANDRIGHT_1, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_GASROBOTSTANDRIGHT_2, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_GASROBOTSTANDRIGHT_3, RGB(63, 72, 204));

		moveLeftAnimation.AddBitmap(IDB_GASROBOTSTANDLEFT_0, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_GASROBOTSTANDLEFT_1, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_GASROBOTSTANDLEFT_2, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_GASROBOTSTANDLEFT_3, RGB(63, 72, 204));

		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_1, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_2, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_3, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_4, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_5, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_6, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_GASROBOTATTACKRIGHT_6, RGB(63, 72, 204));

		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_1, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_2, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_3, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_4, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_5, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_6, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_GASROBOTATTACKLEFT_6, RGB(63, 72, 204));

		DeadAnimation.AddBitmap(IDB_GASROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_GASROBOTDEAD_1, RGB(63, 72, 204));

		GasRobotFireRightAnimation.AddBitmap(IDB_GASROBOTFIRERIGHT_0, RGB(63, 72, 204));
		GasRobotFireRightAnimation.AddBitmap(IDB_GASROBOTFIRERIGHT_1, RGB(63, 72, 204));
		
		GasRobotFireLeftAnimation.AddBitmap(IDB_GASROBOTFIRELEFT_0, RGB(63, 72, 204));
		GasRobotFireLeftAnimation.AddBitmap(IDB_GASROBOTFIRELEFT_1, RGB(63, 72, 204));

		GasAnimation.AddBitmap(IDB_GAS_0, RGB(63, 72, 204));
		GasAnimation.AddBitmap(IDB_GAS_1, RGB(63, 72, 204));
		GasAnimation.AddBitmap(IDB_GAS_2, RGB(63, 72, 204));
		GasAnimation.AddBitmap(IDB_GAS_3, RGB(63, 72, 204));
		GasAnimation.AddBitmap(IDB_GAS_4, RGB(63, 72, 204));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));

		CountDownNumber.AddBitmap(IDB_COUNTDOWN_5, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_4, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_3, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_2, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_1, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_0, RGB(255, 255, 255));
		CountDownNumber.AddBitmap(IDB_COUNTDOWN_0, RGB(255, 255, 255));


		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_GasRobot::OnMove()
	{
		const int STEP_SIZE = 2;
		animation.OnMove();
		animationLeft.OnMove();
		GasRobotFireRightAnimation.OnMove();
		GasRobotFireLeftAnimation.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		AttackLeftAnimation.OnMove();
		AttackRightAnimation.OnMove();
		GasAnimation.OnMove();
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();

		if (attackDelayCount > 0) attackDelayCount--;
		state = DetectHero(state);

		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}

		if (state == GET_HIT  && !HitAnimation.IsFinalBitmap())
		{
			state = GET_HIT;
			HitAnimation.OnMove();
		}

		if (enemyHP <= 0 && !CountDownNumber.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
			CountDownNumber.OnMove();
			if (!DeadAudio)
			{
				CAudio::Instance()->Play(15, false);
				DeadAudio = true;
			}
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

	void CEnemy_GasRobot::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		
		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
			GasRobotFireLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireLeftAnimation.OnShow();
			break;
		case STAND_RIGHT:
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
			GasRobotFireRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireRightAnimation.OnShow();
			break;
		case MOVE_LEFT:
			moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveLeftAnimation.OnShow();
			GasRobotFireLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireLeftAnimation.OnShow();
			break;
		case ATTACK_LEFT:
			if (AttackLeftAnimation.IsFinalBitmap() && !ShowGas)
			{
				ShowGas = true;
				GasAnimation.Reset();
				AttackFlag = true;
			}
			if (AttackLeftAnimation.IsFinalBitmap())
			{
				attackDelayCount = attackDelay;
			}
			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			GasRobotFireLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireLeftAnimation.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			GasRobotFireRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (AttackLeftAnimation.IsFinalBitmap() && !ShowGas)
			{
				ShowGas = true;
				GasAnimation.Reset();
				AttackFlag = true;
			}
			if (AttackRightAnimation.IsFinalBitmap())
			{
				attackDelayCount = attackDelay;
			}
			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			GasRobotFireRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireRightAnimation.OnShow();
			break;
		case GET_HIT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
			GasRobotFireLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 80));
			GasRobotFireLeftAnimation.OnShow();
			if (!HitAnimation.IsFinalBitmap())
			{
				HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HitAnimation.OnShow();
			}
			//if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
			break;
		case DEAD:
			if (enemyHP <= 0)
			{
				if (!CountDownNumber.IsFinalBitmap())
				{
					DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
					DeadAnimation.OnShow();
					CountDownNumber.SetTopLeft(currentMap->ScreenX(x + 70), currentMap->ScreenY(y + 65));
					CountDownNumber.OnShow();
				}
			}
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
		if (ShowGas)
		{
			GasAnimation.SetTopLeft(currentMap->ScreenX(x - 80), currentMap->ScreenY(y - 50));
			GasAnimation.OnShow();
			if (GasAnimation.IsFinalBitmap())
			{
				ShowGas = false;
				AttackFlag = false;
			}
		}
	}

	bool CEnemy_GasRobot::isDead()
	{
		if (enemyHP <= 0 && CountDownNumber.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_GasRobot::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{
				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_RobotA: Enemy class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_RobotA::CEnemy_RobotA(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(3);
		AttackRightAnimation.SetDelayCount(3);
		HitAnimation.SetDelayCount(2);
		GetHitAnimation.SetDelayCount(3);
		AttackVrfxLeft.SetDelayCount(3);
		AttackVrfxRight.SetDelayCount(3);
		enemyHP = 150;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 70;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}

	CEnemy_RobotA::~CEnemy_RobotA()
	{
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
	}

	int CEnemy_RobotA::GetX1()
	{
		return x;
	}

	int CEnemy_RobotA::GetY1()
	{
		return y;
	}

	int CEnemy_RobotA::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_RobotA::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_RobotA::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_RobotA::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_RobotA::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount == 0)
		{
			GetHitAnimation.Reset();
			CAudio::Instance()->Play(13, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_RobotA::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	CEnemy_Action CEnemy_RobotA::DetectHero(CEnemy_Action state)
	{
		if (attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth() / 2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
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

	string CEnemy_RobotA::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_RobotA::LoadBitmap()
	{
		animation.AddBitmap(IDB_ROBOTSTANDRIGHT_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_ROBOTSTANDRIGHT_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_ROBOTSTANDRIGHT_2, RGB(63, 72, 204));

		animationLeft.AddBitmap(IDB_ROBOTSTANDLEFT_0, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_ROBOTSTANDLEFT_1, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_ROBOTSTANDLEFT_2, RGB(63, 72, 204));


		moveRightAnimation.AddBitmap(IDB_ROBOTMOVERIGHT_0, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_ROBOTMOVERIGHT_1, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_ROBOTMOVERIGHT_2, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_ROBOTMOVERIGHT_3, RGB(63, 72, 204));

		moveLeftAnimation.AddBitmap(IDB_ROBOTMOVELEFT_0, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_ROBOTMOVELEFT_1, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_ROBOTMOVELEFT_2, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_ROBOTMOVELEFT_3, RGB(63, 72, 204));

		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_0, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_1, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_2, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_3, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_4, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_5, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_6, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_7, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_8, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_9, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_10, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_11, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_12, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_13, RGB(63, 72, 204));
		AttackRightAnimation.AddBitmap(IDB_ROBOTATTACKRIGHT_13, RGB(63, 72, 204));

		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_0, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_1, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_2, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_3, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_4, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_5, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_6, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_7, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_8, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_9, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_10, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_11, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_12, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_13, RGB(63, 72, 204));
		AttackLeftAnimation.AddBitmap(IDB_ROBOTATTACKLEFT_13, RGB(63, 72, 204));

		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_0, RGB(63, 72, 204));
		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_1, RGB(63, 72, 204));
		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_2, RGB(63, 72, 204));
		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_3, RGB(63, 72, 204));
		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_4, RGB(63, 72, 204));
		AttackVrfxLeft.AddBitmap(IDB_ROBOTATTACKVRFXLEFT_5, RGB(63, 72, 204));

		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_0, RGB(63, 72, 204));
		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_1, RGB(63, 72, 204));
		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_2, RGB(63, 72, 204));
		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_3, RGB(63, 72, 204));
		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_4, RGB(63, 72, 204));
		AttackVrfxRight.AddBitmap(IDB_ROBOTATTACKVRFXRIGHT_5, RGB(63, 72, 204));

		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_ROBOTDEAD_1, RGB(63, 72, 204));

		GetHitAnimation.AddBitmap(IDB_ROBOTSTANDLEFT_0, RGB(63, 72, 204));
		GetHitAnimation.AddBitmap(IDB_ROBOTSTANDLEFT_1, RGB(63, 72, 204));
		GetHitAnimation.AddBitmap(IDB_ROBOTSTANDLEFT_2, RGB(63, 72, 204));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));

	

		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_RobotA::OnMove()
	{
		const int STEP_SIZE = 2;

		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();

		if (attackDelayCount > 0) attackDelayCount--;
		if (AttackLeftAnimation.GetCurrentBitmapNumber() == 0 && AttackRightAnimation.GetCurrentBitmapNumber() == 0)
		{
			state = DetectHero(state);
		}
		
		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}
		if (state == ATTACK_LEFT)
		{
			AttackLeftAnimation.OnMove();
			if (AttackLeftAnimation.GetCurrentBitmapNumber() >= 7)
			{
				if (!AttackAudio)
				{
					CAudio::Instance()->Play(14, false);
					AttackAudio = true;
				}
			}
			if (AttackLeftAnimation.GetCurrentBitmapNumber() >= 9)
			{
				AttackFlag = true;
				if (AttackLeftAnimation.GetCurrentBitmapNumber() == 9)
				{
					for (int i = 0; i < DASH_SIZE; i++) {
						if (currentMap->isSpace(GetX1() - 1, GetY1()) && currentMap->isSpace(GetX1() - 1, GetY2() - 10) && !currentMap->isDoor(GetX1() - 1, GetY1()) && !currentMap->isDoor(GetX1() - 1, GetY2())) // 當x座標還沒碰到牆
						{
							x -= 1;    //正常走
						}
					}
				}
				AttackVrfxLeft.OnMove();
			}
			else
			{
				AttackFlag = false;
				AttackVrfxLeft.Reset();
			}
		}
		
		if (state == ATTACK_RIGHT)
		{
			AttackRightAnimation.OnMove();
			if (AttackRightAnimation.GetCurrentBitmapNumber() >= 7)
			{
				if (!AttackAudio)
				{
					CAudio::Instance()->Play(14, false);
					AttackAudio = true;
				}
			}
			if (AttackRightAnimation.GetCurrentBitmapNumber() >= 9)
			{
				AttackFlag = true;
				if (AttackRightAnimation.GetCurrentBitmapNumber() == 9)
				{
					for (int i = 0; i < DASH_SIZE; i++)
					{
						if (currentMap->isSpace(GetX2() + 1, GetY1()) && currentMap->isSpace(GetX2() + 1, GetY2() - 10) && !currentMap->isDoor(GetX2() + 1, GetY1()) && !currentMap->isDoor(GetX2() + 1, GetY2())) // 當x座標還沒碰到牆
						{
							x += 1;    //正常走
						}
					}
				}
				AttackVrfxRight.OnMove();
			}
			else
			{
				AttackFlag = false;
				AttackVrfxRight.Reset();
			}
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
			if (!DeadAudio)
			{
				CAudio::Instance()->Play(15, false);
				DeadAudio = true;
			}
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

	void CEnemy_RobotA::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
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
			if (AttackLeftAnimation.IsFinalBitmap())
			{
				AttackLeftAnimation.Reset();
				attackDelayCount = attackDelay;
				AttackFlag = false;
				AttackAudio = false;
			}
			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			if (AttackLeftAnimation.GetCurrentBitmapNumber() >= 9)
			{
				AttackVrfxLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 30));
				AttackVrfxLeft.OnShow();
			}
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (AttackRightAnimation.IsFinalBitmap())
			{
				AttackRightAnimation.Reset();
				attackDelayCount = attackDelay;
				AttackFlag = false;
				AttackAudio = false;
			}
			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			if (AttackRightAnimation.GetCurrentBitmapNumber() >= 9)
			{
				AttackVrfxRight.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 30));
				AttackVrfxRight.OnShow();
			}
			break;
		case GET_HIT:
			GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			GetHitAnimation.OnShow();
			if (!HitAnimation.IsFinalBitmap())
			{
				HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HitAnimation.OnShow();
			}

			//if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
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
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_RobotA::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_RobotA::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{

				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Pigeon: Enemy class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Pigeon::CEnemy_Pigeon(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(3);
		AttackRightAnimation.SetDelayCount(3);
		HitAnimation.SetDelayCount(2);
		AttackVrfx.SetDelayCount(3);
		enemyHP = 150;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 70;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		RecordedX1 = 0;
		RecordedY1 = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}

	CEnemy_Pigeon::~CEnemy_Pigeon()
	{
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
	}

	int CEnemy_Pigeon::GetX1()
	{
		return x;
	}

	int CEnemy_Pigeon::GetY1()
	{
		return y;
	}

	int CEnemy_Pigeon::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_Pigeon::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_Pigeon::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_Pigeon::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_Pigeon::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount == 0)
		{
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_Pigeon::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if ((RecordedX1 - 60 + AttackVrfx.Width() >= hero["x1"]) && (hero["x2"] >= RecordedX1 - 60) && (RecordedY1 - 50 + AttackVrfx.Height() >= hero["y1"]) && (hero["y2"] >= RecordedY1 - 50) && AttackFlag)
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	CEnemy_Action CEnemy_Pigeon::DetectHero(CEnemy_Action state)
	{
		if (attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth() / 2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
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

	string CEnemy_Pigeon::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Pigeon::LoadBitmap()
	{
		animation.AddBitmap(IDB_PIGEONSTANDRIGHT_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_PIGEONSTANDRIGHT_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_PIGEONSTANDRIGHT_2, RGB(63, 72, 204));

		animationLeft.AddBitmap(IDB_PIGEONSTANDLEFT_0, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_PIGEONSTANDLEFT_1, RGB(63, 72, 204));
		animationLeft.AddBitmap(IDB_PIGEONSTANDLEFT_2, RGB(63, 72, 204));


		moveRightAnimation.AddBitmap(IDB_PIGEONMOVERIGHT_0, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_PIGEONMOVERIGHT_1, RGB(63, 72, 204));
		moveRightAnimation.AddBitmap(IDB_PIGEONMOVERIGHT_2, RGB(63, 72, 204));

		moveLeftAnimation.AddBitmap(IDB_PIGEONMOVELEFT_0, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_PIGEONMOVELEFT_1, RGB(63, 72, 204));
		moveLeftAnimation.AddBitmap(IDB_PIGEONMOVELEFT_2, RGB(63, 72, 204));

		AttackRightAnimation.AddBitmap(IDB_PIGEONATTACKRIGHT, RGB(63, 72, 204));

		AttackLeftAnimation.AddBitmap(IDB_PIGEONATTACKLEFT, RGB(63, 72, 204));

		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_0, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_1, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_2, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_3, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_4, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_5, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_6, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_7, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_8, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_9, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_10, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_11, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_12, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_13, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_14, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_15, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_16, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_17, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_18, RGB(63, 72, 204));
		AttackVrfx.AddBitmap(IDB_PIGEONATTACKVFX_19, RGB(63, 72, 204));

		DeadAnimation.AddBitmap(IDB_PIGEONDEAD_0, RGB(63, 72, 204));
		DeadAnimation.AddBitmap(IDB_PIGEONDEAD_1, RGB(63, 72, 204));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));



		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_Pigeon::OnMove()
	{
		const int STEP_SIZE = 2;

		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();

		if (attackDelayCount > 0) attackDelayCount--;
		else if (attackDelayCount == 0 && !recorded)		//攻擊前且尚未紀錄主角位置
		{
			RecordedX1 = hero["x1"];
			RecordedY1 = hero["y1"];
			recorded = true;
		}
	
		if (AttackVrfx.GetCurrentBitmapNumber()==0)
		{
			state = DetectHero(state);
		}

		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}
		if (state == ATTACK_LEFT)
		{
			if (!FireBall_Audio_1)
			{
				CAudio::Instance()->Play(17, false);
				FireBall_Audio_1 = true;
			}
			AttackLeftAnimation.OnMove();
			AttackVrfx.OnMove();
			if (AttackVrfx.GetCurrentBitmapNumber() > 9 && AttackVrfx.GetCurrentBitmapNumber() < 15)
			{
				if (!FireBall_Audio_2)
				{
					CAudio::Instance()->Play(18, false);
					FireBall_Audio_2 = true;
				}
				AttackFlag = true;
			}
			else
			{
				AttackFlag = false;
			}

		}

		if (state == ATTACK_RIGHT)
		{
			if (!FireBall_Audio_1)
			{
				CAudio::Instance()->Play(17, false);
				FireBall_Audio_1 = true;
			}
			AttackRightAnimation.OnMove();
			AttackVrfx.OnMove();
			if (AttackVrfx.GetCurrentBitmapNumber() > 9 && AttackVrfx.GetCurrentBitmapNumber() < 15)
			{
				if (!FireBall_Audio_2)
				{
					CAudio::Instance()->Play(18, false);
					FireBall_Audio_2 = true;
				}
				AttackFlag = true;
			}
			else
			{
				AttackFlag = false;
			}
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
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

	void CEnemy_Pigeon::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
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
			if (AttackVrfx.IsFinalBitmap())
			{
				AttackLeftAnimation.Reset();
				AttackVrfx.Reset();
				attackDelayCount = attackDelay;
				AttackFlag = false;
				recorded = false;
				FireBall_Audio_1 = false;
				FireBall_Audio_2 = false;
			}
			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			AttackVrfx.SetTopLeft(currentMap->ScreenX(RecordedX1 - 60), currentMap->ScreenY(RecordedY1 - 50));
			AttackVrfx.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (AttackVrfx.IsFinalBitmap())
			{
				AttackRightAnimation.Reset();
				AttackVrfx.Reset();
				attackDelayCount = attackDelay;
				AttackFlag = false;
				recorded = false;
				FireBall_Audio_1 = false;
				FireBall_Audio_2 = false;
			}
			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			AttackVrfx.SetTopLeft(currentMap->ScreenX(RecordedX1 - 60), currentMap->ScreenY(RecordedY1 - 50));
			AttackVrfx.OnShow();
			break;

			//if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
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
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_Pigeon::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_Pigeon::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{

				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Scorpin: Enemy class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Scorpoin::CEnemy_Scorpoin(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(4);
		AttackRightAnimation.SetDelayCount(4);
		HitAnimation.SetDelayCount(2);
		enemyHP = 150;	//敵人預設生命值
		FullHP = enemyHP;
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 150;
		velocity = initial_velocity;
		state = STAND_LEFT;
		ShowLifeBarDelayCount = 0;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}

	CEnemy_Scorpoin::~CEnemy_Scorpoin()
	{
		//for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) delete *i;
	}

	int CEnemy_Scorpoin::GetX1()
	{
		return x;
	}

	int CEnemy_Scorpoin::GetY1()
	{
		return y;
	}

	int CEnemy_Scorpoin::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_Scorpoin::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_Scorpoin::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_Scorpoin::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_Scorpoin::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount == 0)
		{
			CAudio::Instance()->Play(13, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			ShowLifeBarDelayCount = 150;
		}
	}

	void CEnemy_Scorpoin::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if (state == ATTACK_RIGHT)
		{
			if ((GetX2() + 50 >= hero["x1"]) && (hero["x2"] >= GetX2() - 30) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag_2)
			{
				*Poison = true;
			}
			if ((GetX2() + 50 >= hero["x1"]) && (hero["x2"] >= GetX2() - 30) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
			{
				*heroHP -= enemyAttackDamage;
			}
		}
		if (state == ATTACK_LEFT)
		{
			if ((GetX1() + 30 >= hero["x1"]) && (hero["x2"] >= GetX1() - 50) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag_2)
			{
				*Poison = true;
			}
			if ((GetX1() + 30 >= hero["x1"]) && (hero["x2"] >= GetX1() - 50) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
			{
				*heroHP -= enemyAttackDamage;
			}
		}
	}

	CEnemy_Action CEnemy_Scorpoin::DetectHero(CEnemy_Action state)
	{
		if (attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth() / 2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}

		if (state == GET_HIT)
		{
			return GET_HIT;
		}

		return STAND_LEFT;
	}

	string CEnemy_Scorpoin::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Scorpoin::LoadBitmap()
	{
		animation.AddBitmap(IDB_SCORPOINSTANDRIGHT_0, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SCORPOINSTANDRIGHT_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SCORPOINSTANDRIGHT_2, RGB(255, 255, 255));

		animationLeft.AddBitmap(IDB_SCORPOINSTANDLEFT_0, RGB(255, 255, 255));
		animationLeft.AddBitmap(IDB_SCORPOINSTANDLEFT_1, RGB(255, 255, 255));
		animationLeft.AddBitmap(IDB_SCORPOINSTANDLEFT_2, RGB(255, 255, 255));


		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_0, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_5, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_6, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SCORPOINMOVERIGHT_7, RGB(255, 255, 255));

		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_0, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_5, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_6, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SCORPOINMOVELEFT_7, RGB(255, 255, 255));


		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_0, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_1, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_2, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_3, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_4, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_5, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_6, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_7, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_8, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_9, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_10, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_11, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_12, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_13, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_14, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_15, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SCORPOINATTAACKRIGHT_15, RGB(255, 255, 255));

		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_0, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_1, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_2, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_3, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_4, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_5, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_6, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_7, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_8, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_9, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_10, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_11, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_12, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_13, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_14, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_15, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SCORPOINATTAACKLEFT_15, RGB(255, 255, 255));

		DeadAnimation.AddBitmap(IDB_SCORPOINDEAD_0, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SCORPOINDEAD_1, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SCORPOINDEAD_2, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SCORPOINDEAD_3, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SCORPOINDEAD_4, RGB(255, 255, 255));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_Scorpoin::OnMove()
	{
		const int STEP_SIZE = 2;
		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();

		if (attackDelayCount > 0) attackDelayCount--;

		if (AttackRightAnimation.GetCurrentBitmapNumber() == 0 && AttackLeftAnimation.GetCurrentBitmapNumber() == 0)		//攻擊中無法取消攻擊動作
		{
			state = DetectHero(state);
		}
		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10) && !currentMap->isDoor(GetX1(), GetY1()) && !currentMap->isDoor(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}

		if (state == ATTACK_RIGHT)
		{
			AttackRightAnimation.OnMove();
			if (AttackLeftAnimation.GetCurrentBitmapNumber() == 5 || AttackLeftAnimation.GetCurrentBitmapNumber() == 6)
			{
				AttackFlag = true;
			}
			else if (AttackLeftAnimation.GetCurrentBitmapNumber() == 11 || AttackLeftAnimation.GetCurrentBitmapNumber() == 12)
			{
				AttackFlag_2 = true;
			}
			else
			{
				AttackFlag = false;
				AttackFlag_2 = false;
			}
			if (AttackRightAnimation.IsFinalBitmap())
			{
				attackDelayCount = attackDelay;
				AttackRightAnimation.Reset();
			}
		}
		
		if (state == ATTACK_LEFT)
		{
			AttackLeftAnimation.OnMove();
			if (AttackLeftAnimation.GetCurrentBitmapNumber() == 5 || AttackLeftAnimation.GetCurrentBitmapNumber() == 6)
			{
				AttackFlag = true;
			}
			else if (AttackLeftAnimation.GetCurrentBitmapNumber() == 11 || AttackLeftAnimation.GetCurrentBitmapNumber() == 12)
			{
				AttackFlag_2 = true;
			}
			else
			{
				AttackFlag = false;
				AttackFlag_2 = false;
			}
			if (AttackLeftAnimation.IsFinalBitmap())
			{
				attackDelayCount = attackDelay;
				AttackLeftAnimation.Reset();
			}
		}


		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
			if (!DeadAudio)
			{
				DeadAudio = true;
			}
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

	void CEnemy_Scorpoin::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}

		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
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
			
			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			
			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			break;;
		case DEAD:
			if (enemyHP <= 0)
			{
				if (!DeadAnimation.IsFinalBitmap())
				{
					DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
					DeadAnimation.OnShow();
				}
			}
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_Scorpoin::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}

	void CEnemy_Scorpoin::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{
				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
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
		AttackDelayCount = 0;					// 設定攻擊頻率
		AttackFlag = false;		
		ReadyToAttack = false;
		enemyAttackDamage = 40;
		AttackAnimation.SetDelayCount(4);
		DeadAnimation.SetDelayCount(3);
		animation.SetDelayCount(5);
		GetHitAnimation.SetDelayCount(3);
		HitAnimation.SetDelayCount(3);
		ShowLifeBarDelayCount = 0;
		enemyHP = 300;
		FullHP = enemyHP;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
	}

	CEnemy_Cactus::~CEnemy_Cactus(){}

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
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			GetHitAnimation.Reset();
			HitAnimation.Reset();
			CAudio::Instance()->Play(9, false);
			enemyHP -= damage;
			GetHit = true;
			GetHitDelayCount = 15;
			ShowLifeBarDelayCount = 150;
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

		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_0, RGB(255, 127, 39));
		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_1, RGB(255, 127, 39));
		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_2, RGB(255, 127, 39));
	
		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_3, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_4, RGB(63, 72, 204));

		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
	}

	void CEnemy_Cactus::OnMove()
	{
		const int STEP_SIZE = 0;
		if (GetHitDelayCount > 0)GetHitDelayCount--;
		if (AttackDelayCount > 0) AttackDelayCount--;
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		AttackAnimation.OnMove();
		GetHitAnimation.OnMove();
		HitAnimation.OnMove();
		animation.OnMove();
		if (AttackAnimation.IsFinalBitmap()) ReadyToAttack = false;
		if (ReadyToAttack && AttackDelayCount==0)
		{
			AttackAnimation.Reset();
			AttackDelayCount = 150;
		}
		//if(GetHit) GetHitAnimation.Reset();
		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();

	}

	void CEnemy_Cactus::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
		if (enemyHP <= 0)
		{
			if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}
		}
		else if (ReadyToAttack)
		{
			AttackAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackAnimation.OnShow();
			if (AttackAnimation.GetCurrentBitmapNumber() == 4)
			{
				CAudio::Instance()->Play(10, false);
				AttackFlag = true;   //發出尖刺時攻擊
			}
			else AttackFlag = false;
		}
		else if (GetHit)
		{

			GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			GetHitAnimation.OnShow();
			HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			HitAnimation.OnShow();
			if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
			if (HitAnimation.IsFinalBitmap()) GetHit = false;
		}
		else
		{
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
		}
	}

	void CEnemy_Cactus::AttackByEnemy(int *heroHP, bool *Poison)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) &&(AttackDelayCount==0))
		{
			ReadyToAttack = true;
		}
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	bool CEnemy_Cactus::isDead()
	{
		if (enemyHP <= 0) return true;
		else return false;
	}

	void CEnemy_Cactus::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{

				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
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
		GetHitDelayCount = 0;
		HitAnimation.SetDelayCount(3);
		ShowLifeBarDelayCount = 0;
		enemyHP = 500;
		FullHP = enemyHP;
		for (int i = 0; i < 100; i++) LifeBar_1.push_back(new CMovingBitmap);    //100個血條圖片
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
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			HitAnimation.Reset();
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			GetHit = true;
			enemyHP -= FullHP / 3;
			ShowLifeBarDelayCount = 150;
		}
	}


	void CEnemy_Statue::LoadBitmap()
	{
		Statue.LoadBitmap(IDB_STATUE, RGB(255, 0, 0));
		Statue_Broken.LoadBitmap(IDB_STATUE_BROKEN, RGB(255, 0, 0));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_3, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_4, RGB(63, 72, 204));
		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++) (*i)->LoadBitmap(IDB_ENEMYLIFEBAR_0);
		LifeBar_0.LoadBitmap(IDB_ENEMYLIFEBAR_LONG);
	}

	void CEnemy_Statue::OnMove()
	{
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		if (ShowLifeBarDelayCount > 0) ShowLifeBarDelayCount--;
		HitAnimation.OnMove();
	}

	void CEnemy_Statue::OnShow()
	{
		if (ShowLifeBarDelayCount != 0)
		{
			LifeBar_0.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y - 5));
			LifeBar_0.ShowBitmap();
			changeLifeBarLength();
		}
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
		if (GetHit)
		{
			HitAnimation.SetTopLeft(currentMap->ScreenX(x + 5), currentMap->ScreenY(y + 50));
			HitAnimation.OnShow();
			if (HitAnimation.IsFinalBitmap()) GetHit = false;
		}
	}

	void CEnemy_Statue::AttackByEnemy(int *heroHP, bool *Poison)
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
	
	void CEnemy_Statue::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x);
		int yMove = currentMap->ScreenY(y - 3);
		int counter = 0;
		float lengthOfLifeBar = ((float)enemyHP / (float)FullHP) * 100;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBar_1.begin(); i != LifeBar_1.end(); i++)
		{
			if (xMove + counter < xMove + lengthOfLifeBar)
			{
				(*i)->SetTopLeft(xMove + counter, yMove);
				(*i)->ShowBitmap();
			}
			counter += 1;
		}
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

		if (!currentMap->isSpace(GetX1(), GetY1()))  // 當x座標碰到牆
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