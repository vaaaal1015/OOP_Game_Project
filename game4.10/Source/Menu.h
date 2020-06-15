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
		void LoadBitmap();			// ���J�ϧ�	
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
		CMovingBitmap key_up;
		CMovingBitmap key_left;
		CMovingBitmap key_right;
		CMovingBitmap key_z;
		CMovingBitmap key_x;
		CMovingBitmap key_c;
		CMovingBitmap key_e;
		CMovingBitmap background;
		CAnimation moveRightAnimation;  // �V�k���ʰʵe
		CAnimation moveLeftAnimation;   // �V�����ʰʵe

		MenuState state;
		int selection;
		void ListOnShow();
		void MemberOnShow();
		void TeachingOnShow();
		void TeachingOnMove();
		vector<CMovingBitmap> select;
	};


}