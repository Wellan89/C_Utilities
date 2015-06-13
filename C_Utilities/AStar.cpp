#include "AStar.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe, class Noeud, class Lien>
void AStar<Graphe, Noeud, Lien>::computeShortestPathFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	as[startNode].totalCost = 0;
	as[startNode].totalEstimatedCost = g[startNode].getHeuristic();
	cost_priority_queue<unsigned int, unsigned int> nodesToSee;
	nodesToSee.push(startNode, as[startNode].totalEstimatedCost);

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
				as[targetNode].totalEstimatedCost = newCost + g[targetNode].getHeuristic();
				nodesToSee.push(targetNode, as[targetNode].totalEstimatedCost);
			}
		}
	}
}
template<class Graphe, class Noeud, class Lien>
bool AStar<Graphe, Noeud, Lien>::hasFoundPath() const
{
	return (endNode != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int AStar<Graphe, Noeud, Lien>::getFinalNode() const
{
	return endNode;
}
template<class Graphe, class Noeud, class Lien>
unsigned int AStar<Graphe, Noeud, Lien>::getPathCost() const
{
	if (endNode != (unsigned int)(-1))
		return as[endNode].totalCost;
	return (unsigned int)(-1);
}
template<class Graphe, class Noeud, class Lien>
deque<unsigned int> AStar<Graphe, Noeud, Lien>::getShortestPath() const
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
template<class Graphe, class Noeud, class Lien>
vector<unsigned int> AStar<Graphe, Noeud, Lien>::getReverseShortestPath() const
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
