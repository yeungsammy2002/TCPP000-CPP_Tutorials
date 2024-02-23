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

	void processing_student_tap_p1(Database::Item & student);
	void processing_ipad_tap_p1(Database::Item & ipad);
	void processing_return_ipad_p1(Database::Item & ipad);
	void draw_p1();
	void reset_p1();
	void draw_grid_p1();
	void push_p1();
	void show_empty_p1();

	void draw_p2();
	void draw_grid_p2();
	void redraw_grid_p2(bool is_exist);
	void append_p2();
	void show_empty_p2();
	void move_history_p2(const int id);

	void processing_staff_tap_p3(Database::Item & staff);
	void processing_ipad_tap_p3(Database::Item & ipad);
	void processing_return_ipad_p3(Database::Item & ipad);
	void draw_p3();
	void reset_p3();
	void draw_grid_p3();
	void push_p3();
	void show_empty_p3();

	void draw_p4();
	void draw_grid_p4();
	void redraw_grid_p4(bool is_exist);
	void append_p4();
	void move_history_p4(const int id);
	void show_empty_p4();

	void draw_setup_page();

	void OnCloseClicked(wxCommandEvent & evt);
	void OnKeyEvent(wxKeyEvent & evt);
	void OnButtonClicked(wxCommandEvent & evt);

public:
	Logger * m_logger;
	Database * m_db;

private:
	const int m_width = 1320;
	int m_full_height = 600;
	int m_current_page_num = 1;
	int m_last_page_num = 5;
	wxBoxSizer * m_sizer;

	wxColour m_main_colour = wxColour(0, 50, 110);
	wxColour m_second_colour = wxColour(0, 130, 20);
	wxColour m_third_colour = wxColour(200, 80, 0);
	wxColour m_forth_colour = wxColour(220, 37, 55);
	wxColour m_fifth_colour = wxColour(190, 255, 200);
	wxColour m_sixth_colour = wxColour(50, 50, 50);
	wxColour m_black_colour = wxColour(0, 0, 0);
	wxColour m_bg_colour = wxColour(220, 220, 220);
	wxColour m_warn_colour = wxColour(220, 0, 0);
	wxColour m_white_colour = wxColour(255, 255, 255);

	wxPanel * m_page1 = nullptr;
	int m_header_h_p1 = 150;
	wxPNGHandler * m_logo_handler_p1 = nullptr;
	wxStaticBitmap * m_logo_image_p1 = nullptr;
	wxStaticText * m_title_p1 = nullptr;
	wxStaticText * m_status_p1 = nullptr;
	wxStaticText * m_message1_p1 = nullptr;
	wxStaticText * m_message2_p1 = nullptr;
	wxStaticText * m_message3_p1 = nullptr;
	wxStaticText * m_message4_p1 = nullptr;
	wxStaticText * m_empty_message_p1 = nullptr;
	wxGrid * m_grid_p1 = nullptr;
	const int m_col_num_p1 = 6;
	const int m_col_sizes_p1[6]{ 400,100,350,100,50,300 };
	const char * m_borrower_titles_p1[6]{ "Time","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };
	Database::BList_Item m_borrower_p1;
	bool m_lock_p1 = false;

	wxPanel * m_page2 = nullptr;
	wxStaticText * m_message1_p2 = nullptr;
	wxTextCtrl * m_textctrl1_p2 = nullptr;
	wxStaticText * m_message2_p2 = nullptr;
	wxStaticText * m_message3_p2 = nullptr;
	int m_history_year_p2;
	int m_history_month_p2;
	int m_history_day_p2;
	long long m_history_unixtime_p2;
	int m_header_h_p2 = 130;
	wxStaticText * m_title_p2 = nullptr;
	wxButton * m_button_left_p2 = nullptr;
	wxButton * m_button_right_p2 = nullptr;
	wxStaticText * m_empty_message_p2 = nullptr;
	wxGrid * m_grid_p2 = nullptr;
	const int m_col_num_p2 = 7;
	const int m_col_sizes_p2[7]{ 310,310,100,240,100,50,190 };
	const char * m_rlitem_titles_p2[7]{ "ChiTime","ChiReturnTime","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };

	wxPanel * m_page3 = nullptr;
	int m_header_h_p3 = 150;
	wxPNGHandler * m_logo_handler_p3 = nullptr;
	wxStaticBitmap * m_logo_image_p3 = nullptr;
	wxStaticText * m_title_p3 = nullptr;
	wxStaticText * m_status_p3 = nullptr;
	wxStaticText * m_message1_p3 = nullptr;
	wxStaticText * m_message2_p3 = nullptr;
	wxStaticText * m_message3_p3 = nullptr;
	wxStaticText * m_message4_p3 = nullptr;
	wxStaticText * m_empty_message_p3 = nullptr;
	wxGrid * m_grid_p3 = nullptr;
	const int m_col_num_p3 = 5;
	const int m_col_sizes_p3[5]{ 350,150,250,150,400 };
	const char * m_borrower_titles_p3[5]{ "Time","ChineseName","EnglishName","Login","IpadNumber" };
	Database::BList_Item m_staff_p3;
	bool m_lock_p3 = false;
	bool m_second_lock_p3 = false;

	wxPanel * m_page4 = nullptr;
	wxStaticText * m_message1_p4 = nullptr;
	wxTextCtrl * m_textctrl1_p4 = nullptr;
	wxStaticText * m_message2_p4 = nullptr;
	wxStaticText * m_message3_p4 = nullptr;
	int m_history_year_p4;
	int m_history_month_p4;
	long long m_history_unixtime_p4;
	int m_header_h_p4 = 130;
	wxStaticText * m_title_p4 = nullptr;
	wxButton * m_button_left_p4 = nullptr;
	wxButton * m_button_right_p4 = nullptr;
	wxStaticText * m_empty_message_p4 = nullptr;
	wxGrid * m_grid_p4 = nullptr;
	const int m_col_num_p4 = 6;
	const int m_col_sizes_p4[6]{ 330,330,100,240,100,200 };
	const char * m_rslitem_titles_p4[6]{ "ChiTime","ChiReturnTime","ChineseName","EnglishName","Login","IpadNumber" };

	wxPanel * m_setup_page = nullptr;
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