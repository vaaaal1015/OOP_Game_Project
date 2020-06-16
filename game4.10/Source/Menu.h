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
		void LoadBitmap();			// ¸ü¤J¹Ï§Î	
		void OnShow();
		void OnMove();
		void SetKeyUp();
		void SetKeyDown();
		void SetKeyEnter();
		bool isGameStart();

	private:
		CMovingBitmap Word_start;
		CMovingBitmap Word_member;
		CMovingBitmap Word_end;
		CMovingBitmap Word_teaching;
		CAnimation Word_selection;
		CMovingBitmap logo;
		CMovingBitmap Word_memberList;
		CMovingBitmap teaching;
		CMovingBitmap item;

		MenuState state;
		int selection;
		void ListOnShow();
		void MemberOnShow();
		void TeachingOnShow();
		void ItemOnShow();
		vector<CMovingBitmap> select;
	};


}