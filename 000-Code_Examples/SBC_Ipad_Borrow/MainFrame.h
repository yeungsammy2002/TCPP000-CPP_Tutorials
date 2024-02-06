#pragma once

#include <wx/wx.h>
#include <wx/grid.h>

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

	void OnCloseClicked(wxCommandEvent & evt);

	void OnKeyEvent(wxKeyEvent & evt);

	void reset();

	void draw_grid(wxGrid * grid);

	void append();

	void tap(const string & cardId);

	void show_empty();

public:
	Logger * m_logger;
	Database * m_db;

private:
	wxGrid * m_grid = nullptr;
	Database::BList_Item m_borrower;
	bool m_lock = false;
	wxPanel * m_panel;
	wxStaticText * m_title;
	wxStaticText * m_status;
	wxStaticText * m_message1;
	wxStaticText * m_message2;
	wxStaticText * m_message3;
	wxStaticText * m_empty_message;
	static const char * s_borrower_titles[7];
	static const int col_sizes[7];
};