namespace game_framework {
	class GameOver
	{
	public:
		GameOver();
		~GameOver();
		void LoadBitmap();
		void OnMove();
		void OnShow();

	private:
		CAnimation Ghost;
		CMovingBitmap pressEnter;
	};

}