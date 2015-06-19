#include "AStar.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void AStar<Graphe>::computeShortestPathFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	as[startNode].totalCost = 0;
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	nodesToSee.push(startNode, g[startNode].getHeuristic());

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.top();
		if (g[node].isFinal())
		{
			endNode = node;
			return;
		}
		nodesToSee.pop();
		if (as[node].alreadyVisited)
			continue;

		as[node].alreadyVisited = true;
#ifdef _DEBUG
		nbExploredNodes++;
#endif
		Cout nodeTotalCost = as[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			IndexNoeud targetNode = it->getTargetIndex();
			if (as[targetNode].alreadyVisited)
				continue;

			Cout linkCost = it->getCost();
			if (linkCost < 0)
			{
				// On a trouvé une arête avec un coût négatif : on quitte ici.
				reset();
				return;
			}

			Cout newCost = nodeTotalCost + linkCost;
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
