namespace game_framework {
	class GameOver
	{
	public:
		GameOver();
		~GameOver();
		void LoadBitmap();				// ���J�ϧ�	
		void OnMove();					// ���ʤ���
		void OnShow();					// ��ܹC���e��

	private:
		CAnimation Ghost;				// ��
		CMovingBitmap pressEnter;		// �r��"���UEnter��^"
	};

}