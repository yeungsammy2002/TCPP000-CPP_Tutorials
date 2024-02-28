#include "../MainFrame.h"

void MainFrame::tap(const string & cardId)
{
	Database::Item_Itr item = m_db->find_item(cardId);
	m_message1_p1->SetLabel(wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"));
	m_message1_p1->SetForegroundColour(m_main_colour);

	m_message3_p1->SetLabel(wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"));
	m_message3_p1->SetForegroundColour(m_p3text_colour);

	if ((m_db->m_items).end() == item)
	{
		if (3 == m_current_page_num || 4 == m_current_page_num)
		{
			switch_page(3);
			reset();
			m_message1_p3->SetLabel(wxString::FromUTF8("查無此職員, 學生或 iPad, 請重新拍卡"));
			m_message1_p3->SetForegroundColour(m_warn_colour);
			return;
		}

		switch_page(1);
		reset();
		m_message1_p1->SetLabel(wxString::FromUTF8("查無此學生, 職員或 iPad, 請重新拍卡"));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}

	if (!m_lock_p1 && !m_lock_p3)
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
				processing_staff_tap_p3(*item);
				return;
			}
		}

		if ("false" == (*item)["is_person"])
		{
			if (m_db->m_blist.end() != m_db->find_blitem(cardId))
			{
				processing_return_ipad_p1(*item);
				return;
			}
			else if (m_db->m_bslist.end() != m_db->find_bslitem(cardId))
			{
				processing_return_ipad_p3(*item);
				return;
			}
			else
			{
				reset();
				if (3 == m_current_page_num || 4 == m_current_page_num)
				{
					switch_page(3);
					m_message1_p3->SetLabel(wxString::FromUTF8("此 iPad 沒有借出, 歸還操作已取消"));
				}
				else
				{
					switch_page(1);
					m_message1_p1->SetLabel(wxString::FromUTF8("此 iPad 沒有借出, 歸還操作已取消"));
				}
			}
		}
		return;
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
				reset();
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

	if (m_lock_p3)
	{
		processing_borrow_tap_p3(*item);
		return;
	}
}

void MainFrame::reset()
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
	m_message3_p1->SetForegroundColour(m_second_colour);

	m_staff_p3["Time"] = "";
	m_staff_p3["StaffCardID"] = "";
	m_staff_p3["ChineseName"] = "";
	m_staff_p3["EnglishName"] = "";
	m_staff_p3["Login"] = "";
	m_staff_p3["IpadCardID"] = "";
	m_staff_p3["IpadNumber"] = "";
	m_staff_p3["IpadSN"] = "";
	m_lock_p3 = false;
	m_second_lock_p3 = false;
	m_last_ipad_p3 = "";
	m_status_p3->SetForegroundColour(m_p3text_colour);
	m_status_p3->SetLabel(wxString::FromUTF8("狀態: 借出 / 歸還"));
	m_message2_p3->SetLabel(wxString::FromUTF8("借出 - 如要借出 iPad, 請先讓職員拍卡"));
	m_message2_p3->SetForegroundColour(m_second_colour);
	m_message3_p3->SetLabel(wxString::FromUTF8("歸還 - 如要歸還 iPad, 請讓 iPad 拍卡"));
	m_message3_p3->SetForegroundColour(m_second_colour);

	show_empty(0);
	show_empty(1);
	show_empty(2);
}

void MainFrame::processing_student_tap_p1(Database::Item & student)
{
	switch_page(1);
	Database::BList_Item_Itr p_borrower = m_db->find_borrower(student["CardID"]);
	if ((m_db->m_blist).end() != p_borrower)
	{
		reset();

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
	switch_page(1);
	Database::BList_Item_Itr p_borrowed_ipad = m_db->find_blitem(ipad["CardID"]);
	if ((m_db->m_blist).end() != p_borrowed_ipad)
	{
		reset();
		string reject_message = "iPad " + (*p_borrowed_ipad)["IpadNumber"] + " 已經借給學生 "
			+ (*p_borrowed_ipad)["ChineseName"] + (*p_borrowed_ipad)["ClassName"]
			+ "(" + (*p_borrowed_ipad)["ClassNumber"] + "), 借入操作已取消";

		m_message1_p1->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}
	p_borrowed_ipad = m_db->find_bslitem(ipad["CardID"]);
	if ((m_db->m_bslist).end() != p_borrowed_ipad)
	{
		reset();
		string reject_message = "iPad " + (*p_borrowed_ipad)["IpadNumber"] + " 已經借給職員 "
			+ (*p_borrowed_ipad)["ChineseName"] + " " + (*p_borrowed_ipad)["EnglishName"]
			+ "(" + (*p_borrowed_ipad)["Login"] + "), 借入操作已取消";

		m_message1_p1->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p1->SetForegroundColour(m_warn_colour);
		return;
	}

	std::stringstream ss;
	ss << Time::now().m_unixtime;
	m_borrower_p1["Time"] = ss.str();
	m_borrower_p1["IpadCardID"] = ipad["CardID"];
	m_borrower_p1["IpadNumber"] = ipad["IpadNumber"];
	(m_db->m_blist).insert(m_db->m_blist.begin(), m_borrower_p1);
	push(0);
	m_db->write_blist_to_file("student");

	string success_msg = "iPad " + m_borrower_p1["IpadNumber"] + " 已成功借給 " + m_borrower_p1["ClassName"] + "(" + m_borrower_p1["ClassNumber"] + ") " + m_borrower_p1["ChineseName"];
	m_message1_p1->SetLabel(wxString::FromUTF8(success_msg));
	m_message1_p1->SetForegroundColour(m_main_colour);
	reset();
}

void MainFrame::processing_return_ipad_p1(Database::Item & ipad)
{
	switch_page(1);
	Database::BList_Item_Itr blitem = m_db->find_blitem(ipad["CardID"]);
	Database::BList_Item rlitem = *blitem;
	string message2 = "學生: " + rlitem["ChineseName"] + " " + rlitem["EnglishName"] + " "
		+ rlitem["ClassName"] + " (" + rlitem["ClassNumber"]
		+ ") 已歸還 iPad - " + rlitem["IpadNumber"];

	std::stringstream ss;
	Time t = Time::now();
	ss << t.m_unixtime;
	rlitem["ReturnTime"] = ss.str();
	rlitem["ChiReturnTime"] = t.str_chi_time;
	rlitem["ChiTime"] = Time::get_time(std::atoll(rlitem["Time"].c_str())).str_chi_time;
	(m_db->m_rlist).push_back(rlitem);
	append(1);
	m_db->write_to_history(rlitem, "student");

	int index = blitem - (m_db->m_blist).begin();
	m_grids[0]->DeleteRows(index, 1, true);
	(m_db->m_blist).erase(blitem);
	m_db->write_blist_to_file("student");

	m_message2_p1->SetLabel(wxString::FromUTF8(message2));
	m_message2_p1->SetForegroundColour(m_main_colour);
	m_message3_p1->SetLabel("");
	show_empty(0);
	show_empty(1);
	return;
}

void MainFrame::processing_return_ipad_p3(Database::Item & ipad)
{
	switch_page(3);
	Database::BList_Item_Itr bslitem = m_db->find_bslitem(ipad["CardID"]);
	Database::BList_Item rslitem = *bslitem;
	string message2 = "職員: " + rslitem["ChineseName"] + " " + rslitem["EnglishName"] + " ("
		+ rslitem["Login"] + ") 已歸還 iPad - " + rslitem["IpadNumber"];

	std::stringstream ss;
	Time t = Time::now();
	ss << t.m_unixtime;
	rslitem["ReturnTime"] = ss.str();
	rslitem["ChiReturnTime"] = t.str_chi_time;
	rslitem["ChiTime"] = Time::get_time(std::atoll(rslitem["Time"].c_str())).str_chi_time;
	(m_db->m_rslist).push_back(rslitem);
	append(3);
	m_db->write_to_history(rslitem, "staff");

	int index = bslitem - (m_db->m_bslist).begin();
	m_grids[2]->DeleteRows(index, 1, true);
	(m_db->m_bslist).erase(bslitem);
	m_db->write_blist_to_file("staff");

	m_message1_p3->SetLabel("");
	m_message2_p3->SetLabel(wxString::FromUTF8(message2));
	m_message2_p3->SetForegroundColour(m_p3text_colour);
	m_message3_p3->SetLabel("");
	show_empty(2);
	show_empty(3);
	return;
}

void MainFrame::processing_staff_tap_p3(Database::Item & staff)
{
	switch_page(3);
	m_staff_p3["StaffCardID"] = staff["CardID"];
	m_staff_p3["ChineseName"] = staff["ChineseName"];
	m_staff_p3["EnglishName"] = staff["EnglishName"];
	m_staff_p3["Login"] = staff["Login"];

	m_lock_p3 = true;

	m_status_p3->SetLabel(wxString::FromUTF8("狀態: 借出"));
	m_status_p3->SetForegroundColour(m_second_colour);

	string message2 = "借入職員: " + staff["ChineseName"] + " || " + staff["EnglishName"] + " (" + staff["Login"] + ")";
	m_message2_p3->SetLabel(wxString::FromUTF8(message2));
	m_message2_p3->SetForegroundColour(m_second_colour);
	m_message3_p3->SetLabel(wxString::FromUTF8("請為要借出的 iPad 拍卡: "));
	m_message3_p3->SetForegroundColour(m_second_colour);
}

bool MainFrame::check_ipad_borrowed_p3(Database::Item & ipad)
{
	Database::BList_Item_Itr p_borrowed_ipad = m_db->find_blitem(ipad["CardID"]);
	if ((m_db->m_blist).end() != p_borrowed_ipad)
	{
		reset();
		string reject_message = "iPad " + (*p_borrowed_ipad)["IpadNumber"] + " 已經借給學生"
			+ (*p_borrowed_ipad)["ChineseName"] + (*p_borrowed_ipad)["ClassName"]
			+ "(" + (*p_borrowed_ipad)["ClassNumber"] + "), 借入操作已取消";

		m_message1_p3->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p3->SetForegroundColour(m_warn_colour);
		return true;
	}
	p_borrowed_ipad = m_db->find_bslitem(ipad["CardID"]);
	if ((m_db->m_bslist).end() != p_borrowed_ipad)
	{
		reset();
		string reject_message = "iPad " + (*p_borrowed_ipad)["IpadNumber"] + " 已經借給職員 "
			+ (*p_borrowed_ipad)["ChineseName"] + " " + (*p_borrowed_ipad)["EnglishName"]
			+ "(" + (*p_borrowed_ipad)["Login"] + "), 借入操作已取消";

		m_message1_p3->SetLabel(wxString::FromUTF8(reject_message));
		m_message1_p3->SetForegroundColour(m_warn_colour);
		return true;
	}
	return false;
}

void MainFrame::processing_borrow_tap_p3(Database::Item & item)
{
	switch_page(3);

	if ("false" == item["is_person"] && !m_second_lock_p3)
	{
		Database::Item & ipad = item;
		Database::BList_Item_Itr p_borrowed_ipad = m_db->find_blitem(ipad["CardID"]);
		if (check_ipad_borrowed_p3(ipad))
			return;

		std::stringstream ss;
		ss << Time::now().m_unixtime;
		m_staff_p3["Time"] = ss.str();
		m_staff_p3["IpadCardID"] = ipad["CardID"];
		m_staff_p3["IpadNumber"] = ipad["IpadNumber"];
		m_staff_p3["IpadSN"] = ipad["IpadSN"];
		(m_db->m_bslist).insert((m_db->m_bslist).begin(), m_staff_p3);
		push(2);
		m_db->write_blist_to_file("staff");

		string success_msg = "iPad " + m_staff_p3["IpadNumber"] + " 已成功借給 " + m_staff_p3["ChineseName"] + " " + m_staff_p3["EnglishName"] + "(" + m_staff_p3["Login"] + ")";
		m_message1_p3->SetLabel(wxString::FromUTF8(success_msg));
		m_message1_p3->SetForegroundColour(m_p3text_colour);
		reset();
		return;
	}

	if ("true" == item["is_person"] && !m_second_lock_p3)
	{
		m_second_lock_p3 = true;
		m_status_p3->SetLabel(wxString::FromUTF8("借出多部 iPad 模式"));
		m_status_p3->SetForegroundColour(m_second_colour);
		m_message3_p3->SetLabel(wxString::FromUTF8("已進入多次借出 iPad 模式, 請為要借出的 iPad 拍卡: "));
		m_message3_p3->SetForegroundColour(m_main_colour);
		return;
	}

	if (("false" == item["is_person"] && m_second_lock_p3 && (m_last_ipad_p3 == item["CardID"])) ||
		("true" == item["is_person"] && m_second_lock_p3 && (m_staff_p3["StaffCardID"] == item["CardID"])))
	{
		string success_msg = m_staff_p3["ChineseName"] + " " + m_staff_p3["EnglishName"] + "(" + m_staff_p3["Login"] + ") 已結束借入多部 iPad 模式";
		m_message1_p3->SetLabel(wxString::FromUTF8(success_msg));
		m_message1_p3->SetForegroundColour(m_p3text_colour);
		reset();
		return;
	}

	if ("false" == item["is_person"] && m_second_lock_p3)
	{
		Database::Item & ipad = item;

		Database::BList_Item_Itr p_borrowed_ipad = m_db->find_blitem(ipad["CardID"]);
		if (check_ipad_borrowed_p3(ipad))
			return;

		std::stringstream ss;
		ss << Time::now().m_unixtime;
		m_staff_p3["Time"] = ss.str();
		m_staff_p3["IpadCardID"] = ipad["CardID"];
		m_staff_p3["IpadNumber"] = ipad["IpadNumber"];
		m_staff_p3["IpadSN"] = ipad["IpadSN"];
		(m_db->m_bslist).insert((m_db->m_bslist).begin(), m_staff_p3);
		push(2);
		m_db->write_blist_to_file("staff");

		string success_msg = "iPad " + m_staff_p3["IpadNumber"] + " 已成功借給 " + m_staff_p3["ChineseName"] + " " + m_staff_p3["EnglishName"] + "(" + m_staff_p3["Login"] + ")";
		m_message1_p3->SetLabel(wxString::FromUTF8(success_msg));
		m_message1_p3->SetForegroundColour(m_p3text_colour);

		m_message2_p3->SetLabel(wxString::FromUTF8("請為下一部 iPad 拍卡, 如要結束, 請為最後一部剛剛已借出的 iPad 拍卡"));
		m_message2_p3->SetForegroundColour(m_second_colour);
		m_message3_p3->SetLabel(wxString::FromUTF8(""));

		m_last_ipad_p3 = ipad["CardID"];
		return;
	}

	if ("true" == item["is_person"] && m_second_lock_p3)
	{
		if ("staff" == item["type"])
		{
			string success_msg = m_staff_p3["ChineseName"] + " " + m_staff_p3["EnglishName"] + "(" + m_staff_p3["Login"] + ") 已結束借入多部 iPad 模式";
			m_message1_p3->SetLabel(wxString::FromUTF8(success_msg));
			m_message1_p3->SetForegroundColour(m_p3text_colour);
			reset();
			return;
		}
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

	int page_index = m_current_page_num - 1;
	for (int i = 0; i < m_total_pages; ++i)
	{
		if (i == page_index)
		{
			m_sizer->Prepend(m_pages[i], 0, wxALIGN_CENTER);
			m_pages[i]->Show();
			m_pages[i]->SetFocus();
		}
		else
			m_pages[i]->Hide();
	}

	m_sizer->Layout();
}

void MainFrame::move_history_p2(const int id)
{
	Time now = Time::now();

	if (id == 0)
	{
		long long new_unix = m_history_unixtime_p2 + 86400;
		if (new_unix > (now.m_unixtime + 86400))
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

	redraw_grid(1, is_exist);
}

void MainFrame::move_history_p4(const int id)
{
	Time now = Time::now();

	if (id == 0)
	{
		long long new_unix = m_history_unixtime_p4 + 2592000;
		if (new_unix > (now.m_unixtime + 2592000))
			return;

		m_history_unixtime_p4 = new_unix;
	}

	if (id == 1)
	{
		m_history_unixtime_p4 -= 2592000;
	}

	Time search_time = Time::get_time(m_history_unixtime_p4);
	m_history_year_p4 = search_time.m_year;
	m_history_month_p4 = search_time.m_month;

	Time t = Time::get_time(m_history_unixtime_p4);
	string title = std::to_string(m_history_year_p4) + " 年 " + std::to_string(m_history_month_p4) + " 月 職員歸還 iPad 記錄";
	m_title_p4->SetLabel(wxString::FromUTF8(title));
	bool is_exist = m_db->load_staff_history(m_history_year_p4, m_history_month_p4);

	redraw_grid(3, is_exist);
}