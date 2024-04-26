#include "ComplexPlane.h"
#include <sstream>
#include <iostream>
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

	std::stringstream textStream;


	textStream << "Mandelbrot Set " << endl 
		<< "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl
		<< "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl
		<< "Left-Click to Zoom in" << endl
		<< "Right-Click to Zoom out" << endl;
	    

	text.setString(textStream.str());

}

size_t ComplexPlane::countIterations(Vector2f coord)
{

	size_t iter = 0;

	complex<double> c(coord.x, coord.y);
	complex<double> z(0, 0);
	

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
	
}
