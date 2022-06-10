/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <queue>
#include <typeinfo>
#include <memory>

#include "graph.hpp"

namespace ilrd_5678
{

typedef unsigned int uint;

using IIPair = std::pair<uint, uint>;

class Cmp
{
public:
	bool operator()(const IIPair& lhs, const IIPair& rhs)
	{
		return (lhs.second > rhs.second);
	}

};

/******************************************************************************
*								Class Graph	    							  *
*******************************************************************************/
Graph::Graph() : m_edgesMap()
{
	//empty Ctor
}

void Graph::AddEdge(uint srcNode, uint destNode, uint weight)
{
	using namespace std;

	auto destWtPair = make_pair(destNode, weight);
	
	m_edgesMap[srcNode].emplace_back(destWtPair);
}

std::vector<uint> Graph::Dijkstra()
{
	using namespace std;

	vector<uint> totalWeight(m_edgesMap.size() + 1, INT32_MAX);
	vector<bool> edgesVisited(m_edgesMap.size() + 1, false);
	priority_queue<IIPair, vector<IIPair>, Cmp> edgeWtPQ;

	totalWeight[0] = 0;
	edgeWtPQ.push(make_pair(0U, totalWeight[0]));

	while (!edgeWtPQ.empty())
	{
		auto srcPair = edgeWtPQ.top();
		auto srcEdge = srcPair.first;
		edgeWtPQ.pop();

		for (const auto& edge : m_edgesMap[srcEdge])
		{
			if (!edgesVisited[edge.first])
			{
				auto newDist = totalWeight[srcEdge] + edge.second;
				if (newDist < totalWeight[edge.first])
				{
					totalWeight[edge.first] = newDist;
					edgeWtPQ.push(make_pair(edge.first, newDist));
				}
			}
		}

	}
	return totalWeight;
}

/******************************************************************************/


} // namespace ilrd_5678