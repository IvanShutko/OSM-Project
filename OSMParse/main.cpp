#include"stdafx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <iomanip>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
#define Token pair<string, string>
#define TagLine vector<Token>

class Tag;
class Node;
class Way;
class Relation;
class MemberData;

struct Tag
{
	string name;
	vector<pair<string, string>> paramlist;
	vector<pair<string, Tag>> taglist;
};

class Node
{
public:
	string id;
	string lat;
	string lon;
	vector<pair<string, string>> paramlist;
};

class Way
{
public:
	string id;
	vector<pair<string, string>> paramlist;
	vector<string> nodeidlist;
};

class Relation
{
public:
	string id;
	vector<pair<string, string>> paramlist;
	vector<pair<string, string>> nodeidlist;///STRING хранит айди и роль объекта 
	vector<pair<string, string>> wayidlist;///STRING хранит айди и роль объекта
	vector<pair<string, string>> relationidlist;///STRING хранит айди и роль объекта
};

void read(char* addr, vector<TagLine> &taglist)
{

	TagLine tag;
	ifstream source;
	source.open(addr);
	if (!source)
	{
		cout << "Cannot open file!" << endl;
		_getch();
		exit(1);
	}
	char ch;
	char* buff = new char[200];

	string key;
	string value;
	while (!source.eof())
	{
		source.getline(buff, 200);
		string line = buff;
		if (line.empty()) continue;
		int pos = line.find_first_of('<');
		line.erase(0, pos - 1);
		key = "";
		ch = '\0';
		for (int i = 0; i < strlen(buff); i++)
		{
			ch = buff[i];

			if (ch == '=') key = "equals";
			else if (ch == '<') key = "opentag";
			else if (ch == '>') key = "closetag";
			else if (ch == '?') key = "question";
			else if (ch == '/') key = "slash";
			else if (ch == '"') key = "quote";
			else if (ch == '&') key = "ampersand";
			else if (ch == ' ') key = "space";
			else key = "word";
			value = ch;
			if (key == "opentag")
			{
				tag.clear();
			}
			tag.push_back(make_pair(key, value));
			if (key == "closetag")
			{
				taglist.push_back(tag);
				tag.clear();
			}
		}
		if (!tag.empty())
		{
			taglist.push_back(tag);
		}
		tag.clear();
	}
}

vector<TagLine> groupToWords(vector<TagLine>& list)
{
	vector<TagLine> res;
	TagLine tag;
	string key = "";
	string value = "";
	for (int j = 0; j < list.size(); j++)
	{
		key = "";
		value = "";
		for (int i = 0; i < list[j].size(); i++)
		{
			if (list[j][i].first != key && key == "word")
			{
					tag.push_back(make_pair(key, value));
					key = "";
					value = "";
			}

			key = list[j][i].first;
			if (key == "word")
				value += list[j][i].second;
			else
			{
				value = list[j][i].second;
				tag.push_back(make_pair(key, value));
				key = "";
				value = "";
			}
		}
		if (key != "")
			tag.push_back(make_pair(key, value));
		res.push_back(tag);
		tag.clear();
		list[j].clear();
	}
	list.clear();
	return res;
}

vector<TagLine> classify(vector<TagLine>& list)
{
	string key = "";
	string value = "";
	bool tagProcessing = false;

	vector<TagLine> taglist;
	TagLine currtag;
	string tagname = "";
	string tagtype = "opentag";
	bool isWritingToQuote = false;
	bool isPredicateTag = false;
	bool isSelfTag = false;

	for (int i = 0; i < list.size(); i++)
	{
		
		if (list[i][1].first == "slash")
		{
			tagname = list[i][2].second;
			currtag.push_back(make_pair("endtag", tagname));
			taglist.push_back(currtag);
			currtag.clear();
			list[i].clear();
			continue;
		}

		if (list[i][2].first == "closetag")
		{
			tagname = list[i][1].second;
			currtag.push_back(make_pair("opentag", tagname));
			currtag.push_back(make_pair("closetag", tagname));
			taglist.push_back(currtag);
			currtag.clear();
			list[i].clear();
			continue;
		}

		tagtype = "closetag";
		tagname = "";
		value = "";
		isWritingToQuote = false;
		isPredicateTag = false;
		isSelfTag = false;

		for (int j = 0; j < list[i].size(); j++)
		{
			if (isWritingToQuote)
			{
				if (list[i][j].first == "quote")
				{
					currtag.push_back(make_pair("value", value));
					value = "";
					isWritingToQuote = false;
				}
				else
				{
					value += list[i][j].second;
				}
				continue;
			}

			if (list[i][j].first == "question")
			{
				isPredicateTag = true;
				continue;
			}

			if (list[i][j].first == "word")
			{
				if (tagname == "")
				{
					tagname = list[i][j].second;
					currtag.push_back(make_pair("opentag", tagname));
				}
				else currtag.push_back(make_pair("param", list[i][j].second));
			}

			if (list[i][j].first == "quote")
			{
				isWritingToQuote = true;
			}

			if (list[i][j].first == "slash")
			{
				isSelfTag = true;
			}
		}
		
		if (isPredicateTag)
		{
			list[i].clear();
			currtag.clear();
		}
		else if (isSelfTag)
		{
			currtag.push_back(make_pair("closetag", tagname));
			currtag.push_back(make_pair("endtag", tagname));
			taglist.push_back(currtag);
			list[i].clear();
			currtag.clear();
		}
		else 
		{
			currtag.push_back(make_pair("closetag", tagname));
			taglist.push_back(currtag);
			list[i].clear();
			currtag.clear();
		}
	}

	list.clear();
	
	return taglist;
}


Tag build(vector<TagLine> &list)
{
	vector<TagLine> block;
	Tag tag;
	string tagName = list[0][0].second;
	for(int i = 0; i < list.size(); i++)
	{
		block.push_back(list[i]);
		if (list[i][list[i].size()-1].first == "endtag" && list[i][list[i].size()-1].second == tagName)
		{
			break;
		}
	}


	tag.name = tagName;
	vector<Token> params;
	for (int i = 0; i < block[0].size(); i++)
	{
		if (block[0][i].first == "param" || block[0][i].first == "value")
			params.push_back(block[0][i]);
	}

	block[0].clear();

	for (int i = 0; i < params.size() / 2; i++)
	{
		tag.paramlist.push_back(make_pair(params[i * 2].second, params[i * 2 + 1].second));
	}

	vector<TagLine> tempBlock;
	tagName = "";

	for (int i = 1; i < block.size() - 1; i++)
	{
		tempBlock.push_back(block[i]);
		if (block[i][0].first == "opentag" && tagName == "") 
			tagName = block[i][0].second;
		if (block[i][block[i].size() - 1].first == "endtag" && block[i][block[i].size() - 1].second == tagName)
		{
			tag.taglist.push_back(make_pair(tagName, build(tempBlock)));
			tagName = "";
			tempBlock.clear();
		}
	}

	block.clear();
	return tag;
}


void eraseOSMLogData(Tag& tag)
{
	tag.paramlist.erase(tag.paramlist.begin(), tag.paramlist.begin() + 2);
	tag.taglist.erase(tag.taglist.begin(), tag.taglist.begin() + 2);
}

void separateToElements(Tag& tagStructure, Tag& tagbounds, vector<Tag>& tagnodes, vector<Tag>& tagways, vector<Tag>& tagrelations)
{
	tagbounds = tagStructure.taglist[0].second;
	for (int i = 1; i < tagStructure.taglist.size(); i++)
	{
		if (tagStructure.taglist[i].first == "node")
		{
			tagnodes.push_back(tagStructure.taglist[i].second);
		}
		else if (tagStructure.taglist[i].first == "way")
		{
			tagways.push_back(tagStructure.taglist[i].second);
		}
		else if (tagStructure.taglist[i].first == "relation")
		{
			tagrelations.push_back(tagStructure.taglist[i].second);
		}
		else cout << "FOUND UNKNOWN TAG " << tagStructure.taglist[i].first;
	}
	tagStructure.paramlist.clear();
	tagStructure.taglist.clear();
}

void cleanNode(vector<Tag>& block)
{
	for (int i = 0; i < block.size(); i++)
	{
		block[i].paramlist.erase(block[i].paramlist.begin() + 3, block[i].paramlist.end());
	}
}

void clean(vector<Tag>& block)
{
	for (int i = 0; i < block.size(); i++)
	{
		block[i].paramlist.erase(block[i].paramlist.begin() + 1, block[i].paramlist.end());
	}
}

void moveKeyToParams(vector<Tag>& block)
{
	for (int i = 0; i < block.size(); i++)
	{
		for (int j = 0; j < block[i].taglist.size(); j++)
		{
			if (block[i].taglist[j].first == "tag")
			{
				block[i].paramlist.push_back(pair<string, string>(block[i].taglist[j].second.paramlist[0].second, block[i].taglist[j].second.paramlist[1].second));
				block[i].taglist.erase(block[i].taglist.begin() + j);
				j--;
			}
		}
	}
}

vector<string> extractBounds(Tag& block)
{
	string buffer;
	vector<string> res;
	for (int i = 0; i < 4; i++)
	{
		res.push_back(block.paramlist[i].second);
	}
	return res;
}

vector<Node> extractNodes(vector<Tag>& block)
{
	vector<Node> res;
	Node* currNode = NULL;
	for (int i = 0; i < block.size();i++)
	{
		currNode = new Node();
		currNode->id = block[i].paramlist[0].second;
		currNode->lat = block[i].paramlist[1].second;
		currNode->lon = block[i].paramlist[2].second;
		currNode->paramlist = block[i].paramlist;
		currNode->paramlist.erase(currNode->paramlist.begin(), currNode->paramlist.begin() + 3);
		res.push_back(*currNode);
	}
	block.clear();
	return res;
}

vector<Way> extractWays(vector<Tag>& block)
{
	vector<Way> res;
	Way* currWay = NULL;
	for (int i = 0; i < block.size(); i++)
	{
		currWay = new Way();
		currWay->id = block[i].paramlist[0].second;
		currWay->paramlist = block[i].paramlist;
		currWay->paramlist.erase(currWay->paramlist.begin());
		vector<pair<string, Tag>> taglist = block[i].taglist;
		for (int j = 0; j < block[i].taglist.size(); j++)
		{
			string id = block[i].taglist[j].second.paramlist[0].second;
			currWay->nodeidlist.push_back(id);
		}
		res.push_back(*currWay);
	}
	return res;
}


vector<Relation> extractRelations(vector<Tag>& block)
{
	vector<Relation> res;
	Tag* currTag = NULL;
	Relation* currRel = NULL;
	for (int i = 0; i < block.size(); i++)
	{
		currRel = new Relation();
		currTag = &block[i];
		currRel->id = currTag->paramlist[0].second;
		currRel->paramlist = currTag->paramlist;
		currRel->paramlist.erase(currRel->paramlist.begin());
		vector<pair<string, Tag>> taglist = currTag->taglist;
		for (int j = 0; j < taglist.size(); j++)
		{
			string type = taglist[j].second.paramlist[0].second;
			string id = taglist[j].second.paramlist[1].second;
			string role = taglist[j].second.paramlist[2].second;
			if (type == "node")
			{
				currRel->nodeidlist.push_back(make_pair(id, role));
			}
			else if (type == "way")
			{
				currRel->wayidlist.push_back(make_pair(id, role));
			}
			else if (type == "relation")
			{
				currRel->relationidlist.push_back(make_pair(id, role));
			}
		}
		res.push_back(*currRel);
	}
	return res;
}

ostream& operator<<(ostream& s, vector<string>& bounds)
{
	for (int a = 0; a < bounds.size(); a++)
	{
		s << bounds[a] << endl;
	}
	return s;
}

ostream& operator<<(ostream& s, Node& node)
{
	s << "NODE " << node.id << " " << node.lat << " " << node.lon << endl;
	for (int a = 0; a < node.paramlist.size(); a++)
	{
		s << "PARAM " << node.paramlist[a].first << " " << node.paramlist[a].second << endl;
	}
	return s;
}

ostream& operator<<(ostream& s, vector<Node>& nodes)
{
	for (int a = 0; a < nodes.size(); a++)
	{
		s << nodes[a];
	}
	return s;
}

ostream& operator<<(ostream& s, Way& way)
{
	s << "WAY " << way.id << endl;
	for (int a = 0; a < way.paramlist.size(); a++)
	{
		s << "PARAM " << way.paramlist[a].first << " " << way.paramlist[a].second << endl;
	}
	for (int a = 0; a < way.nodeidlist.size(); a++)
	{
		s << "NODEID " << way.nodeidlist[a] << endl;
	}
	return s;
}

ostream& operator<<(ostream& s, vector<Way>& ways)
{
	for (int a = 0; a < ways.size(); a++)
	{
		s << ways[a];
	}
	return s;
}

ostream& operator<<(ostream& s, Relation& rel)
{
	s << "RELATION " << rel.id << endl;
	for (int a = 0; a < rel.paramlist.size(); a++)
	{
		s << "PARAM " << rel.paramlist[a].first << " " << rel.paramlist[a].second << endl;
	}
	for (int a = 0; a < rel.nodeidlist.size(); a++)
	{
		if (rel.nodeidlist[a].second == "") rel.nodeidlist[a].second = "NULL";
		s << "NODEID " << rel.nodeidlist[a].first << " " << rel.nodeidlist[a].second << endl;
	}
	for (int a = 0; a < rel.wayidlist.size(); a++)
	{
		if (rel.wayidlist[a].second == "") rel.wayidlist[a].second = "NULL";
		s << "WAYID " << rel.wayidlist[a].first << " " << rel.wayidlist[a].second << endl;
	}
	for (int a = 0; a < rel.relationidlist.size(); a++)
	{
		if (rel.relationidlist[a].second == "") rel.relationidlist[a].second = "NULL";
		s << "RELID " << rel.relationidlist[a].first << " " << rel.relationidlist[a].second << endl;
	}
	return s;
}

ostream& operator<<(ostream& s, vector<Relation>& rels)
{
	for (int a = 0; a < rels.size(); a++)
	{
		s << rels[a];
	}
	return s;
}

string replacePoint(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '.') str[i] = ',';
	}
	return str;
}

vector<Node> formNodeList(vector<Node> nodeList, Way way)
{
	vector<Node> res;
	for (int i = 0; i < way.nodeidlist.size(); i++)
	{
		string id = way.nodeidlist[i];
		for (int j = 0; j < nodeList.size(); j++)
		{
			if (id == nodeList[j].id)
			{
				res.push_back(nodeList[j]);
				break;
			}
		}
	}
	return res;
}

void draw(vector<string> bounds, vector<Node> nodelist, vector<Way> waylist)
{
	double minlat = atof(replacePoint(bounds[0]).c_str());
	double minlon = atof(replacePoint(bounds[1]).c_str());
	double maxlat = atof(replacePoint(bounds[2]).c_str());
	double maxlon = atof(replacePoint(bounds[3]).c_str());

	cout << minlat << " " << minlon << " " << maxlat << " " << maxlon << endl;

	double lat = maxlat - minlat;
	double lon = maxlon - minlon;

	cout << lat << " " << lon;

	lat = lat * 100000;
	lon = lon * 100000;


	sf::RenderTexture rendertexture;
	rendertexture.create(lat, lon);

	rendertexture.clear(sf::Color::Black);

	for (int i = 0; i < waylist.size(); i++)
	{
		Way currWay = waylist[i];
		vector<Node> nodes = formNodeList(nodelist, waylist[i]);
		sf::VertexArray line(sf::LineStrip, nodes.size());
		
		for (int j = 0; j < nodes.size(); j++)
		{
			double nlat = atof(replacePoint(nodes[j].lat).c_str());
			double nlon = atof(replacePoint(nodes[j].lon).c_str());
			nlat = nlat - minlat;
			nlon = nlon - minlon;
			nlat = nlat * 100000;
			nlon = nlon * 100000;
			line[j].position = sf::Vector2f(nlat, nlon);
		}
		rendertexture.draw(line);
	}

	rendertexture.display();

	const sf::Texture& texture = rendertexture.getTexture();
	sf::Image image = texture.copyToImage();
	image.saveToFile("c:/osmparce/result.png");
}


int main()
{
	setlocale(LC_ALL, "Russian");

	char* file_adress = new char[200];
	cout << "Введите путь к файлу: ";
	cin.getline(file_adress, 200);

	vector<TagLine> list;

	read(file_adress, list);
	cout << "Чтение завершено" << endl;

	list = groupToWords(list);
	cout << "Группирование завершено" << endl;

	list = classify(list);
	cout << "Классифицирование тегов завершено" << endl;

	Tag tagStructure = build(list);
	cout << "Структура создана" << endl;
	cout << "Обработка данных завершена" << endl;
	list.clear();

	cout << endl << endl << "НАЧАЛО СТРУКТУРИЗАЦИИ" << endl;

	eraseOSMLogData(tagStructure);
	cout << "Удалены лишние данные из блока" << endl;

	Tag tagbounds;
	vector<Tag> tagnodes;
	vector<Tag> tagways;
	vector<Tag> tagrelations;

	separateToElements(tagStructure, tagbounds, tagnodes, tagways, tagrelations);

	cleanNode(tagnodes);
	clean(tagways);
	clean(tagrelations);

	moveKeyToParams(tagnodes);
	moveKeyToParams(tagways);
	moveKeyToParams(tagrelations);

	vector<string> bounds = extractBounds(tagbounds);
	vector<Node> nodelist = extractNodes(tagnodes);
	vector<Way> waylist = extractWays(tagways);
	vector<Relation> relationlist = extractRelations(tagrelations);

	ofstream file;
	file.open("bounds.amf");
	file << bounds;
	file.close();

	file.open("node.amf");
	file << nodelist;
	file.close();

	file.open("way.amf");
	file << waylist;
	file.close();

	file.open("relation.amf");
	file << relationlist;
	file.close();

	//draw(bounds, nodelist, waylist);

	cout << "Запись завершена" << endl;

	_getch();

	return 0;
}