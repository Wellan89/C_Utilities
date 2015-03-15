#include "dijkstra.h"
#include <queue>

using namespace std;

// Classe utilisée par la liste de priorité des noeuds :
// Détermine si un noeud est (strictement) moins prioritaire qu'un autre
// en regardant si son coût total actuel est supérieur.
template<class DjNodeInfo>
class DjNodeComparator
{
protected:
	const std::vector<DjNodeInfo>& dj;

public:
	DjNodeComparator(const std::vector<DjNodeInfo>& djInfos)
		: dj(djInfos) { }
	bool operator()(unsigned int node1, unsigned int node2)
	{
		return (dj[node1].totalCost > dj[node2].totalCost);
	}
};

template<class Graphe, class Noeud, class Lien>
void Dijkstra<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	dj.clear();
	dj.resize(g.size());

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	dj[startNode].totalCost = 0;
	DjNodeComparator<DjNodeInfo> cmp(dj);
	priority_queue<unsigned int, vector<unsigned int>, DjNodeComparator<DjNodeInfo> > nodesToSee(cmp);
	nodesToSee.push(startNode);

	while (!nodesToSee.empty())
	{
		unsigned int node = nodesToSee.top();
		nodesToSee.pop();
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
				nodesToSee.push(targetNode);
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
