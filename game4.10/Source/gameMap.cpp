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
	gameMap::gameMap(string fileName ,int EnemyNumber, int NPCNumber)
		:X(0), Y(0), MW(MIN_MAP_SIZE), MH(MIN_MAP_SIZE), sx(0), sy(0)  //圖片為10*10 初始螢幕畫面位於0,475
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
		for (int i = 0; i < EnemyNumber; i++) allEnemy.push_back(new CEnemy(this));
		for (int i = 0; i < NPCNumber; i++) allNPC.push_back(new NPC(this));
	}

	gameMap::~gameMap()
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) delete (*i);
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) delete (*i);
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

		if (map[gy][gx] == 0) return true;
		else return false;
	}
	int gameMap::GetBlockY(int y)
	{
		int gy = y / MIN_MAP_SIZE;  // 轉換為格座標(整數除法)
		return gy * MH;
	}

	void gameMap::LoadBitmap()
	{
		ground.LoadBitmap(IDB_MAPGROUND, RGB(255, 255, 255));//載入草地圖案
		ground1.LoadBitmap(IDB_MAPGROUND1);//載入泥土圖案
		ground2.LoadBitmap(IDB_MAPSLIDE1, RGB(255, 255, 255));//載入斜坡1圖案
		ground3.LoadBitmap(IDB_MAPSLIDE2);//載入斜坡2圖案
		
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->LoadBitmap();
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
		{
			(*i)->LoadBitmap();
		}
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
				}
			}
		}
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnShow();
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
		{
			(*i)->OnShow();
		}
	}

	void gameMap::AttackByHero(int x1, int x2, int y1, int y2, int damage)		// 攻擊
	{
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->GetAttack(x1, x2, y1, y2, damage);
	}

	void gameMap::OnMove() {
		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++) (*i)->OnMove();
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++) (*i)->OnMove();
		
	}

	int gameMap::ScreenX(int x) // x 為地圖的點座標
	{
		return x - sx; // 回傳螢幕的 x 點座標
	}
	int gameMap::ScreenY(int y) // y 為地圖的 y 點座標
	{
		return y - sy; // 回傳螢幕的點座標
	}

	int gameMap::AttackByEnemy(int x1, int x2, int y1, int y2)
	{
		int damage = 0;

		for (vector<CEnemy*>::iterator i = allEnemy.begin(); i != allEnemy.end(); i++)
			damage += (*i)->AttackByEnemy(x1, x2, y1, y2);
		return damage;
	}

	int gameMap::HeroTouchNPC(int x1, int x2, int y1, int y2)
	{
		int num = 0;
		for (vector<NPC*>::iterator i = allNPC.begin(); i != allNPC.end(); i++)
		{
			if ((*i)->TouchedByHero(x1, x2, y1, y2))
			{
				return num;
			}
			num += 1;
		}
		return -1;
	}


	void gameMap::SetEnemyPosition(int EnemyNumber, int EnemyX, int EnemyY)
	{
		GAME_ASSERT((int)allEnemy.size() > EnemyNumber, "CHero: SetMap input index over range");
		allEnemy[EnemyNumber]->SetXY(EnemyX, EnemyY);
	}

	void gameMap::SetNPCPosition(int NPCNumber, int NPCX, int NPCY)
	{
		GAME_ASSERT((int)allNPC.size() > NPCNumber, "CHero: SetMap input index over range");
		allNPC[NPCNumber]->SetXY(NPCX, NPCY);
	}

	void gameMap::SetDifferentNPC(int NPCNumber, int DifferentNPC)
	{
		GAME_ASSERT((int)allNPC.size() > NPCNumber, "CHero: SetMap input index over range");
		allNPC[NPCNumber]->SetLoadBitMapNumber(DifferentNPC);
	}
}