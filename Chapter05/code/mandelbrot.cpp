#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
using namespace std;
using namespace sf;



int main()
{


    int pixelWidth = VideoMode::getDesktopMode().width / 2;
    int pixelHeight = VideoMode::getDesktopMode().height / 2;

    VideoMode vm(pixelWidth, pixelHeight);

    RenderWindow window(vm, "Mandelbrot Set", Style::Default);
    ComplexPlane plane(pixelWidth, pixelHeight);
    

    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf"))
    {
        // error..
    }

    text.setFont(font);
   // text.setString("Click any three points");
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    plane.zoomIn();
                    plane.setCenter({ event.mouseButton.x, event.mouseButton.y });

                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    plane.zoomOut();
                    plane.setCenter({ event.mouseButton.x, event.mouseButton.y });

                }

            }
            if (event.type == sf::Event::MouseMoved)
            {

                plane.setMouseLocation({ event.mouseButton.x, event.mouseButton.y });
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
        }



        //UPDATE

        plane.updateRender();
        plane.loadText(text);


        //DRAW SCENE
        window.clear();
        window.draw(plane);
        window.draw(text);


        window.display();
    }
}