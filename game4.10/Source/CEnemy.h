namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的敵人
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	enum CEnemy_Action
	{
		STAND_LEFT,
		MOVE_LEFT,
		ATTACK_LEFT,
		STAND_RIGHT,
		MOVE_RIGHT,
		ATTACK_RIGHT,
		GET_HIT,
		DEAD
	};

	enum Item_Type
	{
		BRONZE_COIN,
		SILVER_COIN,
		GOLDEN_COIN
	};

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer, int x, int y);
		virtual ~CEnemy();
		virtual void LoadBitmap() = 0;				// 載入圖形
		virtual void OnMove() = 0;					// 移動敵人
		virtual void OnShow() = 0;					// 將敵人圖形貼到畫面
		virtual int GetX1() = 0;
		virtual int GetY1() = 0;
		virtual int GetX2() = 0;
		virtual int GetY2() = 0;
		void SetHeroXY(int x1, int x2, int y1, int y2);				// 設定英雄位置
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);	// 設定英雄攻擊範圍
		virtual void GetAttack(const int damage) = 0;		// 被主角攻擊
		virtual void AttackByEnemy(int* heroHP, bool *Poison) = 0;		// 攻擊主角
		virtual bool isDead() = 0;
		virtual string GetEnemyType() = 0;
		void SetEnemyXY(int SetX, int SetY);
	protected:
		int x, y;						// 敵人左上角座標
		gameMap* currentMap;		// 所在地圖
		map<string, int> hero;
		map<string, int> heroAttackRange;
		vector<CMovingBitmap*> LifeBar_1;		// 血條
		CMovingBitmap LifeBar_0;
		int ShowLifeBarDelayCount;
		int enemyHP;
		int FullHP;
	};

	class bullet
	{
	public:
		bullet(gameMap* point, int nx, int ny, int step);
		~bullet();
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
	class bullet_sunFlower : public bullet
	{
	public:
		bullet_sunFlower(gameMap* point, int nx, int ny, int step);
		~bullet_sunFlower();
		void LoadBitmap();				// 載入圖形
	};

	class CEnemy_sunFlower : public CEnemy
	{
	public:
		CEnemy_sunFlower(gameMap* pointer, int x, int y);
		~CEnemy_sunFlower();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Sun_Flower";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		vector<bullet_sunFlower*> allBullet;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
	};
	
	class CEnemy_Cloud : public CEnemy
	{
	public:
		CEnemy_Cloud(gameMap* pointer, int x, int y);
		~CEnemy_Cloud();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation HitAnimation;		// 打擊效果
		CAnimation LightningCloud;
		CAnimation TrackLightningCloud1;
		string EnemyType = "Cloud Boss";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
		int AttackFlag = false;
		int HeroXArray[4];
		void SetHeroXArray(int ArrayNumber, int ArrayValue);
		int HeroXCounter = 0;
		int HeroXTrackCounter = 0;
		int SetHeroXTrackCounter = 120;
		int TrackLightningDelayCount;
		int SetTrackLightningDelayCount = 80;
		void ResetTrackLightningCloudAnimation(int CloudNumber);
		void ShowTrackLightningCloud(int CloudNumber);
		void OnMoveTrackLightningCloud(int CloudNumber);
		bool LightningActivated = false;
		int LightningStrikeDelayCount = 10;
		int ShowLightningCloudNumber = 0;
		bool LightningStrike = false;
		bool AttackAudio_1 = false;
		bool AttackAudio_2 = false;
	};

	class CEnemy_GasRobot : public CEnemy
	{
	public:
		CEnemy_GasRobot(gameMap* pointer, int x, int y);
		~CEnemy_GasRobot();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation GasAnimation;
		CAnimation GasRobotFireRightAnimation;
		CAnimation GasRobotFireLeftAnimation;
		CAnimation HitAnimation;		// 打擊效果
		CAnimation CountDownNumber;
		string EnemyType = "RobotA";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
		bool AttackFlag = false;
		bool ShowGas = false;
		bool AttackAudio = false;
		bool DeadAudio = false;
	};

	class CEnemy_RobotA : public CEnemy
	{
	public:
		CEnemy_RobotA(gameMap* pointer, int x, int y);
		~CEnemy_RobotA();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool * Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		CAnimation AttackVrfxRight;
		CAnimation AttackVrfxLeft;
		string EnemyType = "RobotA";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
		const int DASH_SIZE = 100;
		bool AttackFlag = false;
		bool AttackAudio = false;
		bool DeadAudio = false;
	};

	class CEnemy_Pigeon : public CEnemy
	{
	public:
		CEnemy_Pigeon(gameMap* pointer, int x, int y);
		~CEnemy_Pigeon();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool * Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		CAnimation AttackVrfx;
		string EnemyType = "Pigeon";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
		const int DASH_SIZE = 100;
		bool AttackFlag = false;
		bool recorded = false;
		int RecordedX1;
		int RecordedY1;
		bool FireBall_Audio_1 = false;
		bool FireBall_Audio_2 = false;
	};
	
	class CEnemy_Scorpoin : public CEnemy
	{
	public:
		CEnemy_Scorpoin(gameMap* pointer, int x, int y);
		~CEnemy_Scorpoin();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation HitAnimation;		// 打擊效果
		CAnimation CountDownNumber;
		string EnemyType = "Scorpoin";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		CEnemy_Action DetectHero(CEnemy_Action state);
		int attackDelay;
		int attackDelayCount;
		CEnemy_Action state;
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
		bool AttackFlag = false;
		bool DeadAudio = false;
	};

	class CEnemy_Cactus : public CEnemy
	{
	public:
		CEnemy_Cactus(gameMap* pointer, int x, int y);
		~CEnemy_Cactus();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation AttackAnimation;// 攻擊
		CAnimation DeadAnimation;
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Cactus";
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage;		//敵人攻擊力
		int AttackDelayCount;		// 攻擊頻率
		int SetAttackDelay;
		bool AttackFlag;			// 等於true(發出尖刺動畫出現)時才攻擊到主角
		bool ReadyToAttack;			// 開始攻擊動作
		bool GetHit = false;
		int GetHitDelayCount = 0;
		void changeLifeBarLength();			// 改變生命條的長度
	};


	class CEnemy_Statue : public CEnemy
	{
	public:
		CEnemy_Statue(gameMap* pointer, int x, int y);
		~CEnemy_Statue();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP, bool *Poison);
		bool isDead();
		string GetEnemyType();


	private:
		CMovingBitmap Statue;
		CMovingBitmap Statue_Broken;
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Statue";
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyAttackDamage = 0;		//敵人攻擊力
		int GetHitDelayCount = 0;
		bool GetHit = false;
		void changeLifeBarLength();			// 改變生命條的長度
	};

	
}