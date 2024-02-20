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
	show_empty_p2();
}

void MainFrame::processing_student_tap_p1(Database::Item & student)
{
	Database::BList_Item_Itr p_borrower = m_db->find_borrower(student["CardID"]);
	if ((m_db->m_blist).end() != p_borrower)
	{
		reset_p1();

		string reject_message = "學生" + (*p_borrower)["ChineseName"] + " "
			+ (*p_borrower)["ClassName"] + "(" + (*p_borrower)["ClassNumber"] + ") 已借入 iPad "
			+ (*p_borrower)["IpadNumber"] + ", 借入操作已取消";

		m_message1_p1->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}

	m_borrower_p1["BorrowerCardID"] = student["CardID"];
	m_borrower_p1["ChineseName"] = student["ChineseName"];
	m_borrower_p1["EnglishName"] = student["EnglishName"];
	m_borrower_p1["ClassName"] = student["ClassName"];
	m_borrower_p1["ClassNumber"] = student["ClassNumber"];

	m_lock_p1 = true;

	m_status_p1->SetLabel(wxString::FromUTF8("狀態: 借出"));
	m_status_p1->SetForegroundColour(m_second_colour);

	string message1 = "借入學生: " + student["ChineseName"] + " " + student["EnglishName"] + " 班別: " + student["ClassName"] + " 學號: " + student["ClassNumber"];
	m_message2_p1->SetLabel(wxString::FromUTF8(message1));
	m_message2_p1->SetForegroundColour(m_second_colour);
	m_message3_p1->SetLabel(wxString::FromUTF8("請為要借出的 iPad 拍卡: "));
	m_message3_p1->SetForegroundColour(m_second_colour);

}

void MainFrame::processing_ipad_tap_p1(Database::Item & ipad)
{
	Database::BList_Item_Itr p_borrowed_ipad = m_db->find_blitem(ipad["CardID"]);
	if ((m_db->m_blist).end() != p_borrowed_ipad)
	{
		reset_p1();
		string reject_message = "iPad " + (*p_borrowed_ipad)["IpadNumber"] + " 已經借給學生"
			+ (*p_borrowed_ipad)["ChineseName"] + (*p_borrowed_ipad)["ClassName"]
			+ "(" + (*p_borrowed_ipad)["ClassNumber"] + "), 借入操作已取消";

		m_message1_p1->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}

	std::stringstream ss;
	ss << Time::now().m_unixtime;
	m_borrower_p1["Time"] = ss.str();
	m_borrower_p1["IpadCardID"] = ipad["CardID"];
	m_borrower_p1["IpadNumber"] = ipad["IpadNumber"];
	(m_db->m_blist).push_back(m_borrower_p1);
	append_p1();
	m_db->write_to_file(m_borrower_p1);

	string success_message = "iPad " + m_borrower_p1["IpadNumber"] + " 已成功借給 " + m_borrower_p1["ClassName"] + "(" + m_borrower_p1["ClassNumber"] + ") " + m_borrower_p1["ChineseName"];
	m_message1_p1->SetLabel(wxString::FromUTF8(success_message));
	m_message1_p1->SetForegroundColour(m_main_colour);
	reset_p1();
}

void MainFrame::processing_return_ipad_p1(Database::Item & ipad)
{
	Database::BList_Item_Itr blitem = m_db->find_blitem(ipad["CardID"]);
	if (m_db->m_blist.end() != blitem)
	{
		Database::BList_Item rlitem = *blitem;
		string message2 = "學生: " + (*blitem)["ChineseName"] + " " + (*blitem)["EnglishName"] + " "
			+ (*blitem)["ClassName"] + " (" + (*blitem)["ClassNumber"]
			+ ") 已歸還 iPad - " + (*blitem)["IpadNumber"];

		std::stringstream ss;
		Time t = Time::now();
		ss << t.m_unixtime;
		rlitem["ReturnTime"] = ss.str();
		rlitem["ChiReturnTime"] = t.str_chi_time;
		rlitem["ChiTime"] = Time::get_time(std::atoll(((*blitem)["Time"]).c_str())).str_chi_time;
		(m_db->m_rlist).push_back(rlitem);
		append_p2();
		m_db->write_to_history(rlitem);

		int index = blitem - (m_db->m_blist).begin();
		m_grid_p1->DeleteRows(index, 1, true);
		(m_db->m_blist).erase(blitem);
		m_db->update();

		m_message2_p1->SetLabel(wxString::FromUTF8(message2));
		m_message2_p1->SetForegroundColour(m_main_colour);
		m_message3_p1->SetLabel("");
		show_empty_p1();
		show_empty_p2();
		return;
	}

	if (m_db->m_blist.end() == blitem)
	{
		reset_p1();
		m_message1_p1->SetLabel(wxString::FromUTF8("此 iPad 沒有借出, 歸還操作已取消"));
	}
}

void MainFrame::switch_page(int page)
{
	if (1 > m_current_page_num)
	{
		log_error("switch to zero or negative page");
		return;
	}

	if (m_last_page_num < m_current_page_num)
	{
		log_error("switch to beyond last page");
		return;
	}

	m_current_page_num = page;
	switch_page();
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

void MainFrame::move_history_p2(const int id)
{
	Time now = Time::now();

	if (id == 0)
	{
		long long new_unix = m_history_unixtime_p2 + 86400;
		if (new_unix > now.m_unixtime)
			return;

		m_history_unixtime_p2 = new_unix;
	}

	if (id == 1)
	{
		m_history_unixtime_p2 -= 86400;
	}

	Time search_time = Time::get_time(m_history_unixtime_p2);
	m_history_year_p2 = search_time.m_year;
	m_history_month_p2 = search_time.m_month;
	m_history_day_p2 = search_time.m_day;

	Time t = Time::get_time(m_history_unixtime_p2);
	string title = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月 " + std::to_string(m_history_day_p2) + " 號 (" + t.str_chi_weekday + ") 學生歸還 iPad 記錄";
	m_title_p2->SetLabel(wxString::FromUTF8(title));
	bool is_exist = m_db->load_history(m_history_year_p2, m_history_month_p2, m_history_day_p2);

	redraw_grid_p2(is_exist);
}