namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的敵人
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	class CEnemy
	{
	public:
		CEnemy();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 敵人的寬
		int  GetHeight();				// 敵人的高
		int	 GetCenterX();				// 敵人正中央的 x 座標
		int	 GetCenterY();				// 敵人正中央的 y 座標
		void Initialize();				// 設定敵人為初始值
		void LoadBitmap();				// 載入圖形
		void OnMove(gameMap *mymap);	// 移動敵人
		void OnShow(gameMap *mymap);	// 將敵人圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetXY(int nx, int ny);		// 設定敵人左上角座標
		bool isHit(int objectX, int objectY);  //判定物體擊中
	protected:
		CAnimation animation;		// 敵人的動畫
		CAnimation moveRightAnimation;  //向右移動動畫
		CAnimation moveLeftAnimation;   //向左移動動畫
		CAnimation jumpAnimation;		//跳躍動畫
		int x, y;					// 敵人左上角座標
		int enemyHP;				//敵人生命值
		int enemyAttackDamage;		//敵人攻擊力
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
	};
}