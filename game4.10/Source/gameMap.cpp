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
		:X(0), Y(0), MW(MIN_MAP_SIZE), MH(MIN_MAP_SIZE), sx(0), sy(0)  //�Ϥ���10*10 ��l�ù��e�����0,475
	{
		/*for (int i = 0; i < 48; i++)//48��
		{
			for (int j = 0; j < 64; j++)
			{
				if (i == 32)          //�a��
				{
					map[i][j] = 1;    //��a
				}
				else if (i > 32)
				{
					map[i][j] = 2;    //�g
				}
				else
				{
					map[i][j] = 0;    //�Ů�
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
	void gameMap::SetSXSY(int x, int y)   //�]�w
	{
		sx = x;
		sy = y;

	}
	bool gameMap::isSpace(int x, int y)   // (x, y) ���a�Ϫ��I�y��
	{
		int gx = x / MIN_MAP_SIZE; // �ഫ����y��(��ư��k)
		int gy = y / MIN_MAP_SIZE;  // �ഫ����y��(��ư��k) 
		//CreatTxt();
		if (map[gy][gx] == 0) return true;
		else return false;
		//return map[gx][gy] == 0;
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
		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
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

	int gameMap::ScreenX(int x) // x ���a�Ϫ��I�y��
	{
		return x - sx; // �^�ǿù��� x �I�y��
		//return x - sx + SIZE_X / 2; // �^�ǿù��� x �I�y��
	}
	int gameMap::ScreenY(int y) // y ���a�Ϫ� y �I�y��
	{
		return y - sy; // �^�ǿù����I�y��
		//return y - sy + SIZE_Y / 2; // �^�ǿù����I�y��
	}

	void gameMap::CreatTxt()//�إ�txt�ɮ�
	{
		fstream file;      //�ŧifstream����
		fstream file2;
		fstream file3;
		file.open("testMap.txt", ios::out | ios::trunc);
		file2.open("testMapBool.txt", ios::out | ios::trunc);
		file3.open("testMapAll.txt", ios::out | ios::trunc);

		//�}���ɮ׬���X���A�A�Y�ɮפw�s�b�h�M���ɮפ��e���s�g�J

		for (int i = 0; i < 48; i++) {
			for (int j = 0; j < 64; j++) {
				file << map[i][j];		//�Nstr�g�J�ɮ�
				if (map[i][j] == 0) file2 << ' ';
				else file2 << 'X';
			}
			file << endl;
			file2 << endl;
		}

		file.close();       //�����ɮ�
		file2.close();

		for (int i = 0; i < 480; i++) {
			for (int j = 0; j < 640; j++) {
				file3 << isSpace(j, i);		//�Nstr�g�J�ɮ�
			}
			file3 << endl;
		}
		file3.close();
	}
}