#ifndef TOWER_HPP
#define	TOWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Enemy.hpp"
#include "Button.hpp"

using namespace std;
using namespace sf;
class TowerMarket;

class TowerMarket : public Button {
private:
	int val;	//for specifying which towermarket clicked in controller
	int Atk;	//Attack value
	int Rng;	//Range value
	int Spd;	//Attack speed value
	int Price;	//Price value
	Vector2f TowerInfoPos;
	Vector2f TowerInfoSize;
	RectangleShape TowerInfoShape;
	RectangleShape TowerShape;
	CircleShape towerRng;

	Text atkT;
	Text rngT;
	Text spdT;
	Text priceT;
	Font font;

	bool showTower;
	GameController *gameController;

public:
	friend class Tower;
	friend class GameController;
	TowerMarket(Vector2f si, Vector2f p, string fn, int a, int r, int s, int price, GameController* g,int v) ;
	void draw(sf::RenderWindow &window);
	bool isClicked(Vector2i &MousePos);
	void action();


};

class Bullet {
private:
	Enemy* e;
	int Dmg;
	RectangleShape BulletShape;
	Texture BulletTexture;
	Vector2f size;


public:
	Bullet(Enemy* enemy, Vector2f s, Vector2f startPos, int d, string fn);
	void draw(RenderWindow& window){window.draw(BulletShape);}
	void move();
	bool HitEnemy();
	Enemy* getEnemy() { return e; }

};

class BulletList {
private:
	vector<Bullet*> bulletVector;
public:
	friend class TowerList;
	BulletList() {}
	~BulletList();
	void addBullet(Bullet* b) { bulletVector.push_back(b); }
	void draw(RenderWindow &window);
	void move();

};


class Tower {
private:
	int Atk;
	int Rng;
	int Spd;
	int Price;
	Clock clock;
	Time time;
	Vector2f pos;
	Vector2f size;
	string BulletFile;
	RectangleShape TowerShape;
	BulletList bulletlist;
	Texture TowerTexture;

public:
	Tower() {}
	Tower(TowerMarket& tm, Vector2f si, Vector2f p, string fnB);
	friend class TowerList;
	bool EnermyInRng(Vector2f ePos);
	void drawTower(RenderWindow &window)
	{
		window.draw(TowerShape);
	}
	//void drawBullet(RenderWindow& window)
	//{
	//	window.draw(BulletShape);
	//}


	Time getTime() {
		time = clock.getElapsedTime();
		return time;
	}

	void restartTime() {
		clock.restart();
	}
};


class TowerList {
private:
	vector<Tower*> towerList;


public:
	TowerList(){ }

	~TowerList() {
		for (Tower* x : towerList)
			delete x;
	}

	friend class Map;
	void addTower(Tower* t) { towerList.push_back(t); }
	void draw(RenderWindow &window);

	void Shoot(EnemyList* enemylist, int speed);
};


#endif