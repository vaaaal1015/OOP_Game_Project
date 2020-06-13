namespace game_framework {

	class MenuList
	{
	public:
		MenuList();
		void LoadBitmap();			// ���J�ϧ�	
		void OnShow();
		void SetMoveingUp();
		void SetMoveingDown();
		int GetState();

	private:
		CMovingBitmap Word_start;
		CMovingBitmap Word_member;
		CMovingBitmap Word_end;
		CMovingBitmap Word_selection;

		int selection;
		vector<CMovingBitmap> select;
	};


}