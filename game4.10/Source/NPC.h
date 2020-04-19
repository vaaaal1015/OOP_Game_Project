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
		void GetAttack(int x1, int x2, int y1, int y2, int damage);		// �Q����
		int AttackByEnemy(int x1, int x2, int y1, int y2);
	protected:
		CAnimation animation;			// NPC���ʵe
		int x, y;						// NPC���W���y��
	private:
		gameMap* currentMap;
	};

	
}