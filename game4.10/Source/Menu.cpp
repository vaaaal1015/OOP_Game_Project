#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "Menu.h"

namespace game_framework {
	Menu::Menu() : selection(0)
	{
	}

	void Menu::LoadBitmap()
	{
		logo.LoadBitmap(IDB_GREATSWORDLOGO, RGB(255, 255, 255));
		Word_start.LoadBitmap(IDB_WORD_START, RGB(255, 0, 0));
		Word_member.LoadBitmap(IDB_WORD_MEMBER, RGB(255, 0, 0));
		Word_end.LoadBitmap(IDB_WORD_END, RGB(255, 0, 0));
		Word_selection.LoadBitmap(IDB_WORD_SELECTION, RGB(255, 0, 0));
		select.push_back(Word_start);
		select.push_back(Word_member);
		select.push_back(Word_end);
	}

	void Menu::OnShow()
	{
		int x = (SIZE_X - select[0].Width()) / 2;
		int y = SIZE_Y / 10;

		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, y);
		logo.ShowBitmap();

		y += logo.Height() + 20;

		for (unsigned int i = 0; i < select.size(); i++)
		{
			if ((int) i == selection)
			{
				Word_selection.SetTopLeft(x - Word_selection.Width() - 10, y);
				Word_selection.ShowBitmap();
			}

			select[i].SetTopLeft(x, y);
			select[i].ShowBitmap();
			y += select[i].Height() + 10;
		}
	}
	
	void Menu::SetMoveingUp()
	{
		if (selection == 0)
			selection = select.size() - 1;
		else
			selection--;

	}

	void Menu::SetMoveingDown()
	{
		if (selection == select.size() - 1)
			selection = 0;
		else
			selection++;
	}

	int Menu::GetState()
	{
		return selection;
	}
}