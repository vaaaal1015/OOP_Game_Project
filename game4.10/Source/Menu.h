namespace game_framework {
	enum MenuState
	{
		LIST,
		MEMBER,
		TEACHING,
		ITEM
	};

	class Menu
	{
	public:
		Menu();
		void LoadBitmap();					// 載入圖形	
		void OnShow();						// 顯示遊戲畫面
		void OnMove();						// 移動元素
		void SetKeyUp();					// 按下上鍵
		void SetKeyDown();					// 按下下鍵
		void SetKeyEnter();					// 按下Enter鍵
		bool isGameStart();					// 回傳遊戲是否開始
	private:
		CMovingBitmap Word_start;			// 字體"開始遊戲"
		CMovingBitmap Word_member;			// 字體"人員名單"
		CMovingBitmap Word_end;				// 字體"離開遊戲"
		CMovingBitmap Word_teaching;		// 字體"操作教學"
		CAnimation Word_selection;			// 選擇符號
		CMovingBitmap logo;					// 遊戲 LOGO
		CMovingBitmap Word_memberList;		// 人員名單畫面
		CMovingBitmap teaching;				// 按鍵教學畫面
		CMovingBitmap item;					// 物品介紹畫面
		MenuState state;					// 紀錄狀態
		int selection;						// 紀錄選擇的index
		void ListOnShow();					// 顯示按鍵教學畫面
		void MemberOnShow();				// 顯示人員名單畫面
		void TeachingOnShow();				// 顯示按鍵教學畫面
		void ItemOnShow();					// 顯示物品介紹畫面
		vector<CMovingBitmap> select;		// 儲存全部選項
	};


}