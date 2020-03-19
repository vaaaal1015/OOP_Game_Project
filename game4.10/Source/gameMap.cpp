#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameMap.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace game_framework {
	gameMap::gameMap()
		:X(0), Y(0), MW(MIN_MAP_SIZE), MH(MIN_MAP_SIZE), sx(0), sy(0)  //圖片為10*10 初始螢幕畫面位於0,475
	{
		/*for (int i = 0; i < 48; i++)//48格
		{
			for (int j = 0; j < 64; j++)
			{
				if (i == 32)          //地表
				{
					map[i][j] = 1;    //草地
				}
				else if (i > 32)
				{
					map[i][j] = 2;    //土
				}
				else
				{
					map[i][j] = 0;    //空氣
				}
			}
		}*/
		fstream mapFile;
		mapFile.open("MapFile.txt", ios::in);
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
		CreatTxt();
	}
	void gameMap::SetSXSY(int x, int y)   //設定
	{
		sx = x;
		sy = y;

	}
	bool gameMap::isSpace(int x, int y)   // (x, y) 為地圖的點座標
	{
		int gx = x / MIN_MAP_SIZE; // 轉換為格座標(整數除法)
		int gy = y / MIN_MAP_SIZE;  // 轉換為格座標(整數除法) 
		//CreatTxt();
		if (map[gy][gx] == 0) return true;
		else return false;
		//return map[gx][gy] == 0;
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
		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
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
	}

	int gameMap::ScreenX(int x) // x 為地圖的點座標
	{
		return x - sx; // 回傳螢幕的 x 點座標
		//return x - sx + SIZE_X / 2; // 回傳螢幕的 x 點座標
	}
	int gameMap::ScreenY(int y) // y 為地圖的 y 點座標
	{
		return y - sy; // 回傳螢幕的點座標
		//return y - sy + SIZE_Y / 2; // 回傳螢幕的點座標
	}

	void gameMap::CreatTxt()//建立txt檔案
	{
		fstream file;      //宣告fstream物件
		fstream file2;
		fstream file3;
		file.open("testMap.txt", ios::out | ios::trunc);
		file2.open("testMapBool.txt", ios::out | ios::trunc);
		file3.open("testMapAll.txt", ios::out | ios::trunc);

		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入

		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
				file << map[i][j];		//將str寫入檔案
				if (map[i][j] == 0) file2 << ' ';
				else file2 << 'X';
			}
			file << endl;
			file2 << endl;
		}

		file.close();       //關閉檔案
		file2.close();

		for (int i = 0; i < 480; i++) {
			for (int j = 0; j < 640; j++) {
				file3 << isSpace(j, i);		//將str寫入檔案
			}
			file3 << endl;
		}
		file3.close();
	}
}