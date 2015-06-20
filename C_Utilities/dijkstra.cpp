#include "Dijkstra.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe>
void Dijkstra<Graphe>::computeShortestPathsFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	cost_priority_queue<IndexNoeud, Cout> nodesToSee;
	dj[startNode].totalCost = 0;
	nodesToSee.push(startNode, dj[startNode].totalCost);

	while (!nodesToSee.empty())
	{
		IndexNoeud node = nodesToSee.top();
		nodesToSee.pop();
		if (dj[node].alreadyVisited)
			continue;

		dj[node].alreadyVisited = true;
		Cout nodeTotalCost = dj[node].totalCost;

		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			Cout linkCost = it->getCost();
			if (linkCost < 0)
			{
				// On a trouvé une arête avec un coût négatif : on quitte ici.
				reset();
				return;
			}

			IndexNoeud targetNode = it->getTargetIndex();
			if (dj[targetNode].alreadyVisited)
				continue;

			Cout newCost = nodeTotalCost + linkCost;
			if (newCost < dj[targetNode].totalCost)
			{
				dj[targetNode].previousNode = node;
				dj[targetNode].totalCost = newCost;
				nodesToSee.push(targetNode, dj[targetNode].totalCost);
			}
		}
	}
}
template<class Graphe>
bool Dijkstra<Graphe>::canReachNode(IndexNoeud node) const
{
	return (dj[node].totalCost != Graphe::INFINITE_COST());
}
template<class Graphe>
typename Dijkstra<Graphe>::Cout Dijkstra<Graphe>::getCostTo(IndexNoeud node) const
{
	return dj[node].totalCost;
}
template<class Graphe>
deque<typename Dijkstra<Graphe>::IndexNoeud> Dijkstra<Graphe>::getShortestPathTo(IndexNoeud endNode) const
{
	deque<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(endNode);
		endNode = dj[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename Dijkstra<Graphe>::IndexNoeud> Dijkstra<Graphe>::getReverseShortestPathTo(IndexNoeud endNode) const
{
	vector<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(endNode);
		endNode = dj[endNode].previousNode;
	}
	return l;
}
