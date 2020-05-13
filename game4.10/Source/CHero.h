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
		void SetTalkingToNPC(bool flag);// �MNPC���
		void SetHeroRoll(bool flag);	// ½�u
		void SetHeroAttack(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w�^�����W���y��
		void SetHeroHP(int inputHP);	// �]�w�D��HP��
		bool isAttacking;				// ���b����
		//void SetMap(int index);			// 
		int GetHeroFullHP();
		int GetHeroCurrentHP();      
		void AttackByEnemy();
		void SetPreviousMove(int Movement);
		int GetPreviousMove();          //���o�W�@�Ӱʧ@
		void SetMoveDelayCount(int delay);
		void ShowNumber(int Number, int x, int y);
		void HeroLevelUp();             // �ɯ�
		void SetEndTalking();            // �����PNPC���
		bool isTalkingToNPC;			// �PNPC���
		int HeroLevel = 1;
		void SelectMap(int MapNumber);
		void ResetHeroState();      //���m�D�����A(�^�쫰��)
		bool ClearedStage = false;  //�q������
		void OnLButtonDown(int Mx, int My);		//���U�ƹ�����


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
		CAnimation HeroDashLeft;		// �V���Ĩ�
		CAnimation SwordDashLeft;
		CAnimation HeroDashRight;		// �V�k�Ĩ�
		CAnimation SwordDashRight;
		CAnimation HeroRollLeft;		// �V��½�u
		CAnimation SwordRollLeft;
		CAnimation HeroRollRight;		// �V�k½�u
		CAnimation SwordRollRight;
		CMovingBitmap LifeBarHead;		// ���
		CMovingBitmap StartGameBar;		// �}�l���s
		CMovingBitmap WorldMap_UI_1;		// �a��_1
		CInteger Num;				// �ͩR��
		//CInteger DamageTaken;			// ��ܩӨ��ˮ`
		vector<CMovingBitmap*> LifeBarRed;		// ���
		int x, y;						// �^�����W���y��
		int AttackRange;				// �^�������d��
		int heroAttackDamage;			// �^�������O
		bool isMovingDown;				// �O�_���b���U����
		bool isMovingLeft;				// �O�_���b��������
		bool isMovingRight;				// �O�_���b���k����
		bool isMovingUp;				// �O�_���b���W����
		bool isRolling;					// �O�_���b½�u
		bool isInvincible;				// �O�_���L��(�L�k�Q����)
		string faceDirection;			// �H�����諸��V
		int Gold;						// ����
		int floor;						// �a�O��Y�y��
		bool rising;					// true��W�ɡBfalse��U��
		int initial_velocity;			// ��l�t��
		int velocity;					// �ثe���t��(�I/��)
		int SetAttackDelayCount;		// �]�w��������ɶ�
		int AttackDelayCount;			// ��������ɶ�
		int RollDelayCount;				// ½�u����ɶ�
		int MoveDelayCount;				// �W�Ӱʧ@�O�s�ɶ�
		int PreviousMovement;			// �����W�@�Ӱʧ@  0:�L�ʧ@; 1:�V����; 2:�V�k��
		int DashColdDown;				// �Ĩ�N�o�ɶ�
		int InvincibleDelayCount;		// �L�Įɶ�

	private:
		gameMap_village* currentVillage;
		gameMap_wild* currentWild;
		gameMap* currentMap;
		CMovingBitmap BlackMask;
		CMovingBitmap QuitButton;
		int FullHP;
		int CurrentHP;
		void changeLifeBarLength();
		bool isInHome;			// �O�_�b����
		bool isSelectingMap;	// �O�_���b��ܦa��
	};
}