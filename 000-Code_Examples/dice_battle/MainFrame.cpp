#include "MainFrame.h"

MainFrame::MainFrame(const wxString & title) : wxFrame(nullptr, wxID_ANY, title)
{
	wxPanel * panel = new wxPanel(this, wxID_ANY);

	wxButton * attack_b1 = new wxButton(panel, wxID_ANY, "Dice x 1", wxPoint(50, 50), wxSize(50, 50));
	wxButton * attack_b2 = new wxButton(panel, wxID_ANY, "Dice x 2", wxPoint(100, 50), wxSize(50, 50));
	wxButton * attack_b3 = new wxButton(panel, wxID_ANY, "Dice x 3", wxPoint(150, 50), wxSize(50, 50));

	wxButton * defence_b1 = new wxButton(panel, wxID_ANY, "Dice x 1", wxPoint(300, 50), wxSize(50, 50));
	wxButton * defence_b2 = new wxButton(panel, wxID_ANY, "Dice x 2", wxPoint(350, 50), wxSize(50, 50));

	wxButton * dice_b = new wxButton(panel, wxID_ANY, "Dice !", wxPoint(650, 50), wxSize(100, 50));


	for (int i = 0; i < 3; ++i)
	{
		m_attacker_result_text.push_back(new wxStaticText(panel, wxID_ANY, wxString("0"), wxPoint(100, 150 + (i * 70))));
		//m_attacker_result_text.push_back(new wxStaticText(panel, wxID_ANY, wxString("0"), wxPoint(100, 50 + (i * 50))));
		m_attacker_result_text[i]->SetForegroundColour(m_red_colour);
		m_attacker_result_text[i]->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		//m_attacker_result_text[i]->SetBackgroundColour(m_red_colour);
	}

	for (int i = 0; i < 2; ++i)
	{
		m_defender_result_text.push_back(new wxStaticText(panel, wxID_ANY, wxString("0"), wxPoint(300, 150 + (i * 70))));
		//m_defender_result_text.push_back(new wxStaticText(panel, wxID_ANY, wxString("0"), wxPoint(300, 50 + (i * 50))));
		m_defender_result_text[i]->SetForegroundColour(m_blue_colour);
		m_defender_result_text[i]->SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	}

	dice_b->Bind(wxEVT_BUTTON, &MainFrame::OnDiceClicked, this);
	attack_b1->Bind(wxEVT_BUTTON, &MainFrame::OnA1Clicked, this);
	attack_b2->Bind(wxEVT_BUTTON, &MainFrame::OnA2Clicked, this);
	attack_b3->Bind(wxEVT_BUTTON, &MainFrame::OnA3Clicked, this);

	defence_b1->Bind(wxEVT_BUTTON, &MainFrame::OnD1Clicked, this);
	defence_b2->Bind(wxEVT_BUTTON, &MainFrame::OnD2Clicked, this);
}