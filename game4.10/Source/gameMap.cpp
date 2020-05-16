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
	gameMap::gameMap(string fileName) : X(0), Y(0), MW(MIN_MAP_SIZE), MH(MIN_MAP_SIZE), sx(0), sy(0)  //�Ϥ���10*10 ��l�ù��e�����0,475
	{
		fstream mapFile;
		string mapLocation = "Map\\" + fileName;
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
					j++;
				}
				else
				{
					map[i][count] = (int)buffer[j] - (int)48;
					count++;
					j++;
				}
			}
			i += 1;
		}
	}

	gameMap::~gameMap()
	{

	}

	void gameMap::SetSXSY(int x, int y)   // �]�w
	{
		sx = x;
		sy = y;
	}

	bool gameMap::isSpace(int x, int y)   // (x, y)���a�Ϫ��I�y��
	{
		int gx = x / MIN_MAP_SIZE; // �ഫ����y��(��ư��k)
		int gy = y / MIN_MAP_SIZE;  // �ഫ����y��(��ư��k) 

		if (map[gy][gx] == 0) return true;
		else return false;
	}

	int gameMap::GetBlockY(int y)
	{
		int gy = y / MIN_MAP_SIZE;  // �ഫ����y��(��ư��k)
		return gy * MH;
	}

	int gameMap::ScreenX(int x) // x ���a�Ϫ��I�y��
	{
		return x - sx; // �^�ǿù��� x �I�y��
	}

	int gameMap::ScreenY(int y) // y ���a�Ϫ� y �I�y��
	{
		return y - sy; // �^�ǿù����I�y��
	}

	void gameMap::LoadBitmap()
	{
		ground.LoadBitmap(IDB_MAPGROUND, RGB(255, 255, 255));//���J��a�Ϯ�
		ground1.LoadBitmap(IDB_MAPGROUND1);//���J�d�g�Ϯ�
		ground2.LoadBitmap(IDB_MAPSLIDE1, RGB(255, 255, 255));//���J�שY1�Ϯ�
		ground3.LoadBitmap(IDB_MAPSLIDE2);//���J�שY2�Ϯ�
	}

	void gameMap::OnShow()
	{
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 96; j++) {
				int x = j * MW - sx; // ��X��(i, j)�o�@�檺 x �ù��y��
				int y = i * MH - sy; // ��X��(i, j)�o�@�檺 y �ù��y��
				switch (map[i][j])
				{
				case 4:
					ground3.SetTopLeft(x, y); // ���w��(i, j)�o�@�檺�y��
					ground3.ShowBitmap();   //��ܱשY2�Ϯ�
					break;
				case 3:
					ground2.SetTopLeft(x, y); // ���w��(i, j)�o�@�檺�y��
					ground2.ShowBitmap();   //��ܱשY1�Ϯ�
					break;
				case 2:
					ground1.SetTopLeft(x, y); // ���w��(i, j)�o�@�檺�y��
					ground1.ShowBitmap();   //��ܤg�a�Ϯ�
					break;

				case 1:
					ground.SetTopLeft(x, y); // ���w��(i, j)�o�@�檺�y��
					ground.ShowBitmap();    //��ܯ�a�Ϯ�
					break;
				case 0:
					break;
				}
			}
		}
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

	void gameMap_village::HeroTalkToNPC(bool flag)
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
			(*i)->SetIsTalkingToHero(flag);
	}

	void gameMap_village::setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level)
	{
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->SetHeroState(x1, x2, y1, y2, HP, Gold, AttackDamage, Level);
	}

	bool gameMap_village::GetHeroIsTalkingToNPC()
	{
		return HeroIsTalkingToNPC;
	}

	/////////////////////////////////////////////////////////////////////////////
	// gameMap_wild : gameMap_wild class
	/////////////////////////////////////////////////////////////////////////////
	gameMap_wild::gameMap_wild(string fileName) : gameMap(fileName) {}

	

	/////////////////////////////////////////////////////////////////////////////
	// gameMap_Lv1 : gameMap_Lv1 class
	/////////////////////////////////////////////////////////////////////////////
	gameMap_Lv1::gameMap_Lv1() : gameMap_wild("level_1.txt")
	{
		unsigned seed;
		seed = (unsigned)time(NULL);
		srand(seed);
		allEnemy.push_back(new CEnemy_sunFlower(this, 300, 350));
		allEnemy.push_back(new CEnemy_Statue(this, 2950, 325));
		allEnemy.push_back(new CEnemy_Cactus(this, 500, 280));
		allEnemy.push_back(new CEnemy_Cactus(this, 1000, 210));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1500, 350));
		allEnemy.push_back(new CEnemy_Cactus(this, 1650, 20));
		allEnemy.push_back(new CEnemy_sunFlower(this, 400, 350));
		allEnemy.push_back(new CEnemy_sunFlower(this, 1550, 350));
	
	}

	gameMap_Lv1::~gameMap_Lv1()
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) delete (*i);
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) delete (*i);
	}

	void gameMap_Lv1::LoadBitmap()
	{
		gameMap::LoadBitmap();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->LoadBitmap();
	}

	void gameMap_Lv1::OnShow()
	{
		gameMap::OnShow();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnShow();
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) (*i)->OnShow();

	}

	void gameMap_Lv1::OnMove() {

		vector<CEnemy*>::iterator iter = allEnemy.begin();
		while (iter != allEnemy.end())         //�ĤH���`�|�qvector�̳Q�R��
		{
			if ((*iter)->isDead() && (*iter)->GetEnemyType() == "Statue") isStageClear = true;   //�q������
			if ((*iter)->isDead() && (*iter)->GetEnemyType() != "Statue")   //�J���H�~���ĤH�Q����
			{
				DropItem(((*iter)->GetX1()+(*iter)->GetX2()) / 2, ((*iter)->GetY1() + (*iter)->GetY2()) / 2);
				delete *iter;
				iter = allEnemy.erase(iter);
				//isStageClear = true;   //�q������
			}
			else
				iter++;
		}
		for (vector<Item*>::iterator i = allItem.begin(); i != allItem.end(); i++) (*i)->OnMove();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnMove();
	}

	void gameMap_Lv1::setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroXY(x1, x2, y1, y2);
	}

	void gameMap_Lv1::SetHeroAttackRange(int x1, int x2, int y1, int y2)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroAttackRange(x1, x2, y1, y2);
	}

	void gameMap_Lv1::AttackByHero(const int damage)		// ����
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->GetAttack(damage);
	}

	void gameMap_Lv1::AttackByEnemy(int *heroHP)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->AttackByEnemy(heroHP);
	}

	void gameMap_Lv1::HeroGetCoin(int *HeroCoin)
	{
		vector<Item*>::iterator iter = allItem.begin();
		while (iter != allItem.end())
		{
			if (((*iter)->GetX2() >= HeroX1) && (HeroX2 >= (*iter)->GetX1()) && ((*iter)->GetY2() >= HeroY1) && (HeroY2 >= (*iter)->GetY1()))
			{
				CAudio::Instance()->Play(5, false);
				*HeroCoin += (*iter)->GetItemValue();
				delete *iter;
				iter = allItem.erase(iter);
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


	bool gameMap_Lv1::GetisStageClear()
	{
		return isStageClear;
	}

	void gameMap_Lv1::DropItem(int x, int y)
	{
		int num;
		num = (rand() % 4);
		switch (num)
		{
		case 0:
			allItem.push_back(new Item_Bronze_Coin(this, x, y));
			allItem.back()->LoadBitmap();
			break;
		case 1:
			allItem.push_back(new Item_Silver_Coin(this, x, y));
			allItem.back()->LoadBitmap();
			break;
		case 2:
			allItem.push_back(new Item_Golden_Coin(this, x, y));
			allItem.back()->LoadBitmap();
			break;
		default:
			break;
		}

	}

	void gameMap_Lv1::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		HeroX1 = x1;
		HeroY1 = y1;
		HeroX2 = x2;
		HeroY2 = y2;
	}
	/////////////////////////////////////////////////////////////////////////////
	// gameMap_Lv2 : gameMap_Lv2 class
	/////////////////////////////////////////////////////////////////////////////
	gameMap_Lv2::gameMap_Lv2() : gameMap_wild("level_2.txt")
	{
		allEnemy.push_back(new CEnemy_sunFlower(this, 1000, 350));
		allEnemy.push_back(new CEnemy_Statue(this, 2950, 325));
	}

	gameMap_Lv2::~gameMap_Lv2()
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) delete (*i);
	}

	void gameMap_Lv2::LoadBitmap()
	{
		gameMap::LoadBitmap();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->LoadBitmap();
	}

	void gameMap_Lv2::OnShow()
	{
		gameMap::OnShow();
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnShow();
	}

	void gameMap_Lv2::OnMove() {

		vector<CEnemy*>::iterator iter = allEnemy.begin();
		while (iter != allEnemy.end())         //�ĤH���`�|�qvector�̳Q�R��
		{
			if ((*iter)->isDead() && (*iter)->GetEnemyType() == "Statue") isStageClear = true;   //�q������
			if ((*iter)->isDead() && (*iter)->GetEnemyType() != "Statue")   //�J���H�~���ĤH�Q����
			{
				delete *iter;
				iter = allEnemy.erase(iter);
				//isStageClear = true;   //�q������
			}
			else
				iter++;
		}

		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnMove();
	}

	void gameMap_Lv2::setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroXY(x1, x2, y1, y2);
	}

	void gameMap_Lv2::SetHeroAttackRange(int x1, int x2, int y1, int y2)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->SetHeroAttackRange(x1, x2, y1, y2);
	}

	void gameMap_Lv2::AttackByHero(const int damage)		// ����
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->GetAttack(damage);
	}

	void gameMap_Lv2::AttackByEnemy(int *heroHP)
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->AttackByEnemy(heroHP);
	}

	bool gameMap_Lv2::GetisStageClear()
	{
		return isStageClear;
	}
	
	void gameMap_Lv2::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		HeroX1 = x1;
		HeroY1 = y1;
		HeroX2 = x2;
		HeroY2 = y2;
	}
	void gameMap_Lv2::HeroGetCoin(int *HeroCoin)
	{
		vector<Item*>::iterator iter = allItem.begin();
		while (iter != allItem.end())
		{
			if (((*iter)->GetX2() >= HeroX1) && (HeroX2 >= (*iter)->GetX1()) && ((*iter)->GetY2() >= HeroY1) && (HeroY2 >= (*iter)->GetY1()))
			{
				TRACE("PLAy\n");
				CAudio::Instance()->Play(5, false);	
				*HeroCoin += (*iter)->GetItemValue();
				delete *iter;
				iter = allItem.erase(iter);
			}
			else
				iter++;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Item: Item base class
	/////////////////////////////////////////////////////////////////////////////
	Item::Item(gameMap* point, int nx, int ny)
	{
		x = nx;
		y = ny;
		currentMap = point;
		animation.SetDelayCount(3);
	}

	Item::~Item() {}

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
	Item_Bronze_Coin::Item_Bronze_Coin(gameMap* point, int nx, int ny) : Item(point, nx, ny) {}

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
	Item_Silver_Coin::Item_Silver_Coin(gameMap* point, int nx, int ny) : Item(point, nx, ny) {}

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
	Item_Golden_Coin::Item_Golden_Coin(gameMap* point, int nx, int ny) : Item(point, nx, ny) {}

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
}