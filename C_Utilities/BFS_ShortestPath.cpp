#include "BFS_ShortestPath.h"
#include <deque>

using namespace std;

template<class Graphe, class Noeud, class Lien>
void BFS_ShortestPath<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	deque<unsigned int> nodesToSee;
	bfs[startNode].totalCost = 0;
	nodesToSee.push_back(startNode);

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.front();
		nodesToSee.pop_front();

		unsigned int nodeTotalCost = bfs[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			unsigned int newCost = nodeTotalCost + it->getCost();
			if (newCost < bfs[targetNode].totalCost)
			{
				bfs[targetNode].previousNode = node;
				bfs[targetNode].totalCost = newCost;
				nodesToSee.push_back(targetNode);
			}
		}
	}
}
template<class Graphe, class Noeud, class Lien>
bool BFS_ShortestPath<Graphe, Noeud, Lien>::canReachNode(unsigned int node) const
{
	return (bfs[node].totalCost != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int BFS_ShortestPath<Graphe, Noeud, Lien>::getCostTo(unsigned int node) const
{
	return bfs[node].totalCost;
}
template<class Graphe, class Noeud, class Lien>
deque<unsigned int> BFS_ShortestPath<Graphe, Noeud, Lien>::getShortestPathTo(unsigned int endNode) const
{
	deque<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_front(endNode);
		endNode = bfs[endNode].previousNode;
	}
	return l;
}
template<class Graphe, class Noeud, class Lien>
vector<unsigned int> BFS_ShortestPath<Graphe, Noeud, Lien>::getReverseShortestPathTo(unsigned int endNode) const
{
	vector<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_back(endNode);
		endNode = bfs[endNode].previousNode;
	}
	return l;
}
