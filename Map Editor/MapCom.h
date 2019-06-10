#pragma once

#include <string>
#include <vector>
using namespace std;

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

ostream& operator<<(ostream& s, vector<string>& bounds);
ostream& operator<<(ostream& s, Node& node);
ostream& operator<<(ostream& s, Way& way);
ostream& operator<<(ostream& s, Relation& rel);

ostream& operator<<(ostream& s, vector<Node>& nodes);
ostream& operator<<(ostream& s, vector<Way>& ways);
ostream& operator<<(ostream& s, vector<Relation>& rels);

istream& operator>>(istream& s, vector<string>& bounds);
istream& operator>>(istream& s, vector<Node>& nodes);
istream& operator>>(istream& s, vector<Way>& ways);
istream& operator>>(istream& s, vector<Relation>& rels);