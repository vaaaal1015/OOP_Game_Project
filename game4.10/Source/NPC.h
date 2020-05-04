namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* currentMap, int nx, int ny);
		virtual ~NPC();
		virtual void LoadBitmap() = 0;				// ���J�ϧ�
		virtual void OnMove() = 0;					// ����NPC
		virtual void OnShow() = 0;					// �NNPC�ϧζK��e��
		virtual void SetIsTalkingToHero(bool flag) = 0;
		bool isTalkingToHero = false;
		//virtual void TouchedByHero(int x1, int x2, int y1, int y2);
		void SetHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
	protected:
		int x, y;						// NPC���W���y��
		map<string, int> hero;
		gameMap* currentMap;
	};


	class NPC_oldMan : public NPC
	{
	public:
		NPC_oldMan(gameMap* currentMap, int nx, int ny);
		~NPC_oldMan();
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
		
		void SetIsTalkingToHero(bool flag);
		//void TouchedByHero(int x1, int x2, int y1, int y2);
		CMovingBitmap LevelUpBar;
		CMovingBitmap LevelUpInterface;
		CMovingBitmap PauseBar;
	};
}

