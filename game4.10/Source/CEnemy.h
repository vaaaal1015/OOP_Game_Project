namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����ĤH
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	enum ACTION
	{
		STAND_LEFT, 
		MOVE_LEFT, 
		ATTACK_LEFT, 
		STAND_RIGHT, 
		MOVE_RIGHT, 
		ATTACK_RIGHT,
		GET_HIT,
		DEAD
	};

	enum Item_Type
	{
		BRONZE_COIN,
		SILVER_COIN,
		GOLDEN_COIN
	};

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer, int x, int y);
		virtual ~CEnemy();
		virtual void LoadBitmap() = 0;				// ���J�ϧ�
		virtual void OnMove() = 0;					// ���ʼĤH
		virtual void OnShow() = 0;					// �N�ĤH�ϧζK��e��
		virtual int GetX1() = 0;
		virtual int GetY1() = 0;
		virtual int GetX2() = 0;
		virtual int GetY2() = 0;
		void SetHeroXY(int x1, int x2, int y1, int y2);				// �]�w�^����m
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);	// �]�w�^�������d��
		virtual void GetAttack(const int damage) = 0;		// �Q�D������
		virtual void AttackByEnemy(int* heroHP) = 0;		// �����D��
		virtual bool isDead() = 0;
		virtual string GetEnemyType() = 0;
		void SetEnemyXY(int SetX, int SetY);
	protected:
		int x, y;						// �ĤH���W���y��
		gameMap* currentMap;		// �Ҧb�a��
		map<string, int> hero;
		map<string, int> heroAttackRange;
	};

	class bullet
	{
	public:
		bullet(gameMap* point, int nx, int ny, int step);
		~bullet();
		int GetX1();					// �l�u���W�� x �y��
		int GetY1();					// �l�u���W�� y �y��
		int GetX2();					// �l�u�k�U�� x �y��
		int GetY2();					// �l�u�k�U�� y �y��
		void OnMove();					// ���ʼĤH
		void OnShow();					// �N�ĤH�ϧζK��e��
		bool isDelet();

	protected:
		CAnimation animation;
		int x;
		int y;
		gameMap *currentMap;
		int distance;
		int STEP_SIZE;
	};
	class bullet_sunFlower : public bullet
	{
	public:
		bullet_sunFlower(gameMap* point, int nx, int ny, int step);
		~bullet_sunFlower();
		void LoadBitmap();				// ���J�ϧ�
	};

	class CEnemy_sunFlower : public CEnemy
	{
	public:
		CEnemy_sunFlower(gameMap* pointer, int x, int y);
		~CEnemy_sunFlower();
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
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// �ĤH���ʵe
		CAnimation animationLeft;		// �ĤH���ʵe(��)
		CAnimation moveRightAnimation;  // �V�k���ʰʵe
		CAnimation moveLeftAnimation;   // �V�����ʰʵe
		CAnimation AttackRightAnimation;// �����k��
		CAnimation AttackLeftAnimation; // ��������
		CAnimation jumpAnimation;		// ���D�ʵe
		CAnimation DeadAnimation;		// ���`�ʵe
		CAnimation GetHitAnimation;		// �Q���쪺�ʵe
		CAnimation HitAnimation;		// �����ĪG
		string EnemyType = "Sun_Flower";
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
		ACTION DetectHero(ACTION state);		// �����^����m:  1:�Z������200 2:�Z������150 3:�Z���k��150 4:�Z���k��200
		int attackDelay;
		int attackDelayCount;
		ACTION state;
		vector<bullet_sunFlower*> allBullet;
		int GetHitDelayCount = 0;
		bool GetHit = false;
	};
	
	class CEnemy_Cactus : public CEnemy
	{
	public:
		CEnemy_Cactus(gameMap* pointer, int x, int y);
		~CEnemy_Cactus();
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
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// �ĤH���ʵe
		CAnimation AttackAnimation;// ����
		CAnimation DeadAnimation;
		CAnimation GetHitAnimation;		// �Q���쪺�ʵe
		CAnimation HitAnimation;		// �����ĪG
		string EnemyType = "Cactus";
		int floor;				// �a�O��Y�y��
		bool rising;				// true��W�ɡBfalse��U��
		int initial_velocity;		// ��l�t��
		int velocity;				// �ثe���t��(�I/��)
		int enemyHP;					// �ĤH�ͩR��
		int enemyAttackDamage;		//�ĤH�����O
		int AttackDelayCount;		// �����W�v
		int SetAttackDelay;
		bool AttackFlag;			// ����true(�o�X�y��ʵe�X�{)�ɤ~������D��
		bool ReadyToAttack;			// �}�l�����ʧ@
		bool GetHit = false;
		int GetHitDelayCount = 0;
	};


	class CEnemy_Statue : public CEnemy
	{
	public:
		CEnemy_Statue(gameMap* pointer, int x, int y);
		~CEnemy_Statue();
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
		bool isDead();
		string GetEnemyType();

	private:
		CMovingBitmap Statue;
		CMovingBitmap Statue_Broken;
		CAnimation HitAnimation;		// �����ĪG
		string EnemyType = "Statue";
		int floor;				// �a�O��Y�y��
		bool rising;				// true��W�ɡBfalse��U��
		int initial_velocity;		// ��l�t��
		int velocity;				// �ثe���t��(�I/��)
		int enemyHP = 500;					// �ĤH�ͩR��
		int enemyAttackDamage = 0;		//�ĤH�����O
		int GetHitDelayCount = 0;
		bool GetHit = false;
	};

	
}