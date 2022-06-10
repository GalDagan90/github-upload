/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __GRAPH_H_RD5678_ILRD__
#define __GRAPH_H_RD5678_ILRD__

#include <iostream>
#include <unordered_map>
#include <vector>

namespace ilrd_5678
{

class Graph
{
public:
	typedef unsigned int uint;
	using AdjNodesList = std::vector<std::pair<uint, uint>>;

	explicit Graph();

	Graph(const Graph& other) = delete;
	Graph& operator=(const Graph& other) = delete;

	void AddEdge(uint srcNode, uint destNode, uint weight);
	void PrintAdjList() const;
	std::vector<uint> Dijkstra();

	std::unordered_map<uint, AdjNodesList> m_edgesMap;
};



} // namespace ilrd_5678


#endif /* __GRAPH_H_RD5678_ILRD__ */
