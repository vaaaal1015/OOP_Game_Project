namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* pointer);
		int  GetX1();					// NPC左上角 x 座標
		int  GetY1();					// NPC左上角 y 座標
		int  GetX2();					// NPC右下角 x 座標
		int  GetY2();					// NPC右下角 y 座標
		int  GetWidth();				// NPC的寬
		int  GetHeight();				// NPC的高
		void Initialize();				// 設定NPC為初始值
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動NPC
		void OnShow();					// 將NPC圖形貼到畫面
		void SetXY(int nx, int ny);		// 設定NPC左上角座標
		bool TouchedByHero(int x1, int x2, int y1, int y2);
		int InteractionBarFlag;
		void SetLoadBitMapNumber(int Number);
	protected:
		CAnimation animation;			 // NPC的動畫
		CAnimation InteractionBar;      //互動圖
		int x, y;						// NPC左上角座標
	private:
		gameMap* currentMap;
		int LoadBitMapNumber;     //1:老人  //2:長頸鹿
	};

	
}