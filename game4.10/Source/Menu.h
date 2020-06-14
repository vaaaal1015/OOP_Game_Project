namespace game_framework {
	enum MenuState
	{
		LIST,
		MEMBER,
		TEACHING
	};

	class Menu
	{
	public:
		Menu();
		void LoadBitmap();			// ¸ü¤J¹Ï§Î	
		void OnShow();
		void SetKeyUp();
		void SetKeyDown();
		void SetKeyEnter();
		bool isGameStart();

	private:
		CMovingBitmap Word_start;
		CMovingBitmap Word_member;
		CMovingBitmap Word_end;
		CMovingBitmap Word_selection;
		CMovingBitmap logo;
		CMovingBitmap Word_memberList;
		MenuState state;
		int selection;
		void ListOnShow();
		void MemberOnShow();
		vector<CMovingBitmap> select;
	};


}