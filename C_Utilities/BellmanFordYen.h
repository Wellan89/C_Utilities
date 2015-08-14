#ifndef DEF_BELLMAN_FORD_YEN
#define DEF_BELLMAN_FORD_YEN

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman-Ford (avec l'amélioration de Yen) :
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
// Détecte aussi la présence de circuits absorbants dans le graphe.
template<class Graphe = Graph<> >
class BellmanFordYen
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BfyNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BfyNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
			{ }
	};

	std::vector<BfyNodeInfo> bfy;
	const Graphe& g;

	// Indique si un circuit absorbant a été détecté dans le graphe
	bool absorbCycleFound;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bfy.clear();
		bfy.resize(g.size());
		absorbCycleFound = false;
	}

public:
	BellmanFordYen(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût du premier noeud
		bfy[startNode].totalCost = 0;

		// Ici (n + 1) / 2 tours de boucles sont suffisants
		IndexNoeud nodesCount = g.size();
		IndexNoeud lastTurnIndex = 1 + (nodesCount - 1) / 2;
		bool madeChange = true;
		for (IndexNoeud i = 0; madeChange; i++)
		{
			madeChange = false;
			bool lastTurn = (i == lastTurnIndex);

			// Parcours toutes les arêtes e = (u, v) avec u < v dans l'ordre des u croissants
			for (IndexNoeud node = 0; node < nodesCount; node++)
			{
				const auto& links = g[node].getLinks();
				for (auto it = links.begin(); it != links.end(); ++it)
				{
					Cout nodeTotalCost = bfy[node].totalCost;
					IndexNoeud targetNode = it->getTargetIndex();
					Cout newCost = nodeTotalCost + it->getCost();
					if (node < targetNode
						&& nodeTotalCost != Graphe::INFINITE_COST
						&& newCost < bfy[targetNode].totalCost)
					{
						if (!lastTurn)
						{
							bfy[targetNode].totalCost = newCost;
							bfy[targetNode].previousNode = node;
							madeChange = true;
						}
						else
						{
							// On a trouvé un circuit absorbant : on quitte ici.
							reset();
							absorbCycleFound = true;
							return;
						}
					}
				}
			}

			// Parcours toutes les arêtes e = (u, v) avec u > v dans l'ordre des u décroissants
			for (IndexNoeud node_inv = 0; node_inv < nodesCount; node_inv++)
			{
				IndexNoeud node = nodesCount - node_inv - 1;
				const auto& links = g[node].getLinks();
				for (auto it = links.begin(); it != links.end(); ++it)
				{
					Cout nodeTotalCost = bfy[node].totalCost;
					IndexNoeud targetNode = it->getTargetIndex();
					Cout newCost = nodeTotalCost + it->getCost();
					if (node > targetNode
						&& nodeTotalCost != Graphe::INFINITE_COST
						&& newCost < bfy[targetNode].totalCost)
					{
						if (!lastTurn)
						{
							bfy[targetNode].totalCost = newCost;
							bfy[targetNode].previousNode = node;
							madeChange = true;
						}
						else
						{
							// On a trouvé un circuit absorbant : on quitte ici.
							reset();
							absorbCycleFound = true;
							return;
						}
					}
				}
			}
		}

		// Vérifie qu'aucun des noeuds avec une boucle absorbante ne peut être atteint depuis le noeud de départ,
		// sinon on est en présence d'un circuit absorbant (les arêtes e = (u, v) avec u = v n'ont pas été vérifiées précédemment).
		for (IndexNoeud node = 0; node < nodesCount; node++)
		{
			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				if (node == it->getTargetIndex() && it->getCost() < 0
					&& bfy[node].totalCost != Graphe::INFINITE_COST)
				{
					// On a trouvé un circuit absorbant : on quitte ici.
					reset();
					absorbCycleFound = true;
					return;
				}
			}
		}
	}

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
	}
	bool canReachNode(IndexNoeud node) const
	{
		return (bfy[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return bfy[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		std::deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = bfy[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = bfy[endNode].previousNode;
		}
		return l;
	}
};

#endif
