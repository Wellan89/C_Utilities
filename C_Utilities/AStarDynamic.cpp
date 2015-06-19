#include "AStarDynamic.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStarDynamic<Graphe>::computeShortestPathFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	asd[startNode].totalCost = 0;
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.top();
		if (g.isNodeFinal(node))
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();
		if (asd[node].alreadyVisited)
			continue;

		asd[node].alreadyVisited = true;
#ifdef _DEBUG
		nbExploredNodes++;
#endif
		Cout nodeTotalCost = asd[node].totalCost;

		auto links = g.getNodeLinks(node);
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			IndexNoeud targetNode = it->getTargetIndex();
			if (asd[targetNode].alreadyVisited)
				continue;

			Cout newCost = nodeTotalCost + it->getCost();
			if (newCost < asd[targetNode].totalCost)
			{
				asd[targetNode].previousNode = node;
				asd[targetNode].totalCost = newCost;
				nodesToSee.push(targetNode, newCost + g.getNodeHeuristic(targetNode));
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
