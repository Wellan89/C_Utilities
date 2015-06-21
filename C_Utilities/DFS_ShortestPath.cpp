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

#ifndef DFS_NO_CYCLE_DETECTION
	// Indique que ce noeud est en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
	visitedNodes.insert(node);
#else
	// Indique aussi que le co�t du chemin actuellement trouv� est currentCost.
	visitedNodes.insert(DFS_NodeInfo(node, currentCost));
#endif
#endif

	// Parcourt en profondeur tous les fils de ce noeud
	bool pathFound = false;
	const auto& links = g[node].getLinks();
	for (auto it = links.begin(); it != links.end(); ++it)
	{
		IndexNoeud targetNode = it->getTargetIndex();
#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		Cout targetCost = currentCost + it->getCost();
#endif
		
#ifndef DFS_NO_CYCLE_DETECTION
		// V�rifie que ce noeud n'est pas explor� actuellement
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		auto targetNodeIt = visitedNodes.find(targetNode);
#else
		// Cette conversion est n�cessaire � cause du mot-cl� explicit dans le constructeur DFS_NodeInfo(IndexNoeud)
		auto targetNodeIt = visitedNodes.find(DFS_NodeInfo(targetNode));
#endif
		if (targetNodeIt != visitedNodes.end())
		{
#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			// S'il l'est, on v�rifie bien que ce chemin pour l'atteindre est plus long que le chemin pr�c�demment trouv�,
			// sinon c'est qu'on est en pr�sence d'un circuit absorbant.
			if (targetCost < targetNodeIt->totalCost)
			{
				absorbCycleFound = true;
				return false;
			}
#endif
			continue;
		}
#endif

#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		Cout targetCost = currentCost + it->getCost();
#endif
		if (computeShortestPath_Rec(targetNode, targetCost))
		{
			pathFound = true;
			reverseShortestPath.push_back(node);
		}

#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		// Si on a d�tect� un circuit absorbant, on quitte imm�diatement
		if (absorbCycleFound)
			return false;
#endif
	}

#ifndef DFS_NO_CYCLE_DETECTION
	// Efface ce noeud de la liste des noeuds en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
	visitedNodes.erase(node);
#else
	visitedNodes.erase(DFS_NodeInfo(node));
#endif
#endif

	return pathFound;
}
template<class Graphe>
void DFS_ShortestPath<Graphe>::computeShortestPathFrom(IndexNoeud startNode, Cout maxCost)
{
	// R�initialise les informations sur les noeuds
	reset();

	// Indique la profondeur maximale � laquelle on va chercher un noeud final
	pathCost = maxCost;

	// Traite r�cursivement chacun des noeuds pouvant �tre atteints depuis le noeud de d�part
	if (!computeShortestPath_Rec(startNode, 0))
	{
		// R�initialise les informations sur les noeuds si aucun noeud final n'a �t� trouv�,
		// en retenant si un circuit absorbant a �t� d�tect�.
		bool absorbCycle = absorbCycleFound;
		reset();
		absorbCycleFound = absorbCycle;
	}
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
