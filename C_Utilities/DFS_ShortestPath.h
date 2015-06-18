#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'à une profondeur maximale donnée le cas échéant),
// et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph<> >
class DFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	// L'ensemble des noeuds actuellement visités par récursivité
	std::set<IndexNoeud> visitedNodes;

	// Le chemin actuellement trouvé (dans l'ordre inverse)
	std::vector<IndexNoeud> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

	// Le coût pour accéder à ce noeud depuis le noeud de départ
	Cout pathCost;

	// Réinitialise les informations sur les noeuds
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
