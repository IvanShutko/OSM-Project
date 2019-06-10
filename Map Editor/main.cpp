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
	void getData();
	void launch();
	void processEvents();
	void update();
	void render();
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

int main()
{
	App app;
	app.run();
	cout << "Press any key to exit...";
	_getch();
	return 0;
}