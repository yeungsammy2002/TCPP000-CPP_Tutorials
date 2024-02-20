#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	init();
	init_draw();

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_page1, 0, wxALIGN_CENTER);
	SetSizerAndFit(m_sizer);
	m_page2->Hide();
	m_setup_page->Hide();

	init_bind();
}

void MainFrame::tap(const string & cardId)
{
	Database::Item_Itr item = m_db->find_item(cardId);
	m_message1_p1->SetLabel(wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"));
	m_message1_p1->SetForegroundColour(m_main_colour);

	if ((m_db->m_items).end() == item)
	{
		reset_p1();
		m_message1_p1->SetLabel(wxString::FromUTF8("查無此學生或此 iPad, 請重新拍卡"));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}

	if (!m_lock_p1)
	{
		if ("true" == (*item)["is_person"])
		{
			if ("student" == (*item)["type"])
			{
				processing_student_tap_p1(*item);
				return;
			}

			if ("staff" == (*item)["type"])
			{
				m_message1_p1->SetLabel(wxString::FromUTF8("職員版正在開發中, 敬請期待"));
				m_message1_p1->SetForegroundColour(m_main_colour);
				return;
			}
		}

		if ("false" == (*item)["is_person"])
		{
			processing_return_ipad_p1(*item);
			return;
		}
	}

	if (m_lock_p1)
	{
		if ("false" == (*item)["is_person"])
		{
			processing_ipad_tap_p1(*item);
			return;
		}

		if ("true" == (*item)["is_person"])
		{
			if (cardId == m_borrower_p1["BorrowerCardID"])
			{
				string cancel_message = m_borrower_p1["ChineseName"] + m_borrower_p1["ClassName"] + "(" + m_borrower_p1["ClassNumber"] + ") " + "已取消借入 iPad";
				m_message1_p1->SetLabel(wxString::FromUTF8(cancel_message));
				m_message1_p1->SetForegroundColour(m_main_colour);
				reset_p1();
			}
			else
			{
				string change_borrower_message = "借入 iPad 學生已改變";
				m_message1_p1->SetLabel(wxString::FromUTF8(change_borrower_message));
				m_message1_p1->SetForegroundColour(m_main_colour);
				processing_student_tap_p1(*item);
			}
			return;
		}
	}
}

