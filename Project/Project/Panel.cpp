#include "Panel.hpp"

Panel::Panel(Vector2f s, int gameWidth):PanelSize(s) {

	PanelShape.setSize(PanelSize);
	PanelShape.setOutlineThickness(3);
	PanelShape.setOutlineColor(sf::Color::Black);
	PanelShape.setFillColor(sf::Color(175, 235, 255));
	PanelShape.setPosition(gameWidth - PanelSize.x, 0);
	
	if (!font.loadFromFile("arial.ttf"))
	{

	}

	currentSpeed.setFont(font);
	currentSpeed.setString("Current Speed: 1x");
	currentSpeed.setPosition(gameWidth - PanelSize.x + PanelSize.y * 0.06*0.5, PanelSize.y * 0.05);
	currentSpeed.setCharacterSize(22);
	currentSpeed.setStyle(Text::Regular);
	currentSpeed.setFillColor(Color::Black);

	line.setSize(Vector2f(PanelSize.x, 5));
	line.setPosition(gameWidth - PanelSize.x, PanelSize.y * 0.1);
	line.setFillColor(Color::Black);

}

void Panel::addButton(Button *b)
{
	items.push_back(b);
}

float Panel::getWidth() { return PanelSize.x; }
float Panel::getHeight() { return PanelSize.y; }
bool Panel::isClicked(Vector2i &MousePos)
{
	bool clicked = 0;
	for (Button*x : items)
	{
		if (x->isClicked(MousePos))
			clicked = 1;

	}
	return clicked;
}

void Panel::draw(RenderWindow &window) {
	window.draw(PanelShape);
	for (Button*x : items)
	{
		x->draw(window);
	}
	window.draw(line);
	window.draw(currentSpeed);
}

void Panel::setCurrentSpeed(int val)
{
	//currentSpeed.setString("Current Speed: " + to_string(val) + "x");
	currentSpeed.setString("Current Speed: " + to_string(SpeedButton::getSpeed()) + "x");
}