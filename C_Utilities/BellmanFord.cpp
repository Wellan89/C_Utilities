#include "BellmanFord.h"

using namespace std;

template<class Graphe>
void BellmanFord<Graphe>::computeShortestPathsFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Indique le coût du premier noeud
	bf[startNode].totalCost = 0;

	// On sait que la longueur maximale d'un chemin le plus court est de n-1 noeuds
	IndexNoeud nodesCount = g.size();
	IndexNoeud lastTurnIndex = (nodesCount - 1);
	bool madeChange = true;
	for (IndexNoeud i = 0; madeChange; i++)
	{
		madeChange = false;
		bool lastTurn = (i == lastTurnIndex);

		// Parcours toutes les arêtes du graphe
		for (IndexNoeud node = 0; node < nodesCount; node++)
		{
			Cout nodeTotalCost = bf[node].totalCost;
			if (nodeTotalCost == Graphe::INFINITE_COST())
				continue;

			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				IndexNoeud targetNode = it->getTargetIndex();
				Cout newCost = nodeTotalCost + it->getCost();
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
bool BellmanFord<Graphe>::canReachNode(IndexNoeud node) const
{
	return (bf[node].totalCost != Graphe::INFINITE_COST());
}
template<class Graphe>
typename BellmanFord<Graphe>::Cout BellmanFord<Graphe>::getCostTo(IndexNoeud node) const
{
	return bf[node].totalCost;
}
template<class Graphe>
deque<typename BellmanFord<Graphe>::IndexNoeud> BellmanFord<Graphe>::getShortestPathTo(IndexNoeud endNode) const
{
	deque<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename BellmanFord<Graphe>::IndexNoeud> BellmanFord<Graphe>::getReverseShortestPathTo(IndexNoeud endNode) const
{
	vector<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(endNode);
		endNode = bf[endNode].previousNode;
	}
	return l;
}
