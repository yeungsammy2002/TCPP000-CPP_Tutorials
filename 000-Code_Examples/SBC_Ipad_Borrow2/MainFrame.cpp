#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	init();
	init_draw();

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_page1, 0, wxALIGN_CENTER);
	SetSizerAndFit(m_sizer);
	m_page2->Hide();
	m_page3->Hide();
	m_page4->Hide();
	m_setup_page->Hide();

	init_bind();
}
