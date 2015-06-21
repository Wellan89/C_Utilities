#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"

// Permet de diminuer la mémoire utilisée par cette classe (et permet ainsi à cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se révèlera légèrement plus lent sur des graphes contenant de nombreux liens au coût nul,
// mais il sera légèrement plus rapide sur des graphes sans ce type de lien.
#define DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme de Dijkstra : pour un graphe où toutes les arêtes ont un coût positif,
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
template<class Graphe = Graph<> >
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
#ifndef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		DjNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
#ifndef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::vector<DjNodeInfo> dj;
	const Graphe& g;

#ifdef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
	// Le coût du dernier chemin validé par l'algorithme : celui-ci est toujours croissant,
	// et permet ainsi de déterminer les noeuds déjà visités simplement par leur coût.
	Cout currentCost;
#endif

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		dj.clear();
		dj.resize(g.size());
#ifdef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
		currentCost = 0;
#endif
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
