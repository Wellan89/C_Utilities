#include "dijkstra.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe, class Noeud, class Lien>
void Dijkstra<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	dj.clear();
	dj.resize(g.size());

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	cost_priority_queue<unsigned int, unsigned int> nodesToSee;
	dj[startNode].totalCost = 0;
	nodesToSee.push(startNode, dj[startNode].totalCost);

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.top();
		nodesToSee.pop();
		if (dj[node].alreadyVisited)
			continue;

		dj[node].alreadyVisited = true;
		unsigned int nodeTotalCost = dj[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			unsigned int targetNode = it->getTargetIndex();
			if (dj[targetNode].alreadyVisited)
				continue;

			unsigned int newCost = nodeTotalCost + it->getCost();
			if (newCost < dj[targetNode].totalCost)
			{
				dj[targetNode].previousNode = node;
				dj[targetNode].totalCost = newCost;
				nodesToSee.push(targetNode, dj[targetNode].totalCost);
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
template<class Graphe, class Noeud, class Lien>
vector<unsigned int> Dijkstra<Graphe, Noeud, Lien>::getReverseShortestPathTo(unsigned int endNode) const
{
	vector<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_back(endNode);
		endNode = dj[endNode].previousNode;
	}
	return l;
}
