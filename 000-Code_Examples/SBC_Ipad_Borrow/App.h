#pragma once

#include <wx/wx.h>

#include "utilities/Logger.h"
#include "utilities/Singleton.h"

using namespace regulus::utilities;

class App : public wxApp
{
public:
	bool OnInit();
};