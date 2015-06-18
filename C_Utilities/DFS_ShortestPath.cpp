#include "DFS_ShortestPath.h"

using namespace std;

template<class Graphe>
bool DFS_ShortestPath<Graphe>::computeShortestPath_Rec(IndexNoeud node, Cout currentCost)
{
	// V�rifie que le chemin actuellement parcouru n'est pas plus long que le chemin le plus court d�j� trouv�.
	// On quitte lorsqu'il y a �galit� entre la longueur du chemin actuel et celle du chemin le plus court si un noeud final a d�j� �t� trouv�.
	if (currentCost > pathCost
		|| (currentCost >= pathCost && endNode != Graphe::INVALID_NODE_INDEX()))
		return false;

	// V�rifie que ce noeud est un noeud final
	if (g[node].isFinal())
	{
		reverseShortestPath.clear();
		reverseShortestPath.push_back(node);

		endNode = node;
		pathCost = currentCost;
		return true;
	}

	// Indique que ce noeud est en cours d'exploration
	visitedNodes.insert(node);

	// Parcourt en profondeur tous les fils de ce noeud
	bool pathFound = false;
	const auto& links = g[node].getLinks();
	for (auto it = links.begin(); it != links.end(); ++it)
	{
		IndexNoeud targetNode = it->getTargetIndex();
		
		// V�rifie que ce noeud n'est pas explor� actuellement
		if (visitedNodes.find(targetNode) != visitedNodes.end())
			continue;

		if (computeShortestPath_Rec(targetNode, currentCost + it->getCost()))
		{
			pathFound = true;
			reverseShortestPath.push_back(node);
		}
	}

	// Efface ce noeud de la liste des noeuds en cours d'exploration
	visitedNodes.erase(node);

	return pathFound;
}
template<class Graphe>
void DFS_ShortestPath<Graphe>::computeShortestPathFrom(IndexNoeud startNode, Cout maxCost)
{
	// R�initialise les informations sur les noeuds
	reset();

	// Efface la liste des noeuds en cours d'exploration
	visitedNodes.clear();

	// Indique la profondeur maximale � laquelle on va chercher un noeud final
	pathCost = maxCost;

	// Traite r�cursivement chacun des noeuds pouvant �tre atteints depuis le noeud de d�part
	if (!computeShortestPath_Rec(startNode, 0))
	{
		// R�initialise les informations sur les noeuds si aucun noeud final n'a �t� trouv�
		reset();
	}

	// Efface la liste des noeuds en cours d'exploration
	visitedNodes.clear();
}
template<class Graphe>
bool DFS_ShortestPath<Graphe>::hasFoundPath() const
{
	return (endNode != Graphe::INVALID_NODE_INDEX());
}
template<class Graphe>
typename DFS_ShortestPath<Graphe>::IndexNoeud DFS_ShortestPath<Graphe>::getFinalNode() const
{
	return endNode;
}
template<class Graphe>
typename DFS_ShortestPath<Graphe>::Cout DFS_ShortestPath<Graphe>::getPathCost() const
{
	return pathCost;
}
template<class Graphe>
deque<typename DFS_ShortestPath<Graphe>::IndexNoeud> DFS_ShortestPath<Graphe>::getShortestPath() const
{
	deque<IndexNoeud> l;
	for (auto it = reverseShortestPath.begin(); it != reverseShortestPath.end(); ++it)
		l.push_front(*it);
	return l;
}
template<class Graphe>
const vector<typename DFS_ShortestPath<Graphe>::IndexNoeud>& DFS_ShortestPath<Graphe>::getReverseShortestPath() const
{
	return reverseShortestPath;
}
