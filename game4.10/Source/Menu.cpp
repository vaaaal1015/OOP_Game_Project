#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "Counter.h"
#include "Menu.h"

namespace game_framework {

	Menu::Menu()
	{
		jumpCounter.SetDelayCount(50);
		rising = true;
		state = LIST;
		selection = 0;
		Word_selection.SetDelayCount(2);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		animation.SetDelayCount(5);
	}

	void Menu::LoadBitmap()
	{
		logo.LoadBitmap(IDB_GREATSWORDLOGO, RGB(255, 255, 255));
		Word_start.LoadBitmap(IDB_WORD_START);
		Word_member.LoadBitmap(IDB_WORD_MEMBER);
		Word_teaching.LoadBitmap(IDB_WORD_TEACHING);
		Word_end.LoadBitmap(IDB_WORD_END);
		Word_memberList.LoadBitmap(IDB_WORD_MEMBER_LIST);
		key_up.LoadBitmap(IDB_KEY_UP);
		key_left.LoadBitmap(IDB_KEY_LEFT);
		key_right.LoadBitmap(IDB_KEY_RIGHT);
		background.LoadBitmap(IDB_MAPBACKGROUND);

		Word_selection.AddBitmap(IDB_WORD_SELECTION_1);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_2);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_3);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_4);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_5);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_4);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_3);
		Word_selection.AddBitmap(IDB_WORD_SELECTION_2);

		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_5, RGB(255, 255, 255));

		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_5, RGB(255, 255, 255));

		animation.AddBitmap(IDB_HeroNoMove_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_4, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_5, RGB(255, 255, 255));

		

		select.push_back(Word_start);
		select.push_back(Word_teaching);
		select.push_back(Word_member);
		select.push_back(Word_end);
	}

	void Menu::OnMove()
	{
		switch (state)
		{
		case game_framework::LIST:
			Word_selection.OnMove();
			break;
		case game_framework::MEMBER:
			break;
		case game_framework::TEACHING:
			TeachingOnMove();
			break;
		default:
			break;
		}
	}

	void Menu::ListOnShow()
	{
		int x = (SIZE_X - select[0].Width()) / 2;
		int y = 10;

		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, y);
		logo.ShowBitmap();

		y += logo.Height() + 20;

		for (unsigned int i = 0; i < select.size(); i++)
		{
			if ((int)i == selection)
			{
				Word_selection.SetTopLeft(x - Word_selection.Width() - 10, y);
				Word_selection.OnShow();
			}

			select[i].SetTopLeft(x, y);
			select[i].ShowBitmap();
			y += select[i].Height() + 10;
		}
	}

	void Menu::MemberOnShow()
	{
		Word_memberList.SetTopLeft((SIZE_X - Word_memberList.Width()) / 2, (SIZE_Y - Word_memberList.Height()) / 2);
		Word_memberList.ShowBitmap();
	}

	void Menu::TeachingOnShow()
	{
		int x = (SIZE_X - moveLeftAnimation.Width()) / 4;
		int y = SIZE_Y  / 2;
		background.SetTopLeft(0, 0);
		background.ShowBitmap();

		moveLeftAnimation.SetTopLeft(x , y);
		moveLeftAnimation.OnShow();
		key_left.SetTopLeft(x, y + moveLeftAnimation.Height() + 30);
		key_left.ShowBitmap();

		animation.SetTopLeft(x * 2, jump_y);
		animation.OnShow();
		key_up.SetTopLeft(x * 2, y + moveRightAnimation.Height() + 30);
		key_up.ShowBitmap();

		moveRightAnimation.SetTopLeft(x * 3, y);
		moveRightAnimation.OnShow();
		key_right.SetTopLeft(x * 3, y + moveRightAnimation.Height() + 30);
		key_right.ShowBitmap();
	}

	void Menu::TeachingOnMove()
	{
		jumpCounter.OnMove();
		if (jumpCounter.isFinish())
		{
			rising = true;
			jumpCounter.Reset();
		}

		moveLeftAnimation.OnMove();
		moveRightAnimation.OnMove();

		if (rising) {							// 上升狀態
			if (velocity > 0) {
				jump_y -= velocity;					// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;						// 受重力影響，下次的上升速度降低
			}
			else {
				rising = false;					// 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;					// 下降的初速(velocity)為1
			}
		}
		else {									// 下降狀態
			if (jump_y < floor) {  // 當y座標還沒碰到地板
				jump_y += velocity;					// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;						// 受重力影響，下次的下降速度增加
			}
			else {
				floor = jump_y = SIZE_Y / 2;
				velocity = 15;	// 重設上升初始速度
			}
		}
	}

	void Menu::OnShow()
	{
		switch (state)
		{
		case LIST:
			ListOnShow();
			break;
		case MEMBER:
			MemberOnShow();
			break;
		case TEACHING:
			TeachingOnShow();
			break;
		default:
			break;
		}
	} 
	
	void Menu::SetKeyUp()
	{
		switch (state)
		{
		case game_framework::LIST:
			if (selection == 0)
				selection = select.size() - 1;
			else
				selection--;
			break;
		case game_framework::MEMBER:
			break;
		case game_framework::TEACHING:
			break;
		default:
			break;
		}
	}

	void Menu::SetKeyDown()
	{
		switch (state)
		{
		case game_framework::LIST:
			if (selection == select.size() - 1)
				selection = 0;
			else
				selection++;
			break;
		case game_framework::MEMBER:
			break;
		case game_framework::TEACHING:
			break;
		default:
			break;
		}
	}

	void Menu::SetKeyEnter()
	{
		switch (state)
		{
		case game_framework::LIST:
			switch (selection)
			{
			case 1:
				state = TEACHING;
				jumpCounter.Reset();
				floor = jump_y = SIZE_Y / 2;
				velocity = 15;
				rising = true;

				break;
			case 2:
				state = MEMBER;
				break;
			case 3:
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
				break;
			default:
				break;
			}
			break;
		case game_framework::MEMBER:
			state = LIST;
			break;
		case game_framework::TEACHING:
			break;
		default:
			break;
		}
	}

	bool Menu::isGameStart()
	{
		if (state == LIST && selection == 0)
		{
			return true;
		}
		
		return false;
	}

}