#include "../MainFrame.h"

void MainFrame::init_bind()
{
	for (auto page : m_pages)
		page->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
}

void MainFrame::OnKeyEvent(wxKeyEvent & evt)
{
	int keyCode = evt.GetKeyCode();
	static wxString wx_cardId;

	if (13 != keyCode)
	{
		if ('F' == keyCode || 32 == keyCode)
		{
			switch_page(true);
			wx_cardId = "";;
			return;
		}
		if ('D' == keyCode)
		{
			switch_page(false);
			wx_cardId = "";
			return;
		}
		if (61 == keyCode && 2 == m_current_page_num)
		{
			move_history_p2(1);
			return;
		}
		if (45 == keyCode && 2 == m_current_page_num)
		{
			move_history_p2(0);
			return;
		}
		if (61 == keyCode && 4 == m_current_page_num)
		{
			move_history_p4(1);
			return;
		}
		if (45 == keyCode && 4 == m_current_page_num)
		{
			move_history_p4(0);
			return;
		}

		if (std::isdigit(keyCode))
		{
			//string s = string("is_digit: ") + char(keyCode);
			wxChar keyChar = evt.GetUnicodeKey();
			wx_cardId.append(keyChar);
			return;
		}
	}
	else
	{
		string cardId = wx_cardId.ToStdString();

		if ("" == cardId)
			return;

		tap(cardId);
		wx_cardId = "";
	}
}

void MainFrame::OnCloseClicked(wxCommandEvent & evt)
{
	Close();
}

void MainFrame::OnButtonClicked(wxCommandEvent & evt)
{
	int id = evt.GetId();
	move_history_p2(id);
}