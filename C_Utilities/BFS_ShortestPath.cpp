#include "BFS_ShortestPath.h"
#include <deque>

using namespace std;

template<class Graphe>
void BFS_ShortestPath<Graphe>::computeShortestPathsFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	deque<IndexNoeud> nodesToSee;
	bfs[startNode].totalCost = 0;
	nodesToSee.push_back(startNode);

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.front();
		nodesToSee.pop_front();

		Cout nodeTotalCost = bfs[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			IndexNoeud targetNode = it->getTargetIndex();
			Cout newCost = nodeTotalCost + it->getCost();
			if (newCost < bfs[targetNode].totalCost)
			{
				bfs[targetNode].previousNode = node;
				bfs[targetNode].totalCost = newCost;
				nodesToSee.push_back(targetNode);
			}
		}
	}
}
template<class Graphe>
bool BFS_ShortestPath<Graphe>::canReachNode(IndexNoeud node) const
{
	return (bfs[node].totalCost != Graphe::INFINITE_COST);
}
template<class Graphe>
typename BFS_ShortestPath<Graphe>::Cout BFS_ShortestPath<Graphe>::getCostTo(IndexNoeud node) const
{
	return bfs[node].totalCost;
}
template<class Graphe>
deque<typename BFS_ShortestPath<Graphe>::IndexNoeud> BFS_ShortestPath<Graphe>::getShortestPathTo(IndexNoeud endNode) const
{
	deque<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX)
	{
		l.push_front(endNode);
		endNode = bfs[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename BFS_ShortestPath<Graphe>::IndexNoeud> BFS_ShortestPath<Graphe>::getReverseShortestPathTo(IndexNoeud endNode) const
{
	vector<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX)
	{
		l.push_back(endNode);
		endNode = bfs[endNode].previousNode;
	}
	return l;
}
