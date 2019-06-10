#include "MapCom.h"
#include <iostream>
using namespace std;

ostream& operator<<(ostream& s, vector<string>& bounds)
{
	for (int a = 0; a < bounds.size(); a++)
		s << bounds[a] << endl;
	return s;
}

ostream& operator<<(ostream& s, Node& node)
{
	s << "NODE " << node.id << node.lat << node.lon << endl;
	for (int a = 0; a < node.paramlist.size(); a++)
		s << "PARAM " << node.paramlist[a].first << " " << node.paramlist[a].second << endl;
	return s;
}

ostream& operator<<(ostream& s, vector<Node>& nodes)
{
	for (int a = 0; a < nodes.size(); a++)
		s << nodes[a];
	return s;
}

ostream& operator<<(ostream& s, Way& way)
{
	s << "WAY " << way.id << endl;
	for (int a = 0; a < way.paramlist.size(); a++)
		s << "PARAM " << way.paramlist[a].first << " " << way.paramlist[a].second << endl;

	for (int a = 0; a < way.nodeidlist.size(); a++)
		s << "NODEID " << way.nodeidlist[a] << endl;

	return s;
}

ostream& operator<<(ostream& s, vector<Way>& ways)
{
	for (int a = 0; a < ways.size(); a++)
		s << ways[a];
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
		s << rels[a];
	return s;
}

istream& operator>>(istream& s, vector<string>& bounds)
{
	string buff;
	for (int i = 0; i < 4; i++)
	{
		s >> buff;
		bounds.push_back(buff);
	}
	return s;
}

istream& operator>>(istream& s, vector<Node>& nodes)
{
	string buff;
	while (!s.eof())
	{
		s >> buff;
		if (buff == "NODE")
		{
			Node temp;
			s >> buff;
			temp.id = buff;
			s >> buff;
			temp.lat = buff;
			s >> buff;
			temp.lon = buff;
			nodes.push_back(temp);
		}
		else
		{
			string k;
			char* v = new char[200];
			s >> k;
			s.getline(v, 200);
			nodes[nodes.size() - 1].paramlist.push_back(pair<string, string>(k, v));
		}
	}
	return s;
}

istream& operator>>(istream& s, vector<Way>& ways)
{
	string buff;
	while (!s.eof())
	{
		s >> buff;
		if (buff == "WAY")
		{
			Way temp;
			s >> buff;
			temp.id = buff;
			ways.push_back(temp);
		}
		else if (buff == "NODEID")
		{
			s >> buff;
			ways[ways.size() - 1].nodeidlist.push_back(buff);
		}
		else
		{
			string k, v;
			s >> k >> v;
			ways[ways.size() - 1].paramlist.push_back(pair<string, string>(k, v));
		}
	}
	return s;
}

istream& operator>>(istream& s, vector<Relation>& rels)
{
	return s;
	/*
	string buff;
	while (!s.eof())
	{
		s >> buff;
		if (buff == "RELATION")
		{
			Relation temp;
			s >> buff;
			temp.id = buff;
			rels.push_back(temp);
		}
		else if (buff == "NODEID")
		{
			s >> buff;
			rels[rels.size() - 1].nodeidlist.push_back(buff);
		}
		else if (buff == "WAYID")
		{
			s >> buff;
			rels[rels.size() - 1].
		}
	}*/
}