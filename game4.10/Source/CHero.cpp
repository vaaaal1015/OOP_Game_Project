#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "NPC.h"
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
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = isRolling = isInvincible = false;
		PreviousMovement = 0;			//紀錄上一個動作
		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		animation.SetDelayCount(3);
		animation1.SetDelayCount(3);
		sword.SetDelayCount(3);
		HeroDashLeft.SetDelayCount(3);
		HeroDashRight.SetDelayCount(3);
		swordAttack.SetDelayCount(3);
		swordAttack1.SetDelayCount(3);
		HeroAttackMovement.SetDelayCount(3);
		HeroAttackMovement1.SetDelayCount(3);
		sword1.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(4);
		jumpAnimation1.SetDelayCount(4);
		moveLeftAnimation.SetDelayCount(3);
		HeroRollLeft.SetDelayCount(2);
		HeroRollRight.SetDelayCount(2);
		SetAttackDelayCount = AttackDelayCount = DashColdDown = RollDelayCount = 15;
		InvincibleDelayCount = 30;
		MoveDelayCount = 10;
		FullHP = 100;						// 主角預設血量為100
		CurrentHP = FullHP;
		heroAttackDamage = 5;				// 主角預設攻擊力為5
		AttackRange = 100;					// 主角攻擊範圍
		maps.push_back(new gameMap("Home.txt"));
		maps.push_back(new gameMap("level_1.txt"));
		maps.push_back(new gameMap("level_2.txt"));
		
		SetMap(0);

		maps[0]->SetEnemyPosition(0, 600, 600);
		maps[0]->SetNPCPosition(0, 200, 600);
		if (LifeBarRed.size() == 0)
		{
			for (int i = 0; i < 100; i++)
			{
				LifeBarRed.push_back(new CMovingBitmap);    //50個血條圖片
			}
		}
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

		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_1, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_2, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_3, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_4, RGB(255, 255, 255));

		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_1, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_2, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_3, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_4, RGB(255, 255, 255));

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

		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_1, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_2, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_3, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_4, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_5, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_6, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_6, RGB(255, 255, 255));


		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_1, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_2, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_3, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_4, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_5, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_6, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_6, RGB(255, 255, 255));

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
		Health.LoadBitmap();
		//DamageTaken.LoadBitmap();
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

	int CHero::GetPreviousMove()
	{
		return PreviousMovement;
	}

	void CHero::OnMove()
	{
		const int STEP_SIZE = 10;
		Health.SetInteger(CurrentHP);
		animation.OnMove();
		animation1.OnMove();
		sword.OnMove();
		sword1.OnMove();
		HeroAttackMovement.OnMove();
		HeroAttackMovement1.OnMove();
		swordAttack.OnMove();
		swordAttack1.OnMove();
		moveRightAnimation.OnMove();
		HeroDashLeft.OnMove();
		HeroDashRight.OnMove();
		moveLeftAnimation.OnMove();
		jumpAnimation.OnMove();
		jumpAnimation1.OnMove();
		HeroRollLeft.OnMove();
		HeroRollRight.OnMove();

		if(AttackDelayCount !=0) AttackDelayCount--;    //攻速
		if (RollDelayCount != 0) RollDelayCount--;		//翻滾
		if (InvincibleDelayCount != 0) InvincibleDelayCount--;  //無敵時間
		if (DashColdDown != 0) DashColdDown--;      //衝刺
		if (MoveDelayCount != 0) MoveDelayCount--;   //紀錄上個動作的保持時間
		if (MoveDelayCount == 0) SetPreviousMove(0);  //抹除上個動作紀錄
		if (InvincibleDelayCount == 0) isInvincible = false;

		if (isMovingLeft)
		{
			setHeroDirection("left");   //角色向左看
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10)) // 當x座標還沒碰到牆
			{
				if (PreviousMovement == 1 && DashColdDown==0)
				{
					x -= 80;    //衝刺距離
					DashColdDown = 15;    // 衝刺冷卻時間
					HeroDashLeft.Reset();  //重置動畫
				}
				else
				{
					x -= STEP_SIZE;    //正常走
				}
			}
		}
		if (isMovingRight)
		{
			setHeroDirection("right");   //角色向右看
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2()-10)) // 當y座標還沒碰到牆
			{
				if (PreviousMovement == 2 && DashColdDown == 0)
				{
					x += 80;     //衝刺距離
					DashColdDown = 15;    // 衝刺冷卻時間
					HeroDashRight.Reset();   //重置動畫
				}
				else
				{
					x += STEP_SIZE;    //正常走
				}
			}
		}
		if (isRolling)
		{
			if (faceDirection == "right")
			{
				if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當y座標還沒碰到牆
				{
					x += 15;
				}
				if (HeroRollRight.IsFinalBitmap()) isRolling = false;
			}
			else
			{
				if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當y座標還沒碰到牆
				{
					x -= 15;
				}
				if (HeroRollRight.IsFinalBitmap()) isRolling = false;
			}
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

		if (isAttacking)
		{
			if (faceDirection == "right") currentMap->AttackByHero(GetX2(), GetX2() + swordAttack.Width(), GetY1(), GetY1() + swordAttack.Height(), heroAttackDamage);
			else currentMap->AttackByHero(GetX1(), GetX1() + swordAttack1.Width(), GetY1(), GetY1() + swordAttack1.Height(), heroAttackDamage);
		}
		if(!isRolling && !isInvincible) AttackByEnemy();
		TouchNPC();
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
			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isRolling = false;   //角色不能邊走邊攻擊
		}
	}
	void CHero::SetHeroRoll (bool flag)
	{
		if (RollDelayCount == 0 && flag == true) {
			HeroRollLeft.Reset();
			HeroRollRight.Reset();
			isRolling = true;
			RollDelayCount = 15;
			
			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = false;   //角色不能邊走邊攻擊
		}
	}
	void CHero::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}
	
	void CHero::SetPreviousMove(int move)
	{
		PreviousMovement = move;
	}

	void CHero::SetMoveDelayCount(int delay)
	{
		MoveDelayCount = delay;
	}

	void CHero::setHeroDirection(string direction)
	{
		faceDirection = direction;
	}


	void CHero::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x - 250);
		int yMove = currentMap->ScreenY(y - 201);
		float lengthOfLifeBar = ((float)CurrentHP / (float)FullHP) * 100;  //重新計算血條長度
		if ((lengthOfLifeBar < LifeBarRed.size()) && (LifeBarRed.size() != 0))       //血條長度大於實際血量比例
		{
			TRACE("%f,%d\n", lengthOfLifeBar, LifeBarRed.size());
			for (int i = LifeBarRed.size(); i > lengthOfLifeBar; i--)
			{
				LifeBarRed.pop_back();   //血條剪短
			}
		}

		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++)
		{
			(*i)->SetTopLeft(xMove, yMove);
			(*i)->ShowBitmap();
			xMove += 3;
		}
	}
	void CHero::OnShow()
	{
		currentMap->OnShow();
		LifeBarHead.SetTopLeft(currentMap->ScreenX(x-290), currentMap->ScreenY(y-205));
		LifeBarHead.ShowBitmap();  //顯示血條
		Health.SetTopLeft(currentMap->ScreenX(x - 280), currentMap->ScreenY(y - 180));  
		changeLifeBarLength();
		Health.ShowBitmap();     // 顯示生命值
		/*if (AttackByEnemy() != 0)
		{
			DamageTaken.SetInteger(AttackByEnemy());
			DamageTaken.ShowBitmap();
		}*/
		if (isMovingUp)	// 往上跳
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
		else if (isMovingRight)		// 向右走
		{
			if (PreviousMovement != 2 && DashColdDown == 0)
			{
				moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				moveRightAnimation.OnShow();
			}
			else   //右衝刺
			{
				HeroDashRight.SetTopLeft(currentMap->ScreenX(x - 50), currentMap->ScreenY(y));
				HeroDashRight.OnShow();
				if(HeroDashRight.IsFinalBitmap())
				{
					DashColdDown = 0;    //動畫結束
				}
			}
		}
		else if (isMovingLeft)	// 向左走
		{
			if (PreviousMovement != 1 && DashColdDown==0)
			{
				moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				moveLeftAnimation.OnShow();
			}
			else     //左衝刺
			{
				HeroDashLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroDashLeft.OnShow();
				if (HeroDashLeft.IsFinalBitmap())
				{
					DashColdDown = 0;
				}
			}
		}
		else if (isRolling)
		{
			if (faceDirection == "right")
			{
				HeroRollRight.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroRollRight.OnShow();
			}
			else
			{
				HeroRollLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroRollLeft.OnShow();
			}
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
			if (faceDirection == "right")   // 靜止向右看
			{
				sword.SetTopLeft(currentMap->ScreenX(x - 85), currentMap->ScreenY(y + 10));
				sword.OnShow();
			}
			else							// 靜止向左看
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

		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

	int CHero::AttackByEnemy()
	{
		int damageFromLeft = 0;
		int damageFromRight = 0;

		damageFromLeft = currentMap->AttackByEnemy(GetX1(), GetX1(), GetY1(), GetY2());
		damageFromRight = currentMap->AttackByEnemy(GetX2(), GetX2(), GetY1(), GetY2());

		if (damageFromLeft != 0)
		{
			x += 30;
			CurrentHP -= damageFromLeft;
			isInvincible = true;
			InvincibleDelayCount = 30;
			return damageFromLeft;
		}
		if (damageFromRight != 0)
		{
			x -= 30;
			CurrentHP -= damageFromRight;
			isInvincible = true;
			InvincibleDelayCount = 30;
			return damageFromRight;
		}
		return 0;
	}
	int CHero::TouchNPC()
	{
		int NPCNum = -1;
		NPCNum = currentMap->HeroTouchNPC(GetX1(), GetX2(), GetY1(), GetY2());
		return NPCNum;
	}
}