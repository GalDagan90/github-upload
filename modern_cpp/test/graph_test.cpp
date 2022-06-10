/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <iostream>
#include <vector>

#include "graph.hpp"

using namespace ilrd_5678;

void AddEdges(Graph& g);
void Print(Graph& g);

int main()
{
	Graph g;

	AddEdges(g);

	auto distVec= g.Dijkstra();

	for (const auto it : distVec)
	{
		std::cout << it << " ";
	}
	std::cout << "\n";

	return 0;
}

void AddEdges(Graph& g)
{
	g.AddEdge(7, 8, 7);
	g.AddEdge(6, 8, 6);
	g.AddEdge(6, 7, 1);
	g.AddEdge(5, 6, 2);
	g.AddEdge(4, 5, 10);
	g.AddEdge(3, 5, 14);
	g.AddEdge(3, 4, 9);
	g.AddEdge(2, 5, 4);
	g.AddEdge(2, 8, 2);
	g.AddEdge(2, 3, 7);
	g.AddEdge(1, 7, 11);
	g.AddEdge(1, 2, 8);
	g.AddEdge(0, 7, 8);
	g.AddEdge(0, 1, 4);
}

void Print(Graph& g)
{
	for (const auto& outerIter : g.m_edgesMap)
	{
		std::cout << outerIter.first << " -> ";
		for (const auto& innerIter : outerIter.second)
		{
			std::cout << "{" << innerIter.first << "," << innerIter.second << "} ";
		}

		std::cout << "\n";
	}
}
