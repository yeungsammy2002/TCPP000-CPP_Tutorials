#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/display.h>

#include "utilities/Logger.h"
#include "utilities/Singleton.h"
#include "utilities/Time.h"

#include "db/Database.h"

using namespace regulus::utilities;
using namespace regulus::db;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString & title);

	void init();
	void init_draw();
	void init_bind();

	void tap(const string & cardId);
	void switch_page(int page);
	void switch_page(bool move);
	void switch_page();
	void draw_grid(int page_index);
	void push(int page_index);
	void append(int page_index);
	void redraw_grid(int page_index, bool is_exist);
	void draw_empty_messages();
	void show_empty(int page_index);
	void reset();

	void draw_p1();
	void processing_student_tap_p1(Database::Item & student);
	void processing_ipad_tap_p1(Database::Item & ipad);
	void processing_return_ipad_p1(Database::Item & ipad);

	void draw_p2();
	void move_history_p2(const int id);

	void draw_p3();
	void processing_staff_tap_p3(Database::Item & staff);
	bool check_ipad_borrowed_p3(Database::Item & ipad);
	void processing_borrow_tap_p3(Database::Item & ipad);
	void processing_return_ipad_p3(Database::Item & ipad);

	void draw_p4();
	void move_history_p4(const int id);

	void draw_setup_page();

	void OnCloseClicked(wxCommandEvent & evt);
	void OnKeyEvent(wxKeyEvent & evt);
	void OnButtonClicked(wxCommandEvent & evt);

public:
	Logger * m_logger;
	Database * m_db;
	std::vector<std::vector<Database::BList_Item> *> m_plists;
private:
	const int m_width = 1320;
	int m_full_height = 600;
	int m_current_page_num = 1;
	int m_last_page_num = 5;
	wxBoxSizer * m_sizer;

	wxColour m_main_colour = wxColour(0, 50, 110);
	wxColour m_second_colour = wxColour(0, 130, 20);
	wxColour m_bg_colour = wxColour(220, 220, 220);
	wxColour m_warn_colour = wxColour(220, 0, 0);
	wxColour m_black_colour = wxColour(0, 0, 0);
	wxColour m_white_colour = wxColour(255, 255, 255);

	wxColour m_p2bg_colour = wxColour(200, 230, 255);
	wxColour m_p3bg_colour = wxColour(210, 210, 210);
	wxColour m_p4bg_colour = wxColour(190, 255, 200);

	wxColour m_p3text_colour = wxColour(50, 50, 50);

	int m_total_pages = 5;
	std::vector<wxPanel *> m_pages;
	std::vector<wxGrid *> m_grids;
	std::vector<int> m_headers_offset{ 150,130,150,130 };
	std::vector<int> m_col_num{ 6, 7, 5, 6 };
	std::vector<wxColour> m_grid_bg_colour{ m_white_colour,m_p2bg_colour,m_p3bg_colour,m_p4bg_colour };
	std::vector<wxColour> m_cell_bg_colour{ m_main_colour,m_bg_colour,m_second_colour,m_bg_colour };
	std::vector<wxColour> m_cell_text_colour{ m_white_colour,m_black_colour,m_white_colour,m_black_colour };

	std::vector<int> m_col_sizes_p1{ 400,100,350,100,50,300 };
	std::vector<int> m_col_sizes_p2{ 310,310,100,240,100,50,190 };
	std::vector<int> m_col_sizes_p3{ 350,150,150,350,300 };
	std::vector<int> m_col_sizes_p4{ 330,330,100,100,240,200 };
	std::vector<std::vector<int>> m_col_sizes{ m_col_sizes_p1, m_col_sizes_p2, m_col_sizes_p3, m_col_sizes_p4 };

	std::vector<string> m_col_titles_p1{ "借出時間","學生姓名","學生英文姓名","班別","學號","iPad 號碼" };
	std::vector<string> m_col_titles_p2{ "借出時間","歸還時間","學生姓名","學生英文姓名","班別","學號","iPad 號碼" };
	std::vector<string> m_col_titles_p3{ "借出時間","職員姓名","登入名稱","iPad 號碼","iPad 序號" };
	std::vector<string> m_col_titles_p4{ "借出時間","歸還時間","職員姓名","登入名稱","iPad 號碼","iPad 序號" };
	std::vector<std::vector<string>> m_col_titles{ m_col_titles_p1, m_col_titles_p2, m_col_titles_p3, m_col_titles_p4 };

	std::vector<string> m_borrower_titles_p1{ "Time","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };
	std::vector<string> m_rlitem_titles_p2{ "ChiTime","ChiReturnTime","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };
	std::vector<string> m_borrower_titles_p3{ "Time","ChineseName","Login","IpadNumber","IpadSN" };
	std::vector<string> m_rslitem_titles_p4{ "ChiTime","ChiReturnTime","ChineseName","Login","IpadNumber","IpadSN" };
	std::vector<std::vector<string>> m_titles{ m_borrower_titles_p1, m_rlitem_titles_p2, m_borrower_titles_p3, m_rslitem_titles_p4 };

	std::vector<wxStaticText *> m_empty_messages;
	std::vector<wxColour> m_empty_msg_colour{ m_main_colour,m_main_colour,m_p3text_colour,m_black_colour };

	wxPNGHandler * m_logo_handler_p1 = nullptr;
	wxStaticBitmap * m_logo_image_p1 = nullptr;
	wxStaticText * m_title_p1 = nullptr;
	wxStaticText * m_status_p1 = nullptr;
	wxStaticText * m_message1_p1 = nullptr;
	wxStaticText * m_message2_p1 = nullptr;
	wxStaticText * m_message3_p1 = nullptr;
	wxStaticText * m_message4_p1 = nullptr;
	wxStaticText * m_empty_message_p1 = nullptr;
	Database::BList_Item m_borrower_p1;
	bool m_lock_p1 = false;

	wxStaticText * m_message1_p2 = nullptr;
	wxTextCtrl * m_textctrl1_p2 = nullptr;
	wxStaticText * m_message2_p2 = nullptr;
	wxStaticText * m_message3_p2 = nullptr;
	int m_history_year_p2;
	int m_history_month_p2;
	int m_history_day_p2;
	long long m_history_unixtime_p2;
	wxStaticText * m_title_p2 = nullptr;
	wxStaticText * m_empty_message_p2 = nullptr;

	wxPNGHandler * m_logo_handler_p3 = nullptr;
	wxStaticBitmap * m_logo_image_p3 = nullptr;
	wxStaticText * m_title_p3 = nullptr;
	wxStaticText * m_status_p3 = nullptr;
	wxStaticText * m_message1_p3 = nullptr;
	wxStaticText * m_message2_p3 = nullptr;
	wxStaticText * m_message3_p3 = nullptr;
	wxStaticText * m_message4_p3 = nullptr;
	wxStaticText * m_empty_message_p3 = nullptr;
	Database::BList_Item m_staff_p3;
	bool m_lock_p3 = false;
	bool m_second_lock_p3 = false;
	string m_last_ipad_p3 = "";

	wxStaticText * m_message1_p4 = nullptr;
	wxTextCtrl * m_textctrl1_p4 = nullptr;
	wxStaticText * m_message2_p4 = nullptr;
	wxStaticText * m_message3_p4 = nullptr;
	int m_history_year_p4;
	int m_history_month_p4;
	long long m_history_unixtime_p4;
	wxStaticText * m_title_p4 = nullptr;
	wxButton * m_button_left_p4 = nullptr;
	wxButton * m_button_right_p4 = nullptr;
	wxStaticText * m_empty_message_p4 = nullptr;

	wxStaticText * m_title_sp = nullptr;
	wxStaticText * m_message1_sp = nullptr;
	wxTextCtrl * m_textctrl1_sp = nullptr;
	wxStaticText * m_message2_sp = nullptr;
	wxTextCtrl * m_textctrl2_sp = nullptr;
	wxStaticText * m_message3_sp = nullptr;
	wxTextCtrl * m_textctrl3_sp = nullptr;
	wxStaticText * m_message4_sp = nullptr;
	wxTextCtrl * m_textctrl4_sp = nullptr;
	wxStaticText * m_message5_sp = nullptr;
	wxTextCtrl * m_textctrl5_sp = nullptr;
	wxStaticText * m_message6_sp = nullptr;
	wxTextCtrl * m_textctrl6_sp = nullptr;
};