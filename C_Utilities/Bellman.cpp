#include "Bellman.h"

using namespace std;

using namespace std;

template<class Graphe, class Noeud, class Lien>
void Bellman<Graphe, Noeud, Lien>::computeShortestPathsFrom(unsigned int startNode)
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
		unsigned int node = (*nodeIt);
		const auto& incomingLinks = g[node].getIncomingLinks();
		for (auto incLinkIt = incomingLinks.begin(); incLinkIt != incomingLinks.end(); ++incLinkIt)
		{
			unsigned int prevNode = incLinkIt->getFromIndex();
			if (bm[prevNode].totalCost == (unsigned int)(-1))
				continue;

			unsigned int newCost = bm[prevNode].totalCost + incLinkIt->getCost();
			if (newCost < bm[node].totalCost)
			{
				bm[node].totalCost = newCost;
				bm[node].previousNode = prevNode;
			}
		}
	}
}
template<class Graphe, class Noeud, class Lien>
bool Bellman<Graphe, Noeud, Lien>::canReachNode(unsigned int node) const
{
	return (bm[node].totalCost != (unsigned int)(-1));
}
template<class Graphe, class Noeud, class Lien>
unsigned int Bellman<Graphe, Noeud, Lien>::getCostTo(unsigned int node) const
{
	return bm[node].totalCost;
}
template<class Graphe, class Noeud, class Lien>
deque<unsigned int> Bellman<Graphe, Noeud, Lien>::getShortestPathTo(unsigned int endNode) const
{
	deque<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_front(endNode);
		endNode = bm[endNode].previousNode;
	}
	return l;
}
template<class Graphe, class Noeud, class Lien>
vector<unsigned int> Bellman<Graphe, Noeud, Lien>::getReverseShortestPathTo(unsigned int endNode) const
{
	vector<unsigned int> l;
	while (endNode != (unsigned int)(-1))
	{
		l.push_back(endNode);
		endNode = bm[endNode].previousNode;
	}
	return l;
}
