#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"
#include "cost_priority_queue.h"

// Algorithme de Dijkstra : pour un graphe où toutes les arêtes ont un coût positif,
// parcours tout le graphe depuis un point de départ et détermine tous les chemins les plus courts depuis ce point.
template<class Graphe = Graph<> >
class Dijkstra
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct DjNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		DjNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }
	};

	std::vector<DjNodeInfo> dj;
	const Graphe& g;

	// Indique si un lien au coût négatif a été détecté dans le graphe
	bool negativeLinkFound;

	// Détermine si la somme de deux coûts dépasse les valeurs maximales ou minimales d'un coût.
	bool costAddOverflow(Cout a, Cout b)
	{
		if (a >= 0)	// Ce test sera optimisé si Cout est un type non signé
		{
			// On ne veut pas pouvoir ajouter "b = -1" à "a = Graphe::INFINITE_COST" par exemple.
			// De plus, on évite d'obtenir un résultat égal à Graphe::INFINITE_COST.
			return (a == Graphe::INFINITE_COST
				|| b >= std::numeric_limits<Cout>::max() - a);
		}
		else
		{
			// On ne veut pas pouvoir ajouter "a = -1" à "b = Graphe::INFINITE_COST" par exemple.
			return (b == Graphe::INFINITE_COST
				|| b < std::numeric_limits<Cout>::min() - a);
		}
	}

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		dj.clear();
		dj.resize(g.size());
		negativeLinkFound = false;
	}

public:
	Dijkstra(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		dj[startNode].totalCost = 0;
		nodesToSee.push(startNode, dj[startNode].totalCost);

		// Le coût du dernier chemin validé par l'algorithme : celui-ci est toujours croissant,
		// et permet ainsi de déterminer les noeuds déjà visités simplement par leur coût.
		Cout currentCost = 0;

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
			nodesToSee.pop();

			Cout nodeTotalCost = dj[node].totalCost;
			if (nodeTotalCost < currentCost)
				continue;

			currentCost = nodeTotalCost;

			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				Cout linkCost = it->getCost();
				if (linkCost < 0)
				{
					// On a trouvé une arête avec un coût négatif : on quitte ici.
					reset();
					negativeLinkFound = true;
					return;
				}

				// Evite de dépasser le coût maximal en calculant le nouveau coût ici :
				// On ignore ce chemin en cas de dépassement puisqu'on ne peut pas le gérer.
				if (costAddOverflow(linkCost, nodeTotalCost))
					continue;

				Cout newCost = nodeTotalCost + linkCost;
				IndexNoeud targetNode = it->getTargetIndex();
				if (newCost < dj[targetNode].totalCost)
				{
					dj[targetNode].previousNode = node;
					dj[targetNode].totalCost = newCost;
					nodesToSee.push(targetNode, dj[targetNode].totalCost);
				}
			}
		}
	}

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool canReachNode(IndexNoeud node) const
	{
		return (dj[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return dj[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		std::deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = dj[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = dj[endNode].previousNode;
		}
		return l;
	}
};

#endif
