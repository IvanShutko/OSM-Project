#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "MapCom.h"
using namespace std;

class DrawableEntity : public sf::Drawable
{
protected:
	string id;
	DrawableEntity();
public:
	DrawableEntity(string uID);
	string getID();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class DrawableNode : private DrawableEntity
{
protected:
	sf::CircleShape point;
	vector<pair<string, string>> paramlist;
	sf::Vector2f coordinates;
	sf::Vector2f position;
	DrawableNode();
public:
	DrawableNode(Node node);
	sf::Vector2f getCoordinates();
	sf::Vector2f getPosition();
	void setCoordinates(const sf::Vector2f& coords);
	void setPosition(const sf::Vector2f& pos);
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};
