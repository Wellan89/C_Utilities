#include "BellmanFord.h"
#include "cost_priority_queue.h"

using namespace std;

template<class Graphe, class Noeud, class Lien>
void BellmanFord<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
	bf[startNode].totalCost = 0;

	// On sait que la longueur maximale d'un chemin le plus court est de n-1 noeuds
	unsigned int nodesCount = g.size();
	//for (unsigned int i = 0; i < nodesCount; i++)
	for (unsigned int i = 0; true; i++)
	{
		// Parcours toutes les arêtes du graphe
		bool lastTurn = (i == nodesCount - 1);
		bool madeChange = false;
		for (unsigned int node = 0; node < nodesCount; node++)
		{
			unsigned int nodeTotalCost = bf[node].totalCost;
			if (nodeTotalCost == (unsigned int)(-1))
				continue;

			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				unsigned int targetNode = it->getTargetIndex();
				unsigned int newCost = nodeTotalCost + it->getCost();
				if (newCost < bf[targetNode].totalCost)
				{
					if (!lastTurn)
					{
						bf[targetNode].totalCost = newCost;
						bf[targetNode].previousNode = node;
						madeChange = true;
					}
					else
					{
						// On a trouvé un circuit absorbant
						reset();
						absorbCycleFound = true;
						return;
					}
				}
			}
		}
		if (!madeChange)
			break;
	}
}
template<class Graphe, class Noeud, class Lien>
bool BellmanFord<Graphe, Noeud, Lien>::absorbCycleDetected() const
{
	return absorbCycleFound;
}
template<class Graphe, class Noeud, class Lien>
bool BellmanFord<Graphe, Noeud, Lien>::canReachNode(unsigned int node) const
{
	return (bf[node].totalCost != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int BellmanFord<Graphe, Noeud, Lien>::getCostTo(unsigned int node) const
{
	return bf[node].totalCost;
}
template<class Graphe, class Noeud, class Lien>
deque<unsigned int> BellmanFord<Graphe, Noeud, Lien>::getShortestPathTo(unsigned int endNode) const
{
	deque<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_front(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
template<class Graphe, class Noeud, class Lien>
vector<unsigned int> BellmanFord<Graphe, Noeud, Lien>::getReverseShortestPathTo(unsigned int endNode) const
{
	vector<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_back(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
