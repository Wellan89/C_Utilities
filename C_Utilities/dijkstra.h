#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"

// Permet de diminuer la m�moire utilis�e par cette classe (et permet ainsi � cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se r�v�lera l�g�rement plus lent sur des graphes contenant de nombreux liens au co�t nul,
// mais il sera l�g�rement plus rapide sur des graphes sans ce type de lien.
#define DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme de Dijkstra : pour un graphe o� toutes les ar�tes ont un co�t positif,
// parcours tout le graphe depuis un point de d�part et d�termine tous les chemins les plus courts depuis ce point.
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
	// Le co�t du dernier chemin valid� par l'algorithme : celui-ci est toujours croissant,
	// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur co�t.
	Cout currentCost;
#endif

	// R�initialise les informations sur les noeuds
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
