namespace game_framework {
	class GameOver
	{
	public:
		GameOver();
		~GameOver();
		void LoadBitmap();				// 載入圖形	
		void OnMove();					// 移動元素
		void OnShow();					// 顯示遊戲畫面

	private:
		CAnimation Ghost;				// 鬼
		CMovingBitmap pressEnter;		// 字體"按下Enter返回"
	};

}