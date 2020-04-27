namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的敵人
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer, int x, int y);
		~CEnemy();
		virtual void LoadBitmap();				// 載入圖形
		virtual void OnMove();					// 移動敵人
		virtual void OnShow();					// 將敵人圖形貼到畫面
		void SetHeroXY(int x1, int x2, int y1, int y2);				// 設定英雄位置
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);	// 設定英雄攻擊範圍
		virtual void GetAttack(const int damage);		// 被攻擊
		virtual void AttackByEnemy(int* heroHP);

	protected:
		int x, y;						// 敵人左上角座標
		gameMap* currentMap;		// 所在地圖
		map<string, int> hero;
		map<string, int> heroAttackRange;
	};

	class CEnemy_sunFlower : public CEnemy
	{
	public:
		CEnemy_sunFlower(gameMap* pointer, int x, int y);
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
		void AttackByEnemy(int* heroHP);

	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyHP;					// 敵人生命值
		int enemyAttackDamage;		//敵人攻擊力
		int moveingStepCount;
		int moveingStep;
	};

}