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
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct DjNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
		bool alreadyVisited;

		DjNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST), alreadyVisited(false)
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

	void computeShortestPathsFrom(IndexNoeud startNode);

	bool canReachNode(IndexNoeud node) const;
	Cout getCostTo(IndexNoeud node) const;
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const;
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const;
};

#endif
