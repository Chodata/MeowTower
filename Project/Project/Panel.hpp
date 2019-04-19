#ifndef PANEL_HPP
#define PANEL_HPP
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <string>


class Panel {
private:
	vector<Button*> items;
	Vector2f PanelSize;
	RectangleShape PanelShape;
	RectangleShape line;
	Text currentSpeed;
	Font font;


public:
	Panel(Vector2f s, int gameWidth);
	void addButton(Button *b);
	float getWidth();
	float getHeight();
	bool isClicked(Vector2i &MousePos);
	void draw(RenderWindow &window);
	void setCurrentSpeed(int val);

};



#endif // !PANEL_HPP
