#include "../MainFrame.h"

void MainFrame::init_bind()
{
	m_page1->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
	m_page2->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
	//m_button_left_p2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	//m_button_right_p2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	m_setup_page->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);
}

void MainFrame::OnKeyEvent(wxKeyEvent & evt)
{
	int keyCode = evt.GetKeyCode();
	static wxString wx_cardId;
	//wxLogStatus("test");

	//log_debug(std::to_string(keyCode).c_str());

	if (13 != keyCode)
	{
		//wxLogStatus(wxString(keyChar));
		if ('F' == keyCode || 32 == keyCode)
		{
			switch_page(true);
			wx_cardId = "";
			//wxLogStatus("page up");
			return;
		}
		if ('D' == keyCode)
		{
			switch_page(false);
			wx_cardId = "";
			//wxLogStatus("page down");
			return;
		}

		if (1 == m_current_page_num)
		{
			wxChar keyChar = evt.GetUnicodeKey();
			wx_cardId.append(keyChar);
			return;
		}

		if (2 == m_current_page_num)
		{
			if (61 == keyCode || 50 == keyCode)
				move_history_p2(1);

			if (45 == keyCode || 49 == keyCode)
				move_history_p2(0);

			return;
		}
	}
	else
	{
		//wxLogStatus(wx_cardId);
		if (1 == m_current_page_num)
		{
			string cardId = wx_cardId.ToStdString();

			if ("" == cardId)
				return;

			tap(cardId);
			wx_cardId = "";
		}
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