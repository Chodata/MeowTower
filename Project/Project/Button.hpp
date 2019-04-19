#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;


class GameController;

class Button {			
private:
	Vector2f size;
	Vector2f pos;
	string filename;
	Texture buttonTexture;
	RectangleShape buttonFrame;

public:
	Button(Vector2f s, Vector2f p, string fn);
	Button() {}
	virtual void draw(sf::RenderWindow &window);
	virtual bool isClicked(Vector2i &MousePos);
	Texture* getTexture() { return &buttonTexture; }
	RectangleShape* getFrame() { return &buttonFrame; }
	Vector2f getSize() { return size; }
	string getFilename() { return filename; }
	virtual void action() {}
	static GameController* gamecontroller;
	static void setGamecontroller(GameController *g) { gamecontroller = g; }
};

class SpeedButton : public Button {
private:
	int val;
	static int speedMod;
	Vector2f size;


public:
	SpeedButton(Vector2f s, Vector2f p, string fn, int v) :Button(s, p, fn), val(v) {
		getFrame()->setOutlineThickness(-1);
		getFrame()->setOutlineColor(Color::Black);
	}

	void action() {
		speedMod = val;

	}
	int getVal() { return val; }
	static int getSpeed() { return speedMod; }
	static void setSpeed(int v) { speedMod = v; }
};


#endif