#include "Bellman.h"

using namespace std;

template<class Graphe>
void Bellman<Graphe>::computeShortestPathsFrom(IndexNoeud startNode)
{
	// Réinitialise les informations sur les noeuds
	reset();

	// Obtient les noeuds du graphe suivant un ordre topologique inverse
	auto orderedNodes = g.getReverseTopologicalyOrderedNodes();

	// Démarre à partir du noeud spécifié (on ignore les noeuds ne pouvant être atteints depuis celui-ci)
	auto startIt = find(orderedNodes.rbegin(), orderedNodes.rend(), startNode);
	if (startIt == orderedNodes.rend())
		return;
	bm[startNode].totalCost = 0;

	// Attention : on parcours le vecteur en sens inverse !
	for (auto nodeIt = startIt + 1; nodeIt != orderedNodes.rend(); ++nodeIt)
	{
		IndexNoeud node = (*nodeIt);
		const auto& incomingLinks = g[node].getIncomingLinks();
		for (auto incLinkIt = incomingLinks.begin(); incLinkIt != incomingLinks.end(); ++incLinkIt)
		{
			IndexNoeud prevNode = incLinkIt->getFromIndex();
			if (bm[prevNode].totalCost == Graphe::INFINITE_COST())
				continue;

			Cout newCost = bm[prevNode].totalCost + incLinkIt->getCost();
			if (newCost < bm[node].totalCost)
			{
				bm[node].totalCost = newCost;
				bm[node].previousNode = prevNode;
			}
		}
	}
}
template<class Graphe>
bool Bellman<Graphe>::canReachNode(IndexNoeud node) const
{
	return (bm[node].totalCost != Graphe::INFINITE_COST());
}
template<class Graphe>
typename Bellman<Graphe>::Cout Bellman<Graphe>::getCostTo(IndexNoeud node) const
{
	return bm[node].totalCost;
}
template<class Graphe>
deque<typename Bellman<Graphe>::IndexNoeud> Bellman<Graphe>::getShortestPathTo(IndexNoeud endNode) const
{
	deque<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(endNode);
		endNode = bm[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename Bellman<Graphe>::IndexNoeud> Bellman<Graphe>::getReverseShortestPathTo(IndexNoeud endNode) const
{
	vector<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(endNode);
		endNode = bm[endNode].previousNode;
	}
	return l;
}
