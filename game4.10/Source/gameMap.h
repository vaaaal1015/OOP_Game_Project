namespace game_framework {
	class NPC;
	class CEnemy;
	class gameMap {
	public:
		gameMap(string fileName);
		virtual ~gameMap() = default;

		int ScreenX(int x);
		int ScreenY(int y);
		int GetBlockY(int y);
		bool isSpace(int x, int y);  //�P�_�O�_���Ů�A�O�N�^��True
		void SetSXSY(int x, int y);   //�]�w(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��

		virtual void LoadBitmap();    //���J�a��
		virtual void OnShow();		  //��ܦa��
		virtual void OnMove();
		virtual void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level) = 0;
		virtual void AttackByHero(const int damage) = 0;		// ����
		virtual void AttackByEnemy(int *heroHP) = 0;
		virtual void HeroTalkToNPC(bool flag) = 0;
		virtual void SetHeroAttackRange(int x1, int x2, int y1, int y2) = 0;

	protected:
		CMovingBitmap ground; //�إ߯�a�Ϯ�
		CMovingBitmap ground1; //�إߤg�a�Ϯ�
		CMovingBitmap ground2; //�إ߱שY1�Ϯ�
		CMovingBitmap ground3; //�إ߱שY2�Ϯ�
		int map[32][96];    //�إߤ@�Ӧa�ϯx�}��index;
		const int X, Y;		  //�j�a�ϥ��W��x,y�y��
		const int MW, MH;	  //�C�i�p�a�Ϫ��e����
		int sx, sy;			  //(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
	};


	class gameMap_village : public gameMap {
	public:
		gameMap_village(string fileName);
		~gameMap_village();

		void LoadBitmap();    //���J�a��
		void OnShow();		  //��ܦa��
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void HeroTalkToNPC(bool flag);

	private:
		vector<NPC*> allNPC;
		bool HeroIsTalkingToNPC = false;
	};

	class gameMap_wild : public gameMap {
	public:
		gameMap_wild(string fileName);
		~gameMap_wild();

		void LoadBitmap();    //���J�a��
		void OnShow();		  //��ܦa��
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void AttackByHero(const int damage);		// ����
		void AttackByEnemy(int *heroHP);
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);

	private:
		vector<CEnemy*> allEnemy;
		bool isStageClear = false;
	};
}