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
		:X(0), Y(0), MW(32), MH(32), sx(0), sy(475)  //�Ϥ���32*32 ��l�ù��e�����0,475
	{
		for (int i = 0; i < 48; i++)//48��
		{
			for (int j = 0; j < 64; j++)
			{
				if (j == 48)          //�a��
				{
					map[i][j] = 1;    //��a
				}
				else if (j > 48)
				{
					map[i][j] = 2;    //�g
				}
				else
				{
					map[i][j] = 0;    //�Ů�
				}
			}
		}
	}
	void gameMap::SetSXSY(int x, int y)   //�]�w
	{
		sx = x;
		sy = y;
	}
	bool gameMap::isSpace(int x, int y)   // (x, y) ���a�Ϫ��I�y��
	{
		int gx = x / 16; // �ഫ����y��(��ư��k)
		int gy = y / 16;  // �ഫ����y��(��ư��k) 
		return map[gx][gy] == 0;
	}
	void gameMap::LoadBitmap()
	{
		ground.LoadBitmap(IDB_MAPGROUND, RGB(255, 255, 255));//���J��a�Ϯ�
		ground1.LoadBitmap(IDB_MAPGROUND1);//���J�d�g�Ϯ�
	}
	void gameMap::OnShow()
	{
		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
				int x = i * 16 - sx; // ��X��(i, j)�o�@�檺 x �ù��y��
				int y = j * 16 - sy; // ��X��(i, j)�o�@�檺 y �ù��y��
				switch (map[i][j])
				{
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
}