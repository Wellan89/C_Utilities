#include <utility>
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

	// Si l'ordre topologique n'a pas pu être construit, on quitte ici.
	if (startIt == orderedNodes.rend())
	{
		cycleFound = true;
		return;
	}

	// Génère l'ensemble des liens inverse de ce graphe :
	// Pour chaque noeud, l'ensemble des noeuds ayant un lien vers celui-ci et le coût de ce lien.
	vector<vector<pair<IndexNoeud, Cout> > > incomingLinks(g.size());
	IndexNoeud nodesCount = g.size();
	for (IndexNoeud i = 0; i < nodesCount; i++)
	{
		const auto& links = g[i].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
			incomingLinks[it->getTargetIndex()].push_back(pair<IndexNoeud, Cout>(i, it->getCost()));
	}

	// Indique le coût du premier noeud
	bm[startNode].totalCost = 0;

	// Attention : on parcours le vecteur en sens inverse !
	for (auto nodeIt = startIt + 1; nodeIt != orderedNodes.rend(); ++nodeIt)
	{
		IndexNoeud node = (*nodeIt);
		const auto& incLinks = incomingLinks[node];
		for (auto incLinkIt = incLinks.begin(); incLinkIt != incLinks.end(); ++incLinkIt)
		{
			IndexNoeud prevNode = incLinkIt->first;
			if (bm[prevNode].totalCost == Graphe::INFINITE_COST())
				continue;

			Cout newCost = bm[prevNode].totalCost + incLinkIt->second;
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
