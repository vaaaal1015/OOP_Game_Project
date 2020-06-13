namespace game_framework {

	class Menu
	{
	public:
		Menu();
		void LoadBitmap();			// ¸ü¤J¹Ï§Î	
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