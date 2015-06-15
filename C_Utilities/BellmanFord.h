#ifndef DEF_BELLMAN_FORD
#define DEF_BELLMAN_FORD

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman-Ford :
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
// Détecte aussi la présence de circuits absorbants dans le graphe.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class BellmanFord
{
protected:
	struct BfNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;

		BfNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1))
			{ }
	};

	std::vector<BfNodeInfo> bf;
	const Graphe& g;

	// Indique si un circuit absorbant a été détecté dans le graphe
	bool absorbCycleFound;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bf.clear();
		bf.resize(g.size());
		absorbCycleFound = false;
	}

public:
	BellmanFord(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(unsigned int startNode);

	bool absorbCycleDetected() const;
	bool canReachNode(unsigned int node) const;
	unsigned int getCostTo(unsigned int node) const;
	std::deque<unsigned int> getShortestPathTo(unsigned int endNode) const;
	std::vector<unsigned int> getReverseShortestPathTo(unsigned int endNode) const;
};

#endif
