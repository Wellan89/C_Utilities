#ifndef DEF_A_STAR
#define DEF_A_STAR

#include <vector>
#include <deque>
#include "cost_priority_queue.h"
#include "Graph.h"

// Permet de diminuer la mémoire utilisée par cette classe (et permet ainsi à cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se révèlera légèrement plus lent sur des graphes contenant de nombreux liens au coût nul,
// mais il sera légèrement plus rapide sur des graphes sans ce type de lien.
#define A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme A* : pour un graphe où toutes les arêtes ont un coût positif,
// parcours le graphe depuis un point de départ en suivant une heuristique, et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph<> >
class AStar
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct ASNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		ASNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::vector<ASNodeInfo> as;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explorés (marqués comme visités) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

	// Indique si un lien au coût négatif a été détecté dans le graphe
	bool negativeLinkFound;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		as.clear();
		as.resize(g.size());
		endNode = Graphe::INVALID_NODE_INDEX;
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
		negativeLinkFound = false;
	}

public:
	AStar(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		as[startNode].totalCost = 0;
		nodesToSee.push(startNode, g[startNode].getHeuristic());

#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		// La priorité du dernier noeud validé par l'algorithme : celle-ci est toujours croissante,
		// et permet ainsi de déterminer les noeuds déjà visités simplement par leur priorité.
		Cout currentPriority = 0;
#endif

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
			Cout nodePriority = nodesToSee.top_cost();
#endif
			if (g[node].isFinal())
			{
				endNode = node;
				return;
			}
			nodesToSee.pop();

#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
			if (as[node].alreadyVisited)
				continue;
			as[node].alreadyVisited = true;
#else
			if (nodePriority < currentPriority)
				continue;
			currentPriority = nodePriority;
#endif

#ifdef _DEBUG
			nbExploredNodes++;
#endif
			Cout nodeTotalCost = as[node].totalCost;

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

				IndexNoeud targetNode = it->getTargetIndex();
				Cout newCost = nodeTotalCost + linkCost;
				if (newCost < as[targetNode].totalCost)
				{
					as[targetNode].previousNode = node;
					as[targetNode].totalCost = newCost;

					Cout targetPriority = newCost + g[targetNode].getHeuristic();
#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
					// On vérifie que ce noeud aura bien une priorité supérieure à la priorité du noeud actuel,
					// afin qu'il ne soit pas considéré comme déjà vu lorsqu'il sera retiré de la file de priorité.
					if (targetPriority < currentPriority)
						targetPriority = currentPriority;
#endif

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
	Cout getPathCost() const
	{
		if (hasFoundPath())
			return as[endNode].totalCost;
		return Graphe::INFINITE_COST;
	}
	std::deque<IndexNoeud> getShortestPath() const
	{
		std::deque<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(node);
			node = as[node].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPath() const
	{
		std::vector<IndexNoeud> l;
		IndexNoeud node = endNode;
		while (node != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(node);
			node = as[node].previousNode;
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
