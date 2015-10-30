#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Désactive la détection des cycles dans cet algorithme :
// améliore largement ses performances sur des graphes sans cycles,
// mais il fera des stack overflow sur des graphes avec cycles
// si sa fonction de calcul computeShortestPathFrom est appelée avec un maxCost trop grand.
//#define DFS_NO_CYCLE_DETECTION

#ifndef DFS_NO_CYCLE_DETECTION

// Permet à cet algorithme de détecter les circuits absorbants durant son parcours,
// mais cette vérification ralentit légèrement son exécution.
// Si cette macro n'est pas définie, DFS n'entrera pas dans une boucle infinie,
// mais ses résultats seront nécessairement faux en cas de cycle négatif dans le graphe d'entrée, et cela ne sera pas détecté.
#define DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION

#endif

// Algorithme du plus court chemin suivant un parcours en profondeur :
// parcours le graphe en profondeur (jusqu'à une profondeur maximale donnée le cas échéant),
// et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph<> >
class DFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
#ifndef DFS_NO_CYCLE_DETECTION
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
	// L'ensemble des noeuds actuellement visités par récursivité
	std::set<IndexNoeud> visitedNodes;
#else
	struct DFS_NodeInfo
	{
		IndexNoeud node;
		Cout totalCost;

		// Opérateur inférieur strict, ne comparant que les indices des noeuds des structures.
		// Ceci permet de considérer deux DFS_NodeInfo égales si leurs noeuds sont égaux,
		// indépendamment du coût du chemin trouvé pour les atteindre.
		bool operator<(const DFS_NodeInfo& other) const
		{
			return (node < other.node);
		}

		DFS_NodeInfo() : node(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }

		// Ce constructeur pourrait aussi permettre de convertir des noeuds (IndexNoeud) en DFS_NodeInfo implicitement.
		// Cependant on l'interdit ici (car cela obfusque le code) avec le mot clé explicit.
		explicit DFS_NodeInfo(IndexNoeud _node) : node(_node), totalCost(Graphe::INFINITE_COST)
		{ }

		DFS_NodeInfo(IndexNoeud _node, Cout _totalCost) : node(_node), totalCost(_totalCost)
		{ }
	};

	// L'ensemble des noeuds actuellement visités par récursivité, ainsi que leur distance au noeud de départ
	std::set<DFS_NodeInfo> visitedNodes;
#endif
#endif

	// Le chemin actuellement trouvé (dans l'ordre inverse)
	std::vector<IndexNoeud> reverseShortestPath;

	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

	// Le coût pour accéder à ce noeud depuis le noeud de départ
	Cout pathCost;

	// Indique si un circuit absorbant a été détecté dans le graphe
	bool absorbCycleFound;

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
#ifndef DFS_NO_CYCLE_DETECTION
		visitedNodes.clear();
#endif
		reverseShortestPath.clear();
		endNode = Graphe::INVALID_NODE_INDEX;
		pathCost = Graphe::INFINITE_COST;
		absorbCycleFound = false;
	}

	bool computeShortestPath_Rec(IndexNoeud node, Cout currentCost)
	{
		// Vérifie que le chemin actuellement parcouru n'est pas plus long que le chemin le plus court déjà trouvé.
		// On quitte lorsqu'il y a égalité entre la longueur du chemin actuel et celle du chemin le plus court si un noeud final a déjà été trouvé.
		if (currentCost > pathCost
			|| (currentCost >= pathCost && endNode != Graphe::INVALID_NODE_INDEX))
			return false;

		// Vérifie que ce noeud est un noeud final
		if (g[node].isFinal())
		{
			reverseShortestPath.clear();
			reverseShortestPath.push_back(node);

			endNode = node;
			pathCost = currentCost;
			return true;
		}

#ifndef DFS_NO_CYCLE_DETECTION
		// Indique que ce noeud est en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		visitedNodes.insert(node);
#else
		// Indique aussi que le coût du chemin actuellement trouvé est currentCost.
		visitedNodes.insert(DFS_NodeInfo(node, currentCost));
#endif
#endif

		// Parcourt en profondeur tous les fils de ce noeud
		bool pathFound = false;
		const auto& links = g[node].getLinks();
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			Cout linkCost = it->getCost();

			// Evite de dépasser le coût maximal en calculant le nouveau coût ici :
			// On ignore ce chemin en cas de dépassement puisqu'on ne peut pas le gérer.
			if (costAddOverflow(currentCost, linkCost))
				continue;

			Cout targetCost = currentCost + linkCost;
			IndexNoeud targetNode = it->getTargetIndex();

#ifndef DFS_NO_CYCLE_DETECTION
			// Vérifie que ce noeud n'est pas exploré actuellement
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			auto targetNodeIt = visitedNodes.find(targetNode);
#else
			// Cette conversion est nécessaire à cause du mot-clé explicit dans le constructeur DFS_NodeInfo(IndexNoeud)
			auto targetNodeIt = visitedNodes.find(DFS_NodeInfo(targetNode));
#endif
			if (targetNodeIt != visitedNodes.end())
			{
#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
				// S'il l'est, on vérifie bien que ce chemin pour l'atteindre est plus long que le chemin précédemment trouvé,
				// sinon c'est qu'on est en présence d'un circuit absorbant.
				if (targetCost < targetNodeIt->totalCost)
				{
					absorbCycleFound = true;
					return false;
				}
#endif
				continue;
			}
#endif

			if (computeShortestPath_Rec(targetNode, targetCost))
			{
				pathFound = true;
				reverseShortestPath.push_back(node);
			}

#ifdef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
			// Si on a détecté un circuit absorbant, on quitte immédiatement
			if (absorbCycleFound)
				return false;
#endif
		}

#ifndef DFS_NO_CYCLE_DETECTION
		// Efface ce noeud de la liste des noeuds en cours d'exploration
#ifndef DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION
		visitedNodes.erase(node);
#else
		visitedNodes.erase(DFS_NodeInfo(node));
#endif
#endif

		return pathFound;
	}

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode, Cout maxCost = Graphe::INFINITE_COST)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique la profondeur maximale à laquelle on va chercher un noeud final
		pathCost = maxCost;

		// Traite récursivement chacun des noeuds pouvant être atteints depuis le noeud de départ
		if (!computeShortestPath_Rec(startNode, 0))
		{
			// Réinitialise les informations sur les noeuds si aucun noeud final n'a été trouvé,
			// en retenant si un circuit absorbant a été détecté.
			bool absorbCycle = absorbCycleFound;
			reset();
			absorbCycleFound = absorbCycle;
		}
	}

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
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
		return pathCost;
	}
	std::deque<IndexNoeud> getShortestPath() const
	{
		std::deque<IndexNoeud> l;
		for (auto it = reverseShortestPath.begin(); it != reverseShortestPath.end(); ++it)
			l.push_front(*it);
		return l;
	}
	const std::vector<IndexNoeud>& getReverseShortestPath() const
	{
		return reverseShortestPath;
	}
};

#endif
