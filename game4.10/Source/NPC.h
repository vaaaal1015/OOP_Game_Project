namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* pointer);
		int  GetX1();					// NPC���W�� x �y��
		int  GetY1();					// NPC���W�� y �y��
		int  GetX2();					// NPC�k�U�� x �y��
		int  GetY2();					// NPC�k�U�� y �y��
		int  GetWidth();				// NPC���e
		int  GetHeight();				// NPC����
		void Initialize();				// �]�wNPC����l��
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ����NPC
		void OnShow();					// �NNPC�ϧζK��e��
		void SetXY(int nx, int ny);		// �]�wNPC���W���y��
		bool TouchedByHero(int x1, int x2, int y1, int y2);
		int InteractionBarFlag;
		void SetLoadBitMapNumber(int Number);
	protected:
		CAnimation animation;			 // NPC���ʵe
		CAnimation InteractionBar;      //���ʹ�
		int x, y;						// NPC���W���y��
	private:
		gameMap* currentMap;
		int LoadBitMapNumber;     //1:�ѤH  //2:���V��
	};

	
}