#include <iostream>
#include <fstream>
#include <vector>
#include "MapCom.h"
#include <conio.h>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


string replacePoint(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '.') str[i] = ',';
	}
	return str;
}

class App
{
protected:
	vector<string> boundlist;
	vector<Node> nodelist;
	vector<Way> waylist;
	vector<Relation> relationlist;
	sf::RenderWindow window;

	vector<sf::VertexArray> linearr;
	vector<sf::CircleShape> pointarr;

	void getData();
	void launch();
	void processEvents();
	void update();
	void render();
	void saveData();
public:
	App() { }
	void run();
};

void App::getData()
{
	char dir[200];
	cout << "¬ведите путь к директории с файлами карты (*.amf): ";
	cin.getline(dir, 200);

	ifstream source;
	string adress;

	adress = string(dir) + "bounds.amf";
	source.open(adress);
	source >> boundlist;
	source.close();

	adress = string(dir) + "node.amf";
	source.open(adress);
	source >> nodelist;
	source.close();

	adress = string(dir) + "way.amf";
	source.open(adress);
	source >> waylist;
	source.close();

	adress = string(dir) + "relation.amf";
	source.open(adress);
	source >> relationlist;
	source.close();
}

void App::saveData()
{
	char dir[200];
	cout << "¬ведите путь к директории с будущими файлами карты (*.amf): ";
	cin.getline(dir, 200);

	ofstream source;
	string adress;

	adress = string(dir) + "bounds.amf";
	source.open(adress);
	source << boundlist;
	source.close();

	adress = string(dir) + "node.amf";
	source.open(adress);
	source << nodelist;
	source.close();

	adress = string(dir) + "way.amf";
	source.open(adress);
	source << waylist;
	source.close();

	adress = string(dir) + "relation.amf";
	source.open(adress);
	source << relationlist;
	source.close();
}

void App::launch()
{
	double minlat = atof(replacePoint(boundlist[0]).c_str());
	double minlon = atof(replacePoint(boundlist[1]).c_str());
	double maxlat = atof(replacePoint(boundlist[2]).c_str());
	double maxlon = atof(replacePoint(boundlist[3]).c_str());
	double lat = maxlat - minlat;
	double lon = maxlon - minlon;
	lat = lat * 100000;
	lon = lon * 100000;
	window.create(VideoMode(lon, lat), "–едактор");
}

void App::run()
{
	getData();
	launch();
	while (window.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void App::processEvents()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		if (event.type == Event::Resized)
		{
			sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
			window.setView(sf::View(visibleArea));
		}
	}
}

void App::update()
{
	/*double minlat = atof(replacePoint(boundlist[0]).c_str());
	double minlon = atof(replacePoint(boundlist[1]).c_str());
	double maxlat = atof(replacePoint(boundlist[2]).c_str());
	double maxlon = atof(replacePoint(boundlist[3]).c_str());
	double lat = maxlat - minlat;
	double lon = maxlon - minlon;
	lat = lat * 100000;
	lon = lon * 100000;
	for (int i = 0; i < waylist.size(); i++)
	{
		Way currWay = waylist[i];
		sf::VertexArray line(sf::LineStrip, currWay.nodelist.size());
		vector<Node> nodes = currWay.nodeidlist;
		for (int j = 0; j < nodes.size(); j++)
		{
			double nlat = atof(replacePoint(nodes[j].lat).c_str());
			double nlon = atof(replacePoint(nodes[j].lon).c_str());
			nlat = -(nlat - maxlat);
			nlon = nlon - minlon;
			nlat = nlat * 100000;
			nlon = nlon * 100000;
			line[j].position = sf::Vector2f(nlon, nlat);
			line[j].color = sf::Color::Black;
		}
		linearr.push_back(line);
	}

	sf::CircleShape point(3);
	for (int j = 0; j < nodelist.size(); j++)
	{
		double nlat = atof(replacePoint(nodelist[j].lat).c_str());
		double nlon = atof(replacePoint(nodelist[j].lon).c_str());
		nlat = -(nlat - maxlat);
		nlon = nlon - minlon;
		nlat = nlat * 100000;
		nlon = nlon * 100000;
		point.setPosition(sf::Vector2f(nlon - 3, nlat - 3));
		point.setFillColor(sf::Color::Black);
		pointarr.push_back(point);
	}*/
}

void App::render()
{
	window.clear(Color::Black);

	window.display();
}

int main()
{
	App app;
	app.run();
	cout << "Press any key to exit...";
	_getch();
	return 0;
}