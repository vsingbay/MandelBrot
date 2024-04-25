#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <complex>

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth; //careful of integer divide
	m_plane_center = { 0, 0 };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray = VertexArray(Points, pixelWidth * pixelHeight);

}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };
				Vector2f coordinate = mapPixelToCoords({ j, i });
				size_t count = countIterations(coordinate);

				Uint8 r;
				Uint8 g;
				Uint8 b;

				iterationsToRGB(count, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
		}
		m_state = State::DISPLAYING;
	}
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float xCoord = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float yCoord = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xCoord, yCoord };
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float xCoord = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float yCoord = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xCoord, yCoord };
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	Vector2f newCenter = mapPixelToCoords(mousePixel);
	m_plane_center = newCenter;
	m_state = State::CALCULATING;
}


void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream ss;

	// Add to stringstream
	ss << "Mandelbrot Set\n";
	ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
	ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
	ss << "Left-click to Zoom in\n";
	ss << "Right-click to Zoom out\n";

	// create a text string
	text.setString(ss.str());

}

size_t ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y);
	complex<double>x(0, 0);
	size_t count = 0;
	while (abs(x) < 2 && count < MAX_ITER)
	{
		x = pow(x, 2) + c;
		count++;
	}
	return count;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)	//At MAX_ITER color the pixel black
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else if (count < 10)		//purple
	{
		r = 127;
		g = 0;
		b = 255;
	}
	else if (count < 20)		//purple
	{
		r = 0;
		g = 0;
		b = 255;
	}

	else if (count < 30)		//just green
	{
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count < 40)		//just blue
	{
		r = 0;
		g = 0;
		b = 255;
	}
	else if (count < 50)		//yellow
	{
		r = 255;
		g = 255;
		b = 0;
	}
	else if (count < 63)		//fully red
	{
		r = 255;
		g = 0;
		b = 0;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	float xRatio = static_cast<float>(mousePixel.x) / static_cast<float>(m_pixel_size.x);
	float yRatio = static_cast<float>(mousePixel.y) / static_cast<float>(m_pixel_size.y);

	// Calculate the corresponding coordinates in the complex plane
	float xCoord = m_plane_center.x - 0.5f * m_plane_size.x + xRatio * m_plane_size.x;
	float yCoord = m_plane_center.y - 0.5f * m_plane_size.y + yRatio * m_plane_size.y;

	return { xCoord, yCoord };
}



/*#include "ComplexPlane.h"
#include <sstream>
#include <iostream>
#include <complex> 


using namespace sf;
using namespace std;



void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xSize, ySize };
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xSize, ySize };
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	Vector2f newCenter = mapPixelToCoords(mousePixel);
	m_plane_center = newCenter;
	m_state = State::CALCULATING;

}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	sf::Font font;

	//sf::Text text;

	std::stringstream textStream;
	if (!font.loadFromFile("KOMIKAP_.ttf"))
	{
		cout << "error" << endl;
	}

	text.setFont(font);



	textStream << "Mandelbrot Set " << endl 
		<< "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl
		<< "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl
		<< "Left-Click to Zoom in" << endl
		<< "Right-Click to Zoom out";

	text.setString(textStream.str());

	text.setCharacterSize(40);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	text.setPosition(0, 0);
}

void ComplexPlane::updateRender()
{
	int pixelHeight = m_pixel_size.y;
	int pixelWidth = m_pixel_size.x;
	size_t iterations = 0;
	Uint8 red = 0;
	Uint8 green = 0;
	Uint8 blue = 0;
	Vector2f newguy;

	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < pixelHeight; i++)
		{
			for (int j = 0; j < pixelWidth; j++)
			{
				m_vArray[j + i * pixelWidth].position = { (float)j,(float)i };
				
				newguy = mapPixelToCoords({ j,i });
				
				iterations = countIterations(newguy);//FIX THISSSSS
				m_vArray[j + i * pixelWidth].color = { red,green,blue };
				iterationsToRGB(iterations, red, green, blue);
				
				
				
			}

		}
		m_state = State::DISPLAYING;
	}
}

size_t ComplexPlane::countIterations(Vector2f coord)
{

		
	size_t iter = 0;

		complex<double> c(coord.x, coord.y);
		complex<double> z(0, 0);
		//cout << "c = " << c << "\t";
		//cout << "|c| = " << abs(c) << endl;
		//cout << "Real component of c: " << c.real() << endl;
		//cout << "Imaginary component of c: " << c.imag() << endl;

		while (abs(z) < 2.0 && iter < 64)
		{
			z = z * z + c;
			iter += 1;
			//cout << abs(z);
		}
		
		return iter;
}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count <= 15)
	{
		r = 158;
		g = 0;
		b = 119;
	}
	else if (count <= 28)
	{
		r = 56;
		g = 255;
		b = 238;
	}
	else if (count <= 38)
	{
		r = 0;
		g = 87;
		b = 16;
	}
	else if (count <= 48)
	{
		r = 243;
		g = 236;
		b = 27;
	}
	else if (count <= 63)
	{
		r = 207;
		g = 2;
		b = 2;
	}

	else if (count == 64)
	{
		r = 0;
		g = 0;
		b = 0;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{

	float xRatio = static_cast<float>(mousePixel.x) / static_cast<float>(m_pixel_size.x);
	float yRatio = static_cast<float>(mousePixel.y) / static_cast<float>(m_pixel_size.y);

	// Calculate the corresponding coordinates in the complex plane
	float xCoord = m_plane_center.x - 0.5f * m_plane_size.x + xRatio * m_plane_size.x;
	float yCoord = m_plane_center.y - 0.5f * m_plane_size.y + yRatio * m_plane_size.y;

	return { xCoord, yCoord };
	/*Vector2f newpix;
	//VideoMode vm(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2);
	newpix.x = ((mousePixel.x - 0.0) / (1920 - 0.0)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);

	newpix.y = ((mousePixel.y - 1080) / (0 - 1080)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return newpix;
}
*/