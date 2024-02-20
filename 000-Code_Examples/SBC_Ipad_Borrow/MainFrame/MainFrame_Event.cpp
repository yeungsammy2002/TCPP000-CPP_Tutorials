#include "../MainFrame.h"

void MainFrame::init_bind()
{
	m_page1->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
	m_page2->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
	m_setup_page->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
}

void MainFrame::OnKeyEvent(wxKeyEvent & evt)
{
	int keyCode = evt.GetKeyCode();
	static wxString wx_cardId;

	//log_debug(std::to_string(keyCode).c_str());

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

		if (std::isdigit(keyCode))
		{
			//string s = string("is_digit: ") + char(keyCode);
			//log_debug(s.c_str());
			wxChar keyChar = evt.GetUnicodeKey();
			wx_cardId.append(keyChar);
			return;
		}
	}
	else
	{
		switch_page(1);
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