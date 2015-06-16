#include "AStar.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStar<Graphe>::computeShortestPathFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	as[startNode].totalCost = 0;
	cost_priority_queue<unsigned int, unsigned int> nodesToSee;
	nodesToSee.push(startNode, g[startNode].getHeuristic());

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.top();
		if (g[node].isFinal())
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();
		if (as[node].alreadyVisited)
			continue;

		as[node].alreadyVisited = true;
		unsigned int nodeTotalCost = as[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			if (as[targetNode].alreadyVisited)
				continue;

			unsigned int newCost = nodeTotalCost + it->getCost();
			if (newCost < as[targetNode].totalCost)
			{
				as[targetNode].previousNode = node;
				as[targetNode].totalCost = newCost;
				nodesToSee.push(targetNode, newCost + g[targetNode].getHeuristic());
			}
		}
	}
}
template<class Graphe>
bool AStar<Graphe>::hasFoundPath() const
{
	return (endNode != (unsigned int)(-1));
}
template<class Graphe>
unsigned int AStar<Graphe>::getFinalNode() const
{
	return endNode;
}
template<class Graphe>
unsigned int AStar<Graphe>::getPathCost() const
{
	if (endNode != (unsigned int)(-1))
		return as[endNode].totalCost;
	return (unsigned int)(-1);
}
template<class Graphe>
deque<unsigned int> AStar<Graphe>::getShortestPath() const
{
	deque<unsigned int> l;
	unsigned int node = endNode;
	while (node != (unsigned int)(-1))
	{
		l.push_front(node);
		node = as[node].previousNode;
	}
	return l;
}
template<class Graphe>
vector<unsigned int> AStar<Graphe>::getReverseShortestPath() const
{
	vector<unsigned int> l;
	unsigned int node = endNode;
	while (node != (unsigned int)(-1))
	{
		l.push_back(node);
		node = as[node].previousNode;
	}
	return l;
}
