#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	init();
	init_draw();

	m_sizer = new wxBoxSizer(wxVERTICAL);
	m_sizer->Add(m_pages[0], 0, wxALIGN_CENTER);
	SetSizerAndFit(m_sizer);

	for (int i = 1; i < m_total_pages; ++i)
		m_pages[i]->Hide();

	init_bind();
}
