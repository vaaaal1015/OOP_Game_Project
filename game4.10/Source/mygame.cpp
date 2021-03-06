/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Counter.h"
#include "NPC.h"
#include "CEnemy.h"
#include "gameMap.h"
#include "CHero.h"
#include "Menu.h"
#include "GameOver.h"
#include "mygame.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////
	

	
CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
	isLoad = false;
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	menu.LoadBitmap();
	//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
	CAudio::Instance()->Load(AUDIO_MENU, "sounds\\menu.wav");	// 載入編號3的聲音load.mp3
	isLoad = true;
	CAudio::Instance()->Play(AUDIO_MENU, false);
}

void CGameStateInit::OnBeginState()
{
	if(isLoad)
		CAudio::Instance()->Play(AUDIO_MENU, false);			// 撥放 mp3
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_ENTER = 13;
	const char KEY_F1 = 0x70;	 // keyboard F1
	const char KEY_UP = 0x26; // keyboard上箭頭
	const char KEY_DOWN = 0x28; // keyboard下箭頭

	if (nChar == KEY_UP)
		menu.SetKeyUp();
	if (nChar == KEY_DOWN)
		menu.SetKeyDown();
	if (nChar == KEY_F1)
	{
		CAudio::Instance()->Stop(AUDIO_MENU);
		GotoGameState(GAME_STATE_OVER);
	}

	if (nChar == KEY_ENTER)
	{
		menu.SetKeyEnter();
		if (menu.isGameStart())
		{
			CAudio::Instance()->Stop(AUDIO_MENU);
			GotoGameState(GAME_STATE_RUN);
		}
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CGameStateInit::OnMove()
{
	menu.OnMove();
}

void CGameStateInit::OnShow()
{
	//
	// 貼上logo
	//

	menu.OnShow();
	//
	// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
	//
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	gameOver.OnMove();
}

void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Stop(AUDIO_STAGE1);			// 撥放 WAVE
	CAudio::Instance()->Stop(AUDIO_DING);		// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_GAMEOVER);
	counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	gameOver.LoadBitmap();
	CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\game_over.mp3");
	//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	gameOver.OnShow();
}

void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ENTER = 13;

	if (nChar == KEY_ENTER)
	{
		CAudio::Instance()->Stop(AUDIO_GAMEOVER);
		GotoGameState(GAME_STATE_INIT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	const int BALL_GAP = 90;
	const int BALL_XY_OFFSET = 45;
	const int BALL_PER_ROW = 7;
	const int HITS_LEFT = 10;
	const int HERO_LIFEBAR_X = 0;
	const int HERO_LIFEBAR_Y = 0;
	const int BACKGROUND_X = 0;
	const int ANIMATION_SPEED = 15;
	hero.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// 設定背景的起始座標
	help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
	CAudio::Instance()->Play(AUDIO_STAGE1, true);			// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
	CAudio::Instance()->Stop(AUDIO_MENU);
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	hero.OnMove();
	if (!hero.isAlive())
	{
		GotoGameState(GAME_STATE_OVER);
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%

	//
	// 開始載入資料
	//

	hero.LoadBitmap();
	background.LoadBitmap(IDB_MAPBACKGROUND);					// 載入背景的圖形
	//
	// 完成部分Loading動作，提高進度
	//
	ShowInitProgress(50);
	//
	// 繼續載入其他資料
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 載入說明的圖形
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
	CAudio::Instance()->Load(AUDIO_GETITEM,  "sounds\\collect_item.wav");	// 載入編號5的聲音getitem
	CAudio::Instance()->Load(AUDIO_SUPERCAR, "sounds\\super_car.mp3");	// 載入編號4的聲音ntut.mid
	CAudio::Instance()->Load(AUDIO_GAMESTART, "sounds\\stage_start.wav");
	CAudio::Instance()->Load(AUDIO_STAGECLEAR, "sounds\\stage_clear.wav");
	CAudio::Instance()->Load(AUDIO_STAGE1, "sounds\\stage1.wav");
	CAudio::Instance()->Load(AUDIO_CACTUSGETHIT, "sounds\\damaged_cactus.wav");
	CAudio::Instance()->Load(AUDIO_CACTUSATTACK, "sounds\\cactus_attack.wav");
	CAudio::Instance()->Load(AUDIO_HIT_8, "sounds\\hit_8.wav");
	CAudio::Instance()->Load(AUDIO_HIT_10, "sounds\\hit_10.wav");
	CAudio::Instance()->Load(AUDIO_ROBOT_DAMAGE, "sounds\\robot_damaged.wav");
	CAudio::Instance()->Load(AUDIO_ROBOT_A_ATTACK, "sounds\\robot_A_attack_1.wav");
	CAudio::Instance()->Load(AUDIO_ROBOT_DIE, "sounds\\robot_die_bomb.wav");
	CAudio::Instance()->Load(AUDIO_CLOUD_ATTACK, "sounds\\cloudy_cloud_attack.wav");
	CAudio::Instance()->Load(AUDIO_PIGEON_ATTACK_1, "sounds\\pigeon_fireball_enable.wav");
	CAudio::Instance()->Load(AUDIO_PIGEON_ATTACK_2, "sounds\\pigeon_fireball_disable.wav");
	CAudio::Instance()->Load(AUDIO_BOMB, "sounds\\bomb.wav");
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_E = 0x45;	 // keyboard E
	const char KEY_X = 0x58;	 // keyboard X
	const char KEY_F1 = 0x70;	 // keyboard F1
	const char KEY_F2 = 0x71;	 // keyboard F2
	const char KEY_F3 = 0x72;	 // keyboard F3
	const char KEY_Z = 0x5A;
	const char KEY_C = 0x43;
	const char KEY_D = 0x44;
	if(nChar == KEY_LEFT)
		hero.SetMovingLeft(true);
	if (nChar == KEY_RIGHT)    //英雄不能取消翻滾動作
		hero.SetMovingRight(true);
	if (nChar == KEY_UP)
		hero.SetMovingUp(true);
	if (nChar == KEY_DOWN)
		hero.SetMovingDown(true);
	if (nChar == KEY_X)
		hero.SetHeroAttack(true);
	if (nChar == KEY_Z)
		hero.SetHeroRoll(true);
	if (nChar == KEY_E)
		hero.SetTalkingToNPC(true);
	if (nChar == KEY_C)
		hero.SetHeroThrow(true);
	if (nChar == KEY_D)
		hero.AddHeroGold();
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25; // keyboard左箭頭
	const char KEY_UP = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN = 0x28; // keyboard下箭頭
	const char KEY_X = 0x58;	 // keyboard X
	const char KEY_E = 0x45;	 // keyboard E
	if (nChar == KEY_LEFT)
	{
		hero.SetMovingLeft(false);
	}
	if (nChar == KEY_RIGHT)
	{
		hero.SetMovingRight(false);
	}
	if (nChar == KEY_DOWN)
		hero.SetMovingDown(false);

}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	hero.OnLButtonDown(point.x, point.y);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}


void CGameStateRun::OnShow()
{
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	background.ShowBitmap();// 貼上背景圖
	help.ShowBitmap();					// 貼上說明圖
	hero.OnShow();			// 貼上英雄

}


}