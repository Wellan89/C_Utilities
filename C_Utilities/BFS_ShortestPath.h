#ifndef DEF_BFS_SHORTEST_PATH
#define DEF_BFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <map>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en largeur : pour un graphe o� toutes les ar�tes ont un co�t positif,
// parcours le graphe en largeur et trouve tous les chemins les plus courts � partir d'un point donn�.
template<class Graphe = Graph<> >
class BFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	// Informations sur un noeud
	struct BFSNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BFSNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
			{ }
	};

	std::vector<BFSNodeInfo> bfs;
	const Graphe& g;

	// Indique si un lien au co�t n�gatif a �t� d�tect� dans le graphe
	bool negativeLinkFound;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		bfs.clear();
		bfs.resize(g.size());
		negativeLinkFound = false;
	}

public:
	BFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode);

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool canReachNode(IndexNoeud node) const;
	Cout getCostTo(IndexNoeud node) const;
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const;
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const;
};

#endif
