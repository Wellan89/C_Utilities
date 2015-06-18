#ifndef DEF_BELLMAN_FORD_YEN
#define DEF_BELLMAN_FORD_YEN

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman-Ford (avec l'amélioration de Yen) :
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
// Détecte aussi la présence de circuits absorbants dans le graphe.
template<class Graphe = Graph>
class BellmanFordYen
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BfyNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BfyNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
			{ }
	};

	std::vector<BfyNodeInfo> bfy;
	const Graphe& g;

	// Indique si un circuit absorbant a été détecté dans le graphe
	bool absorbCycleFound;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bfy.clear();
		bfy.resize(g.size());
		absorbCycleFound = false;
	}

public:
	BellmanFordYen(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode);

	bool absorbCycleDetected() const;
	bool canReachNode(IndexNoeud node) const;
	Cout getCostTo(IndexNoeud node) const;
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const;
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const;
};

#endif
