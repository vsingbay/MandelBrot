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
    ComplexPlane CompPlane(pixelWidth, pixelHeight);

 
    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf"))
    {
        // error..
    }

    text.setFont(font);
    text.setString("Click any three points");
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(0, 0);

    





    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                //handles event to close window
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    CompPlane.zoomOut();
                    CompPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    CompPlane.zoomIn();
                    CompPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                }
            } //end of mouse clicks
            if (event.type == sf::Event::MouseMoved)
            {
                CompPlane.setMouseLocation({ event.mouseButton.x, event.mouseButton.y });
            } //end of mouse move
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            } //check if window will close from keyboard click

            //update scene segment
            CompPlane.updateRender();
            CompPlane.loadText(text);

            //draw scene segment
            window.clear();
            window.draw(CompPlane);
            window.draw(text);
            window.display();
        }
    } //end of while loop
}


/*// mandelbrot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"


using namespace sf;
using namespace std;





int main()
{
   
    VideoMode vm(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height /2);
    
    RenderWindow window(vm, "Mandelbrot Set!!", Style::Default);

    ComplexPlane plane(vm.width ,vm.height); 


    View view;
    view.setSize(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
    view.setCenter(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2);
    window.setView(view);


    sf::Text text;
    

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
*/
