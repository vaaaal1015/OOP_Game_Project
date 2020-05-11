namespace game_framework {
	class NPC;
	class CEnemy;
	class gameMap {
	public:
		gameMap(string fileName);
		virtual ~gameMap() = default;

		int ScreenX(int x);
		int ScreenY(int y);
		int GetBlockY(int y);
		bool isSpace(int x, int y);  //判斷是否為空氣，是就回傳True
		void SetSXSY(int x, int y);   //設定(sx, sy)為螢幕(的左上角)在地圖上的點座標

		virtual void LoadBitmap();    //載入地圖
		virtual void OnShow();		  //顯示地圖
		virtual void OnMove();
		virtual void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level) = 0;
		virtual void AttackByHero(const int damage) = 0;		// 攻擊
		virtual void AttackByEnemy(int *heroHP) = 0;
		virtual void HeroTalkToNPC(bool flag) = 0;
		virtual void SetHeroAttackRange(int x1, int x2, int y1, int y2) = 0;

	protected:
		CMovingBitmap ground; //建立草地圖案
		CMovingBitmap ground1; //建立土地圖案
		CMovingBitmap ground2; //建立斜坡1圖案
		CMovingBitmap ground3; //建立斜坡2圖案
		int map[32][96];    //建立一個地圖矩陣的index;
		const int X, Y;		  //大地圖左上角x,y座標
		const int MW, MH;	  //每張小地圖的寬高度
		int sx, sy;			  //(sx, sy)為螢幕(的左上角)在地圖上的點座標
	};


	class gameMap_village : public gameMap {
	public:
		gameMap_village(string fileName);
		~gameMap_village();

		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void HeroTalkToNPC(bool flag);

	private:
		vector<NPC*> allNPC;
		bool HeroIsTalkingToNPC = false;
	};

	class gameMap_wild : public gameMap {
	public:
		gameMap_wild(string fileName);
		~gameMap_wild();

		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void AttackByHero(const int damage);		// 攻擊
		void AttackByEnemy(int *heroHP);
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);

	private:
		vector<CEnemy*> allEnemy;
		bool isStageClear = false;
	};
}