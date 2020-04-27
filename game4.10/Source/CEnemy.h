namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����ĤH
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer, int x, int y);
		~CEnemy();
		virtual void LoadBitmap();				// ���J�ϧ�
		virtual void OnMove();					// ���ʼĤH
		virtual void OnShow();					// �N�ĤH�ϧζK��e��
		void SetHeroXY(int x1, int x2, int y1, int y2);				// �]�w�^����m
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);	// �]�w�^�������d��
		virtual void GetAttack(const int damage);		// �Q����
		virtual void AttackByEnemy(int* heroHP);

	protected:
		int x, y;						// �ĤH���W���y��
		gameMap* currentMap;		// �Ҧb�a��
		map<string, int> hero;
		map<string, int> heroAttackRange;
	};

	class CEnemy_sunFlower : public CEnemy
	{
	public:
		CEnemy_sunFlower(gameMap* pointer, int x, int y);
		int  GetX1();					// �ĤH���W�� x �y��
		int  GetY1();					// �ĤH���W�� y �y��
		int  GetX2();					// �ĤH�k�U�� x �y��
		int  GetY2();					// �ĤH�k�U�� y �y��
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʼĤH
		void OnShow();					// �N�ĤH�ϧζK��e��
		void GetAttack(const int damage);		// �Q����
		void AttackByEnemy(int* heroHP);

	private:
		CAnimation animation;			// �ĤH���ʵe
		CAnimation moveRightAnimation;  // �V�k���ʰʵe
		CAnimation moveLeftAnimation;   // �V�����ʰʵe
		CAnimation jumpAnimation;		// ���D�ʵe
		CAnimation DeadAnimation;		// ���`�ʵe
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		int floor;				// �a�O��Y�y��
		bool rising;				// true��W�ɡBfalse��U��
		int initial_velocity;		// ��l�t��
		int velocity;				// �ثe���t��(�I/��)
		int enemyHP;					// �ĤH�ͩR��
		int enemyAttackDamage;		//�ĤH�����O
		int moveingStepCount;
		int moveingStep;
	};

}