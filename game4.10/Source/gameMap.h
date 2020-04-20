namespace game_framework {
	class gameMap {
	public:
		gameMap(string fileName);
		~gameMap();
		void LoadBitmap();    //���J�a��
		void OnShow();		  //��ܦa��
		bool isSpace(int x, int y);  //�P�_�O�_���Ů�A�O�N�^��True
		void SetSXSY(int x, int y);   //�]�w(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
		int ScreenX(int x);
		int ScreenY(int y);
		int GetBlockY(int y);
		void OnMove();
		void AttackByHero(int x1, int x2, int y1, int y2, int damage);		// ����
		int AttackByEnemy(int x1, int x2, int y1, int y2);  
		int HeroTouchNPC(int x1, int x2, int y1, int y2);   //�^�ǸI�����NPC�A���S�I��N�^��-1
		void SetEnemyPosition(int EnemyNumber, int EnemyX, int EnemyY);   //�]�w�ĤH��m
		void SetNPCPosition(int NPCNumber, int NPCX, int NPCY);   //�]�wNPC��m
	protected:
		CMovingBitmap ground; //�إ߯�a�Ϯ�
		CMovingBitmap ground1; //�إߤg�a�Ϯ�
		CMovingBitmap ground2; //�إ߱שY1�Ϯ�
		CMovingBitmap ground3; //�إ߱שY2�Ϯ�
		int map[48][64];    //�إߤ@�Ӧa�ϯx�}��index;
		const int X, Y;		  //�j�a�ϥ��W��x,y�y��
		const int MW, MH;	  //�C�i�p�a�Ϫ��e����
		int sx, sy;			  //(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
	private:
		vector<CEnemy*> allEnemy;
		vector<NPC*> allNPC;
	};
}