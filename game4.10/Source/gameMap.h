namespace game_framework {
	class NPC;
	class CEnemy;
	class gameMap {
	public:
		gameMap(string fileName);
		~gameMap();
		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		bool isSpace(int x, int y);  //判斷是否為空氣，是就回傳True
		void SetSXSY(int x, int y);   //設定(sx, sy)為螢幕(的左上角)在地圖上的點座標
		int ScreenX(int x);
		int ScreenY(int y);
		int GetBlockY(int y);
		void OnMove();
		void AttackByHero(const int damage);		// 攻擊
		void AttackByEnemy(int *heroHP);
		void HeroTalkToNPC(bool flag);
		void setHeroState(int x1, int x2, int y1, int y2,int HP, int Gold, int AttackDamage, int Level);
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);
		bool HeroIsTalkingToNPC = false;
	protected:
		CMovingBitmap ground; //建立草地圖案
		CMovingBitmap ground1; //建立土地圖案
		CMovingBitmap ground2; //建立斜坡1圖案
		CMovingBitmap ground3; //建立斜坡2圖案
		int map[32][96];    //建立一個地圖矩陣的index;
		const int X, Y;		  //大地圖左上角x,y座標
		const int MW, MH;	  //每張小地圖的寬高度
		int sx, sy;			  //(sx, sy)為螢幕(的左上角)在地圖上的點座標
	private:
		vector<CEnemy*> allEnemy;
		vector<NPC*> allNPC;
	};
}