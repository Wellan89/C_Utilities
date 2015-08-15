#ifndef DEF_BELLMAN_FORD
#define DEF_BELLMAN_FORD

#include <deque>
#include <vector>
#include "Graph.h"

// Algorithme de Bellman-Ford :
// parcours tout le graphe depuis un point de d�part et d�termine tous les chemins les plus courts depuis ce point.
// D�tecte aussi la pr�sence de circuits absorbants dans le graphe.
template<class Graphe = Graph<> >
class BellmanFord
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BfNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BfNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }
	};

	std::vector<BfNodeInfo> bf;
	const Graphe& g;

	// Indique si un circuit absorbant a �t� d�tect� dans le graphe
	bool absorbCycleFound;

	// D�termine si la somme de deux co�ts d�passe les valeurs maximales ou minimales d'un co�t.
	bool costAddOverflow(Cout a, Cout b)
	{
		if (a >= 0)	// Ce test sera optimis� si Cout est un type non sign�
		{
			// On ne veut pas pouvoir ajouter "b = -1" � "a = Graphe::INFINITE_COST" par exemple.
			// De plus, on �vite d'obtenir un r�sultat �gal � Graphe::INFINITE_COST.
			return (a == Graphe::INFINITE_COST
				|| b >= std::numeric_limits<Cout>::max() - a);
		}
		else
		{
			// On ne veut pas pouvoir ajouter "a = -1" � "b = Graphe::INFINITE_COST" par exemple.
			return (b == Graphe::INFINITE_COST
				|| b < std::numeric_limits<Cout>::min() - a);
		}
	}

	// R�initialise les informations sur les noeuds
	void reset()
	{
		bf.clear();
		bf.resize(g.size());
		absorbCycleFound = false;
	}

public:
	BellmanFord(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique le co�t du premier noeud
		bf[startNode].totalCost = 0;

		// On sait que la longueur maximale d'un chemin le plus court est de n-1 noeuds
		IndexNoeud nodesCount = g.size();
		IndexNoeud lastTurnIndex = (nodesCount - 1);
		bool madeChange = true;
		for (IndexNoeud i = 0; madeChange; i++)
		{
			madeChange = false;
			bool lastTurn = (i == lastTurnIndex);

			// Parcours toutes les ar�tes du graphe
			for (IndexNoeud node = 0; node < nodesCount; node++)
			{
				Cout nodeTotalCost = bf[node].totalCost;
				if (nodeTotalCost == Graphe::INFINITE_COST)
					continue;

				const auto& links = g[node].getLinks();
				for (auto it = links.begin(); it != links.end(); ++it)
				{
					Cout linkCost = it->getCost();

					// Evite de d�passer le co�t maximal en calculant le nouveau co�t ici :
					// On ignore ce chemin en cas de d�passement pusiqu'on ne peut pas le g�rer.
					if (costAddOverflow(linkCost, nodeTotalCost))
						continue;

					Cout newCost = nodeTotalCost + linkCost;
					IndexNoeud targetNode = it->getTargetIndex();
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
							// On a trouv� un circuit absorbant : on quitte ici.
							reset();
							absorbCycleFound = true;
							return;
						}
					}
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
		return (bf[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return bf[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = bf[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = bf[endNode].previousNode;
		}
		return l;
	}
};

#endif
