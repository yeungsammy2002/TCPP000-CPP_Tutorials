# Setup SFML in CLion

#### 1. Head over to `https://www.sfml-dev.org/download/sfml/2.5.1/` to download the ***SMFL library*** `GCC 7.3.0 MinGW (SEH) - 64-bit`

#### 2. You have to download the ***C++ compiler*** that the SMFL suggested in `https://www.sfml-dev.org/download/sfml/2.5.1/`. For example:
 - `https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download`

#### 3. Copy the following to your project root:
   - `cmake_modules` folder with `FindSFML.cmake`
   - `CMakeLists.txt` file

#### 4. Copy the following example code to check if the SMFL library is working:
```
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
    sf::CircleShape shape;
    shape.setRadius(40.f);
    shape.setPosition(100.f, 100.f);
    shape.setFillColor(sf::Color::Cyan);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}
```

#### 5. Setup the working directory for the project in order to use relative path of `images` folder
`Run` (top menu) -> `Edit Configurations...` -> `Working directory:` (i.e. `E:\CLion_Projects\hello_sfml`)

