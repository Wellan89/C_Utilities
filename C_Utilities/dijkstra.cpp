#include "dijkstra.h"
#include <queue>

using namespace std;

template<class Graphe, class Noeud, class Lien>
void Dijkstra<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	dj.clear();
	dj.resize(g.size());

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	dj[startNode].totalCost = 0;
	vector<unsigned int> nodesToSee;
	nodesToSee.reserve(g.size());
	nodesToSee.push_back(startNode);

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.back();
		nodesToSee.pop_back();
		unsigned int nodeTotalCost = dj[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			unsigned int newCost = nodeTotalCost + it->getCost();
			if (newCost < dj[targetNode].totalCost)
			{
				dj[targetNode].totalCost = newCost;
				dj[targetNode].previousNode = node;
				nodesToSee.push_back(targetNode);
			}
		}
	}
}
template<class Graphe, class Noeud, class Lien>
bool Dijkstra<Graphe, Noeud, Lien>::canReachNode(unsigned int node) const
{
	return (dj[node].totalCost != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int Dijkstra<Graphe, Noeud, Lien>::getCostTo(unsigned int node) const
{
	return dj[node].totalCost;
}
template<class Graphe, class Noeud, class Lien>
deque<unsigned int> Dijkstra<Graphe, Noeud, Lien>::getShortestPathTo(unsigned int endNode) const
{
	deque<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_front(endNode);
		endNode = dj[endNode].previousNode;
	}
	return l;
}
