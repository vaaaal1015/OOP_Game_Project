#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "Menu.h"

namespace game_framework {

	Menu::Menu()
	{
		state = LIST;
		selection = 0;
		Word_selection.SetDelayCount(2);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
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
		background.LoadBitmap(IDB_BACKGROUND_W);

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

		

		select.push_back(Word_start);
		select.push_back(Word_teaching);
		select.push_back(Word_member);
		select.push_back(Word_end);
	}

	void Menu::OnMove()
	{
		Word_selection.OnMove();
		TeachingOnMove();
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
		int x = (SIZE_X - moveLeftAnimation.Width() - moveRightAnimation.Width()) / 4;
		int y = 100;
		background.SetTopLeft(0, 0);
		background.ShowBitmap();

		moveLeftAnimation.SetTopLeft(x , y);
		moveLeftAnimation.OnShow();
		key_left.SetTopLeft(x, y + moveLeftAnimation.Height() + 30);
		key_left.ShowBitmap();
	}

	void Menu::TeachingOnMove()
	{
		moveLeftAnimation.OnMove();
		moveRightAnimation.OnMove();
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