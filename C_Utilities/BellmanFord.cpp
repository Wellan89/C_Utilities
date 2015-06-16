#include "BellmanFord.h"

using namespace std;

template<class Graphe>
void BellmanFord<Graphe>::computeShortestPathsFrom(unsigned int startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud
	bf[startNode].totalCost = 0;

	// On sait que la longueur maximale d'un chemin le plus court est de n-1 noeuds
	unsigned int nodesCount = g.size();
	unsigned int lastTurnIndex = (nodesCount - 1);
	bool madeChange = true;
	for (unsigned int i = 0; madeChange; i++)
	{
		madeChange = false;
		bool lastTurn = (i == lastTurnIndex);

		// Parcours toutes les arêtes du graphe
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
	}
}
template<class Graphe>
bool BellmanFord<Graphe>::absorbCycleDetected() const
{
	return absorbCycleFound;
}
template<class Graphe>
bool BellmanFord<Graphe>::canReachNode(unsigned int node) const
{
	return (bf[node].totalCost != (unsigned int)(-1));
}
template<class Graphe>
unsigned int BellmanFord<Graphe>::getCostTo(unsigned int node) const
{
	return bf[node].totalCost;
}
template<class Graphe>
deque<unsigned int> BellmanFord<Graphe>::getShortestPathTo(unsigned int endNode) const
{
	deque<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_front(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<unsigned int> BellmanFord<Graphe>::getReverseShortestPathTo(unsigned int endNode) const
{
	vector<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_back(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
