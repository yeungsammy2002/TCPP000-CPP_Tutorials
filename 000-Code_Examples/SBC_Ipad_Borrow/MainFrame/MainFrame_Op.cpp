#include "../MainFrame.h"

void MainFrame::reset_p1()
{
	m_borrower_p1["Time"] = "";
	m_borrower_p1["BorrowerCardID"] = "";
	m_borrower_p1["ChineseName"] = "";
	m_borrower_p1["EnglishName"] = "";
	m_borrower_p1["ClassName"] = "";
	m_borrower_p1["ClassNumber"] = "";
	m_borrower_p1["IpadCardID"] = "";
	m_borrower_p1["IpadNumber"] = "";
	m_lock_p1 = false;
	m_status_p1->SetForegroundColour(m_main_colour);
	m_status_p1->SetLabel(wxString::FromUTF8("狀態: 借出 / 歸還"));
	m_message2_p1->SetLabel(wxString::FromUTF8("借出 - 如要借出 iPad, 請先讓學生拍卡"));
	m_message2_p1->SetForegroundColour(m_second_colour);
	m_message3_p1->SetLabel(wxString::FromUTF8("歸還 - 如要歸還 iPad, 請讓 iPad 拍卡"));
	m_message3_p1->SetForegroundColour(m_third_colour);
	show_empty_p1();
}

void MainFrame::switch_page(bool move)
{
	move ? ++m_current_page_num : --m_current_page_num;

	if (1 > m_current_page_num)
		m_current_page_num = m_last_page_num;

	if (m_last_page_num < m_current_page_num)
		m_current_page_num = 1;

	switch_page();
}

void MainFrame::switch_page()
{
	m_sizer->Detach(0);

	switch (m_current_page_num)
	{
	case 1:
	{
		m_page2->Hide();
		m_setup_page->Hide();
		m_sizer->Prepend(m_page1, 0, wxALIGN_CENTER);
		m_page1->Show();
		m_page1->SetFocus();
	}
	break;
	case 2:
	{
		m_page1->Hide();
		m_setup_page->Hide();
		m_sizer->Prepend(m_page2, 0, wxALIGN_CENTER);
		m_page2->Show();
		m_page2->SetFocus();
	}
	break;
	case 3:
	{
		m_page1->Hide();
		m_page2->Hide();
		m_sizer->Prepend(m_setup_page, 0, wxALIGN_CENTER);
		m_setup_page->Show();
		m_setup_page->SetFocus();
	}
	break;
	default:
		break;
	}

	m_sizer->Layout();
}