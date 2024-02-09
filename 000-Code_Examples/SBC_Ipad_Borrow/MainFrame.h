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
	void switch_page(bool move);
	void switch_page();

	void draw_p1();
	void reset_p1();
	void draw_grid_p1();
	void append_p1();
	void show_empty_p1();

	void draw_p2();
	void draw_grid_p2();
	void append_p2();

	void draw_setup_page();

	void OnCloseClicked(wxCommandEvent & evt);
	void OnKeyEvent(wxKeyEvent & evt);

public:
	Logger * m_logger;
	Database * m_db;

private:
	const int m_width = 1320;
	int m_full_height = 600;
	int m_current_page_num = 1;
	int m_last_page_num = 3;
	wxBoxSizer * m_sizer;

	wxColour m_main_colour = wxColour(2, 52, 108);
	wxColour m_second_colour = wxColour(0, 98, 105);
	wxColour m_third_colour = wxColour(58, 0, 58);
	wxColour m_forth_colour = wxColour(50, 50, 50);
	wxColour m_bg_colour = wxColour(220, 220, 220);
	wxColour m_warn_colour = wxColour(220, 0, 0);

	wxPanel * m_page1 = nullptr;
	int m_header_h_p1 = 150;
	wxStaticText * m_title_p1 = nullptr;
	wxStaticText * m_status_p1 = nullptr;
	wxStaticText * m_message1_p1 = nullptr;
	wxStaticText * m_message2_p1 = nullptr;
	wxStaticText * m_message3_p1 = nullptr;
	wxStaticText * m_empty_message_p1 = nullptr;
	wxGrid * m_grid_p1 = nullptr;
	const int m_col_num_p1 = 6;
	const int m_col_sizes_p1[6]{ 400,100,350,100,50,300 };
	const char * m_borrower_titles_p1[6]{ "Time","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };
	Database::BList_Item m_borrower_p1;
	bool m_lock_p1 = false;

	wxPanel * m_page2 = nullptr;
	int m_header_h_p2 = 100;
	wxStaticText * m_title_p2 = nullptr;
	wxGrid * m_grid_p2 = nullptr;
	const int m_col_num_p2 = 7;
	const int m_col_sizes_p2[7]{ 300,300,100,250,100,50,200 };
	const char * m_rlitem_titles_p2[7]{ "ChiTime","ChiReturnTime","ChineseName","EnglishName","ClassName","ClassNumber","IpadNumber" };

	wxPanel * m_setup_page = nullptr;
	wxStaticText * m_title_sp = nullptr;
};