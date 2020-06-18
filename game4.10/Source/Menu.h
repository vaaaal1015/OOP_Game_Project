namespace game_framework {
	enum MenuState
	{
		LIST,
		MEMBER,
		TEACHING,
		ITEM
	};

	class Menu
	{
	public:
		Menu();
		void LoadBitmap();					// ���J�ϧ�	
		void OnShow();						// ��ܹC���e��
		void OnMove();						// ���ʤ���
		void SetKeyUp();					// ���U�W��
		void SetKeyDown();					// ���U�U��
		void SetKeyEnter();					// ���UEnter��
		bool isGameStart();					// �^�ǹC���O�_�}�l
	private:
		CMovingBitmap Word_start;			// �r��"�}�l�C��"
		CMovingBitmap Word_member;			// �r��"�H���W��"
		CMovingBitmap Word_end;				// �r��"���}�C��"
		CMovingBitmap Word_teaching;		// �r��"�ާ@�о�"
		CAnimation Word_selection;			// ��ܲŸ�
		CMovingBitmap logo;					// �C�� LOGO
		CMovingBitmap Word_memberList;		// �H���W��e��
		CMovingBitmap teaching;				// ����оǵe��
		CMovingBitmap item;					// ���~���еe��
		MenuState state;					// �������A
		int selection;						// ������ܪ�index
		void ListOnShow();					// ��ܫ���оǵe��
		void MemberOnShow();				// ��ܤH���W��e��
		void TeachingOnShow();				// ��ܫ���оǵe��
		void ItemOnShow();					// ��ܪ��~���еe��
		vector<CMovingBitmap> select;		// �x�s�����ﶵ
	};


}