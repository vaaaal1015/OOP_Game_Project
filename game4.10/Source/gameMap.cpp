#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CHero.h"
#include "gameMap.h"

namespace game_framework {
	gameMap::gameMap()
		:X(0), Y(0), MW(32), MH(32), sx(0), sy(475)  //圖片為32*32 初始螢幕畫面位於0,475
	{
		for (int i = 0; i < 48; i++)//48格
		{
			for (int j = 0; j < 64; j++)
			{
				if (j == 48)          //地表
				{
					map[i][j] = 1;    //草地
				}
				else if (j > 48)
				{
					map[i][j] = 2;    //土
				}
				else
				{
					map[i][j] = 0;    //空氣
				}
			}
		}
	}
	void gameMap::SetSXSY(int x, int y)   //設定
	{
		sx = x;
		sy = y;
	}
	bool gameMap::isSpace(int x, int y)   // (x, y) 為地圖的點座標
	{
		int gx = x / 16; // 轉換為格座標(整數除法)
		int gy = y / 16;  // 轉換為格座標(整數除法) 
		return map[gx][gy] == 0;
	}
	void gameMap::LoadBitmap()
	{
		ground.LoadBitmap(IDB_MAPGROUND, RGB(255, 255, 255));//載入草地圖案
		ground1.LoadBitmap(IDB_MAPGROUND1);//載入泥土圖案
	}
	void gameMap::OnShow()
	{
		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
				int x = i * 16 - sx; // 算出第(i, j)這一格的 x 螢幕座標
				int y = j * 16 - sy; // 算出第(i, j)這一格的 y 螢幕座標
				switch (map[i][j])
				{
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
}