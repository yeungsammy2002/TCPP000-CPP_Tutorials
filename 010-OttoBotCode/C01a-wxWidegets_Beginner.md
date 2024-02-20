# Section 02 - Installing wxWidgets

We will install ***wxWidgets*** so we can start programming GUI. I'll show you the installation on ***Windows*** using ***Visual Studio***. Before we begin, let me just say that installing library can be quite frastrating in the first time you do it. But it really is an essential skill, and it is not hard if you know what you are doing.

I will take you through the entire process, step by step, so just follow along. Let's get started.


### Step 1 - Download the wxWidgets Library
The first step is to **download the library**. Go to:

**https://wxwidgets.org/downloads**

We will build it from source. Go ahead and download the source code. I will choose the `Windows ZIP` (`wxWidgets-3.2.2.1.zip` 2023-04-22). Once that done, extract it somewhere that make sense to you. I will put (extract) it in a folder ***"Libraries"*** under my ***C drive*** like this:
```
C:\Libraries\wxWidgets-3.1.5        (C:\cpplibraries\wxWidgets-3.2.2.1)
```
It doesn't really matter where you put it, but try to avoid paths containing `space`s, because they can something cause problems.

Now we have all the source code for ***wxWidgets*** inside `C:\Libraries\wxWidgets-3.1.5`.


### Step 2 - Add a New Environment Variable
Next we will add a new environment variable which points to this `C:\Libraries\wxWidgets-3.1.5` directory. So go to your system environment variable (type ***"env"*** in the search bar, then click ***"Edit the system environment variables"***).

Inside ***System Properties*** window's ***Advanced*** tab, click ***"Environment Variables..."***.

Inside ***"Environment Variables"*** window, click ***"New..."*** to add an environment variable. We will call it ***"WXWIN"*** (Variable name:), and for the value (Variable value:) put in the path to your ***wxWidgets directory*** `C:\Libraries\wxWidgets-3.1.5`.

This variable will simplify things in ***Visual Studio*** later.


### Step 3 - Build wxWidgets Library
Back to the ***wxWidgets directory*** `C:\Libraries\wxWidgets-3.1.5`. Let's first take a look inside the `lib` folder. Right now it only contains a few file, but this is where the compiled ***wxWidgets library*** will go once we build it.
```
// C:\Libraries\wxWidgets-3.1.5\lib

abicheck
vms.opt
vms_gtk.opt
vms_gtk2.opt
vms_x11_univ.opt
```

Let's do that now. We have to go back to the `C:\Libraries\wxWidgets-3.1.5` main directory, and then open the `build` folder. Next, open the folder called `msw`, which short for ***Microsoft Windows*** `C:\Libraries\wxWidgets-3.1.5\build\msw`. There are bunch of files here, but if you scroll down, you should see solution files for different verions for ***Visual Studio***:
```
// C:\Libraries\wxWidgets-3.1.5\build\msw
...
wx_vc10.sln
wx_vc11.sln
wx_vc12.sln
wx_vc13.sln
wx_vc14.sln
wx_vc15.sln
wx_vc16.sln         // (choose this in the course)
wx_vc17.sln         // (choose this in my case)
...
```
We will use the most recent one, so go ahead and open `wx_vc16.sln`.

Inside the ***Visual Studio* IDE**, to build ***wxWidgets***, we have to choose a ***configuration*** and a ***platform***. You can choose ***Debug*** and ***Release*** either as a ***static library*** or a ***dynamic library***. If you don't know what that means, don't worry about it right now. I will build ***wxWidgets*** as a ***static library*** for ***32-bit*** and ***64-bit* platforms**.

1. Build ***debug Win32 static library*** - First choose ***Debug*** configuration, and set the platform to ***Win32*** in the ***Visual Studio* IDE**. Then click ***Build*** tab -> ***Build Solution***. Once that's done. A new folder called `vc_lib` has been created inside the `lib` folder `C:\Libraries\wxWidgets-3.1.5\lib\vc_lib`. It contains the ***debug static libraries***, and you can see that they are suffixed with a `d` for ***debug***.

2. Build ***release Win32 static library*** - Now go back to ***Visual Studio***, change to ***Release*** configuration, and build the solution again like before. After the installation, in the `vc_lib` folder, we now also have the ***release static libraries*** `C:\Libraries\wxWidgets-3.1.5\lib\vc_lib\mswu`.

3. Build ***debug x64 static library*** - Finally, we will build the library for ***64-bit* platforms** as well.

4. Build ***release x64 static library***


### Step 4 - Create You First wxWidgets GUI Project
We have now built ***wxWidgets*** from source. Let's create a simple project to check that everything works. Head back to the ***Visual Studio***, and create an **empty C++ project** and name it ***FirstGUI***.

Inside the editor page, you now can start by adding a new source file `main.cpp`. Then copy and paste the following code into `main.cpp`. This is purely to check if everything works:
```
#include <wx/wx.h>
 
class App : public wxApp {
public:
    bool OnInit() {
        wxFrame* window = new wxFrame(NULL, wxID_ANY, "GUI Test", wxDefaultPosition, wxSize(600, 400));
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* text = new wxStaticText(window, wxID_ANY, "Well Done!\nEverything seems to be working",
                                              wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
        text->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        sizer->Add(text, 1, wxALIGN_CENTER);
        window->SetSizer(sizer);
        window->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);
```
As you can see, ***Visual Studio*** is not happy. That's because we have to tell it where you can find the ***wxWidgets* header and library files**.

So right click on your project on the ***left pane***, and choose ***Properties***.

Inside ***"FirstGUI Property Pages"*** window, first make sure that ***Configuration:*** is set to ***All configurations***, and ***Platforms:*** is set to ***All Platforms***. 

Then on the left pane, choose ***C/C++***. These settings won't show up if you don't have a **C++ file** in your project that's why we added one before. At the top, where it says ***Additional Include Directories***, click ***down arrow*** -> ***<Edit...>***, you have to tell ***Visual Studio*** where it can find the ***wxWidgets header files***.

Inside the ***Additional Include Directories*** window, we will add two directories here using the environment variable we defined before. The first directory is `$(WXWIN)\include` and the second one is `$(WXWIN)\include\msvc`. You can see down here (***Evaluated value:***). The `WXWIN` variable has been expanded to the path of the ***wxWidgets directory***:
```
$(WXWIN)\include
$(WXWIN)\include\msvc

Evaluated value:
C:\Libraries\wxWidgets-3.1.5\include\msvc
C:\Libraries\wxWidgets-3.1.5\include
```

Don't close the ***"FirstGUI Property Pages"*** window, expand the ***Linker*** on the ***left pane*** here. Then click on ***System***. Here (main pane) we have to change the ***SubSystem*** from ***console***:
```
Console (/SUBSYSTEM:CONSOLE)
``` 
to ***Windows***:
```
Windows (/SUBSYSTEM:WINDOWS)
```
This let the compiler know that we don't want a console application. We want a ***Windows GUI application***.

Finally, we have to tell the ***linker*** where it can find the static libraries. And that depends on the target platform. So first, set ***Platform:*** to ***Win32*** on the top. Then go to the ***Linker*** -> ***General*** on the ***left pane***. Under ***Additional Library Directories***, we have to add the location of the ***static libraries*** for ***32-bit platforms***:
```
...
Additional Library Directories          $(WXWIN)\lib\vc_lib
...
```
click ***Apply***. 

Then change the target ***Platform:*** to ***x64***. The static libraries for ***64-bit platforms*** are at `$(WXWIN)\lib\vc_x64_lib`:
```
...
Additional Library Directories          $(WXWIN)\lib\vc_x64_lib
...
```
click ***OK*** to close.

Now the program should run in both ***Debug*** and ***Release*** mode on both ***32-bit*** and ***64-bit***. You should successfully built ***wxWidgets*** from source, and setup a project which links to it. For some other libraries, you would also have to tell the ***linker*** which `.lib` files your program will use. For ***wxWidgets***, that is not required, and that's because it uses a special feature of the ***Microsoft Virtual C++ Compiler***.


### Step 5 - Turn Off Visual Studio Secure Warnings
1. Right click on your project on the ***left pane***, and choose ***Properties***.

2. Make sure ***Configuration***: ***"All Configurations"***, and ***Platform***: ***"All Platforms"***
   
3. On left pane, expand ***"C/C++"*** > ***"All Options"***
   
4. On the right main pane, scroll down until you see ***"Preprocessor Definitions"*** field, expand the right field and then click ***"<Edit...>"***, then add the following:
```
_CRT_SECURE_NO_DEPRECATE
_CRT_NONSTDC_NO_DEPRECATE
```

5. Check the ***"Inherit from parent or project defaults"*** box, then click ***"OK"***