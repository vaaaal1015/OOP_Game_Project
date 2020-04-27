namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* currentMap, int nx, int ny);
		~NPC();
		virtual void LoadBitmap();				// ���J�ϧ�
		virtual void OnMove();					// ����NPC
		virtual void OnShow();					// �NNPC�ϧζK��e��
		void SetHeroXY(int x1, int x2, int y1, int y2);

	protected:
		int x, y;						// NPC���W���y��
		map<string, int> hero;
		gameMap* currentMap;
	};


	class NPC_oldMan : public NPC
	{
	public:
		NPC_oldMan(gameMap* currentMap, int nx, int ny);
		int GetX1();
		int GetY1();
		int GetX2();
		int GetY2();
		bool isAroundHero();
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ����NPC
		void OnShow();					// �NNPC�ϧζK��e��

	private:
		CAnimation animation;			 // NPC���ʵe
		CAnimation InteractionBar;      //���ʹ�
	};
}

