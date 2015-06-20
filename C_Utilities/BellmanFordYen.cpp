#include "BellmanFordYen.h"

using namespace std;

template<class Graphe>
void BellmanFordYen<Graphe>::computeShortestPathsFrom(IndexNoeud startNode)
{
	// R�initialise les informations sur les noeuds
	reset();

	// Indique le co�t du premier noeud
	bfy[startNode].totalCost = 0;

	// Partitionne l'ensemble des ar�tes en deux ensemble :
	// edgesF = { ar�tes e = (u, v) dans g tq : u < v }
	// edgesB = { ar�tes e = (u, v) dans g tq : u > v }
	// edgesF et edgesB doivent �tre ordonn�s dans l'ordre des u croissants.
	// Les boucles (les ar�tes pour lesquelles u = v) sont ignor�es si leur co�t est positif ou nul,
	// mais sont d�tect�es en tant que circuit absorbant si leur co�t est strictement n�gatif
	// et que ces points sont atteignables depuis le noeud de d�part.
	std::vector<const Graphe::Lien*> edgesF;
	std::vector<const Graphe::Lien*> edgesB;
	std::vector<IndexNoeud> absorbNodes;
	IndexNoeud nodesCount = g.size();
	for (IndexNoeud node = 0; node < nodesCount; node++)
	{
		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			IndexNoeud targetNode = it->getTargetIndex();
			if (node < targetNode)
				edgesF.push_back(&(*it));
			else if (node > targetNode)
				edgesB.push_back(&(*it));
			else if (it->getCost() < 0)
				absorbNodes.push_back(node);
		}
	}

	// Ici (n + 1) / 2 tours de boucles sont suffisants
	IndexNoeud lastTurnIndex = 1 + (nodesCount - 1) / 2;
	bool madeChange = true;
	for (IndexNoeud i = 0; madeChange; i++)
	{
		madeChange = false;
		bool lastTurn = (i == lastTurnIndex);

		// Parcours toutes les ar�tes de edgesF dans l'ordre des u croissants
		for (auto it = edgesF.begin(); it != edgesF.end(); ++it)
		{
			const Graphe::Lien* l = (*it);
			IndexNoeud node = l->getFromIndex();
			Cout nodeTotalCost = bfy[node].totalCost;
			if (nodeTotalCost == Graphe::INFINITE_COST())
				continue;

			IndexNoeud targetNode = l->getTargetIndex();
			Cout newCost = nodeTotalCost + l->getCost();
			if (newCost < bfy[targetNode].totalCost)
			{
				if (!lastTurn)
				{
					bfy[targetNode].totalCost = newCost;
					bfy[targetNode].previousNode = node;
					madeChange = true;
				}
				else
				{
					// On a trouv� un circuit absorbant
					reset();
					absorbCycleFound = true;
					return;
				}
			}
		}

		// Parcours toutes les ar�tes de edgesB dans l'ordre des u d�croissants
		for (auto it = edgesB.rbegin(); it != edgesB.rend(); ++it)
		{
			const Graphe::Lien* l = (*it);
			IndexNoeud node = l->getFromIndex();
			Cout nodeTotalCost = bfy[node].totalCost;
			if (nodeTotalCost == Graphe::INFINITE_COST())
				continue;

			IndexNoeud targetNode = l->getTargetIndex();
			Cout newCost = nodeTotalCost + l->getCost();
			if (newCost < bfy[targetNode].totalCost)
			{
				if (!lastTurn)
				{
					bfy[targetNode].totalCost = newCost;
					bfy[targetNode].previousNode = node;
					madeChange = true;
				}
				else
				{
					// On a trouv� un circuit absorbant
					reset();
					absorbCycleFound = true;
					return;
				}
			}
		}
	}

	// V�rifie qu'aucun des noeuds avec une boucle absorbante ne peut �tre atteint depuis le noeud de d�part
	for (auto it = absorbNodes.begin(); it != absorbNodes.end(); ++it)
	{
		if (bfy[*it].totalCost != Graphe::INFINITE_COST())
		{
			// On a trouv� un circuit absorbant
			reset();
			absorbCycleFound = true;
			return;
		}
	}
}
template<class Graphe>
bool BellmanFordYen<Graphe>::absorbCycleDetected() const
{
	return absorbCycleFound;
}
template<class Graphe>
bool BellmanFordYen<Graphe>::canReachNode(IndexNoeud node) const
{
	return (bfy[node].totalCost != Graphe::INFINITE_COST());
}
template<class Graphe>
typename BellmanFordYen<Graphe>::Cout BellmanFordYen<Graphe>::getCostTo(IndexNoeud node) const
{
	return bfy[node].totalCost;
}
template<class Graphe>
deque<typename BellmanFordYen<Graphe>::IndexNoeud> BellmanFordYen<Graphe>::getShortestPathTo(IndexNoeud endNode) const
{
	deque<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_front(endNode);
		endNode = bfy[endNode].previousNode;
	}
	return l;
}
template<class Graphe>
vector<typename BellmanFordYen<Graphe>::IndexNoeud> BellmanFordYen<Graphe>::getReverseShortestPathTo(IndexNoeud endNode) const
{
	vector<IndexNoeud> l;
	while (endNode != Graphe::INVALID_NODE_INDEX())
	{
		l.push_back(endNode);
		endNode = bfy[endNode].previousNode;
	}
	return l;
}
