#include "AStarDynamic.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStarDynamic<Graphe>::computeShortestPathFrom(IndexNoeud startNode)
{
	// R�initialise les informations sur les noeuds
	reset();

	// Indique le co�t du premier noeud et l'ajoute � la liste des noeuds � parcourir
	asd[startNode].totalCost = 0;
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
	// La priorit� du dernier noeud valid� par l'algorithme : celle-ci est toujours croissante,
	// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur priorit�.
	Cout currentPriority = 0;
#endif

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.top();
#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
		Cout nodePriority = nodesToSee.top_cost();
#endif
		if (g.isNodeFinal(node))
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();

#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
		if (asd[node].alreadyVisited)
			continue;
		asd[node].alreadyVisited = true;
#else
		if (nodePriority < currentPriority)
			continue;
		currentPriority = nodePriority;
#endif

#ifdef _DEBUG
		nbExploredNodes++;
#endif
		Cout nodeTotalCost = asd[node].totalCost;

		IndexNoeud linksCount = g.getNodeLinksCount(node);
		for (IndexNoeud i = 0; i < linksCount; i++)
		{
			Graphe::Lien l = g.getNodeLink(node, i);
			Cout linkCost = l.getCost();
			if (linkCost < 0)
			{
				// On a trouv� une ar�te avec un co�t n�gatif : on quitte ici.
				reset();
				negativeLinkFound = true;
				return;
			}

			IndexNoeud targetNode = l.getTargetIndex();
			Cout newCost = nodeTotalCost + linkCost;
			if (newCost < asd[targetNode].totalCost)
			{
				asd[targetNode].previousNode = node;
				asd[targetNode].totalCost = newCost;

				Cout targetPriority = newCost + g.getNodeHeuristic(targetNode);
#ifdef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
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
bool AStarDynamic<Graphe>::hasFoundPath() const
{
	return (endNode != Graphe::INVALID_NODE_INDEX());
}
template<class Graphe>
typename AStarDynamic<Graphe>::IndexNoeud AStarDynamic<Graphe>::getFinalNode() const
{
	return endNode;
}
template<class Graphe>
typename AStarDynamic<Graphe>::Cout AStarDynamic<Graphe>::getPathCost()
{
	if (hasFoundPath())
		return asd[endNode].totalCost;
	return Graphe::INFINITE_COST();
}
template<class Graphe>
deque<typename AStarDynamic<Graphe>::IndexNoeud> AStarDynamic<Graphe>::getShortestPath()
{
	deque<IndexNoeud> l;
	IndexNoeud node = endNode;
	while (node != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(node);
		node = asd[node].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename AStarDynamic<Graphe>::IndexNoeud> AStarDynamic<Graphe>::getReverseShortestPath()
{
	vector<IndexNoeud> l;
	IndexNoeud node = endNode;
	while (node != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(node);
		node = asd[node].previousNode;
	}
	return l;
}
