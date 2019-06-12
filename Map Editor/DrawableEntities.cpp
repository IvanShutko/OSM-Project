#include "DrawableEntities.h"
using namespace std;


DrawableEntity::DrawableEntity(string uID)
{
	id = uID;
}

string DrawableEntity::getID()
{
	return id;
}

DrawableNode::DrawableNode(Node node)
{
	id = node.id;
	coordinates.x = atof(node.lat.c_str());
	coordinates.y = atof(node.lat.c_str());
	position.x = 0;
	position.y = 0;
	paramlist = node.paramlist;
}

sf::Vector2f DrawableNode::getCoordinates()
{
	return coordinates;
}

sf::Vector2f DrawableNode::getPosition()
{
	return position;
}

void DrawableNode::setCoordinates(const sf::Vector2f& coords)
{
	coordinates = coords;
}

void DrawableNode::setPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void DrawableNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(point);
}

