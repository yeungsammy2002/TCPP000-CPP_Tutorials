#include "App.h"

#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame * mainFrame = new MainFrame("SBC iPad Borrow");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();

	//log_debug("name is %s, age is %d", "Chris", 38);
	//log_info("info message");
	//log_warn("warn message");
	//log_error("error message");
	//log_fatal("fatal message");

	return true;
}