#ifndef BASE_HPP
#define BASE_HPP


#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

class Base {
private:
	int Hp;
	RectangleShape BaseShape;
	Texture BaseTexture;
	friend class GameController;
	friend class Map;
	
public:
	Base() {}
	Base(Vector2f s, Vector2f p, int hp, string fn) :Hp(hp){
		BaseTexture.loadFromFile(fn);
		BaseShape.setTexture(&BaseTexture);
		BaseShape.setSize(s);
		BaseShape.setPosition(p);
	}

	void draw(RenderWindow &window)
	{
		window.draw(BaseShape);
	}
	int getHp() { return Hp; }
};
#endif