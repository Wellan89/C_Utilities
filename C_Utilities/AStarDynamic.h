#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "cost_priority_queue.h"
#include "DynamicGraph.h"

// Algorithme A* : parcours le graphe depuis un point de départ en suivant une heuristique,
// et s'arrête dès qu'un point final est trouvé.
// Gère aussi les graphes dynamiques.
template<class Graphe = DynamicGraph<> >
class AStarDynamic
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct ASDNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		ASDNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }
	};

	std::map<IndexNoeud, ASDNodeInfo> asd;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explorés (marqués comme visités) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

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
		asd.clear();
		endNode = Graphe::INVALID_NODE_INDEX;
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
		negativeLinkFound = false;
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
		asd[startNode].totalCost = 0;
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		nodesToSee.push(startNode, g.getNodeHeuristic(startNode));

		// La priorité du dernier noeud validé par l'algorithme : celle-ci est toujours croissante,
		// et permet ainsi de déterminer les noeuds déjà visités simplement par leur priorité.
		Cout currentPriority = 0;

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
			Cout nodePriority = nodesToSee.top_cost();
			if (g.isNodeFinal(node))
			{
				endNode = node;
				return;
			}
			nodesToSee.pop();

			if (nodePriority < currentPriority)
				continue;
			currentPriority = nodePriority;

#ifdef _DEBUG
			nbExploredNodes++;
#endif
			Cout nodeTotalCost = asd[node].totalCost;

			IndexNoeud linksCount = g.getNodeLinksCount(node);
			for (IndexNoeud i = 0; i < linksCount; i++)
			{
				Graphe::Lien l = g.getNodeLink(node, i);
				Cout linkCost = l.getCost();
				if (linkCost < 0)
				{
					// On a trouvé une arête avec un coût négatif : on quitte ici.
					reset();
					negativeLinkFound = true;
					return;
				}

				// Evite de dépasser le coût maximal en calculant le nouveau coût ici :
				// On ignore ce chemin en cas de dépassement pusiqu'on ne peut pas le gérer.
				if (costAddOverflow(linkCost, nodeTotalCost))
					continue;

				Cout newCost = nodeTotalCost + linkCost;
				IndexNoeud targetNode = l.getTargetIndex();
				if (newCost < asd[targetNode].totalCost)
				{
					asd[targetNode].previousNode = node;
					asd[targetNode].totalCost = newCost;

					// Evite de dépasser la priorité maximale ici.
					Cout targetPriority;
					Cout targetNodeHeuristic = g.getNodeHeuristic(targetNode);
					if (costAddOverflow(newCost, targetNodeHeuristic))
						targetPriority = Graphe::INFINITE_COST;
					else
						targetPriority = newCost + targetNodeHeuristic;

					// On vérifie que ce noeud aura bien une priorité supérieure à la priorité du noeud actuel,
					// afin qu'il ne soit pas considéré comme déjà vu lorsqu'il sera retiré de la file de priorité.
					if (targetPriority < currentPriority)
						targetPriority = currentPriority;

					nodesToSee.push(targetNode, targetPriority);
				}
			}
		}
	}

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool hasFoundPath() const
	{
		return (endNode != Graphe::INVALID_NODE_INDEX);
	}
	IndexNoeud getFinalNode() const
	{
		return endNode;
	}
	Cout getPathCost()
	{
		if (hasFoundPath())
			return asd[endNode].totalCost;
		return Graphe::INFINITE_COST;
	}
	std::deque<IndexNoeud> getShortestPath()
	{
		std::deque<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(node);
			node = asd[node].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPath()
	{
		std::vector<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(node);
			node = asd[node].previousNode;
		}
		return l;
	}

#ifdef _DEBUG
	IndexNoeud getNbExploredNodes() const
	{
		return nbExploredNodes;
	}
#endif
};

#endif
