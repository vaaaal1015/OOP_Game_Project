namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����ĤH
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer);
		int  GetX1();					// �ĤH���W�� x �y��
		int  GetY1();					// �ĤH���W�� y �y��
		int  GetX2();					// �ĤH�k�U�� x �y��
		int  GetY2();					// �ĤH�k�U�� y �y��
		int  GetWidth();				// �ĤH���e
		int  GetHeight();				// �ĤH����
		void Initialize();				// �]�w�ĤH����l��
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʼĤH
		void OnShow();					// �N�ĤH�ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w�ĤH���W���y��
		void GetAttack(int x1, int x2, int y1, int y2, int damage);		// �Q����
		int AttackByEnemy(int x1, int x2, int y1, int y2);
		void SetLoadBitMapNumber(int Number);

	protected:
		CAnimation animation;			// �ĤH���ʵe
		CAnimation moveRightAnimation;  // �V�k���ʰʵe
		CAnimation moveLeftAnimation;   // �V�����ʰʵe
		CAnimation jumpAnimation;		// ���D�ʵe
		CAnimation DeadAnimation;		// ���`�ʵe
		int x, y;						// �ĤH���W���y��
		int enemyHP;					// �ĤH�ͩR��
		int enemyAttackDamage;		//�ĤH�����O
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		int floor;				// �a�O��Y�y��
		bool rising;				// true��W�ɡBfalse��U��
		int initial_velocity;		// ��l�t��
		int velocity;				// �ثe���t��(�I/��)
	private:
		gameMap* currentMap;
		int moveingStepCount;
		int moveingStep;
		int LoadBitMapNumber;
	};
}