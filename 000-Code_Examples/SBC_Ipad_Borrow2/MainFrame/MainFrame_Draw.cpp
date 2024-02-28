#include "../MainFrame.h"

void MainFrame::init_draw()
{
	m_full_height = wxDisplay(wxDisplay::GetFromWindow(this)).GetClientArea().GetHeight();

	for (int i = 0; i < m_total_pages; ++i)
	{
		wxPanel * page = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(m_width, m_full_height));
		m_pages.push_back(page);
	}

	draw_p1();
	draw_p2();
	draw_p3();
	draw_p4();
	draw_setup_page();

	for (int i = 0; i < (m_total_pages - 1); ++i)
	{
		wxGrid * grid = new wxGrid(m_pages[i], wxID_ANY, wxPoint(0, m_headers_offset[i]), wxSize(m_width, m_full_height - m_headers_offset[i]));
		m_grids.push_back(grid);
		draw_grid(i);
	}

	draw_empty_messages();
}

void MainFrame::draw_p1()
{
	m_pages[0]->SetBackgroundColour(m_white_colour);

	m_logo_handler_p1 = new wxPNGHandler;
	wxImage::AddHandler(m_logo_handler_p1);
	m_logo_image_p1 = new wxStaticBitmap(m_pages[0], wxID_ANY, wxBitmap("C:\\Users\\Public\\Documents\\ipad_borrow_data\\sbc_logo.png", wxBITMAP_TYPE_PNG), wxPoint(13, 13), wxSize(80, 80));

	m_title_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("歡迎使用聖文德書院學生 iPad 借用管理系統"), wxPoint(120, 13));
	m_title_p1->SetForegroundColour(m_main_colour);
	m_title_p1->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_status_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("狀態: 借出 / 歸還"), wxPoint(m_width - 180, 13));
	m_status_p1->SetForegroundColour(m_main_colour);
	m_status_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("如要借出 iPad, 請先讓學生拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"), wxPoint(120, 50));
	m_message1_p1->SetForegroundColour(m_main_colour);
	m_message1_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message2_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("借出 - 請先讓學生拍卡, 然後為 iPad 拍卡"), wxPoint(120, 80));
	m_message2_p1->SetForegroundColour(m_second_colour);
	m_message2_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("歸還 - 請讓 iPad 拍卡"), wxPoint(600, 80));
	m_message3_p1->SetForegroundColour(m_second_colour);
	m_message3_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message4_p1 = new wxStaticText(m_pages[0], wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint((m_width / 2) - 260, 120));
	m_message4_p1->SetForegroundColour(m_main_colour);
	m_message4_p1->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void MainFrame::draw_p2()
{
	m_pages[1]->SetBackgroundColour(m_p2bg_colour);
	Time t = Time::get_time(m_history_unixtime_p2);
	string title = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號 (" + t.str_chi_weekday + ") 學生歸還 iPad 記錄";
	m_title_p2 = new wxStaticText(m_pages[1], wxID_ANY, wxString::FromUTF8(title), wxPoint(13, 13));
	m_title_p2->SetForegroundColour(m_main_colour);
	m_title_p2->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p2 = new wxStaticText(m_pages[1], wxID_ANY, wxString::FromUTF8("如要查看所有學生歸還 iPad 記錄, 請瀏覽路徑:"), wxPoint(13, 50));
	m_message1_p2->SetForegroundColour(m_main_colour);
	m_message1_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_textctrl1_p2 = new wxTextCtrl(m_pages[1], wxID_ANY, wxString::FromUTF8(m_db->m_history_folder), wxPoint(500, 46), wxSize(600, 30), wxTE_READONLY);
	m_textctrl1_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message2_p2 = new wxStaticText(m_pages[1], wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint(100, 100));
	m_message2_p2->SetForegroundColour(m_main_colour);
	m_message2_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p2 = new wxStaticText(m_pages[1], wxID_ANY, wxString::FromUTF8("按 \"=\" 跳去後一日     ||     按 \"-\" 跳去前一日"), wxPoint(800, 100));
	m_message3_p2->SetForegroundColour(m_second_colour);
	m_message3_p2->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void MainFrame::draw_p3()
{
	m_pages[2]->SetBackgroundColour(m_p3bg_colour);

	m_logo_handler_p3 = new wxPNGHandler;
	wxImage::AddHandler(m_logo_handler_p3);
	m_logo_image_p3 = new wxStaticBitmap(m_pages[2], wxID_ANY, wxBitmap("C:\\Users\\Public\\Documents\\ipad_borrow_data\\sbc_logo2.png", wxBITMAP_TYPE_PNG), wxPoint(13, 13), wxSize(80, 80));

	m_title_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("歡迎使用聖文德書院職員 iPad 借用管理系統"), wxPoint(120, 13));
	m_title_p3->SetForegroundColour(m_p3text_colour);
	m_title_p3->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_status_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("狀態: 借出 / 歸還"), wxPoint(m_width - 180, 13));
	m_status_p3->SetForegroundColour(m_p3text_colour);
	m_status_p3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("如要借出 iPad, 請先讓職員拍卡; 如要歸還 iPad, 請讓 iPad 拍卡"), wxPoint(120, 50));
	m_message1_p3->SetForegroundColour(m_p3text_colour);
	m_message1_p3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message2_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("借出 - 請先讓職員拍卡, 然後為 iPad 拍卡"), wxPoint(120, 80));
	m_message2_p3->SetForegroundColour(m_second_colour);
	m_message2_p3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("歸還 - 請讓 iPad 拍卡"), wxPoint(600, 80));
	m_message3_p3->SetForegroundColour(m_second_colour);
	m_message3_p3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message4_p3 = new wxStaticText(m_pages[2], wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint((m_width / 2) - 260, 120));
	m_message4_p3->SetForegroundColour(m_main_colour);
	m_message4_p3->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void MainFrame::draw_p4()
{
	m_pages[3]->SetBackgroundColour(m_p4bg_colour);
	Time t = Time::get_time(m_history_unixtime_p4);
	string title = std::to_string(m_history_year_p4) + " 年 " + std::to_string(m_history_month_p4) + " 月 職員歸還 iPad 記錄";
	m_title_p4 = new wxStaticText(m_pages[3], wxID_ANY, wxString::FromUTF8(title), wxPoint(13, 13));
	m_title_p4->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_p4 = new wxStaticText(m_pages[3], wxID_ANY, wxString::FromUTF8("如要查看所有職員歸還 iPad 記錄, 請瀏覽路徑:"), wxPoint(13, 50));
	m_message1_p4->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_textctrl1_p4 = new wxTextCtrl(m_pages[3], wxID_ANY, wxString::FromUTF8(m_db->m_staff_history_folder), wxPoint(500, 46), wxSize(600, 30), wxTE_READONLY);
	m_textctrl1_p4->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message2_p4 = new wxStaticText(m_pages[3], wxID_ANY, wxString::FromUTF8("按 \"F\" 或 \"space\" 跳去下一頁     ||     按 \"D\" 跳去上一頁"), wxPoint(100, 100));
	m_message2_p4->SetForegroundColour(m_main_colour);
	m_message2_p4->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message3_p4 = new wxStaticText(m_pages[3], wxID_ANY, wxString::FromUTF8("按 \"=\" 跳去下一個月     ||     按 \"-\" 跳去前一個月"), wxPoint(800, 100));
	m_message3_p4->SetForegroundColour(m_second_colour);
	m_message3_p4->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void MainFrame::draw_setup_page()
{
	m_title_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("iPad 借用管理系統設定頁面"), wxPoint(13, 13));
	m_title_sp->SetForegroundColour(m_main_colour);
	m_title_sp->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	m_message1_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("注意: 所有預設數據庫資料路徑均為固定, 不能修改學生及iPad的儲存路徑, 包括所有此程式要用的數據庫Folder總路徑為:"), wxPoint(50, 60));
	m_message1_sp->SetForegroundColour(m_main_colour);
	m_message1_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl1_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("C:\\Users\\Public\\Documents\\ipad_borrow_data\\"), wxPoint(50, 85), wxSize(600, 30), wxTE_READONLY);
	m_textctrl1_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message2_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("學生歸還 iPad 記錄資料庫Folder路徑為:"), wxPoint(50, 150));
	m_message2_sp->SetForegroundColour(m_main_colour);
	m_message2_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl2_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8(m_db->m_history_folder), wxPoint(50, 175), wxSize(600, 30), wxTE_READONLY);
	m_textctrl2_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message3_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("學生借出 iPad 記錄資料庫.txt文檔路徑為:"), wxPoint(50, 250));
	m_message3_sp->SetForegroundColour(m_main_colour);
	m_message3_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl3_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8(m_db->m_blist_path), wxPoint(50, 275), wxSize(600, 30), wxTE_READONLY);
	m_textctrl3_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message4_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("學生姓名, 班別, 學號, 智能卡ID資料庫.csv文檔路徑為: (eclass-user.csv下載後預設為UTF-16, 須要用NotePad++改為UTF-8後重新儲存)"), wxPoint(50, 350));
	m_message4_sp->SetForegroundColour(m_main_colour);
	m_message4_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl4_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8(m_db->m_students_path), wxPoint(50, 375), wxSize(600, 30), wxTE_READONLY);
	m_textctrl4_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message5_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("iPad 編號及 iPad 智能卡ID資料庫.txt文檔路徑為:"), wxPoint(50, 450));
	m_message5_sp->SetForegroundColour(m_main_colour);
	m_message5_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl5_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8(m_db->m_ipads_path), wxPoint(50, 475), wxSize(600, 30), wxTE_READONLY);
	m_textctrl5_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	m_message6_sp = new wxStaticText(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8("程式運行日誌(system log)Folder路徑為:"), wxPoint(50, 550));
	m_message6_sp->SetForegroundColour(m_main_colour);
	m_message6_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_textctrl6_sp = new wxTextCtrl(m_pages[m_total_pages - 1], wxID_ANY, wxString::FromUTF8(Singleton<Logger>::instance()->m_folder_path), wxPoint(50, 575), wxSize(600, 30), wxTE_READONLY);
	m_textctrl6_sp->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void MainFrame::draw_grid(int page_index)
{
	std::vector<Database::BList_Item> & list = *(m_plists[page_index]);
	int total_borrowers = list.size();

	m_grids[page_index]->CreateGrid(total_borrowers, m_col_num[page_index]);
	m_grids[page_index]->SetRowLabelSize(0);
	m_grids[page_index]->SetColLabelSize(36);
	m_grids[page_index]->SetLabelBackgroundColour(m_grid_bg_colour[page_index]);
	m_grids[page_index]->SetLabelFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	m_grids[page_index]->SetBackgroundColour(m_grid_bg_colour[page_index]);
	m_grids[page_index]->SetDefaultCellBackgroundColour(m_grid_bg_colour[page_index]);

	for (int i = 0; i < m_col_num[page_index]; ++i)
	{
		m_grids[page_index]->SetColSize(i, m_col_sizes[page_index][i]);
		m_grids[page_index]->SetColLabelValue(i, wxString::FromUTF8(m_col_titles[page_index][i]));
	}

	for (int i = 0; i < total_borrowers; ++i)
	{
		m_grids[page_index]->SetRowSize(i, 45);
		for (int j = 0; j < m_col_num[page_index]; ++j)
		{

			if (("Time" == m_titles[page_index][j] && (0 == page_index || 2 == page_index)))
			{
				string str_time = list[i][m_titles[page_index][j]];
				long long unix = std::atoll(str_time.c_str());
				Time t = Time::get_time(unix);
				str_time = t.str_chi_time;
				m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(str_time));
			}
			else
			{
				m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(list[i][m_titles[page_index][j]]));
			}
			m_grids[page_index]->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			m_grids[page_index]->SetCellBackgroundColour(i, j, m_cell_bg_colour[page_index]);
			m_grids[page_index]->SetCellTextColour(i, j, m_cell_text_colour[page_index]);
			m_grids[page_index]->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grids[page_index]->SetReadOnly(i, j);
		}
	}
}

void MainFrame::push(int page_index)
{
	std::vector<Database::BList_Item> & list = *(m_plists[page_index]);

	m_grids[page_index]->InsertRows();
	int i = 0;
	m_grids[page_index]->SetRowSize(i, 45);
	for (int j = 0; j < m_col_num[page_index]; ++j)
	{
		m_grids[page_index]->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
		m_grids[page_index]->SetCellBackgroundColour(i, j, m_cell_bg_colour[page_index]);
		m_grids[page_index]->SetCellTextColour(i, j, m_cell_text_colour[page_index]);
		if ("Time" == m_titles[page_index][j] && (0 == page_index || 2 == page_index))
		{
			m_grids[page_index]->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			string str_time = list[i][m_titles[page_index][j]];
			long long unix = std::atoll(str_time.c_str());
			Time t = Time::get_time(unix);
			str_time = t.str_chi_time;
			m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(str_time));
		}
		else
		{
			m_grids[page_index]->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(list[i][m_titles[page_index][j]]));
		}
		m_grids[page_index]->SetReadOnly(i, j);
	}
	show_empty(page_index);
}

void MainFrame::append(int page_index)
{
	const Time t = Time::now();
	switch (page_index)
	{
	case 1: {
		if (t.m_year != m_history_year_p2 || t.m_month != m_history_month_p2 || t.m_day != m_history_day_p2)
			return;
	}
		  break;
	case 3: {
		if (t.m_year != m_history_year_p4 || t.m_month != m_history_month_p4)
			return;
	}
		  break;
	default:
		return;
	}

	std::vector <Database::BList_Item> & list = *(m_plists[page_index]);

	m_grids[page_index]->AppendRows(1, true);
	int i = list.size() - 1;
	m_grids[page_index]->SetRowSize(i, 45);
	for (int j = 0; j < m_col_num[page_index]; ++j)
	{
		m_grids[page_index]->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
		m_grids[page_index]->SetCellBackgroundColour(i, j, m_bg_colour);
		m_grids[page_index]->SetCellTextColour(i, j, m_black_colour);
		m_grids[page_index]->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(list[i][m_titles[page_index][j]]));
		m_grids[page_index]->SetReadOnly(i, j);
	}
}

void MainFrame::draw_empty_messages()
{
	std::vector<wxString> msg{
		wxString::FromUTF8("暫未有學生借用 iPad"),
		wxString::FromUTF8(std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號沒有學生借出 / 歸還 iPad 記錄"),
		wxString::FromUTF8("暫未有職員借用 iPad"),
		wxString::FromUTF8(std::to_string(m_history_year_p4) + " 年 " + std::to_string(m_history_month_p4) + " 月 沒有職員借出 / 歸還 iPad 記錄")
	};

	std::vector<wxPoint> pt{
		wxPoint((m_width / 2) - 130, 100),
		wxPoint((m_width / 2) - 260, 100),
		wxPoint((m_width / 2) - 130, 100),
		wxPoint((m_width / 2) - 260, 100)
	};

	for (int i = 0; i < (m_total_pages - 1); ++i)
	{
		wxStaticText * p_text = new wxStaticText(m_grids[i], wxID_ANY, msg[i], pt[i]);
		m_empty_messages.push_back(p_text);
		m_empty_messages[i]->SetForegroundColour(m_empty_msg_colour[i]);
		m_empty_messages[i]->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		show_empty(i);
	}
}

void MainFrame::show_empty(int page_index)
{
	switch (page_index)
	{
	case 1: {
		Time t = Time::get_time(m_history_unixtime_p2);
		string empty_message = std::to_string(m_history_year_p2) + " 年 " + std::to_string(m_history_month_p2) + " 月" + std::to_string(m_history_day_p2) + " 號 (" + t.str_chi_weekday + ") 沒有學生歸還 iPad 記錄";
		m_empty_messages[1]->SetLabel(wxString::FromUTF8(empty_message));
	}
		  break;
	case 3: {
		Time t = Time::get_time(m_history_unixtime_p4);
		string empty_message = std::to_string(m_history_year_p4) + " 年 " + std::to_string(m_history_month_p4) + " 月 沒有職員歸還 iPad 記錄";
		m_empty_messages[3]->SetLabel(wxString::FromUTF8(empty_message));
	}
		  break;
	default:
		break;
	}
	const int size = m_plists[page_index]->size();

	if (0 == size)
		m_empty_messages[page_index]->Show();
	else
		m_empty_messages[page_index]->Hide();
}

void MainFrame::redraw_grid(int page_index, bool is_exist)
{
	if (1 != page_index && 3 != page_index)
		return;

	int old_rows = m_grids[page_index]->GetNumberRows();

	if (old_rows > 0)
		m_grids[page_index]->DeleteRows(0, old_rows, true);

	if (!is_exist)
	{
		show_empty(page_index);
		return;
	}

	auto list = *m_plists[page_index];
	int total_items = list.size();
	m_grids[page_index]->AppendRows(total_items, true);

	for (int i = 0; i < total_items; ++i)
	{
		m_grids[page_index]->SetRowSize(i, 45);
		for (int j = 0; j < m_col_num[page_index]; ++j)
		{
			m_grids[page_index]->SetCellAlignment(i, j, wxALIGN_CENTER, wxALIGN_CENTER);
			m_grids[page_index]->SetCellBackgroundColour(i, j, m_bg_colour);
			m_grids[page_index]->SetCellTextColour(i, j, m_black_colour);
			m_grids[page_index]->SetCellFont(i, j, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			m_grids[page_index]->SetCellValue(i, j, wxString::FromUTF8(list[i][m_titles[page_index][j]]));
			m_grids[page_index]->SetReadOnly(i, j);
		}
	}

	show_empty(page_index);
}

