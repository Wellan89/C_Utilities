#ifndef DEF_BELLMAN
#define DEF_BELLMAN

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman : pour un graphe sans circuits (i.e. avec un ordre topologique),
// calcule tous les chemins les plus courts depuis un sommmet donn�.
template<class Graphe = Graph<> >
class Bellman
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BmNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BmNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
			{ }
	};

	std::vector<BmNodeInfo> bm;
	const Graphe& g;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		bm.clear();
		bm.resize(g.size());
	}

public:
	Bellman(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode);

	bool canReachNode(IndexNoeud node) const;
	Cout getCostTo(IndexNoeud node) const;
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const;
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const;
};

#endif
