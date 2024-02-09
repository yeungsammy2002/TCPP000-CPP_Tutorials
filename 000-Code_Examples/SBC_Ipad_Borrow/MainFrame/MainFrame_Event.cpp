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
	//wxLogStatus("test");

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