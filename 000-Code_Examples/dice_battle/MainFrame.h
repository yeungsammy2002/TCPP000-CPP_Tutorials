#pragma once

#include <wx/wx.h>

#include <vector>
#include <string>

#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <thread>
#include <chrono>

using std::string;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString & title);

	void dice(const int att_no, const int def_no);
	void set_dice(const string & player, const int dice_no);

private:
	wxColour m_white_colour = wxColour(255, 255, 255);
	wxColour m_red_colour = wxColour(255, 0, 0);
	wxColour m_blue_colour = wxColour(0, 0, 255);

	int m_attacker_result_d1 = 0;
	int m_attacker_result_d2 = 0;
	int m_attacker_result_d3 = 0;

	int m_defender_result_d1 = 0;
	int m_defender_result_d2 = 0;

	int m_attacker_dice_no = 3;
	int m_defender_dice_no = 2;

	std::vector<int> m_attacker_die;
	std::vector<int> m_defender_die;

	std::vector<wxStaticText *> m_attacker_result_text;
	std::vector<wxStaticText *> m_defender_result_text;

	std::vector<int> m_attacker_result{ 0,0,0 };
	std::vector<int> m_defender_result{ 0,0 };

	void OnDiceClicked(wxCommandEvent & evt);

	void OnA1Clicked(wxCommandEvent & evt);
	void OnA2Clicked(wxCommandEvent & evt);
	void OnA3Clicked(wxCommandEvent & evt);

	void OnD1Clicked(wxCommandEvent & evt);
	void OnD2Clicked(wxCommandEvent & evt);
};