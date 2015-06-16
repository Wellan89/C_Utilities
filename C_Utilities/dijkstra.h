#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Dijkstra : pour un graphe où toutes les arêtes ont un coût positif,
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
template<class Graphe = Graph>
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

	std::vector<DjNodeInfo> dj;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		dj.clear();
		dj.resize(g.size());
	}

public:
	Dijkstra(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(unsigned int startNode);

	bool canReachNode(unsigned int node) const;
	unsigned int getCostTo(unsigned int node) const;
	std::deque<unsigned int> getShortestPathTo(unsigned int endNode) const;
	std::vector<unsigned int> getReverseShortestPathTo(unsigned int endNode) const;
};

#endif
