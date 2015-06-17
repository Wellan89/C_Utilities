#include "AStarDynamic.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStarDynamic<Graphe>::computeShortestPathFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	asd[startNode].totalCost = 0;
	cost_priority_queue<unsigned int, unsigned int> nodesToSee;
	nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.top();
		if (g.isNodeFinal(node))
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();
		if (asd[node].alreadyVisited)
			continue;

		asd[node].alreadyVisited = true;
		unsigned int nodeTotalCost = asd[node].totalCost;

		auto links = g.getNodeLinks(node);
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			if (asd[targetNode].alreadyVisited)
				continue;

			unsigned int newCost = nodeTotalCost + it->getCost();
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
	return (endNode != (unsigned int)(-1));
}
template<class Graphe>
unsigned int AStarDynamic<Graphe>::getFinalNode() const
{
	return endNode;
}
template<class Graphe>
unsigned int AStarDynamic<Graphe>::getPathCost()
{
	if (endNode != (unsigned int)(-1))
		return asd[endNode].totalCost;
	return (unsigned int)(-1);
}
template<class Graphe>
deque<unsigned int> AStarDynamic<Graphe>::getShortestPath()
{
	deque<unsigned int> l;
	unsigned int node = endNode;
	while (node != (unsigned int)(-1))
	{
		l.push_front(node);
		node = asd[node].previousNode;
	}
	return l;
}
template<class Graphe>
vector<unsigned int> AStarDynamic<Graphe>::getReverseShortestPath()
{
	vector<unsigned int> l;
	unsigned int node = endNode;
	while (node != (unsigned int)(-1))
	{
		l.push_back(node);
		node = asd[node].previousNode;
	}
	return l;
}
