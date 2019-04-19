#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Button.hpp"
using namespace std;
using namespace sf;

class Enemy {
private:
	int Atk = 10;
	int Spd = 3;
	int FullHP;
	int Hp;
	int index = 0;
	RectangleShape EnemyShape;
	Texture EnemyTexture;
	Vector2f size;
	Vector2f pos;
	Vector2f Startpos;
	RectangleShape HealthBar;
	Vector2f HealthBarSize;
	friend class Map;

public:
	Enemy(Vector2f spos, Vector2f s, string fn, int h) :Startpos(spos), size(s), FullHP(h)
	{
		EnemyShape.setSize(size);
		pos = spos;
		EnemyShape.setPosition(pos);
		EnemyTexture.loadFromFile(fn);
		EnemyShape.setTexture(&EnemyTexture);
		EnemyShape.setOrigin(size / 2.0f);
		HealthBarSize = Vector2f(s.x * 9 / 10, s.y / 10);
		HealthBar.setSize(HealthBarSize);

		HealthBar.setFillColor(Color::Green);
		HealthBar.setOutlineColor(Color::White);
		HealthBar.setOutlineThickness(-1);
		HealthBar.setOrigin(HealthBarSize.x / 2,HealthBarSize.y/2);
		HealthBar.setPosition(pos.x, pos.y + s.y);
		Hp = FullHP;
	}

	Vector2f getPos() { return EnemyShape.getPosition(); }
	Vector2f getSize() { return EnemyShape.getSize(); }
	void setPos(Vector2f p) { 
		pos = p;
		EnemyShape.setPosition(p);
		HealthBar.setPosition(p.x,p.y-size.y/2);
	}
	void draw(RenderWindow &window) {
		window.draw(EnemyShape);
		window.draw(HealthBar);
	}
	void move(float x, float y)
	{
		setPos(Vector2f(pos.x + x, pos.y + y));
	}

	int getSpd() { return Spd; }
	void shot(int dmg)
	{
		Hp -= dmg;
		HealthBar.setSize(Vector2f(HealthBarSize.x * Hp*1.0f / FullHP,HealthBarSize.y));
	}
	int getHp() { return Hp; }
	
};

class Map;
class EnemyList {
private:
	vector<Enemy*> enemyList;
	Map*map;
public:
	EnemyList(){}
	friend class Map;
	friend class TowerList;
	
	~EnemyList() {
		for (auto x : enemyList)
			delete x;
	}
	void addEnemy(Enemy*m) { enemyList.push_back(m); }
	void draw(RenderWindow &window)
	{
		for (auto x : enemyList)
		{
			x->draw(window);
		}
	}
	void removeEnemy(Enemy* e)
	{
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i] == e)
			{
				delete e;
				enemyList.erase(enemyList.begin() + i);

				return;
			}
		}
	}

};
#endif