namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����^��
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class CHero
	{
	public:
		CHero();
		~CHero();
		int  GetX1();					// �^�����W�� x �y��
		int  GetY1();					// �^�����W�� y �y��
		int  GetX2();					// �^���k�U�� x �y��
		int  GetY2();					// �^���k�U�� y �y��
		int  GetWidth();				// �^�����e
		int  GetHeight();				// �^������
		int	 GetCenterX();				// �^���������� x �y��
		int	 GetCenterY();				// �^���������� y �y��
		void Initialize();				// �]�w�^������l��
		void LoadBitmap();				// ���J�ϧ�
		void setHeroDirection(string direction);	// �]�w�H�����諸��V		
		void OnMove();					// ���ʭ^��
		void OnShow();					// �N�^���ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetHeroAttack(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w�^�����W���y��
		int HitEnemyAndReternDamage(int x1, int x2, int y1, int y2);	// �P�_���L�ĤH x1(�ĤH����x�y��),x2(�ĤH�k��y��),y1(�ĤH�W��y�y��),y2(�ĤH�U��y�y��)
		void beHit(int objectX, int objectY, int objectDamage);			//�P�w��������
		void SetHeroHP(int inputHP);	// �]�w�D��HP��
		bool isAttacking;				// ���b����
		void SetMap(int index);			// 
		int GetHeroFullHP();
		int GetHeroCurrentHP();
	protected:
		CAnimation animation;			// �^�����ʵe(�V�k)
		CAnimation animation1;			// �^�����ʵe(�V��)
		CAnimation moveRightAnimation;  // �V�k���ʰʵe
		CAnimation moveLeftAnimation;   // �V�����ʰʵe
		CAnimation jumpAnimation;		// ���D�ʵe(�V�k)
		CAnimation jumpAnimation1;		// ���D�ʵe(�V��)
		CAnimation HeroAttackMovement;  // �^�������ʵe<�V�k)
		CAnimation HeroAttackMovement1;	// �^�������ʵe<�V��)
		CAnimation sword;				// ���J�C���ʵe(�V�k)
		CAnimation sword1;				// ���J�C���ʵe(�V��)
		CAnimation swordAttack;			// �C�������ʵe(�V�k)
		CAnimation swordAttack1;		// �C�������ʵe(�V��)
		CMovingBitmap LifeBarHead;		// ���
		vector<CMovingBitmap*> LifeBarRed;		// ���
		int x, y;						// �^�����W���y��
		int AttackRange;				// �^�������d��
		int heroAttackDamage;			// �^�������O
		bool isMovingDown;				// �O�_���b���U����
		bool isMovingLeft;				// �O�_���b��������
		bool isMovingRight;				// �O�_���b���k����
		bool isMovingUp;				// �O�_���b���W����
		string faceDirection;			//�H�����諸��V
		int floor;						// �a�O��Y�y��
		bool rising;					// true��W�ɡBfalse��U��
		int initial_velocity;			// ��l�t��
		int velocity;					// �ثe���t��(�I/��)
		int SetAttackDelayCount;		// �]�w��������ɶ�
		int AttackDelayCount;			// ��������ɶ�
		
	private:
		vector<gameMap*> maps;
		gameMap* currentMap;
		int FullHP;
		int CurrentHP;
	};
}