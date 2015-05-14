#include "AStar.h"
#include <functional>
#include <queue>

using namespace std;

// Classe utilisée par la liste de priorité des noeuds :
// Détermine si un noeud est (strictement) moins prioritaire qu'un autre
// en regardant si son coût total estimé est supérieur.
template<class ASNodeInfo>
class AStarNodeComparator
{
protected:
	const vector<ASNodeInfo>& as;

public:
	AStarNodeComparator(const vector<ASNodeInfo>& asInfos) : as(asInfos) { }
	bool operator()(unsigned int node1, unsigned int node2)
	{
		return (as[node1].totalEstimatedCost > as[node2].totalEstimatedCost);
	}
};

template<class Graphe, class Noeud, class Lien>
void AStar<Graphe, Noeud, Lien>::computeShortestPathFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	as.clear();
	as.resize(g.size());
	endNode = (unsigned int)(-1);

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	as[startNode].totalCost = 0;
	as[startNode].totalEstimatedCost = g[startNode].getHeuristic();
	AStarNodeComparator<ASNodeInfo> cmp(as);
	priority_queue<unsigned int, vector<unsigned int>, AStarNodeComparator<ASNodeInfo> > nodesToSee(cmp);
	nodesToSee.push(startNode);

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
				as[targetNode].totalCost = newCost;
				as[targetNode].totalEstimatedCost = newCost + g[targetNode].getHeuristic();
				as[targetNode].previousNode = node;
				nodesToSee.push(targetNode);
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
