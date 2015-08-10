#include "AStar.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStar<Graphe>::computeShortestPathFrom(IndexNoeud startNode)
{
	// R�initialise les informations sur les noeuds
	reset();

	// Indique le co�t du premier noeud et l'ajoute � la liste des noeuds � parcourir
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	as[startNode].totalCost = 0;
	nodesToSee.push(startNode, g[startNode].getHeuristic());

#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
	// La priorit� du dernier noeud valid� par l'algorithme : celle-ci est toujours croissante,
	// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur priorit�.
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
				// On a trouv� une ar�te avec un co�t n�gatif : on quitte ici.
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
				// On v�rifie que ce noeud aura bien une priorit� sup�rieure � la priorit� du noeud actuel,
				// afin qu'il ne soit pas consid�r� comme d�j� vu lorsqu'il sera retir� de la file de priorit�.
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
