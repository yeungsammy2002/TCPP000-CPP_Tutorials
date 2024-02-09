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

	//CreateStatusBar();
}

void MainFrame::tap(const string & cardId)
{
	//wxLogStatus("tap");
	Database::Item_Itr item = m_db->find_item(cardId);
	m_message1_p1->SetLabel(wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"));

	if ((m_db->m_items).end() == item)
	{
		reset_p1();
		m_message1_p1->SetLabel(wxString::FromUTF8("查無此學生或此 iPad, 請重新拍卡"));
		//wxLogStatus(wxString::FromUTF8("無此學生或 iPad 請重新輸入"));
		return;
	}

	if (!m_lock_p1)
	{
		if ("true" == (*item)["is_person"])
		{

			Database::BList_Item_Itr borrower = m_db->find_borrower(cardId);
			if ((m_db->m_blist).end() != borrower)
			{
				reset_p1();

				string unsuccess_message = "學生" + (*borrower)["ChineseName"] + " "
					+ (*borrower)["ClassName"] + "(" + (*borrower)["ClassNumber"] + ") 已借入 iPad "
					+ (*borrower)["IpadNumber"] + ", 如要歸還, 請為歸還 iPad 拍卡即可";

				m_message1_p1->SetLabel(wxString::FromUTF8(unsuccess_message));
				//wxLogStatus("此學生已借入 iPad, 不能再借, 如要歸還, 請為歸還 iPad 拍卡即可");
				return;
			}
			//wxLogStatus("tap");
			if ("student" == (*item)["type"])
			{
				Database::Item & student = *item;

				m_borrower_p1["BorrowerCardID"] = cardId;
				m_borrower_p1["ChineseName"] = student["ChineseName"];
				m_borrower_p1["EnglishName"] = student["EnglishName"];
				m_borrower_p1["ClassName"] = student["ClassName"];
				m_borrower_p1["ClassNumber"] = student["ClassNumber"];

				m_lock_p1 = true;

				m_status_p1->SetLabel(wxString::FromUTF8("狀態: 借出"));
				m_status_p1->SetForegroundColour(m_second_colour);
				m_message2_p1->SetForegroundColour(m_second_colour);
				m_message3_p1->SetForegroundColour(m_second_colour);

				string message1 = "借入學生: " + student["ChineseName"] + " " + student["EnglishName"] + " 班別: " + student["ClassName"] + " 學號: " + student["ClassNumber"];
				m_message2_p1->SetLabel(wxString::FromUTF8(message1));
				m_message3_p1->SetLabel(wxString::FromUTF8("請為要借出的 iPad 拍卡: "));
				//wxLogStatus("請為iPad拍卡: ");
				return;
			}

			//if ("staff" == (*item)["type"])
			//{
			//	Database::Item & staff = *item;
			//	wxLogStatus("職員版正在開發中, 敬請期待");
			//	return;
			//}
		}

		if ("false" == (*item)["is_person"])
		{
			Database::BList_Item_Itr blitem = m_db->find_blitem(cardId);
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

				m_message2_p1->SetForegroundColour(m_third_colour);
				m_message3_p1->SetForegroundColour(m_third_colour);
				m_message2_p1->SetLabel(wxString::FromUTF8(message2));
				//wxLogStatus("iPad 已歸還");
				show_empty_p1();
				return;
			}
			else
			{
				reset_p1();
				m_message1_p1->SetLabel(wxString::FromUTF8("此 iPad 沒有借出, 如要借出 iPad, 請先讓學生拍卡, 再讓借出 iPad 拍卡"));
				//wxLogStatus("此 iPad 沒有借出, 如要借出 iPad, 請先讓學生拍卡, 再讓借出 iPad 拍卡");
				return;
			}
		}
	}

	if (m_lock_p1)
	{
		if ("false" == (*item)["is_person"])
		{
			Database::BList_Item_Itr borrowed_ipad = m_db->find_blitem(cardId);
			if ((m_db->m_blist).end() != borrowed_ipad)
			{
				reset_p1();
				string unsuccess_message = "iPad " + (*borrowed_ipad)["IpadNumber"] + " 已經借給學生"
					+ (*borrowed_ipad)["ChineseName"] + (*borrowed_ipad)["ClassName"]
					+ "(" + (*borrowed_ipad)["ClassNumber"] + "), 請先拍卡歸還";

				m_message1_p1->SetLabel(wxString::FromUTF8(unsuccess_message));
				//wxLogStatus("此 iPad 不能再借出, 請先拍卡歸還");
				return;
			}

			Database::Item & ipad = *item;

			std::stringstream ss;
			ss << Time::now().m_unixtime;
			m_borrower_p1["Time"] = ss.str();
			m_borrower_p1["IpadCardID"] = cardId;
			m_borrower_p1["IpadNumber"] = ipad["IpadNumber"];
			(m_db->m_blist).push_back(m_borrower_p1);
			append_p1();
			m_db->write_to_file(m_borrower_p1);

			string success_message = "iPad " + m_borrower_p1["IpadNumber"] + " 已成功借給 " + m_borrower_p1["ClassName"] + "(" + m_borrower_p1["ClassNumber"] + ") " + m_borrower_p1["ChineseName"];
			m_message1_p1->SetLabel(wxString::FromUTF8(success_message));
			//wxLogStatus("此 iPad 已成功借出");
			reset_p1();
			return;
		}
		else
		{
			m_message1_p1->SetLabel(wxString::FromUTF8("此卡為學生卡，請為要借出的 iPad 拍卡"));
			//wxLogStatus("此卡為學生卡，請為要借出的 iPad 拍卡");
			return;
		}
	}
}

