#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "NPC.h"
#include "CEnemy.h"
#include "gameMap.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;



namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// gameMap : gameMap base class
	/////////////////////////////////////////////////////////////////////////////
	gameMap::gameMap(string fileName) : X(0), Y(0), MW(MIN_MAP_SIZE), MH(MIN_MAP_SIZE), sx(0), sy(0)  //圖片為10*10 初始螢幕畫面位於0,475
	{
		fstream mapFile;
		string mapLocation = "Map\\" + fileName;

		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 96; j++)
			{
				map[i][j] = 0;
			}
		}

		mapFile.open(mapLocation, ios::in);
		if (!mapFile) cout << "error";
		int i = 0;
		while (!mapFile.eof())
		{
			string buffer;
			getline(mapFile, buffer);
			int j = 0;
			int count = 0;
			while (buffer[j] != '\0')
			{
				if (buffer[j] == ',')
				{
					count++;
					j++;
				}
				else
				{
					map[i][count] = map[i][count] * 10;
					map[i][count] += (int)(buffer[j] - '0');
					//count++;
					j++;
				}
			}
			i += 1;
		}
	}

	gameMap::~gameMap()
	{

	}

	void gameMap::SetSXSY(int x, int y)   // 設定
	{
		sx = x;
		sy = y;
	}

	bool gameMap::isSpace(int x, int y)   // (x, y)為地圖的點座標
	{
		int gx = x / MIN_MAP_SIZE; // 轉換為格座標(整數除法)
		int gy = y / MIN_MAP_SIZE;  // 轉換為格座標(整數除法)

		if (map[gy][gx] == 1 || map[gy][gx] == 2 || map[gy][gx] == 3 || map[gy][gx] == 4 || map[gy][gx] == 8) return false;
		return true;
	}

	int gameMap::GetBlockY(int y)
	{
		int gy = y / MIN_MAP_SIZE;  // 轉換為格座標(整數除法)
		return gy * MIN_MAP_SIZE;
	}

	int gameMap::ScreenX(int x) // x 為地圖的點座標
	{
		return x - sx; // 回傳螢幕的 x 點座標
	}

	int gameMap::ScreenY(int y) // y 為地圖的 y 點座標
	{
		return y - sy; // 回傳螢幕的點座標
	}

	void gameMap::LoadBitmap()
	{
		ground.LoadBitmap(IDB_MAPGROUND, RGB(255, 255, 255));//載入草地圖案
		ground1.LoadBitmap(IDB_MAPGROUND1);//載入泥土圖案
		ground2.LoadBitmap(IDB_MAPSLIDE1, RGB(255, 255, 255));//載入斜坡1圖案
		ground3.LoadBitmap(IDB_MAPSLIDE2);//載入斜坡2圖案
	}

	void gameMap::OnShow()
	{
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 96; j++) {
				int x = j * MW - sx; // 算出第(i, j)這一格的 x 螢幕座標
				int y = i * MH - sy; // 算出第(i, j)這一格的 y 螢幕座標
				switch (map[i][j])
				{
				case 4:
					ground3.SetTopLeft(x, y); // 指定第(i, j)這一格的座標
					ground3.ShowBitmap();   //顯示斜坡2圖案
					break;
				case 3:
					ground2.SetTopLeft(x, y); // 指定第(i, j)這一格的座標
					ground2.ShowBitmap();   //顯示斜坡1圖案
					break;
				case 2:
					ground1.SetTopLeft(x, y); // 指定第(i, j)這一格的座標
					ground1.ShowBitmap();   //顯示土地圖案
					break;

				case 1:
					ground.SetTopLeft(x, y); // 指定第(i, j)這一格的座標
					ground.ShowBitmap();    //顯示草地圖案
					break;
				case 0:
					break;
				default:
					break;
				}
			}
		}
	}

	bool gameMap::isDoor(int x, int y)
	{
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++)
		{
			if ((*i)->GetObjectType() == 2)
			{
				/*if (((*i)->GetX2() >= HeroX1) && (HeroX2 >= (*i)->GetX1()) && ((*i)->GetY2() >= HeroY1) && (HeroY2 >= (*i)->GetY1()))
				{
					return true;
				}*/
				if ((x <= (*i)->GetX2()) && (x>=(*i)->GetX1()) && (y>=(*i)->GetY1() && y<=(*i)->GetY2()))
				{
					return true;
				}
			}
		}
		return false;
	}

	void gameMap::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		HeroX1 = x1;
		HeroY1 = y1;
		HeroX2 = x2;
		HeroY2 = y2;
	}
	/////////////////////////////////////////////////////////////////////////////
	// gameMap_village : gameMap_village class
	/////////////////////////////////////////////////////////////////////////////
	gameMap_village::gameMap_village() : gameMap("Home.txt")
	{
		allNPC.push_back(new NPC_oldMan(this, 200, 350));
	}

	gameMap_village::~gameMap_village()
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) delete (*i);
	}

	void gameMap_village::LoadBitmap()
	{
		gameMap::LoadBitmap();
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->LoadBitmap();
	}

	void gameMap_village::OnShow()
	{
		gameMap::OnShow();
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->OnShow();
	}

	void gameMap_village::OnMove()
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->OnMove();
		if (allNPC.size() != 0) HeroIsTalkingToNPC = allNPC[0]->isTalkingToHero;
	}

	bool gameMap_village::HeroTalkToNPC()
	{
		bool success = false;
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
		{
			if ((*i)->SetIsTalkingToHero())
			{
				success = true;
			}
		}
		return success;
	}

	void gameMap_village::OnLButtonDown(int Mx, int My, bool *isTalkingToNPC)
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
		{
			(*i)->OnLButtonDown(Mx, My, isTalkingToNPC);
		}
	}

	void gameMap_village::setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level)
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->SetHeroState(x1, x2, y1, y2, HP, Gold, AttackDamage, Level);
	}

	/*
	bool gameMap_village::GetHeroIsTalkingToNPC()
	{
		return HeroIsTalkingToNPC;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////
	// gameMap_wild : gameMap_wild class
	/////////////////////////////////////////////////////////////////////////////
	gameMap_wild::gameMap_wild(string fileName) : gameMap(fileName)
	{
		unsigned seed;
		seed = (unsigned)time(NULL);
		srand(seed);

		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 96; j++) {
				int x = j * MW - sx; // 算出第(i, j)這一格的 x 螢幕座標
				int y = i * MH - sy; // 算出第(i, j)這一格的 y 螢幕座標

				if (map[i][j] / 10 == 2)
				{
					allObject.push_back(new Switch(this, x, y, true, map[i][j] % 10));
				}

				if (map[i][j] / 10 == 3)
				{
					allObject.push_back(new Spike(this, x, y + 10, true, map[i][j] % 10 * (-1)));
					allObject.push_back(new Spike(this, x + 10, y + 10, true, map[i][j] % 10 * (-1)));
				}

				if (map[i][j] / 10 == 5)
				{
					allObject.push_back(new Door(this, x, y + 10, true, map[i][j] % 10 * (-1)));
				}

				switch (map[i][j])
				{
				case 9:
					allEnemy.push_back(new CEnemy_Statue(this, x, y));
					break;
				case 10:
					allEnemy.push_back(new CEnemy_Cloud(this, x, y));
					break;
				case 40:
					allEnemy.push_back(new CEnemy_sunFlower(this, x, y));
					break;
				case 41:
					allEnemy.push_back(new CEnemy_Cactus(this, x, y));
					break;
				case 42:
					allEnemy.push_back(new CEnemy_GasRobot(this, x, y));
					break;
				case 43:
					allEnemy.push_back(new CEnemy_RobotA(this, x, y));
					break;
				case 44:
					allEnemy.push_back(new CEnemy_Pigeon(this, x, y));
					break;
				default:
					break;
				}
			}
		}

		/*
		allEnemy.push_back(new CEnemy_sunFlower(this, 300, 350));
		allEnemy.push_back(new CEnemy_Cactus(this, 500, 280));
		allEnemy.push_back(new CEnemy_Cactus(this, 1000, 210));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1500, 350));
		allEnemy.push_back(new CEnemy_Cactus(this, 1650, 20));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1650, 20));
		allEnemy.push_back(new CEnemy_sunFlower(this, 400, 350));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1550, 350));
		allEnemy.push_back(new CEnemy_Statue(this, 2950, 325));
		allEnemy.push_back(new CEnemy_sunFlower(this, 2150, 20));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1950, 20));
		allEnemy.push_back(new CEnemy_Cactus(this, 2400, 405));
		allEnemy.push_back(new CEnemy_Cactus(this, 2550, 405));
		allObject.push_back(new Switch(this, 600, 350, true, 1));
		allObject.push_back(new Spike(this, 800, 350, true, -1));
		allObject.push_back(new Spike(this, 820, 350, true, -1));
		allObject.push_back(new Spike(this, 840, 350, true, -1));
		allObject.push_back(new Spike(this, 860, 350, true, -1));
		allObject.push_back(new Switch(this, 1000, 350, true, 2));
		allObject.push_back(new Spike(this, 1100, 350, true, -2));
		allObject.push_back(new Spike(this, 1120, 350, true, -2));
		allObject.push_back(new Spike(this, 1140, 350, true, -2));
		allObject.push_back(new Spike(this, 1160, 350, true, -2));
		*/
		/*
		allItem.push_back(new Item_Fire_Stone(this, 300, 350, ItemExistTime));
		allItem.back()->LoadBitmap();
		allItem.push_back(new Item_RedPot_Small(this, 500, 350, ItemExistTime));
		allItem.back()->LoadBitmap();
		allItem.push_back(new Item_RedPot_Medium(this, 600, 350, ItemExistTime));
		allItem.back()->LoadBitmap();
		allItem.push_back(new Item_RedPot_Full(this, 700, 350, ItemExistTime));
		allItem.back()->LoadBitmap();
		allItem.push_back(new Item_RedPot_Stone(this, 750, 450, ItemExistTime));
		allItem.back()->LoadBitmap();
		allItem.push_back(new Item_Shurikan(this, 650, 450, ItemExistTime));
		allItem.back()->LoadBitmap();
		*/
	}

	gameMap_wild::~gameMap_wild()
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) delete (*i);
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) delete (*i);
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) delete(*i);
	}

	void gameMap_wild::LoadBitmap()
	{
		gameMap::LoadBitmap();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->LoadBitmap();
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) (*i)->LoadBitmap();
	}

	void gameMap_wild::OnShow()
	{
		gameMap::OnShow();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnShow();
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) (*i)->OnShow();
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) (*i)->OnShow();

	}

	void gameMap_wild::OnMove() {
		vector<CEnemy*>::iterator iter = allEnemy.begin();
		MapObjectInteration();
		while (iter != allEnemy.end())         //敵人死亡會從vector裡被刪除
		{
			if ((*iter)->isDead() && (*iter)->GetEnemyType() == "Statue")
			{
				//CAudio::Instance()->Play(7, false);
				isStageClear = true;   //通關完成
			}
			if ((*iter)->isDead() && (*iter)->GetEnemyType() != "Statue")   //雕像以外的敵人被打死
			{
				DropItem(((*iter)->GetX1() + (*iter)->GetX2()) / 2, ((*iter)->GetY1() + (*iter)->GetY2()) / 2);
				delete *iter;
				iter = allEnemy.erase(iter);
				//isStageClear = true;   //通關完成
			}
			else
				iter++;
		}
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) (*i)->OnMove();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnMove();
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) (*i)->OnMove();
	}

	void gameMap_wild::setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroXY(x1, x2, y1, y2);
	}

	void gameMap_wild::SetHeroAttackRange(int x1, int x2, int y1, int y2)
	{
		HeroAttackX1 = x1;
		HeroAttackY1 = y1;
		HeroAttackX2 = x2;
		HeroAttackY2 = y2;
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroAttackRange(x1, x2, y1, y2);
	}

	void gameMap_wild::AttackByHero(const int damage)		// 攻擊
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->GetAttack(damage);
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) (*i)->GetAttack(HeroAttackX1, HeroAttackY1, HeroAttackX2, HeroAttackY2);
	}

	void gameMap_wild::AttackByEnemy(int *heroHP, bool *Poison)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->AttackByEnemy(heroHP, Poison);
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++) (*i)->AttackByObject(HeroX1, HeroY1, HeroX2, HeroY2, heroHP);
	}

	void gameMap_wild::HeroGetItem(int *HeroCoin, int *SpecialEffect, int *SpecialEffectCount, int *HeroHP, int FullHP, int *ShurikanNumber)
	{
		vector<Item*>::iterator iter = allItem.begin();
		while (iter != allItem.end())
		{
			if (((*iter)->GetX2() >= HeroX1) && (HeroX2 >= (*iter)->GetX1()) && ((*iter)->GetY2() >= HeroY1) && (HeroY2 >= (*iter)->GetY1()))
			{
				CAudio::Instance()->Play(5, false);
				switch ((*iter)->GetItemValue())
				{
				case 1:						//火焰石
					*SpecialEffect = 1;
					*SpecialEffectCount = 3;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				case 2:						// 小血瓶
					if (*HeroHP + 30 >= FullHP) *HeroHP = FullHP;
					else *HeroHP += 30;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				case 3:						// 中血瓶
					if (*HeroHP + 50 >= FullHP) *HeroHP = FullHP;
					else *HeroHP += 50;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				case 4:						// 大血瓶
					*HeroHP = FullHP;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				case 5:						// 手裡劍
					*ShurikanNumber += 10;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				case 9:						// 紅石
					*SpecialEffect = 2;
					delete *iter;
					iter = allItem.erase(iter);
					break;
				default:					//金幣
					*HeroCoin += (*iter)->GetItemValue();
					delete *iter;
					iter = allItem.erase(iter);
					break;
				}
			}
			else if ((*iter)->isDelete())
			{
				delete *iter;
				iter = allItem.erase(iter);
			}
			else
				iter++;
		}
	}

	bool gameMap_wild::GetisStageClear()
	{
		return isStageClear;
	}

	void gameMap_wild::DropItem(int x, int y)
	{
		int num;
		num = (rand() % 9);
		switch (num)
		{
		case 0:
			allItem.push_back(new Item_Bronze_Coin(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 1:
			allItem.push_back(new Item_Silver_Coin(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 2:
			allItem.push_back(new Item_Golden_Coin(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 3:
			allItem.push_back(new Item_Fire_Stone(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 4:
			allItem.push_back(new Item_RedPot_Small(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 5:
			allItem.push_back(new Item_RedPot_Full(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 6:
			allItem.push_back(new Item_RedPot_Stone(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		case 7:
			allItem.push_back(new Item_Shurikan(this, x, y, ItemExistTime));
			allItem.back()->LoadBitmap();
			break;
		default:
			break;
		}

	}

	
	void gameMap_wild::MapObjectInteration()
	{
		for (vector<MapObject*>::iterator i = allObject.begin(); i != allObject.end(); i++)
		{
			for (vector<MapObject*>::iterator j = i; j != allObject.end(); j++)
			{
				if (((*i)->GetInterationCode() == ((-1)*(*j)->GetInterationCode())) && (*i)->GetControl())
				{
					(*j)->SetState((*i)->GetState());
				}
			}
		}
	}
	

	/////////////////////////////////////////////////////////////////////////////
	// Item: Item base class
	/////////////////////////////////////////////////////////////////////////////
	Item::Item(gameMap* point, int nx, int ny, int ExistTime)
	{
		x = nx;
		y = ny;
		currentMap = point;
		animation.SetDelayCount(3);
	}

	//Item::~Item() {}

	int Item::GetX1()
	{
		return x;
	}

	int Item::GetY1()
	{
		return y;
	}

	int Item::GetX2()
	{
		return x + animation.Width();
	}

	int Item::GetY2()
	{
		return y + animation.Height();
	}

	bool Item::isDelete()
	{
		if (ExistTime == 0)
		{
			return true;
		}
		return false;
	}

	void Item::OnMove()
	{
		animation.OnMove();
		if (ExistTime > 0) ExistTime--;
	}
	void Item::OnShow()
	{
		animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		animation.OnShow();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_Bronze_Coin : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_Bronze_Coin::Item_Bronze_Coin(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_Bronze_Coin::~Item_Bronze_Coin() {}

	void Item_Bronze_Coin::LoadBitmap()
	{
		animation.AddBitmap(IDB_BRONZECOIN_0, RGB(0, 162, 232));
		animation.AddBitmap(IDB_BRONZECOIN_1, RGB(0, 162, 232));
		animation.AddBitmap(IDB_BRONZECOIN_2, RGB(0, 162, 232));
		animation.AddBitmap(IDB_BRONZECOIN_3, RGB(0, 162, 232));

	}
	
	int Item_Bronze_Coin::GetItemValue()
	{
		return 10;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_Silver_Coin : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_Silver_Coin::Item_Silver_Coin(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_Silver_Coin::~Item_Silver_Coin() {}

	void Item_Silver_Coin::LoadBitmap()
	{
		animation.AddBitmap(IDB_SILVERCOIN_0, RGB(0, 162, 232));
		animation.AddBitmap(IDB_SILVERCOIN_1, RGB(0, 162, 232));
		animation.AddBitmap(IDB_SILVERCOIN_2, RGB(0, 162, 232));
		animation.AddBitmap(IDB_SILVERCOIN_3, RGB(0, 162, 232));
	}

	int Item_Silver_Coin::GetItemValue()
	{
		return 30;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_Silver_Coin : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_Golden_Coin::Item_Golden_Coin(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_Golden_Coin::~Item_Golden_Coin() {}

	void Item_Golden_Coin::LoadBitmap()
	{
		animation.AddBitmap(IDB_GOLDCOIN_0, RGB(0, 162, 232));
		animation.AddBitmap(IDB_GOLDCOIN_1, RGB(0, 162, 232));
		animation.AddBitmap(IDB_GOLDCOIN_2, RGB(0, 162, 232));
		animation.AddBitmap(IDB_GOLDCOIN_3, RGB(0, 162, 232));
	}

	int Item_Golden_Coin::GetItemValue()
	{
		return 100;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_Fire_Stone : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_Fire_Stone::Item_Fire_Stone(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_Fire_Stone::~Item_Fire_Stone() {}

	void Item_Fire_Stone::LoadBitmap()
	{
		animation.AddBitmap(IDB_FIRESTONE, RGB(255, 255, 255));
	}

	int Item_Fire_Stone::GetItemValue()
	{
		return 1;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Item_Redpot_Stone : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_RedPot_Stone::Item_RedPot_Stone(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_RedPot_Stone::~Item_RedPot_Stone() {}

	void Item_RedPot_Stone::LoadBitmap()
	{
		animation.AddBitmap(IDB_REDSTONE, RGB(63, 72, 204));
	}

	int Item_RedPot_Stone::GetItemValue()
	{
		return 9;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_RedPot_Small : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_RedPot_Small::Item_RedPot_Small(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_RedPot_Small::~Item_RedPot_Small() {}

	void Item_RedPot_Small::LoadBitmap()
	{
		animation.AddBitmap(IDB_REDPOTSMALL, RGB(63, 72, 204));
	}

	int Item_RedPot_Small::GetItemValue()
	{
		return 2;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_RedPot_Medium : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_RedPot_Medium::Item_RedPot_Medium(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_RedPot_Medium::~Item_RedPot_Medium() {}

	void Item_RedPot_Medium::LoadBitmap()
	{
		animation.AddBitmap(IDB_REDPOTMEDIUM, RGB(63, 72, 204));
	}

	int Item_RedPot_Medium::GetItemValue()
	{
		return 3;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_RedPot_Full : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_RedPot_Full::Item_RedPot_Full(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_RedPot_Full::~Item_RedPot_Full() {}

	void Item_RedPot_Full::LoadBitmap()
	{
		animation.AddBitmap(IDB_REDPOTFULL, RGB(63, 72, 204));
	}

	int Item_RedPot_Full::GetItemValue()
	{
		return 4;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item_Shurikan : Item class
	/////////////////////////////////////////////////////////////////////////////
	Item_Shurikan::Item_Shurikan(gameMap* point, int nx, int ny, int ExistTime) : Item(point, nx, ny, ExistTime) {}

	Item_Shurikan::~Item_Shurikan() {}

	void Item_Shurikan::LoadBitmap()
	{
		animation.AddBitmap(IDB_ITEMSHURIKAN, RGB(63, 72, 204));
	}

	int Item_Shurikan::GetItemValue()
	{
		return 5;
	}

	/////////////////////////////////////////////////////////////////////////////
	// MapObject
	/////////////////////////////////////////////////////////////////////////////
	MapObject::MapObject(gameMap* point, int nx, int ny, bool InitialState, int SetInterationCode)
	{
		x = nx;
		y = ny;
		currentMap = point;
		InterationCode = SetInterationCode;
		ObjectState = InitialState;
	}

	int MapObject::GetObjectType()
	{
		return ObjectType;
	}

	bool MapObject::GetControl()
	{
		return Control;
	}
	/////////////////////////////////////////////////////////////////////////////
	// class Switch : class MapObject
	/////////////////////////////////////////////////////////////////////////////
	Switch::Switch(gameMap* point, int nx, int ny, bool InitialState , int SetInterationCode) : MapObject(point, nx, ny, InitialState, SetInterationCode) 
	{
		ObjectType = 0;
		Control = true;
	}
	
	Switch::~Switch(){}

	int Switch::GetX1()
	{
		return x;
	}

	int Switch::GetY1()
	{
		return y;
	}

	int Switch::GetX2()
	{
		return x + SwitchOff.Width();
	}

	int Switch::GetY2()
	{
		return y + SwitchOff.Height();
	}

	void Switch::LoadBitmap()
	{
		SwitchOff.LoadBitmap(IDB_SWITCH_OFF, RGB(255, 255, 255));
		SwitchOn.LoadBitmap(IDB_SWITCH_ON, RGB(255, 255, 255));
	}

	void Switch::OnMove()
	{
		if (GetHitDelayCount > 0) GetHitDelayCount--;
	}

	void Switch::OnShow()
	{
		if (ObjectState)
		{
			SwitchOn.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			SwitchOn.ShowBitmap();
		}
		else
		{
			SwitchOff.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			SwitchOff.ShowBitmap();
		}
	}
	void Switch::GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2)
	{
		if ((GetX2() >= HeroX1) && (HeroX2 >= GetX1()) && (GetY2() >= HeroY1) && (HeroY2 >= GetY1()) && GetHitDelayCount == 0)
		{
			ObjectState = !ObjectState;
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
		}
	}
	int Switch::GetInterationCode()
	{
		return InterationCode;
	}

	void Switch::SetState(bool State)
	{
		ObjectState = State;
	}

	bool Switch::GetState()
	{
		return ObjectState;
	}

	void Switch::AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP)
	{
	}
	/////////////////////////////////////////////////////////////////////////////
	// class Spike : class MapObject
	/////////////////////////////////////////////////////////////////////////////
	Spike::Spike(gameMap* point, int nx, int ny, bool InitialState , int SetInterationCode) : MapObject(point, nx, ny, InitialState, SetInterationCode) 
	{
		ObjectType = 1;
		Control = false;
	}

	Spike::~Spike() {}

	int Spike::GetX1()
	{
		return x;
	}

	int Spike::GetY1()
	{
		return y;
	}

	int Spike::GetX2()
	{
		return x + SpikeUp.Width();
	}

	int Spike::GetY2()
	{
		return y + SpikeUp.Height();
	}

	void Spike::LoadBitmap()
	{
		SpikeDown.LoadBitmap(IDB_POPUPSPIKE_3, RGB(63, 72, 204));
		SpikeUp.LoadBitmap(IDB_POPUPSPIKE_0, RGB(63, 72, 204));
	}

	void Spike::OnMove()
	{
	}

	void Spike::OnShow()
	{
		if (ObjectState)
		{
			SpikeUp.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			SpikeUp.ShowBitmap();
		}
		else
		{
			SpikeDown.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			SpikeDown.ShowBitmap();
		}
	}
	void Spike::GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2)
	{
	}
	int Spike::GetInterationCode()
	{
		return InterationCode;
	}

	void Spike::SetState(bool State)
	{
		ObjectState = State;
	}

	bool Spike::GetState()
	{
		return ObjectState;
	}
	void Spike::AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP)
	{
		if ((GetX2() >= HeroX1) && (HeroX2 >= GetX1()) && (GetY2() >= HeroY1) && (HeroY2 >= GetY1()) && ObjectState)
		{
			*heroHP -= SpikeDamage;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// class Door : class MapObject
	/////////////////////////////////////////////////////////////////////////////
	Door::Door(gameMap* point, int nx, int ny, bool InitialState, int SetInterationCode) : MapObject(point, nx, ny, InitialState, SetInterationCode) 
	{
		InitialY = ny;
		ObjectType = 2;
		Control = false;
	}

	Door::~Door() {}

	int Door::GetX1()
	{
		return x;
	}

	int Door::GetY1()
	{
		return y;
	}

	int Door::GetX2()
	{
		return x + DoorPicture.Width();
	}

	int Door::GetY2()
	{
		return y + DoorPicture.Height();
	}

	void Door::LoadBitmap()
	{
		DoorPicture.LoadBitmap(IDB_DOOR, RGB(255, 255, 255));
	}

	void Door::OnMove()
	{
		if (ObjectState)
		{
			y = InitialY;
		}
		else
		{
			y = InitialY - 100;
		}
	}

	void Door::OnShow()
	{
		DoorPicture.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		DoorPicture.ShowBitmap();
	}
	void Door::GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2)
	{
	}
	int Door::GetInterationCode()
	{
		return InterationCode;
	}

	void Door::SetState(bool State)
	{
		ObjectState = State;
	}

	bool Door::GetState()
	{
		return ObjectState;
	}
	void Door::AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP)
	{
	}
}