#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Dijkstra : parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class Dijkstra
{
protected:
	struct DjNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;
		bool alreadyVisited;

		DjNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1)), alreadyVisited(false)
			{ }
	};

	const Graphe& g;
	std::vector<DjNodeInfo> dj;

public:
	Dijkstra(const Graphe& gr) : g(gr) { }

	void computeShortestPathsFrom(unsigned int startNode);

	bool canReachNode(unsigned int node) const;
	unsigned int getCostTo(unsigned int node) const;
	std::deque<unsigned int> getShortestPathTo(unsigned int endNode) const;
};

#endif
