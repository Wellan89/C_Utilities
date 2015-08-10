#include "AStar.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStar<Graphe>::computeShortestPathFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	as[startNode].totalCost = 0;
	nodesToSee.push(startNode, g[startNode].getHeuristic());

#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
	// La priorité du dernier noeud validé par l'algorithme : celle-ci est toujours croissante,
	// et permet ainsi de déterminer les noeuds déjà visités simplement par leur priorité.
	Cout currentPriority = 0;
#endif

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.top();
#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		Cout nodePriority = nodesToSee.top_cost();
#endif
		if (g[node].isFinal())
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();

#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		if (as[node].alreadyVisited)
			continue;
		as[node].alreadyVisited = true;
#else
		if (nodePriority < currentPriority)
			continue;
		currentPriority = nodePriority;
#endif

#ifdef _DEBUG
		nbExploredNodes++;
#endif
		Cout nodeTotalCost = as[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			Cout linkCost = it->getCost();
			if (linkCost < 0)
			{
				// On a trouvé une arête avec un coût négatif : on quitte ici.
				reset();
				negativeLinkFound = true;
				return;
			}

			IndexNoeud targetNode = it->getTargetIndex();
			Cout newCost = nodeTotalCost + linkCost;
			if (newCost < as[targetNode].totalCost)
			{
				as[targetNode].previousNode = node;
				as[targetNode].totalCost = newCost;

				Cout targetPriority = newCost + g[targetNode].getHeuristic();
#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
				// On vérifie que ce noeud aura bien une priorité supérieure à la priorité du noeud actuel,
				// afin qu'il ne soit pas considéré comme déjà vu lorsqu'il sera retiré de la file de priorité.
				if (targetPriority < currentPriority)
					targetPriority = currentPriority;
#endif

				nodesToSee.push(targetNode, targetPriority);
			}
		}
	}
}
template<class Graphe>
bool AStar<Graphe>::hasFoundPath() const
{
	return (endNode != Graphe::INVALID_NODE_INDEX());
}
template<class Graphe>
typename AStar<Graphe>::IndexNoeud AStar<Graphe>::getFinalNode() const
{
	return endNode;
}
template<class Graphe>
typename AStar<Graphe>::Cout AStar<Graphe>::getPathCost() const
{
	if (hasFoundPath())
		return as[endNode].totalCost;
	return Graphe::INFINITE_COST();
}
template<class Graphe>
deque<typename AStar<Graphe>::IndexNoeud> AStar<Graphe>::getShortestPath() const
{
	deque<typename AStar<Graphe>::IndexNoeud> l;
	typename AStar<Graphe>::IndexNoeud node = endNode;
	while (node != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(node);
		node = as[node].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename AStar<Graphe>::IndexNoeud> AStar<Graphe>::getReverseShortestPath() const
{
	vector<typename AStar<Graphe>::IndexNoeud> l;
	typename AStar<Graphe>::IndexNoeud node = endNode;
	while (node != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(node);
		node = as[node].previousNode;
	}
	return l;
}
