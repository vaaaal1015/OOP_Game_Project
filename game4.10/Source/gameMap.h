namespace game_framework {
	class gameMap {
	public:
		gameMap();
		void LoadBitmap();    //���J�a��
		void OnShow();		  //��ܦa��
		bool isSpace(int x, int y);  //�P�_�O�_���Ů�A�O�N�^��True
		void SetSXSY(int x, int y);   //�]�w(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
	protected:
		CMovingBitmap ground; //�إ߯�a�Ϯ�
		CMovingBitmap ground1; //�إߤg�a�Ϯ�
		int map[480][640];    //�إߤ@�Ӧa�ϯx�}��index;
		const int X, Y;		  //�j�a�ϥ��W��x,y�y��
		const int MW, MH;	  //�C�i�p�a�Ϫ��e����
		int sx, sy;			  //(sx, sy)���ù�(�����W��)�b�a�ϤW���I�y��
	};
}