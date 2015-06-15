#ifndef DEF_FLOYD_WARSHALL
#define DEF_FLOYD_WARSHALL

//#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Floyd-Warshall : détermine toutes les plus courtes distances entre toutes les paires de points.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class FloydWarshall
{
protected:
	struct FwPathInfo
	{
		unsigned int totalCost;

		FwPathInfo() : totalCost((unsigned int)(-1))
			{ }
	};

	std::vector<std::vector<FwPathInfo> > fw;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		fw.clear();
		unsigned int nodesCount = g.size();
		fw.resize(nodesCount);
		for (unsigned int i = 0; i < nodesCount; i++)
			fw[i].resize(nodesCount);
	}

public:
	FloydWarshall(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPaths();

	bool pathExists(unsigned int startNode, unsigned int endNode) const;
	unsigned int getPathCost(unsigned int startNode, unsigned int endNode) const;
	//std::deque<unsigned int> getShortestPath(unsigned int startNode, unsigned int endNode) const;
	//std::vector<unsigned int> getReverseShortestPath(unsigned int startNode, unsigned int endNode) const;
};

#endif
