namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* currentMap, int nx, int ny);
		~NPC();
		virtual void LoadBitmap();				// 載入圖形
		virtual void OnMove();					// 移動NPC
		virtual void OnShow();					// 將NPC圖形貼到畫面
		void SetHeroXY(int x1, int x2, int y1, int y2);

	protected:
		int x, y;						// NPC左上角座標
		map<string, int> hero;
		gameMap* currentMap;
	};


	class NPC_oldMan : public NPC
	{
	public:
		NPC_oldMan(gameMap* currentMap, int nx, int ny);
		int GetX1();
		int GetY1();
		int GetX2();
		int GetY2();
		bool isAroundHero();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動NPC
		void OnShow();					// 將NPC圖形貼到畫面

	private:
		CAnimation animation;			 // NPC的動畫
		CAnimation InteractionBar;      //互動圖
	};
}

