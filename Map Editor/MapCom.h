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
	vector<pair<string, string>> nodeidlist;///STRING ������ ���� � ���� ������� 
	vector<pair<string, string>> wayidlist;///STRING ������ ���� � ���� �������
	vector<pair<string, string>> relationidlist;///STRING ������ ���� � ���� �������
};

istream& operator>>(istream& s, vector<string>& bounds);
istream& operator>>(istream& s, vector<Node>& nodes);
istream& operator>>(istream& s, vector<Way>& ways);
istream& operator>>(istream& s, vector<Relation>& rels);