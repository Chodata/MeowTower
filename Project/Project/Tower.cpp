#include "Tower.hpp"

TowerMarket::TowerMarket(Vector2f si, Vector2f p, string fn, int a, int r, int s, int price, GameController* g, int v) :Button(Vector2f(si.x / 3, si.y), p, fn), Atk(a), Rng(r), Spd(s), Price(price), gameController(g) ,val(v){
	TowerInfoPos = Vector2f(p.x + si.x / 3, p.y);
	TowerInfoSize = Vector2f(Vector2f(si.x*2.0f / 3, si.y));
	TowerInfoShape.setSize(TowerInfoSize);
	TowerInfoShape.setPosition(TowerInfoPos);
	TowerInfoShape.setFillColor(Color(220, 220, 220));
	TowerInfoShape.setOutlineColor(Color::Black);
	TowerInfoShape.setOutlineThickness(-1);

	TowerShape = *((*this).getFrame());
	towerRng.setRadius(Rng);
	towerRng.setFillColor(Color(255, 255, 255, 80));
	towerRng.setOutlineColor(Color(255, 255, 255));
	towerRng.setOutlineThickness(-2);
	TowerShape.setOrigin(TowerShape.getSize().x / 2, TowerShape.getSize().y / 2);
	towerRng.setOrigin(Rng, Rng);

	font.loadFromFile("arial.ttf");
	atkT.setFont(font);
	rngT.setFont(font);
	spdT.setFont(font);
	priceT.setFont(font);

	atkT.setString("Atk: " + to_string(Atk));
	rngT.setString("Spd: " + to_string(Spd));
	spdT.setString("Rng: " + to_string(Rng));
	priceT.setString("Price: $" + to_string(Price));
	int fontsize = si.y / 5;
	int spaceSize = fontsize *1.1;
	atkT.setCharacterSize(fontsize);
	spdT.setCharacterSize(fontsize);
	rngT.setCharacterSize(fontsize);
	priceT.setCharacterSize(fontsize);
	atkT.setPosition(TowerInfoPos.x*1.02, TowerInfoPos.y*1.02);
	spdT.setPosition(TowerInfoPos.x*1.02, TowerInfoPos.y*1.02 + spaceSize);
	rngT.setPosition(TowerInfoPos.x*1.02, TowerInfoPos.y*1.02 + spaceSize * 2);
	priceT.setPosition(TowerInfoPos.x*1.02, TowerInfoPos.y*1.02 + spaceSize * 3);
	atkT.setFillColor(Color::Black);
	spdT.setFillColor(Color::Black);
	rngT.setFillColor(Color::Black);
	priceT.setFillColor(Color::Black);



}

Bullet::Bullet(Enemy* enemy, Vector2f s, Vector2f startPos, int d, string fn) :e(enemy), Dmg(d), size(s)
{
	BulletTexture.loadFromFile(fn);
	BulletShape.setTexture(&BulletTexture);
	BulletShape.setSize(size);
	BulletShape.setPosition(startPos);
}
void Bullet::move()
{
	int Spd = 5;
	if (e->getPos().x > BulletShape.getPosition().x)
		BulletShape.setPosition(BulletShape.getPosition().x + e->getSpd() *Spd, BulletShape.getPosition().y);
	if (e->getPos().y > BulletShape.getPosition().y)
		BulletShape.setPosition(BulletShape.getPosition().x + 0, BulletShape.getPosition().y + e->getSpd() *Spd);
	if (e->getPos().x < BulletShape.getPosition().x)
		BulletShape.setPosition(BulletShape.getPosition().x + e->getSpd() * -Spd, BulletShape.getPosition().y);
	if (e->getPos().y < BulletShape.getPosition().y)
		BulletShape.setPosition(BulletShape.getPosition().x + 0, BulletShape.getPosition().y + e->getSpd() * -Spd);
}
bool Bullet::HitEnemy() {

	if (BulletShape.getPosition().x >= e->getPos().x - e->getSize().x / 2 && BulletShape.getPosition().y >= e->getPos().y - e->getSize().y / 2 &&
		BulletShape.getPosition().x <= e->getPos().x + e->getSize().x / 2 && BulletShape.getPosition().y <= e->getPos().y + e->getSize().y / 2)
	{
		e->shot(Dmg);

		return 1;
	}
	return 0;
}


BulletList::~BulletList() {
	for (Bullet* b : bulletVector)
	{
		delete b;
	}
}
void BulletList::draw(RenderWindow &window) {
	for (Bullet* x : bulletVector)
	{
		x->draw(window);
	}
}
void BulletList::move() {
	int index = 0;
	for (Bullet*x : bulletVector)
	{
		x->move();

		if (x->HitEnemy() || x->getEnemy() == NULL)
		{
			delete x;
			bulletVector.erase(bulletVector.begin() + index);
			continue;
		}
		index++;
	}
}

Tower::Tower(TowerMarket& tm, Vector2f si, Vector2f p, string fnB) :size(si), pos(p), Atk(tm.Atk), Rng(tm.Rng), Spd(tm.Spd), Price(tm.Spd)
{
	clock.restart();
	TowerTexture.loadFromFile(tm.getFilename());
	TowerShape.setTexture(&TowerTexture);
	TowerShape.setPosition(pos);
	TowerShape.setSize(si);
	BulletFile = fnB;

}

bool Tower::EnermyInRng(Vector2f ePos)
{
	float distance = pow((pos.x - ePos.x), 2) + pow((pos.y - ePos.y), 2);
	if (distance < pow(Rng, 2))
	{
		return 1;
	}
	return 0;
}

void TowerList::Shoot(EnemyList* enemylist, int speed)
{
	if (speed != 0)
	{
		for (Tower* t : towerList)
		{
			t->bulletlist.move();
		}
	}

	for (Tower* t : towerList)
	{
		if (speed == 0)
			t->restartTime();
		if (t->getTime().asMilliseconds() * speed < 1000.0f * 1 / t->Spd)
			continue;

		for (auto e : enemylist->enemyList)
		{

			if (e->getHp() <= 0)
			{
				enemylist->removeEnemy(e);

			}
			if (t->EnermyInRng(e->getPos()))
			{
				//t->Shoot(e);
				t->bulletlist.addBullet(new Bullet(e, Vector2f(20, 20), t->pos, 10, t->BulletFile));
				t->restartTime();
				break;

			}

		}
	}


}

void TowerList::draw(RenderWindow &window)
{
	for (Tower*x : towerList)
	{
		x->drawTower(window);
		for (Bullet* b : x->bulletlist.bulletVector)
		{
			b->draw(window);
		}
	}

}