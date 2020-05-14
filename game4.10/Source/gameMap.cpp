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
		allEnemy.push_back(new CEnemy_sunFlower(this, 300, 350));
		allEnemy.push_back(new CEnemy_Statue(this, 2950, 325));
	}

	gameMap_Lv1::~gameMap_Lv1()
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) delete (*i);
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
	}

	void gameMap_Lv1::OnMove() {

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

	bool gameMap_Lv1::GetisStageClear()
	{
		return isStageClear;
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

}