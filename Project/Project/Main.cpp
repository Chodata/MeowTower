#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <vld.h>

#include "Button.hpp"
#include "Panel.hpp"
#include "Enemy.hpp"
#include "Base.hpp"
#include "Tower.hpp"
using namespace sf;
using namespace std;


bool showMap = 0; //Had trouble with static class member so I declare global variable and have map/tile classes in the Main.cpp


class Tile {

protected:
	RectangleShape TileShape;
	Vector2f size;
	Vector2f pos;
	int isEmpty = 0;
	Color color;
public:
	friend class Map;
	Tile(Vector2f s, Vector2f p) :size(s), pos(p) {
		TileShape.setPosition(p);
		TileShape.setSize(s);
		TileShape.setFillColor(Color(255, 255, 255, 80));
	}

	bool isClicked(Vector2i &MousePos)
	{
		if (MousePos.x > pos.x && MousePos.y > pos.y && MousePos.x < pos.x + size.x && MousePos.y < pos.y + size.y)
		{
			action();
			return 1;
		}
		return 0;
	}
	virtual void action()
	{
		showMap = 0;
	}
	virtual bool isPointed(Vector2i &MousePos)
	{
		if (MousePos.x > pos.x && MousePos.y > pos.y && MousePos.x < pos.x + size.x && MousePos.y < pos.y + size.y)
		{
			if (isEmpty == 1)
				TileShape.setFillColor(Color(0, 255, 0));
			else
				TileShape.setFillColor(Color(255, 0, 0, 230));
			return 1;
		}
		TileShape.setFillColor(color);
		return 0;

	}
	virtual void draw(RenderWindow &window) { window.draw(TileShape); }
	void setIsEmpty(int i) { isEmpty = i; }
	void setColor(Color c) { color = c; }
	int getIsEmpty() { return isEmpty; }
	Vector2f getPos() { return pos; }
	Vector2f getSize() { return size; }
};

class EnemyTile : public Tile {
private:

public:
	EnemyTile(Vector2f s, Vector2f p) :Tile(s, p) {
		setColor(Color(200, 100, 0));
		TileShape.setFillColor(color);
		setIsEmpty(0);

	}

	bool IsIn(Vector2f p)
	{
			if ( p.x >= pos.x && p.y >= pos.y && p.x <= pos.x + size.x && p.y <= pos.y + size.y)
			{
				return 1;
			}
			return 0;
	}
	void action()
	{
		showMap = 0;
		TileShape.setFillColor(Color(200, 100, 0));
	}
	

};


class TowerTile :public Tile { //Tile where tower can be place
private:

public:
	TowerTile(Vector2f s, Vector2f p) :Tile(s, p) {
		setColor(Color(255, 255, 255, 80));
		TileShape.setOutlineColor(Color::White);
		TileShape.setOutlineThickness(-2);
		setIsEmpty(1);
	}
	//bool isPointed(Vector2i &MousePos)
	//{
	//	if (MousePos.x > pos.x && MousePos.y > pos.y && MousePos.x < pos.x + size.x && MousePos.y < pos.y + size.y)
	//	{
	//		TileShape.setFillColor(Color(60, 255, 60, 170));
	//		return 1;
	//	}
	//	TileShape.setFillColor(Color(255, 255, 255, 80));
	//	return 0;
	//}

	void draw(RenderWindow &window) { if (showMap == 1)window.draw(TileShape); }
};

class Map {
private:
	vector<Tile*> tilevector;
	vector<EnemyTile*> enemyTileList;
	Tile* TileClicked;
	Base *base;
public:

	friend class GameController;
	friend class EnemyList;
	Map(float mapWidth, float mapHeight, int NumofTile) {
		Vector2f tileSize(mapWidth / NumofTile, mapHeight / NumofTile);
		//int arry[][2] = { { 2,0 },{ 2,1 },{ 2,2 },{ 2,3 },{ 2,4 },{ 2,5 },{ 3,5 },{ 4,5 },{ 5,5 },{ 5,4 },{ 5,3 },{ 5,2 },{ 4,2 },{ 3,2 },{ 2,2 },{ 2,3 },{ 2,4 },{ 2,5 },{ 2,6 },{ 2,7 },{ 2,8 },{2,9} };
	
		int arry[][2] = { {1,0},{ 1,1 },{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{2,8},{ 3,8 },{ 4,8 },{ 5,8 },{ 6,8 },{ 7,8 },{ 8,8 },
		{8,7},{ 8,6 },{ 8,5 },{ 8,4 },{ 8,3 },{ 8,2 },{ 8,1 },{ 7,1 },{ 6,1 },{ 6,2 },{ 6,3 },{ 5,3 },{ 4,3 },{ 3,3 },{ 3,2 },{ 3,1 },
		{ 4,1 },{ 5,1 },{ 6,1 },{ 6,2 },{ 6,3 },{ 6,4 },{ 6,5 },{ 5,5 },{ 4,5 },{ 3,5 },{ 3,6 }
		};
		int index = 0;

		for (auto& x : arry)
		{
			addEnemyTile(new EnemyTile(tileSize, Vector2f(tileSize.x*x[0], tileSize.y*x[1])));
			index++;
		}
		int empty = 1;
		for (int i = 0; i < NumofTile; i++)
		{
			for (int j = 0; j < NumofTile; j++)
			{
				empty = 1;
				for (EnemyTile* x : enemyTileList)
				{
					if (x->pos == Vector2f(tileSize.x*j, tileSize.y*i))
					{
						empty = 0;
						break;
					}
				}
				if (empty)
					addTile(new TowerTile(tileSize, Vector2f(tileSize.x*j, tileSize.y*i)));

			}
		}
		base = new Base(tileSize, enemyTileList[enemyTileList.size()-1]->getPos(), 100, "Base.png");
	}
	~Map() {
		for (auto x : tilevector)
		{
			delete x;
		}
		delete base;

	}
	void addTile(Tile *t) { tilevector.push_back(t); }
	void addEnemyTile(EnemyTile *t) {
		enemyTileList.push_back(t);
		addTile(t);
	}
	void draw(RenderWindow &window) {
		for (Tile *x : tilevector)
		{
			x->draw(window);
		}
		base->draw(window);
	}
	bool TileisClicked(Vector2i &MousePos)
	{
		for (Tile* x : tilevector)
		{
			if (x->isClicked(MousePos))
			{
				TileClicked = x;
				return 1;
			}
		}
		return 0;
	}

	bool TileisPointed(Vector2i &MousePos)
	{
		int t = 0;
		for (auto x : tilevector)
		{
			if (x->isPointed(MousePos))
				t = 1;
		}
		return t;
	}


	void EnemyEnteredBase(Enemy *e) {
		base->Hp -= e->Atk;
	}
	Vector2f isinEnemyTile(Vector2f p, Enemy* e) {
		int i;
		for (i = e->index; i < enemyTileList.size() - 1; i++)
		{
			if (enemyTileList[i]->IsIn(p))
			{
				e->index = i;
				return (enemyTileList[i + 1]->getPos())+(enemyTileList[i]->size)/2.0f;
			}

		}
		return p;
	}
	Tile* getTileClicked() { return TileClicked; }
	int moveEnemy(EnemyList *enemylist)
	{

		for (Enemy* e : enemylist->enemyList)
		{
			Vector2f tomove(0, 0);
			tomove = isinEnemyTile(e->EnemyShape.getPosition() , e);
			if (enemyTileList[enemyTileList.size() - 1]->IsIn(e->getPos()))
			{
				EnemyEnteredBase(e);
				enemylist->removeEnemy(e);
				continue;
			}

			if (tomove.x > (e->EnemyShape.getPosition()).x)
				e->move(e->Spd * SpeedButton::getSpeed(), 0);
			if (tomove.y > (e->EnemyShape.getPosition()).y)
				e->move(0, e->Spd* SpeedButton::getSpeed());
			if (tomove.x < (e->EnemyShape.getPosition()).x)
				e->move(-(e->Spd* SpeedButton::getSpeed()), 0);
			if (tomove.y < (e->EnemyShape.getPosition()).y)
				e->move(0, -(e->Spd)* SpeedButton::getSpeed());
			
		}
		return base->getHp();
		
	}


};

int SpeedButton::speedMod = 1;

class GameController {
public:
	GameController() {
		money = 2400;
	}
	void run() {
		sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Tower defense", sf::Style::Titlebar | Style::Close /*| Style::Fullscreen*/);
		window.setFramerateLimit(40);

		Vector2f PanelSize((gameWidth - gameHeight)*0.6, gameHeight);
		TowerSize = Vector2f((gameWidth-PanelSize.x) / NumofTile, gameHeight / NumofTile);
		TowerMarketSize = Vector2f(PanelSize.x*0.95, PanelSize.y*0.12);

		Panel panel(PanelSize, gameWidth);
		TowerList towerlist;

		Vector2f speedbuttonSize(panel.getHeight() * 0.06, panel.getHeight() * 0.03);
		Vector2f speedbuttonPos(gameWidth - panel.getWidth() + speedbuttonSize.x*0.5, speedbuttonSize.y*0.5);
		SpeedButton speed1x(speedbuttonSize, speedbuttonPos, "1x.png", 1);

		SpeedButton speed2x(speedbuttonSize, Vector2f(speedbuttonPos.x + speedbuttonSize.x, speedbuttonPos.y), "2x.png", 2);
		//SpeedButton speed4x(speedbuttonSize, Vector2f(speedbuttonPos.x + speedbuttonSize.x * 2, speedbuttonPos.y), "4x.png", 4);
		SpeedButton speedStop(speedbuttonSize, Vector2f(speedbuttonPos.x + speedbuttonSize.x * 4, speedbuttonPos.y), "stop.png", 0);
		panel.addButton(&speed1x);
		panel.addButton(&speed2x);
		//panel.addButton(&speed4x);
		panel.addButton(&speedStop);
		
		TowerMarket towerM1(TowerMarketSize, Vector2f(gameWidth - PanelSize.x *0.98, gameHeight*0.11), "pusheen1.png", 30, 300, 1, 400,this,1); //atk,rng,spd,price
		TowerMarket towerM2(TowerMarketSize, Vector2f(gameWidth - PanelSize.x *0.98, TowerMarketSize.y + gameHeight*0.11), "pusheen2.png", 40, 500, 1, 750, this,2);
		panel.addButton(&towerM1);
		panel.addButton(&towerM2);

		Map map(gameWidth - PanelSize.x, gameHeight,NumofTile);
	

		EnemyList enemylist;


		showMap = 0;
		Text moneyText;
		Font font;
		font.loadFromFile("arial.ttf");
		moneyText.setFont(font);
		moneyText.setCharacterSize(gameWidth / 40);
		moneyText.setPosition((gameWidth) * 25 / 30, gameHeight - gameHeight / 10);
		moneyText.setFillColor(Color::Black);
		moneyText.setString("Money: $" + to_string(money));
		Text Hp;
		Hp.setCharacterSize(gameWidth / 40);
		Hp.setFillColor(Color::Black);
		Hp.setFont(font);
		Hp.setString("Hp: " + to_string(map.base->getHp()));
		Hp.setPosition((gameWidth) * 25 / 30, gameHeight * 9 / 10.0f - Hp.getCharacterSize());

		Clock clock;
		clock.restart();
		Time time;

		
		while (window.isOpen())
		{
			
			Event event;
			while (window.pollEvent(event))
			{
				MousePos = Mouse::getPosition(window);
				if ((event.type == sf::Event::Closed) ||
					((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
				{
					window.close();
					break;
				}
				
				if (Mouse::isButtonPressed(Mouse::Left) && event.type != sf::Event::MouseMoved)
				{
					
					if (panel.isClicked(MousePos))
					{
						panel.setCurrentSpeed(SpeedButton::getSpeed());
					}		

					if (showMap == 1 && map.TileisClicked(MousePos) && TowerMarketClicked->Price <= money &&map.getTileClicked()->getIsEmpty())
					{
						if(TowerMarketClicked->val == 1)
							towerlist.addTower(new Tower(towerM1, TowerSize, map.getTileClicked()->getPos(), "Bullet1.png"));
						if (TowerMarketClicked->val == 2)
							towerlist.addTower(new Tower(towerM2, TowerSize, map.getTileClicked()->getPos(), "Bullet2.png"));
							money -= TowerMarketClicked->Price;
							map.getTileClicked()->setIsEmpty(0);

						moneyText.setString("Money: $" + to_string(money));
					}

				}
				if (showMap == 1)
				{
					if(Event::MouseMoved)
						map.TileisPointed(MousePos);
				}
			}
			if (map.moveEnemy(&enemylist) <= 0) // moveEnemy return base Hp
			{
				SpeedButton::setSpeed(0);
			}
			time = clock.getElapsedTime();
			if (SpeedButton::getSpeed() != 0)
			{
				if (time.asSeconds() >= 2.0f / SpeedButton::getSpeed())
				{
					enemylist.addEnemy(new Enemy (map.enemyTileList[0]->getPos(), Vector2f(100, 100), "Enemy1.png",100+time.asSeconds()*5));
					clock.restart();
					setMoney(getMoney() + 25);
					moneyText.setString("Money: $" + to_string(money));
				}
			}

			towerlist.Shoot(&enemylist,SpeedButton::getSpeed());
			Hp.setString("Hp: " + to_string(map.base->getHp()));

			window.clear(Color(140, 240, 140));

			panel.draw(window);
			window.draw(moneyText);
			window.draw(Hp);
			map.draw(window);
			towerlist.draw(window);
			enemylist.draw(window);

			window.display();
		}
	}
	int getMoney() { return money; }
	void setMoney(int m) {  money = m; }
	Vector2i getMousePos() { return MousePos; }
	void setTowerMarketClicked(TowerMarket* t) { TowerMarketClicked = t; }
	TowerMarket* getTowerMarketClicked() { return TowerMarketClicked; }
	
private:
	const float gameWidth = 1280;
	const float gameHeight = 760;
	Vector2i MousePos;
	Vector2f TowerSize;
	Vector2f TowerMarketSize;
	int money;
	const int NumofTile = 10;
	TowerMarket* TowerMarketClicked ;
	Clock clock;
	Time time;

};

void TowerMarket::draw(sf::RenderWindow &window)
{
	Button::draw(window);
	window.draw(TowerInfoShape);
	window.draw(atkT);
	window.draw(spdT);
	window.draw(rngT);
	window.draw(priceT);
	if (showMap == 1 && val == gameController->getTowerMarketClicked()->val)
	{

		Vector2f mousepos((gameController->getMousePos()).x, (gameController->getMousePos()).y);
		TowerShape.setPosition(mousepos);
		towerRng.setPosition(mousepos);
		
		window.draw(TowerShape);
		window.draw(towerRng);
	}
}
bool TowerMarket::isClicked(Vector2i &MousePos)
{
	if (MousePos.x >= TowerInfoPos.x - TowerInfoSize.x / 3 && MousePos.y >= TowerInfoPos.y  && MousePos.x <= TowerInfoPos.x + TowerInfoSize.x && MousePos.y <= TowerInfoPos.y + TowerInfoSize.y)
	{
		action();
		gameController->setTowerMarketClicked(this);
		return 1;
	}
	return 0;
}
void TowerMarket::action() {
	showMap = 1;

}
int main()
{
	GameController controller;
	controller.run();
}