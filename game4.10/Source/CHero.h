namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����^��
	/////////////////////////////////////////////////////////////////////////////

	enum Hero_Action
	{
		STAND,
		WALK,
		RUN,
		ROLL,
		ATTACK,
		TALK,
		THROW
	};
	
	enum Hero_Direction
	{
		LEFT, RIGHT
	};


	class HeroBullet
	{
	public:
		HeroBullet(gameMap* point, int nx, int ny, int step);
		~HeroBullet();
		int GetX1();							// �l�u���W�� x �y��
		int GetY1();							// �l�u���W�� y �y��
		int GetX2();							// �l�u�k�U�� x �y��
		int GetY2();							// �l�u�k�U�� y �y��
		void OnMove();							// ���ʼĤH
		void OnShow();							// �N�ĤH�ϧζK��e��
		bool isDelet();							// �O�_�R��
	protected:
		CAnimation animation;					// �l�u�ʵe
		int x;									// x�y��
		int y;									// y�y��
		gameMap *currentMap;					// �ثe�a��
		int distance;							// �`���ʶZ��
		int STEP_SIZE;							// �@�����ʶZ��
	};

	class Shurikan : public HeroBullet
	{
	public:
		Shurikan(gameMap* point, int nx, int ny, int step);
		~Shurikan();
		void LoadBitmap();						// ���J�ϧ�
	};

	class CHero
	{
	public:
		CHero();
		~CHero();
		int  GetX1();											// �^�����W�� x �y��
		int  GetY1();											// �^�����W�� y �y��
		int  GetX2();											// �^���k�U�� x �y��
		int  GetY2();											// �^���k�U�� y �y��
		int  GetWidth();										// �^�����e
		int  GetHeight();										// �^������
		int	 GetCenterX();										// �^���������� x �y��
		int	 GetCenterY();										// �^���������� y �y��
		void Initialize();										// �]�w�^������l��
		void LoadBitmap();										// ���J�ϧ�		
		void OnMove();											// ���ʭ^��
		void OnShow();											// �N�^���ϧζK��e��
		void SetMovingDown(bool flag);							// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);							// �]�w�O�_���b��������
		void SetMovingRight(bool flag);							// �]�w�O�_���b���k����
		void SetMovingUp(bool flag);							// �]�w�O�_���b���W����
		void SetTalkingToNPC(bool flag);						// �MNPC���
		void SetHeroRoll(bool flag);							// ½�u
		void SetHeroAttack(bool flag);							// �]�w�O�_���b���W����
		void SetHeroThrow(bool flag);							// �]�w�O�_����F��
		void SetXY(int nx, int ny);								// �]�w�^�����W���y��
		void SetHeroHP(int inputHP);							// �]�w�D��HP��
		void AddHeroGold();										// �W�[�^��
		bool isAttacking;										// ���b����
		int GetHeroFullHP();									// ���o�^���������q
		int GetHeroCurrentHP();									// ���o�^���ثe��q
		int AttackByEnemy();									// �^���Q�ĤH����
		int HeroGetItem();										// �^�����o���~
		int GetPreviousMove();									// ���o�W�@�Ӱʧ@
		void SetMoveDelayCount(int delay);						// �]�w���ʩ���
		void ShowNumber(int colot, int Number, int x, int y);	//color:1:black,2:red
		void HeroLevelUp();										// �ɯ�
		void SetEndTalking();									// �����PNPC���
		bool isTalkingToNPC;									// �PNPC���
		int HeroLevel = 1;										// �^������
		void SelectMap(int MapNumber);							// ��ܦa��
		void ResetHeroState();									// ���m�D�����A(�^�쫰��)
		bool ClearedStage = false;								// �q������
		void OnLButtonDown(int Mx, int My);						// ���U�ƹ�����
		bool GetHeroIsRolling();								// ���o�^���O�_½�u
		bool isAlive();											// �^�ǥD���O�_����
	protected:
		CAnimation animation;									// �^�����ʵe(�V�k)
		CAnimation animation1;									// �^�����ʵe(�V��)
		CAnimation moveRightAnimation;							// �V�k���ʰʵe
		CAnimation moveLeftAnimation;							// �V�����ʰʵe
		CAnimation jumpAnimation;								// ���D�ʵe(�V�k)
		CAnimation jumpAnimation1;								// ���D�ʵe(�V��)
		CAnimation HeroAttackMovement;							// �^�������ʵe(�V�k)
		CAnimation HeroAttackMovement1;							// �^�������ʵe(�V��)
		CAnimation sword;										// ���J�C���ʵe(�V�k)
		CAnimation sword1;										// ���J�C���ʵe(�V��)
		CAnimation swordAttack;									// �C�������ʵe(�V�k)
		CAnimation swordAttack1;								// �C�������ʵe(�V��)
		CAnimation HeroDashLeft;								// �V���Ĩ�
		CAnimation SwordDashLeft;								// �C�����аʵe(�V��)
		CAnimation HeroDashRight;								// �V�k�Ĩ�	
		CAnimation SwordDashRight;								// �C�����аʵe(�V�k)
		CAnimation HeroRollLeft;								// �V��½�u
		CAnimation SwordRollLeft;								// ½�u�ɼC���ʵe(�V��)
		CAnimation HeroRollRight;								// �V�k½�u
		CAnimation SwordRollRight;								// ½�u�ɼC���ʵe(�V�k)
		CAnimation ThrowingLeft;								// �^�����Y�ʵe(��)
		CAnimation ThrowingRight;								// �^�����Y�ʵe(�k)
		CAnimation FireSwordRightAnimation;						
		CAnimation FireSwordLeftAnimation;
		CAnimation FireCircle;
		CAnimation Fire1;
		CAnimation Fire2;
		CAnimation Fire3;
		CAnimation gain_life;
		CMovingBitmap Infected_UI;
		CMovingBitmap LifeBarHead;		// ���
		CMovingBitmap StartGameBar;		// �}�l���s
		CMovingBitmap WorldMap_UI_1;	// �a��_1
		CMovingBitmap White;
		CMovingBitmap ShurikanUI;
		CInteger Num;					// �¦�Ʀr
		CInteger_Red Num_Red;			// ����Ʀr
		CInteger_Gold Num_Gold;			// ����Ʀr
		CMovingBitmap Word_G;
		CMovingBitmap Word_Gold;
		CMovingBitmap Shield;
		CMovingBitmap GainLifeUI;
		CMovingBitmap B;
		//CInteger DamageTaken;			// ��ܩӨ��ˮ`
		vector<CMovingBitmap*> LifeBarRed;		// ���
		int x, y;												// �^�����W���y��
		int heroAttackDamage;									// �^�������O
		bool isMovingDown;										// �O�_���b���U����
		bool isMovingLeft;										// �O�_���b��������
		bool isMovingRight;										// �O�_���b���k����
		bool isMovingUp;										// �O�_���b���W����
		bool isRolling;											// �O�_���b½�u
		bool isInvincible;										// �O�_���L��(�L�k�Q����)
		bool isThrowing;
		//string faceDirection;									// �H�����諸��V
		int Gold;												// ����
		int floor;												// �a�O��Y�y��
		bool rising;											// true��W�ɡBfalse��U��
		int initial_velocity;									// ��l�t��
		int velocity;											// �ثe���t��(�I/��)
		int SetAttackDelayCount;								// �]�w��������ɶ�
		int AttackDelayCount;									// ��������ɶ�
		int RollDelayCount;										// ½�u����ɶ�
		int MoveDelayCount;										// �W�Ӱʧ@�O�s�ɶ�
		int GainLifeDelayCount;									// ��ܦ^��S�Ī���
		int PreviousMovement;									// �����W�@�Ӱʧ@  0:�L�ʧ@; 1:�V����; 2:�V�k��
		int DashColdDown;										// �Ĩ�N�o�ɶ�
		int InvincibleDelayCount;								// �L�Įɶ�
		int ShowGoldDelayCount;									// �����X�{���ɶ�
		int GainHealthDelayCount;								// ���o���᪺۫�^��ɶ�����
		int bleed = 0;											// �����D�����L����
		int GetGold = 0;										// �o�쪺����
		int SpecialEffect = 0;									// 0:�L�ĪG;1:���K;2:����^��
		int SpecialEffectCount = 0;								// �����S��ĪG
	private:
		gameMap_village* currentVillage;						// �����ثe������
		gameMap_wild* currentWild;								// �����ثe�����~
		gameMap* currentMap;									// �����ثe���a��
		CMovingBitmap BlackMask;								// �¦�B�n
		CMovingBitmap QuitButton;								// ���}���s
		vector<Shurikan*> allShurikan;							// ���ؼC
		int FullHP;												// �D���`��q
		int CurrentHP;											// �D���ثe����q
		void changeLifeBarLength();								// ���ܥͩR��������
		bool isInHome;											// �O�_�b����
		bool isSelectingMap;									// �O�_���b��ܦa��
		int AttackRange;
		Hero_Action heroActoin;									// �^���ʧ@
		Hero_Direction heroDirection;							// �^�����諸��V
		void setHeroAction();									// �]�w�^���ʧ@
		void WalkOnMove();
		void RollOnMove();
		void StandOnMove();
		void WalkOnShow();
		void RollOnShow();
		void StandOnShow();
		void AttackOnMove();
		void AttackOnShow();
		void ThrowOnMove();
		void ThrowOnShow();
		CAnimation currentAnimation;	// �ثe���^���ʵe
		bool HasFireStone = false;
		int ShurikanNumber = 10;
		bool Poison = false;
		int PoisonDelayCount = 0;
		bool isInit;
	};

}