#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'� une profondeur maximale donn�e le cas �ch�ant),
// et s'arr�te d�s qu'un point final est trouv�.
template<class Graphe = Graph<> >
class DFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	// L'ensemble des noeuds actuellement visit�s par r�cursivit�
	std::set<IndexNoeud> visitedNodes;

	// Le chemin actuellement trouv� (dans l'ordre inverse)
	std::vector<IndexNoeud> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

	// Le co�t pour acc�der � ce noeud depuis le noeud de d�part
	Cout pathCost;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		reverseShortestPath.clear();
		endNode = Graphe::INVALID_NODE_INDEX();
		pathCost = Graphe::INFINITE_COST();
	}

	bool computeShortestPath_Rec(IndexNoeud node, Cout currentCost);

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode, Cout maxCost = Graphe::INFINITE_COST());

	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost() const;
	std::deque<IndexNoeud> getShortestPath() const;
	const std::vector<IndexNoeud>& getReverseShortestPath() const;
};

#endif
