#include "App.h"

#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame * mainFrame = new MainFrame("SBC iPad Borrow");
	mainFrame->ShowFullScreen(true, wxFULLSCREEN_ALL);
	mainFrame->SetClientSize(800, 800);
	mainFrame->Center();
	mainFrame->Show();

	return true;
}