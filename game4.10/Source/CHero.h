namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的英雄
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	enum Hero_Action
	{
		STAND,
		WALK,
		RUN,
		ROLL,
		ATTACK,
		TALK,
		THROW
	};
	
	enum Hero_Direction
	{
		LEFT, RIGHT
	};


	class HeroBullet
	{
	public:
		HeroBullet(gameMap* point, int nx, int ny, int step);
		~HeroBullet();
		int GetX1();					// 子彈左上角 x 座標
		int GetY1();					// 子彈左上角 y 座標
		int GetX2();					// 子彈右下角 x 座標
		int GetY2();					// 子彈右下角 y 座標
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		bool isDelet();
	protected:
		CAnimation animation;
		int x;
		int y;
		gameMap *currentMap;
		int distance;
		int STEP_SIZE;
	};

	class Shurikan : public HeroBullet
	{
	public:
		Shurikan(gameMap* point, int nx, int ny, int step);
		~Shurikan();
		void LoadBitmap();				// 載入圖形
	};

	class CHero
	{
	public:
		CHero();
		~CHero();
		int  GetX1();					// 英雄左上角 x 座標
		int  GetY1();					// 英雄左上角 y 座標
		int  GetX2();					// 英雄右下角 x 座標
		int  GetY2();					// 英雄右下角 y 座標
		int  GetWidth();				// 英雄的寬
		int  GetHeight();				// 英雄的高
		int	 GetCenterX();				// 英雄正中央的 x 座標
		int	 GetCenterY();				// 英雄正中央的 y 座標
		void Initialize();				// 設定英雄為初始值
		void LoadBitmap();				// 載入圖形		
		void OnMove();					// 移動英雄
		void OnShow();					// 將英雄圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetTalkingToNPC(bool flag);// 和NPC對話
		void SetHeroRoll(bool flag);	// 翻滾
		void SetHeroAttack(bool flag);	// 設定是否正在往上移動
		void SetHeroThrow(bool flag);
		void SetXY(int nx, int ny);		// 設定英雄左上角座標
		void SetHeroHP(int inputHP);	// 設定主角HP值
		bool isAttacking;				// 正在攻擊
		int GetHeroFullHP();
		int GetHeroCurrentHP();      
		int AttackByEnemy();
		int HeroGetItem();
		void SetPreviousMove(int Movement);
		int GetPreviousMove();          //取得上一個動作
		void SetMoveDelayCount(int delay);
		void ShowNumber(int colot, int Number, int x, int y);//color:1:black,2:red
		void HeroLevelUp();             // 升級
		void SetEndTalking();            // 關閉與NPC對話
		bool isTalkingToNPC;			// 與NPC對話
		int HeroLevel = 1;
		void SelectMap(int MapNumber);
		void ResetHeroState();      //重置主角狀態(回到城鎮)
		bool ClearedStage = false;  //通關結算
		void OnLButtonDown(int Mx, int My);		//按下滑鼠左鍵
		bool GetHeroIsRolling();

	protected:
		CAnimation animation;			// 英雄的動畫(向右)
		CAnimation animation1;			// 英雄的動畫(向左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation jumpAnimation;		// 跳躍動畫(向右)
		CAnimation jumpAnimation1;		// 跳躍動畫(向左)
		CAnimation HeroAttackMovement;  // 英雄攻擊動畫(向右)
		CAnimation HeroAttackMovement1;	// 英雄攻擊動畫(向左)
		CAnimation sword;				// 載入劍的動畫(向右)
		CAnimation sword1;				// 載入劍的動畫(向左)
		CAnimation swordAttack;			// 劍的攻擊動畫(向右)
		CAnimation swordAttack1;		// 劍的攻擊動畫(向左)
		CAnimation HeroDashLeft;		// 向左衝刺
		CAnimation SwordDashLeft;
		CAnimation HeroDashRight;		// 向右衝刺
		CAnimation SwordDashRight;
		CAnimation HeroRollLeft;		// 向左翻滾
		CAnimation SwordRollLeft;
		CAnimation HeroRollRight;		// 向右翻滾
		CAnimation SwordRollRight;
		CAnimation ThrowingLeft;
		CAnimation ThrowingRight;
		CAnimation FireSwordRightAnimation;
		CAnimation FireSwordLeftAnimation;
		CAnimation FireCircle;
		CAnimation Fire1;
		CAnimation Fire2;
		CAnimation Fire3;
		CAnimation gain_life;
		CMovingBitmap LifeBarHead;		// 血條
		CMovingBitmap StartGameBar;		// 開始按鈕
		CMovingBitmap WorldMap_UI_1;	// 地圖_1
		CMovingBitmap White;
		CMovingBitmap ShurikanUI;
		CInteger Num;					// 黑色數字
		CInteger_Red Num_Red;			// 紅色數字
		CInteger_Gold Num_Gold;			// 黃色數字
		CMovingBitmap Word_G;
		CMovingBitmap Word_Gold;
		CMovingBitmap Shield;
		CMovingBitmap GainLifeUI;
		//CInteger DamageTaken;			// 顯示承受傷害
		vector<CMovingBitmap*> LifeBarRed;		// 血條
		int x, y;						// 英雄左上角座標
		int heroAttackDamage;			// 英雄攻擊力
		bool isMovingDown;				// 是否正在往下移動
		bool isMovingLeft;				// 是否正在往左移動
		bool isMovingRight;				// 是否正在往右移動
		bool isMovingUp;				// 是否正在往上移動
		bool isRolling;					// 是否正在翻滾
		bool isInvincible;				// 是否為無敵(無法被攻擊)
		bool isThrowing;
		//string faceDirection;			// 人物面對的方向
		int Gold;						// 金幣
		int floor;						// 地板的Y座標
		bool rising;					// true表上升、false表下降
		int initial_velocity;			// 初始速度
		int velocity;					// 目前的速度(點/次)
		int SetAttackDelayCount;		// 設定攻擊延遲時間
		int AttackDelayCount;			// 攻擊延遲時間
		int RollDelayCount;				// 翻滾延遲時間
		int MoveDelayCount;				// 上個動作保存時間
		int GainLifeDelayCount;			// 顯示回血特效長度
		int PreviousMovement;			// 紀錄上一個動作  0:無動作; 1:向左走; 2:向右走
		int DashColdDown;				// 衝刺冷卻時間
		int InvincibleDelayCount;		// 無敵時間
		int ShowGoldDelayCount;			// 金幣出現的時間
		int GainHealthDelayCount;		// 取得紅石後的回血時間長度
		int bleed = 0;					// 偵測主角有無扣血
		int GetGold = 0;				// 得到的金幣
		int SpecialEffect = 0;			// 0:無效果;1:火焰;2:持續回血
		int SpecialEffectCount = 0;		// 偵測特殊效果
		
	private:
		//Counter AttackDelayCounter;		// 攻擊延遲時間

		gameMap_village* currentVillage;	// 紀錄目前的村莊
		gameMap_wild* currentWild;			// 紀錄目前的野外
		gameMap* currentMap;				// 紀錄目前的地圖
		CMovingBitmap BlackMask;			// 黑色遮罩
		CMovingBitmap QuitButton;			// 離開按鈕
		vector<Shurikan*> allShurikan;
		int FullHP;							// 主角總血量
		int CurrentHP;						// 主角目前的血量
		void changeLifeBarLength();			// 改變生命條的長度
		bool isInHome;						// 是否在村莊
		bool isSelectingMap;				// 是否正在選擇地圖
		int AttackRange;
		Hero_Action heroActoin;				// 英雄動作
		Hero_Direction heroDirection;		// 英雄面對的方向
		void setHeroAction();		// 設定英雄動作
		void WalkOnMove();
		void RollOnMove();
		void StandOnMove();
		void WalkOnShow();
		void RollOnShow();
		void StandOnShow();
		void AttackOnMove();
		void AttackOnShow();
		void ThrowOnMove();
		void ThrowOnShow();
		void RunOnMove();
		void RunOnShow();
		void TalkOnMove();
		//void TalkOnShow();
		CAnimation currentAnimation;	// 目前的英雄動畫
		bool HasFireStone = false;
		int ShurikanNumber = 1;
	};

}