#include "DFS_ShortestPath.h"

using namespace std;

template<class Graphe>
bool DFS_ShortestPath<Graphe>::computeShortestPath_Rec(IndexNoeud node, Cout currentCost)
{
	// Vérifie que le chemin actuellement parcouru n'est pas plus long que le chemin le plus court déjà trouvé.
	// On quitte lorsqu'il y a égalité entre la longueur du chemin actuel et celle du chemin le plus court si un noeud final a déjà été trouvé.
	if (currentCost > pathCost
		|| (currentCost >= pathCost && endNode != Graphe::INVALID_NODE_INDEX()))
		return false;

	// Vérifie que ce noeud est un noeud final
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
	// Indique aussi que le coût du chemin actuellement trouvé est currentCost.
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
		// Vérifie que ce noeud n'est pas exploré actuellement
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		auto targetNodeIt = visitedNodes.find(targetNode);
#else
		// Cette conversion est nécessaire à cause du mot-clé explicit dans le constructeur DFS_NodeInfo(IndexNoeud)
		auto targetNodeIt = visitedNodes.find(DFS_NodeInfo(targetNode));
#endif
		if (targetNodeIt != visitedNodes.end())
		{
#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			// S'il l'est, on vérifie bien que ce chemin pour l'atteindre est plus long que le chemin précédemment trouvé,
			// sinon c'est qu'on est en présence d'un circuit absorbant.
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
		// Si on a détecté un circuit absorbant, on quitte immédiatement
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
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique la profondeur maximale à laquelle on va chercher un noeud final
	pathCost = maxCost;

	// Traite récursivement chacun des noeuds pouvant être atteints depuis le noeud de départ
	if (!computeShortestPath_Rec(startNode, 0))
	{
		// Réinitialise les informations sur les noeuds si aucun noeud final n'a été trouvé,
		// en retenant si un circuit absorbant a été détecté.
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
