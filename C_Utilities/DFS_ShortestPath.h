#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'à une profondeur maximale donnée le cas échéant),
// et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class DFS_ShortestPath
{
protected:
	// L'ensemble des noeuds actuellement visités par récursivité
	std::set<unsigned int> visitedNodes;

	// Le chemin actuellement trouvé (dans l'ordre inverse)
	std::vector<unsigned int> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	unsigned int endNode;

	// Le coût pour accéder à ce noeud depuis le noeud de départ
	unsigned int pathCost;

	// Réinitialise les informations sur les noeuds
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
