#include "Button.hpp"

using namespace std;
using namespace sf;


Button::Button(sf::Vector2f s, sf::Vector2f p, string fn) :size(s), pos(p), filename(fn)
{

	buttonFrame.setSize(size);
	buttonTexture.loadFromFile(filename);	//the method will handle the case where file cannot be found
	
	buttonFrame.setTexture(&buttonTexture);
	buttonFrame.setPosition(pos.x, pos.y);
	buttonFrame.setOutlineThickness(-1);
	buttonFrame.setOutlineColor(Color::Black);

}

void Button::draw(sf::RenderWindow &window)
{
	window.draw(buttonFrame);
}

bool Button::isClicked(Vector2i &MousePos)
{
	if (MousePos.x >= pos.x && MousePos.y >= pos.y && MousePos.x <= pos.x + size.x && MousePos.y <= pos.y + size.y)
	{
		action();
		return 1;
	}
	return 0;

}

