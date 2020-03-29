namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�����^��
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	class CHero
	{
	public:
		CHero();
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
		void setHeroDirection(string direction);  //�]�w�H�����諸��V		
		void OnMove(gameMap *mymap);					// ���ʭ^��
		void OnShow(gameMap *mymap);					// �N�^���ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w�^�����W���y��
		void CreatTxt(gameMap*);
		bool isHit(int objectX, int objectY);  //�P�w��������
	protected:
		CAnimation animation;		// �^�����ʵe(�V�k)
		CAnimation animation1;		// �^�����ʵe(�V��)
		CAnimation moveRightAnimation;  //�V�k���ʰʵe
		CAnimation moveLeftAnimation;   //�V�����ʰʵe
		CAnimation jumpAnimation;		//���D�ʵe
		CAnimation sword;				//���J�C���ʵe(�V�k)
		CAnimation sword1;				//���J�C���ʵe(�V��)
		int x, y;					// �^�����W���y��
		int heroHP;						//�^���ͩR��
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		string faceDirection;		//�H�����諸��V
		int floor;				// �a�O��Y�y��
		bool rising;				// true��W�ɡBfalse��U��
		int initial_velocity;		// ��l�t��
		int velocity;				// �ثe���t��(�I/��)
	};
}