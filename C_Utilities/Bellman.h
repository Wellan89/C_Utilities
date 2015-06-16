#ifndef DEF_BELLMAN
#define DEF_BELLMAN

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman : pour un graphe sans circuits (i.e. avec un ordre topologique),
// calcule tous les chemins les plus courts depuis un sommmet donné.
template<class Graphe = Graph>
class Bellman
{
protected:
	struct BmNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;

		BmNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1))
			{ }
	};

	std::vector<BmNodeInfo> bm;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bm.clear();
		bm.resize(g.size());
	}

public:
	Bellman(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(unsigned int startNode);

	bool canReachNode(unsigned int node) const;
	unsigned int getCostTo(unsigned int node) const;
	std::deque<unsigned int> getShortestPathTo(unsigned int endNode) const;
	std::vector<unsigned int> getReverseShortestPathTo(unsigned int endNode) const;
};

#endif
