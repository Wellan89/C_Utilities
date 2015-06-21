#ifndef DEF_DFS_SHORTEST_PATH
#define DEF_DFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <set>
#include "Graph.h"

// Permet à cet algorithme de détecter les circuits absorbants durant son parcours,
// mais cette vérification ralentit légèrement son exécution.
// Si cette macro n'est pas définie, DFS n'entrera pas dans une boucle infinie,
// mais ses résultats seront nécessairement faux en cas de cycle négatif dans le graphe d'entrée, et cela ne sera pas détecté.
#define DFS_SHORTEST_PATH_NEGATIVE_CYCLE_DETECTION

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

		DFS_NodeInfo() : node(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
			{ }

		// Ce constructeur pourrait aussi permettre de convertir des noeuds (IndexNoeud) en DFS_NodeInfo implicitement.
		// Cependant on l'interdit ici (car cela obfusque le code) avec le mot clé explicit.
		explicit DFS_NodeInfo(IndexNoeud _node) : node(_node), totalCost(Graphe::INFINITE_COST())
			{ }

		DFS_NodeInfo(IndexNoeud _node, Cout _totalCost) : node(_node), totalCost(_totalCost)
			{ }
	};

	// L'ensemble des noeuds actuellement visités par récursivité, ainsi que leur distance au noeud de départ
	std::set<DFS_NodeInfo> visitedNodes;
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

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		visitedNodes.clear();
		reverseShortestPath.clear();
		endNode = Graphe::INVALID_NODE_INDEX();
		pathCost = Graphe::INFINITE_COST();
		absorbCycleFound = false;
	}

	bool computeShortestPath_Rec(IndexNoeud node, Cout currentCost);

public:
	DFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode, Cout maxCost = Graphe::INFINITE_COST());

	bool absorbCycleDetected() const
	{
		return absorbCycleFound;
	}
	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost() const;
	std::deque<IndexNoeud> getShortestPath() const;
	const std::vector<IndexNoeud>& getReverseShortestPath() const;
};

#endif
