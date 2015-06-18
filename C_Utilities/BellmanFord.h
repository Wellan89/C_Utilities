#ifndef DEF_BELLMAN_FORD
#define DEF_BELLMAN_FORD

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman-Ford :
// parcours tout le graphe depuis un point de d�part et d�termine tous les chemins les plus courts depuis ce point.
// D�tecte aussi la pr�sence de circuits absorbants dans le graphe.
template<class Graphe = Graph>
class BellmanFord
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BfNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BfNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
			{ }
	};

	std::vector<BfNodeInfo> bf;
	const Graphe& g;

	// Indique si un circuit absorbant a �t� d�tect� dans le graphe
	bool absorbCycleFound;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		bf.clear();
		bf.resize(g.size());
		absorbCycleFound = false;
	}

public:
	BellmanFord(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode);

	bool absorbCycleDetected() const;
	bool canReachNode(IndexNoeud node) const;
	Cout getCostTo(IndexNoeud node) const;
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const;
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const;
};

#endif
