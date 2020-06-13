#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "MenuList.h"

namespace game_framework {
	MenuList::MenuList() : selection(0)
	{
	}

	void MenuList::LoadBitmap()
	{
		Word_start.LoadBitmap(IDB_WORD_START, RGB(255, 0, 0));
		Word_member.LoadBitmap(IDB_WORD_MEMBER, RGB(255, 0, 0));
		Word_end.LoadBitmap(IDB_WORD_END, RGB(255, 0, 0));
		Word_selection.LoadBitmap(IDB_WORD_SELECTION, RGB(255, 0, 0));
		select.push_back(Word_start);
		select.push_back(Word_member);
		select.push_back(Word_end);
	}

	void MenuList::OnShow()
	{
		int x = 280;
		int y = 350;

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
	
	void MenuList::SetMoveingUp()
	{
		if (selection == 0)
			selection = select.size() - 1;
		else
			selection--;

	}

	void MenuList::SetMoveingDown()
	{
		if (selection == select.size() - 1)
			selection = 0;
		else
			selection++;
	}

	int MenuList::GetState()
	{
		return selection;
	}
}