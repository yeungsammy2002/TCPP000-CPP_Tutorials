# Setup SFML in CLion

First, head over to `https://www.sfml-dev.org/download/sfml/2.5.1/` to download the ***SMFL library*** `GCC 7.3.0 MinGW (SEH) - 64-bit`

You have to download the ***C++ compiler*** that the SMFL suggested:
- `https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download`

Copy the following to your project root:
- `cmake_modules` folder with `FindSFML.cmake`
- `CMakeLists.txt` file

Copy the following example code to check if the SMFL library is working:
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