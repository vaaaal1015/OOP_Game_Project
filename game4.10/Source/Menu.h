namespace game_framework {

	class Menu
	{
	public:
		Menu();
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
		CMovingBitmap logo;

		int selection;
		vector<CMovingBitmap> select;
	};


}