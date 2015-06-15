#ifndef DEF_BFS_SHORTEST_PATH
#define DEF_BFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <map>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en largeur :
// parcours le graphe en largeur et trouve tous les chemins les plus courts à partir d'un point donné.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class BFS_ShortestPath
{
protected:
	// Informations sur un noeud
	struct BFSNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;

		BFSNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1))
			{ }
	};

	std::vector<BFSNodeInfo> bfs;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bfs.clear();
		bfs.resize(g.size());
	}

public:
	BFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(unsigned int startNode);

	bool canReachNode(unsigned int node) const;
	unsigned int getCostTo(unsigned int node) const;
	std::deque<unsigned int> getShortestPathTo(unsigned int endNode) const;
	std::vector<unsigned int> getReverseShortestPathTo(unsigned int endNode) const;
};

#endif
