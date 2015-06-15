#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'� une profondeur maximale donn�e le cas �ch�ant),
// et s'arr�te d�s qu'un point final est trouv�.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class DFS_ShortestPath
{
protected:
	// L'ensemble des noeuds actuellement visit�s par r�cursivit�
	std::set<unsigned int> visitedNodes;

	// Le chemin actuellement trouv� (dans l'ordre inverse)
	std::vector<unsigned int> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	unsigned int endNode;

	// Le co�t pour acc�der � ce noeud depuis le noeud de d�part
	unsigned int pathCost;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		reverseShortestPath.clear();
		endNode = (unsigned int)(-1);
		pathCost = (unsigned int)(-1);
	}

	bool computeShortestPath_Rec(unsigned int node, unsigned int currentCost);

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(unsigned int startNode, unsigned int maxCost = (unsigned int)(-1));

	bool hasFoundPath() const;
	unsigned int getFinalNode() const;
	unsigned int getPathCost() const;
	std::deque<unsigned int> getShortestPath() const;
	const std::vector<unsigned int>& getReverseShortestPath() const;
};

#endif
