#include "./MainFrame.h"

void MainFrame::set_dice(const string & player, const int dice_no)
{
	if (player == "attacker")
	{
		m_attacker_dice_no = dice_no;

		switch (dice_no)
		{
		case 1:
			m_attacker_result_text[0]->Show();
			m_attacker_result_text[1]->Hide();
			m_attacker_result_text[2]->Hide();
			break;
		case 2:
			m_attacker_result_text[0]->Show();
			m_attacker_result_text[1]->Show();
			m_attacker_result_text[2]->Hide();
			break;
		case 3:
			m_attacker_result_text[0]->Show();
			m_attacker_result_text[1]->Show();
			m_attacker_result_text[2]->Show();
			break;
		default:
			break;
		}

		return;
	}
	if (player == "defender")
	{
		m_defender_dice_no = dice_no;

		switch (dice_no)
		{
		case 1:
			m_defender_result_text[0]->Show();
			m_defender_result_text[1]->Hide();
			break;
		case 2:
			m_defender_result_text[0]->Show();
			m_defender_result_text[1]->Show();
			break;
		default:
			break;
		}

		return;
	}
}

void MainFrame::dice(const int att_no, const int def_no)
{
	std::srand(std::time(NULL));

	std::vector<int> attacker_die;
	for (int i = 0; i < att_no; ++i)
	{
		int random_time = rand() % 50;
		std::this_thread::sleep_for(std::chrono::milliseconds(random_time));
		int random = (rand() % 6) + 1;
		attacker_die.push_back(random);
	}

	std::vector<int> defender_die;
	for (int i = 0; i < def_no; ++i)
	{
		int random_time = rand() % 50;
		std::this_thread::sleep_for(std::chrono::milliseconds(random_time));
		int random = (rand() % 6) + 1;
		defender_die.push_back(random);
	}

	sort(attacker_die.rbegin(), attacker_die.rend());
	sort(defender_die.rbegin(), defender_die.rend());

	for (int i = 0; i < att_no; ++i)
	{
		m_attacker_result[i] = attacker_die[i];
		m_attacker_result_text[i]->SetLabel(wxString(std::to_string(attacker_die[i])));
	}

	for (int i = 0; i < def_no; ++i)
	{
		m_defender_result[i] = defender_die[i];
		m_defender_result_text[i]->SetLabel(wxString(std::to_string(defender_die[i])));
	}
}

void MainFrame::OnDiceClicked(wxCommandEvent & evt)
{
	dice(m_attacker_dice_no, m_defender_dice_no);
}

void MainFrame::OnA1Clicked(wxCommandEvent & evt)
{
	set_dice("attacker", 1);
}
void MainFrame::OnA2Clicked(wxCommandEvent & evt)
{
	set_dice("attacker", 2);
}
void MainFrame::OnA3Clicked(wxCommandEvent & evt)
{
	set_dice("attacker", 3);
}

void MainFrame::OnD1Clicked(wxCommandEvent & evt)
{
	set_dice("defender", 1);
}
void MainFrame::OnD2Clicked(wxCommandEvent & evt)
{
	set_dice("defender", 2);
}