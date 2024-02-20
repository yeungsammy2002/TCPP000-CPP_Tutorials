#include "../MainFrame.h"

void MainFrame::init_draw()
{
	m_full_height = wxDisplay(wxDisplay::GetFromWindow(this)).GetClientArea().GetHeight();
	draw_p1();
	draw_p2();
	draw_setup_page();
}

void MainFrame::draw_p2()
{
	m_page2 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(m_width, m_full_height));
	Time t = Time::get_time(m_history_unixtime_p2);
	string title = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號 (" + t.str_chi_weekday + ") 學生歸還 iPad 記錄";
	m_title_p2 = new wxStaticText(m_page2, wxID_ANY, wxString::FromUTF8(title), wxPoint(13, 13));
	m_title_p2->SetForegroundColour(m_main_colour);
	m_title_p2->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p2 = new wxStaticText(m_page2, wxID_ANY, wxString::FromUTF8("如要查看所有學生歸還 iPad 記錄, 請瀏覽路徑:"), wxPoint(13, 50));
	m_message1_p2->SetForegroundColour(m_main_colour);
	m_message1_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_textctrl1_p2 = new wxTextCtrl(m_page2, wxID_ANY, wxString::FromUTF8(m_db->m_history_folder), wxPoint(500, 46), wxSize(600, 30), wxTE_READONLY);
	m_textctrl1_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message2_p2 = new wxStaticText(m_page2, wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint(100, 100));
	m_message2_p2->SetForegroundColour(m_forth_colour);
	m_message2_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p2 = new wxStaticText(m_page2, wxID_ANY, wxString::FromUTF8("按 \"=\" 跳去後一日     ||     按 \"-\" 跳去前一日"), wxPoint(800, 100));
	m_message3_p2->SetForegroundColour(m_third_colour);
	m_message3_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	draw_grid_p2();
}

void MainFrame::draw_p1()
{
	m_page1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(m_width, m_full_height));
	m_page1->SetBackgroundColour(m_white_colour);

	m_logo_handler_p1 = new wxPNGHandler;
	wxImage::AddHandler(m_logo_handler_p1);
	m_logo_image_p1 = new wxStaticBitmap(m_page1, wxID_ANY, wxBitmap("C:\\Users\\Public\\Documents\\ipad_borrow_data\\sbc_logo.png", wxBITMAP_TYPE_PNG), wxPoint(13, 13), wxSize(80, 80));

	m_title_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("歡迎使用聖文德書院學生 iPad 借用管理系統"), wxPoint(120, 13));
	m_title_p1->SetForegroundColour(m_main_colour);
	m_title_p1->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_status_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("狀態: 借出 / 歸還"), wxPoint(m_width - 180, 13));
	m_status_p1->SetForegroundColour(m_main_colour);
	m_status_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"), wxPoint(120, 50));
	m_message1_p1->SetForegroundColour(m_main_colour);
	m_message1_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message2_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("借出 - 如要借出 iPad, 請先讓學生拍卡"), wxPoint(120, 80));
	m_message2_p1->SetForegroundColour(m_second_colour);
	m_message2_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("歸還 - 如要歸還 iPad, 請讓 iPad 拍卡"), wxPoint(600, 80));
	m_message3_p1->SetForegroundColour(m_third_colour);
	m_message3_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message4_p1 = new wxStaticText(m_page1, wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint((m_width / 2) - 260, 120));
	m_message4_p1->SetForegroundColour(m_forth_colour);
	m_message4_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	draw_grid_p1();
}

void MainFrame::draw_grid_p2()
{
	m_grid_p2 = new wxGrid(m_page2, wxID_ANY, wxPoint(1, m_header_h_p2), wxSize(m_width, m_full_height - m_header_h_p2));

	int total_items = (m_db->m_rlist).size();
	m_grid_p2->CreateGrid(total_items, m_col_num_p2);
	m_grid_p2->SetRowLabelSize(0);
	m_grid_p2->SetColLabelSize(36);
	m_grid_p2->SetColLabelValue(0, wxString::FromUTF8("借出時間"));
	m_grid_p2->SetColLabelValue(1, wxString::FromUTF8("歸還時間"));
	m_grid_p2->SetColLabelValue(2, wxString::FromUTF8("學生姓名"));
	m_grid_p2->SetColLabelValue(3, wxString::FromUTF8("學生英文姓名"));
	m_grid_p2->SetColLabelValue(4, wxString::FromUTF8("班別"));
	m_grid_p2->SetColLabelValue(5, wxString::FromUTF8("學號"));
	m_grid_p2->SetColLabelValue(6, wxString::FromUTF8("iPad 號碼"));

	for (int i = 0; i < m_col_num_p2; ++i)
	{
		m_grid_p2->SetColSize(i, m_col_sizes_p2[i]);
	}

	for (int i = 0; i < total_items; ++i)
	{
		m_grid_p2->SetRowSize(i, 45);
	}

	for (int i = 0; i < total_items; ++i)
	{
		for (int j = 0; j < m_col_num_p2; ++j)
		{
			m_grid_p2->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			m_grid_p2->SetCellBackgroundColour(i, j, m_bg_colour);
			m_grid_p2->SetCellTextColour(i, j, m_black_colour);
			m_grid_p2->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grid_p2->SetCellValue(i, j, wxString::FromUTF8((m_db->m_rlist)[i][m_rlitem_titles_p2[j]]));
			m_grid_p2->SetReadOnly(i, j);
		}
	}

	string empty_message = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號沒有學生借出 / 歸還 iPad 記錄";
	m_empty_message_p2 = new wxStaticText(m_grid_p2, wxID_ANY, wxString::FromUTF8(empty_message), wxPoint((m_width / 2) - 260, 100));
	m_empty_message_p2->SetForegroundColour(m_main_colour);
	m_empty_message_p2->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	show_empty_p2();

}

void MainFrame::redraw_grid_p2(bool is_exist)
{

	int old_rows = m_grid_p2->GetNumberRows();

	if (old_rows > 0)
		m_grid_p2->DeleteRows(0, old_rows, true);

	if (!is_exist)
	{
		show_empty_p2();
		return;
	}

	int total_items = (m_db->m_rlist).size();

	m_grid_p2->AppendRows(total_items, true);

	for (int i = 0; i < total_items; ++i)
	{
		m_grid_p2->SetRowSize(i, 45);
		for (int j = 0; j < m_col_num_p2; ++j)
		{
			m_grid_p2->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			m_grid_p2->SetCellBackgroundColour(i, j, m_bg_colour);
			m_grid_p2->SetCellTextColour(i, j, m_black_colour);
			m_grid_p2->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grid_p2->SetCellValue(i, j, wxString::FromUTF8((m_db->m_rlist)[i][m_rlitem_titles_p2[j]]));
			m_grid_p2->SetReadOnly(i, j);
		}
	}

	show_empty_p2();
}

void MainFrame::draw_grid_p1()
{
	int total_borrowers = (m_db->m_blist).size();

	m_grid_p1 = new wxGrid(m_page1, wxID_ANY, wxPoint(0, m_header_h_p1), wxSize(m_width, m_full_height - m_header_h_p1));
	m_grid_p1->CreateGrid(total_borrowers, m_col_num_p1);
	m_grid_p1->SetRowLabelSize(0);
	m_grid_p1->SetColLabelSize(36);
	m_grid_p1->SetColLabelValue(0, wxString::FromUTF8("借出時間"));
	m_grid_p1->SetColLabelValue(1, wxString::FromUTF8("學生姓名"));
	m_grid_p1->SetColLabelValue(2, wxString::FromUTF8("學生英文姓名"));
	m_grid_p1->SetColLabelValue(3, wxString::FromUTF8("班別"));
	m_grid_p1->SetColLabelValue(4, wxString::FromUTF8("學號"));
	m_grid_p1->SetColLabelValue(5, wxString::FromUTF8("iPad 號碼"));

	for (int i = 0; i < m_col_num_p1; ++i)
	{
		m_grid_p1->SetColSize(i, m_col_sizes_p1[i]);
	}

	for (int i = 0; i < total_borrowers; ++i)
	{
		m_grid_p1->SetRowSize(i, 45);
	}

	for (int i = 0; i < total_borrowers; ++i)
	{
		for (int j = 0; j < m_col_num_p1; ++j)
		{
			m_grid_p1->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			m_grid_p1->SetCellBackgroundColour(i, j, m_main_colour);
			m_grid_p1->SetCellTextColour(i, j, wxColour(255, 255, 255));
			if ("Time" == m_borrower_titles_p1[j])
			{
				m_grid_p1->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				string str_time = (m_db->m_blist)[i][m_borrower_titles_p1[j]];

				if ("" != str_time)
				{
					long long unix = std::atoll(str_time.c_str());
					Time t = Time::get_time(unix);
					str_time = t.str_chi_time;
				}
				else
					str_time = "-- N/A --";

				m_grid_p1->SetCellValue(i, j, wxString::FromUTF8(str_time));
			}
			else
			{
				m_grid_p1->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				m_grid_p1->SetCellValue(i, j, wxString::FromUTF8((m_db->m_blist)[i][m_borrower_titles_p1[j]]));
			}
			m_grid_p1->SetReadOnly(i, j);
		}
	}

	m_empty_message_p1 = new wxStaticText(m_grid_p1, wxID_ANY, wxString::FromUTF8("暫未有學生借用 iPad"), wxPoint((m_width / 2) - 130, 100));
	m_empty_message_p1->SetForegroundColour(m_main_colour);
	m_empty_message_p1->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	show_empty_p1();
}

void MainFrame::append_p2()
{
	const Time t = Time::now();
	if (t.m_year != m_history_year_p2 || t.m_month != m_history_month_p2 || t.m_day != m_history_day_p2)
		return;

	m_grid_p2->AppendRows(1, true);
	int i = (m_db->m_rlist).size() - 1;
	m_grid_p2->SetRowSize(i, 45);
	for (int j = 0; j < m_col_num_p2; ++j)
	{
		m_grid_p2->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
		m_grid_p2->SetCellBackgroundColour(i, j, m_bg_colour);
		m_grid_p2->SetCellTextColour(i, j, m_black_colour);
		m_grid_p2->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		m_grid_p2->SetCellValue(i, j, wxString::FromUTF8((m_db->m_rlist)[i][m_rlitem_titles_p2[j]]));
		m_grid_p2->SetReadOnly(i, j);
	}
}

void MainFrame::append_p1()
{
	m_grid_p1->AppendRows(1, true);
	int i = (m_db->m_blist).size() - 1;
	m_grid_p1->SetRowSize(i, 45);
	for (int j = 0; j < m_col_num_p1; ++j)
	{
		m_grid_p1->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
		m_grid_p1->SetCellBackgroundColour(i, j, m_main_colour);
		m_grid_p1->SetCellTextColour(i, j, wxColour(255, 255, 255));
		if ("Time" == m_borrower_titles_p1[j] || "ReturnTime" == m_borrower_titles_p1[j])
		{
			m_grid_p1->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			string str_time = (m_db->m_blist)[i][m_borrower_titles_p1[j]];

			if ("" != str_time)
			{
				long long unix = std::atoll(str_time.c_str());
				Time t = Time::get_time(unix);
				str_time = t.str_chi_time;
			}
			else
				str_time = "-- N/A --";

			m_grid_p1->SetCellValue(i, j, wxString::FromUTF8(str_time));
		}
		else
		{
			m_grid_p1->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grid_p1->SetCellValue(i, j, wxString::FromUTF8((m_db->m_blist)[i][m_borrower_titles_p1[j]]));
		}
		m_grid_p1->SetReadOnly(i, j);
	}
}

void MainFrame::show_empty_p1()
{
	if (0 == (m_db->m_blist).size())
		m_empty_message_p1->Show();
	else
		m_empty_message_p1->Hide();
}

void MainFrame::show_empty_p2()
{
	if (0 == (m_db->m_rlist).size())
	{
		Time t = Time::get_time(m_history_unixtime_p2);
		string empty_message = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號 (" + t.str_chi_weekday + ") 沒有學生歸還 iPad 記錄";
		m_empty_message_p2->SetLabel(wxString::FromUTF8(empty_message));
		m_empty_message_p2->Show();
	}
	else
		m_empty_message_p2->Hide();
}

void MainFrame::draw_setup_page()
{
	m_setup_page = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(m_width, m_full_height));

	m_title_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("iPad 借用管理系統設定頁面"), wxPoint(13, 13));
	m_title_sp->SetForegroundColour(m_main_colour);
	m_title_sp->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("注意: 所有預設數據庫資料路徑均為固定, 不能修改學生及iPad的儲存路徑, 包括所有此程式要用的數據庫Folder總路徑為:"), wxPoint(50, 60));
	m_message1_sp->SetForegroundColour(m_main_colour);
	m_message1_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl1_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8("C:\\Users\\Public\\Documents\\ipad_borrow_data\\"), wxPoint(50, 85), wxSize(600, 30), wxTE_READONLY);
	m_textctrl1_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message2_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("學生歸還 iPad 記錄資料庫Folder路徑為:"), wxPoint(50, 150));
	m_message2_sp->SetForegroundColour(m_main_colour);
	m_message2_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl2_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8(m_db->m_history_folder), wxPoint(50, 175), wxSize(600, 30), wxTE_READONLY);
	m_textctrl2_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message3_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("學生借出 iPad 記錄資料庫.txt文檔路徑為:"), wxPoint(50, 250));
	m_message3_sp->SetForegroundColour(m_main_colour);
	m_message3_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl3_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8(m_db->m_blist_path), wxPoint(50, 275), wxSize(600, 30), wxTE_READONLY);
	m_textctrl3_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message4_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("學生姓名, 班別, 學號, 智能卡ID資料庫.csv文檔路徑為: (eclass-user.csv下載後預設為UTF-16, 須要用NotePad++改為UTF-8後重新儲存)"), wxPoint(50, 350));
	m_message4_sp->SetForegroundColour(m_main_colour);
	m_message4_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl4_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8(m_db->m_students_path), wxPoint(50, 375), wxSize(600, 30), wxTE_READONLY);
	m_textctrl4_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message5_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("iPad 編號及 iPad 智能卡ID資料庫.txt文檔路徑為:"), wxPoint(50, 450));
	m_message5_sp->SetForegroundColour(m_main_colour);
	m_message5_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl5_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8(m_db->m_ipads_path), wxPoint(50, 475), wxSize(600, 30), wxTE_READONLY);
	m_textctrl5_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message6_sp = new wxStaticText(m_setup_page, wxID_ANY, wxString::FromUTF8("程式運行日誌(system log)Folder路徑為:"), wxPoint(50, 550));
	m_message6_sp->SetForegroundColour(m_main_colour);
	m_message6_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl6_sp = new wxTextCtrl(m_setup_page, wxID_ANY, wxString::FromUTF8(Singleton<Logger>::instance()->m_folder_path), wxPoint(50, 575), wxSize(600, 30), wxTE_READONLY);
	m_textctrl6_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}