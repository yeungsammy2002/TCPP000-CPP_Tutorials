#include "App.h"

#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame * mainFrame = new MainFrame("Die Battle");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Show();

	return true;
}