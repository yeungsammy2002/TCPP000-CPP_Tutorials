#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	m_logger = Singleton<Logger>::instance();
	m_logger->open("./test.log");

	m_db = Singleton<Database>::instance();
	m_db->m_logger = Singleton<Logger>::instance();
	m_db->set_path("student", "C:\\Users\\tyeung\\Desktop\\demo_data\\eclass-user.csv");
	m_db->load("student");
	m_db->set_path("ipad", "C:\\Users\\tyeung\\Desktop\\demo_data\\ipad_list.txt");
	m_db->load("ipad");
	m_db->set_path("blist", "C:\\Users\\tyeung\\Desktop\\demo_data\\borrowed_list.txt");
	m_db->load("blist");

	wxPanel * panel = new wxPanel(this);

	//wxButton * closeButton = new wxButton(panel, wxID_ANY, "Close", wxPoint(750, 0), wxSize(50, 50));
	//closeButton->SetBackgroundColour(wxColour(255, 90, 90));
	//closeButton->Bind(wxEVT_BUTTON, &MainFrame::OnCloseClicked, this);

	wxGrid * grid = new wxGrid(panel, wxID_ANY, wxPoint(0, 100), wxSize(800, 500));
	m_grid = grid;
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);

	draw_grid(grid);

	panel->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyEvent, this);

	CreateStatusBar();

	wxLogStatus("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡");
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
		tap(cardId);
		wx_cardId = "";

		//if (PERSON == m_turn)
		//{
		//	m_student = m_db->find_item(cardId, "student");

		//	if (nullptr == m_student)
		//	{
		//		wxLogStatus("查無此卡，請學生再拍卡: ");
		//		return;
		//	}

		//	Database::Item & student = *m_student;
		//	//string student_data = student["ClassName"] + "(" + student["ClassNumber"] + ") " + student["ChineseName"] + " " + student["EnglishName"];
		//	//wxLogStatus(wxString(student_data));

		//	m_borrower["BorrowerCardID"] = cardId;
		//	m_borrower["ChineseName"] = student["ChineseName"];
		//	m_borrower["EnglishName"] = student["EnglishName"];
		//	m_borrower["ClassName"] = student["ClassName"];
		//	m_borrower["ClassNumber"] = student["ClassNumber"];

		//	m_turn = IPAD;
		//	wxLogStatus("請為iPad拍卡: ");
		//	return;
		//}

		//if (IPAD == m_turn)
		//{
		//	if (nullptr == m_student)
		//	{
		//		wxLogStatus("請學生再拍卡: ");
		//		m_turn = PERSON;
		//		return;
		//	}

		//	Database::Item * p_ipad = m_db->find_item(cardId, "ipad");
		//	if (nullptr == p_ipad)
		//	{
		//		wxLogStatus("查無此iPad，請為iPad再拍卡: ");
		//		return;
		//	}

		//	Time t = Time::now();
		//	std::stringstream ss;

		//	// testing only
		//	//ss << t.m_year << "-" << t.m_month << "-" << t.m_day << " (" << t.str_weekday << ") " << t.str_hour << ":" << t.str_minute << ":" << t.str_second;

		//	ss << t.m_unixtime;
		//	m_borrower["Time"] = ss.str();
		//	m_borrower["ReturnTime"] = "";
		//	m_borrower["IpadCardID"] = cardId;
		//	m_borrower["IpadNumber"] = (*p_ipad)["IpadNumber"];
		//	(m_db->m_blist).push_back(m_borrower);

		//	// testing only
		//	//string borrower_data = "借出時間: " + (m_db->m_blist)[(m_db->m_blist).size() - 1]["Time"] + " " +
		//	//	//(m_db->m_blist)[(m_db->m_blist).size() - 1]["ReturnTime"] + " " +
		//	//	//(m_db->m_blist)[(m_db->m_blist).size() - 1]["BorrowerCardID"] + " " +
		//	//	"學生姓名: " + (m_db->m_blist)[(m_db->m_blist).size() - 1]["ChineseName"] + " " +
		//	//	(m_db->m_blist)[(m_db->m_blist).size() - 1]["EnglishName"] + " " +
		//	//	"班別: " + (m_db->m_blist)[(m_db->m_blist).size() - 1]["ClassName"] + " " +
		//	//	"學號: " + (m_db->m_blist)[(m_db->m_blist).size() - 1]["ClassNumber"] + " " +
		//	//	//(m_db->m_blist)[(m_db->m_blist).size() - 1]["IpadCardID"] + " " +
		//	//	"iPad號碼: " + (m_db->m_blist)[(m_db->m_blist).size() - 1]["IpadNumber"];
		//	//wxLogStatus(wxString(borrower_data));

		//	clear_round();
		//	m_turn = PERSON;
		//	wxLogStatus("請學生拍卡: ");

		//	//m_grid->DeleteRows(0, 1, true);

		//	//m_grid->AppendRows(1, true);
		//	append();

		//	return;
		//}
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

const int MainFrame::col_sizes[7]{ 200,200,80,120,40,30,100 };

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

	if (!m_lock)
	{
		Database::Item_Itr item = m_db->find_item(cardId);

		if ("true" == (*item)["is_person"])
		{

			Database::BList_Item_Itr borrower = m_db->find_borrower(cardId);
			if ((m_db->m_blist).end() != borrower)
			{
				wxLogStatus("此學生已借入 iPad, 不能再借, 如要歸還, 請為歸還 iPad 拍卡即可");
				reset();
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

				std::stringstream ss;
				ss << Time::now().m_unixtime;
				rlitem["ReturnTime"] = ss.str();
				(m_db->m_rlist).push_back(rlitem);

				int index = blitem - (m_db->m_blist).begin();
				m_grid->DeleteRows(index, 1, true);
				(m_db->m_blist).erase(blitem);
				wxLogStatus("iPad 已歸還");
				return;
			}
			else
			{
				wxLogStatus("此 iPad 沒有借出, 如要借出 iPad, 請先讓學生拍卡, 再讓借出 iPad 拍卡");
				return;
			}
		}
	}
	else
	{
		Database::Item_Itr item = m_db->find_item(cardId);
		if ("false" == (*item)["is_person"])
		{
			Database::BList_Item_Itr borrowed_ipad = m_db->find_blitem(cardId);
			if ((m_db->m_blist).end() != borrowed_ipad)
			{
				wxLogStatus("此 iPad 不能再借出, 請先拍卡歸還");
				reset();
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
			wxLogStatus("此 iPad 已成功借出");
			reset();
			return;
		}
	}
}