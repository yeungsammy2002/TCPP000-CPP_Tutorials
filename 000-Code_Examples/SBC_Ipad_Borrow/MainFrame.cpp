#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	m_logger = Singleton<Logger>::instance();
	m_logger->open("C:\\Users\\tyeung\\Desktop\\demo_data\\log.txt");

	m_db = Singleton<Database>::instance();
	m_db->m_logger = Singleton<Logger>::instance();
	m_db->load("student", "C:\\Users\\tyeung\\Desktop\\demo_data\\eclass-user.csv");
	m_db->load("ipad", "C:\\Users\\tyeung\\Desktop\\demo_data\\ipad_list.txt");
	m_db->load("blist", "C:\\Users\\tyeung\\Desktop\\demo_data\\borrowed_list.txt");
	m_db->load("rlist", "C:\\Users\\tyeung\\Desktop\\demo_data\\history\\history.txt");

	m_panel = new wxPanel(this);
	m_panel->SetClientSize(800, 800);
	m_panel->Center();

	m_title = new wxStaticText(m_panel, wxID_ANY, "歡迎使用 iPad 借用管理系統", wxPoint(13, 13));
	m_title->SetForegroundColour(wxColour(2, 52, 108));
	m_title->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_status = new wxStaticText(m_panel, wxID_ANY, "狀態: 借出 / 歸還", wxPoint(600, 13));
	m_status->SetForegroundColour(wxColour(2, 52, 108));
	m_status->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1 = new wxStaticText(m_panel, wxID_ANY, "如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡", wxPoint(13, 50));
	m_message1->SetForegroundColour(wxColour(2, 52, 108));
	m_message1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxLogStatus("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡");

	m_message2 = new wxStaticText(m_panel, wxID_ANY, "借出 - 如要借出 iPad, 請先讓學生拍卡", wxPoint(13, 80));
	m_message2->SetForegroundColour(wxColour(255, 0, 0));
	m_message2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3 = new wxStaticText(m_panel, wxID_ANY, "歸還 - 如要歸還 iPad, 請讓 iPad 拍卡", wxPoint(13, 110));
	m_message3->SetForegroundColour(wxColour(0, 101, 27));
	m_message3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	wxGrid * grid = new wxGrid(m_panel, wxID_ANY, wxPoint(0, 150), wxSize(800, 500));
	m_grid = grid;
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);

	draw_grid(grid);

	show_empty();

	m_panel->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);

	CreateStatusBar();
}

void MainFrame::OnCloseClicked(wxCommandEvent & evt)
{
	Close();
}

void MainFrame::OnKeyEvent(wxKeyEvent & evt)
{
	int keyCode = evt.GetKeyCode();
	static wxString wx_cardId;

	if (13 != keyCode)
	{
		wxChar keyChar = evt.GetUnicodeKey();
		wx_cardId.append(keyChar);
	}
	else
	{
		//wxLogStatus(wx_cardId);
		string cardId = wx_cardId.ToStdString();
		//log_debug(str.c_str());

		if ("" == cardId)
			return;

		tap(cardId);
		wx_cardId = "";
	}
}

void MainFrame::reset()
{
	m_borrower["Time"] = "";
	m_borrower["ReturnTime"] = "";
	m_borrower["BorrowerCardID"] = "";
	m_borrower["ChineseName"] = "";
	m_borrower["EnglishName"] = "";
	m_borrower["ClassName"] = "";
	m_borrower["ClassNumber"] = "";
	m_borrower["IpadCardID"] = "";
	m_borrower["IpadNumber"] = "";
	m_lock = false;
	m_status->SetForegroundColour(wxColour(2, 52, 108));
	m_status->SetLabel("狀態: 借出 / 歸還");
	m_message2->SetLabel("借出 - 如要借出 iPad, 請先讓學生拍卡");
	m_message2->SetForegroundColour(wxColour(255, 0, 0));
	m_message3->SetLabel("歸還 - 如要歸還 iPad, 請讓 iPad 拍卡");
	m_message3->SetForegroundColour(wxColour(0, 101, 27));
}

const char * MainFrame::s_borrower_titles[7]{
	"Time",
	"ReturnTime",
	"ChineseName",
	"EnglishName",
	"ClassName",
	"ClassNumber",
	"IpadNumber"
};

const int MainFrame::col_sizes[7]{ 200,200,80,120,40,30,130 };

void MainFrame::draw_grid(wxGrid * grid)
{
	int total_borrowers = (m_db->m_blist).size();

	grid->CreateGrid(total_borrowers, 7);

	for (int i = 0; i < 7; ++i)
	{
		grid->SetColSize(i, col_sizes[i]);
	}

	for (int i = 0; i < total_borrowers; ++i)
	{
		grid->SetRowSize(i, 45);
	}

	for (int i = 0; i < total_borrowers; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			grid->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			grid->SetCellBackgroundColour(i, j, wxColour(2, 52, 108));
			grid->SetCellTextColour(i, j, wxColour(255, 255, 255));
			if ("Time" == s_borrower_titles[j] || "ReturnTime" == s_borrower_titles[j])
			{
				grid->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				string str_time = (m_db->m_blist)[i][s_borrower_titles[j]];
				std::stringstream ss;

				if ("" != str_time)
				{
					long long unix = std::atoll(str_time.c_str());
					Time t = Time::get_time(unix);
					ss << t.m_year << "-" << t.m_month << "-" << t.m_day << " (" << t.str_weekday << ") " << t.str_hour << ":" << t.str_minute << ":" << t.str_second;
				}
				else
				{
					ss << "-- N/A --";
				}

				grid->SetCellValue(i, j, ss.str());
			}
			else
			{
				grid->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				grid->SetCellValue(i, j, (m_db->m_blist)[i][s_borrower_titles[j]]);
			}
			grid->SetReadOnly(i, j);
		}
	}
}

void MainFrame::append()
{
	m_grid->AppendRows(1, true);

	int i = (m_db->m_blist).size() - 1;
	m_grid->SetRowSize(i, 45);
	for (int j = 0; j < 7; ++j)
	{
		m_grid->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
		m_grid->SetCellBackgroundColour(i, j, wxColour(2, 52, 108));
		m_grid->SetCellTextColour(i, j, wxColour(255, 255, 255));
		if ("Time" == s_borrower_titles[j] || "ReturnTime" == s_borrower_titles[j])
		{
			m_grid->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			string str_time = (m_db->m_blist)[i][s_borrower_titles[j]];
			std::stringstream ss;

			if ("" != str_time)
			{
				long long unix = std::atoll(str_time.c_str());
				Time t = Time::get_time(unix);
				ss << t.m_year << "-" << t.m_month << "-" << t.m_day << " (" << t.str_weekday << ") " << t.str_hour << ":" << t.str_minute << ":" << t.str_second;
			}
			else
			{
				ss << "-- N/A --";
			}

			m_grid->SetCellValue(i, j, ss.str());
		}
		else
		{
			m_grid->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grid->SetCellValue(i, j, (m_db->m_blist)[i][s_borrower_titles[j]]);
		}
		m_grid->SetReadOnly(i, j);
	}
}

void MainFrame::tap(const string & cardId)
{
	//wxLogStatus("tap");
	Database::Item_Itr item = m_db->find_item(cardId);
	m_message1->SetLabel("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡");

	if ((m_db->m_items).end() == item)
	{
		reset();
		m_message1->SetLabel("查無此學生或此 iPad, 請重新拍卡");
		wxLogStatus("無此學生或 iPad 請重新輸入");
		return;
	}

	if (!m_lock)
	{
		if ("true" == (*item)["is_person"])
		{

			Database::BList_Item_Itr borrower = m_db->find_borrower(cardId);
			if ((m_db->m_blist).end() != borrower)
			{
				reset();

				string unsuccess_message = "學生" + (*borrower)["ChineseName"] + " "
					+ (*borrower)["ClassName"] + "(" + (*borrower)["ClassNumber"] + ") 已借入 iPad "
					+ (*borrower)["IpadNumber"] + ", 如要歸還, 請為歸還 iPad 拍卡即可";

				m_message1->SetLabel(unsuccess_message);
				wxLogStatus("此學生已借入 iPad, 不能再借, 如要歸還, 請為歸還 iPad 拍卡即可");
				return;
			}
			//wxLogStatus("tap");
			if ("student" == (*item)["type"])
			{
				Database::Item & student = *item;

				m_borrower["BorrowerCardID"] = cardId;
				m_borrower["ChineseName"] = student["ChineseName"];
				m_borrower["EnglishName"] = student["EnglishName"];
				m_borrower["ClassName"] = student["ClassName"];
				m_borrower["ClassNumber"] = student["ClassNumber"];

				m_lock = true;

				m_status->SetLabel("狀態: 借出");
				m_status->SetForegroundColour(wxColour(255, 0, 0));
				m_message2->SetForegroundColour(wxColour(255, 0, 0));
				m_message3->SetForegroundColour(wxColour(255, 0, 0));

				string message1 = "借入學生: " + student["ChineseName"] + " " + student["EnglishName"] + " 班別: " + student["ClassName"] + " 學號: " + student["ClassNumber"];
				m_message2->SetLabel(message1);
				m_message3->SetLabel("請為要借出的 iPad 拍卡: ");
				wxLogStatus("請為iPad拍卡: ");
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
				string message2 = "學生" + (*blitem)["ChineseName"] + " " + (*blitem)["EnglishName"] + " " + (*blitem)["ClassName"] + " " + (*blitem)["ClassNumber"];
				string message3 = "已歸還 iPad: " + (*blitem)["IpadNumber"];

				std::stringstream ss;
				ss << Time::now().m_unixtime;
				rlitem["ReturnTime"] = ss.str();
				(m_db->m_rlist).push_back(rlitem);
				m_db->write_to_history(rlitem);

				int index = blitem - (m_db->m_blist).begin();
				m_grid->DeleteRows(index, 1, true);
				//m_db->remove(*blitem);
				(m_db->m_blist).erase(blitem);
				m_db->update();

				m_message2->SetForegroundColour(wxColour(0, 101, 27));
				m_message3->SetForegroundColour(wxColour(0, 101, 27));
				m_message2->SetLabel(message2);
				m_message3->SetLabel(message3);
				wxLogStatus("iPad 已歸還");
				show_empty();
				return;
			}
			else
			{
				reset();
				m_message1->SetLabel("此 iPad 沒有借出, 如要借出 iPad, 請先讓學生拍卡, 再讓借出 iPad 拍卡");
				wxLogStatus("此 iPad 沒有借出, 如要借出 iPad, 請先讓學生拍卡, 再讓借出 iPad 拍卡");
				return;
			}
		}
	}
	else
	{
		if ("false" == (*item)["is_person"])
		{
			Database::BList_Item_Itr borrowed_ipad = m_db->find_blitem(cardId);
			if ((m_db->m_blist).end() != borrowed_ipad)
			{
				reset();
				string unsuccess_message = "iPad " + (*borrowed_ipad)["IpadNumber"] + " 已經借給學生"
					+ (*borrowed_ipad)["ChineseName"] + (*borrowed_ipad)["ClassName"]
					+ "(" + (*borrowed_ipad)["ClassNumber"] + "), 請先拍卡歸還";
				m_message1->SetLabel(unsuccess_message);
				wxLogStatus("此 iPad 不能再借出, 請先拍卡歸還");
				return;
			}

			Database::Item & ipad = *item;

			std::stringstream ss;
			ss << Time::now().m_unixtime;
			m_borrower["Time"] = ss.str();
			m_borrower["ReturnTime"] = "";
			m_borrower["IpadCardID"] = cardId;
			m_borrower["IpadNumber"] = ipad["IpadNumber"];
			(m_db->m_blist).push_back(m_borrower);
			append();
			m_db->write_to_file(m_borrower);

			string success_message = "iPad " + m_borrower["IpadNumber"] + " 已成功借給 " + m_borrower["ClassName"] + "(" + m_borrower["ClassNumber"] + ") " + m_borrower["ChineseName"];
			m_message1->SetLabel(success_message);
			wxLogStatus("此 iPad 已成功借出");
			reset();
			return;
		}
		else
		{
			wxLogStatus("此卡為學生卡，請為要借出的 iPad 拍卡");
			return;
		}
	}
}

void MainFrame::show_empty()
{
	if (0 == (m_db->m_blist).size())
	{
		m_empty_message = new wxStaticText(m_panel, wxID_ANY, "暫未有學生借用 iPad", wxPoint(280, 200));
		m_empty_message->SetForegroundColour(wxColour(2, 52, 108));
		m_empty_message->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	}
	else
	{
		delete m_empty_message;
		m_empty_message = nullptr;
	}
}