#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <climits>
#include <deque>
#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Types nécessaire de la classe Graphe :
//		typedef <> IndexNoeud;
//		typedef <> Cout;
//			Note :	Ces types doivent être des types entiers.
// Constantes nécessaires de la classe Graphe :
//		static const IndexNoeud INVALID_NODE_INDEX;
//		static const Cout INFINITE_COST;
//			Note :	Cette dernière constante doit être supérieure stricte à tout autre coût possible.
// Fonctions nécessaires de la classe Graphe :
//		unsigned int size() const;
//		const Noeud& operator[](Graphe::IndexNoeud index) const;
// Fonctions supplémentaires nécessaire pour l'algorithme de Bellman :
//		std::vector<Graphe::IndexNoeud> getReverseTopologicalyOrderedNodes() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de unsigned int pouvant être itéré.
// Type nécessaire de la classe Graphe pour l'algorithme de Bellman-Ford-Yen :
//		typedef <> Lien;

// Fonction nécessaire de la classe Noeud :
//		const std::vector<Lien*>& getLinks() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de Lien* pouvant être itéré.
// Fonction supplémentaire nécessaire pour les algorithmes A* et DFS :
//		bool isFinal() const;
// Fonction supplémentaire nécessaire pour l'algorithme A* :
//		Graphe::Cout getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'à un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					Plus simplement, l'heuristique d'un noeud x doit toujours être inférieure
//					à la distance réelle entre x et le point final le plus proche. Plus cette heuristique est proche
//					de cette distance réelle, meilleure est l'heuristique, et l'algorithme est alors plus efficace.
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.
// Fonction supplémentaire nécessaire pour l'algorithme de Bellman :
//		const std::vector<Lien*>& getIncomingLinks() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de Lien* pouvant être itéré.

// Fonctions nécessaires de la classe Lien :
//		Graphe::Cout getCost() const;
//		Graphe::IndexNoeud getTargetIndex() const;
// Fonction supplémentaire nécessaire pour les algorithmes de Bellman et Bellman-Ford-Yen :
//		Graphe::IndexNoeud getFromIndex() const;



// Implémentation minimale de ces classes
class Graph
{
public:
	class Link;
	class Node;
	typedef Link Lien;
	typedef Node Noeud;
	typedef unsigned int IndexNoeud;
	static const IndexNoeud INVALID_NODE_INDEX = (unsigned int)(-1);
	typedef long long int Cout;
	static const Cout INFINITE_COST = LLONG_MAX;

	class Link
	{
		friend Graph;

	protected:
		IndexNoeud fromIndex;
		IndexNoeud targetIndex;
		Cout cost;

		Link(IndexNoeud _fromIndex, IndexNoeud _targetIndex, Cout _cost)
			: fromIndex(_fromIndex), targetIndex(_targetIndex), cost(_cost) { }

	public:
		IndexNoeud getFromIndex() const		{ return fromIndex; }
		IndexNoeud getTargetIndex() const	{ return targetIndex; }
		Cout getCost() const				{ return cost; }
	};
	class Node
	{
		friend Graph;

	protected:
		std::vector<Lien> links;
		std::vector<Lien> incomingLinks;
		IndexNoeud index;

		Cout heuristic;
		bool finalNode;

		Node(IndexNoeud _index)
			: index(_index), finalNode(false), heuristic(INFINITE_COST) { }

	public:
		IndexNoeud getIndex() const							{ return index; }
		const std::vector<Lien>& getLinks() const			{ return links; }
		const std::vector<Lien>& getIncomingLinks() const	{ return incomingLinks; }

		bool isFinal() const								{ return finalNode; }
		Cout getHeuristic() const							{ return heuristic; }
	};

protected:
	// Enum définissant l'état d'un noeud lors de la construction d'un ordre topologique
	enum E_TOPOLOGICAL_ORDER_NODE_STATE
	{
		ETONS_UNSEEN,
		ETONS_SEEN,
		ETONS_ADDED
	};

	std::vector<Noeud> nodes;

	// Ajoute un noeud et ses fils à un ordre topologique.
	// Renvoit false si on détecte une erreur lors de la construction de l'ordre topologique.
	bool addToTopologicalOrder(IndexNoeud index, std::vector<IndexNoeud>& v,
		std::vector<E_TOPOLOGICAL_ORDER_NODE_STATE>& nodesState) const
	{
		nodesState[index] = ETONS_SEEN;
		const auto& links = nodes[index].links;
		for (auto it = links.begin(); it != links.end(); ++it)
		{
			IndexNoeud node = it->getTargetIndex();
			if (nodesState[node] == ETONS_UNSEEN)
			{
				if (!addToTopologicalOrder(node, v, nodesState))
					return false;
			}
			else if (nodesState[node] == ETONS_SEEN)
				return false;
		}
		nodesState[index] = ETONS_ADDED;
		v.push_back(index);
		return true;
	}

public:
	Graph(IndexNoeud nodesNb)
	{
		nodes.reserve(nodesNb);
		for (unsigned int i = 0; i < nodesNb; i++)
			nodes.push_back(Noeud(i));
	}

	void addLink(IndexNoeud start, IndexNoeud end, Cout cost, bool unidirectionnal = false)
	{
		nodes[start].links.push_back(Lien(start, end, cost));
		nodes[end].incomingLinks.push_back(Lien(start, end, cost));
		if (!unidirectionnal)
		{
			nodes[end].links.push_back(Lien(end, start, cost));
			nodes[start].incomingLinks.push_back(Lien(end, start, cost));
		}
	}
	void removeLink(IndexNoeud start, IndexNoeud end, bool oneDirection = false, bool costCheck = false, Cout cost = INFINITE_COST)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target, costCheck, cost)									\
			do {																					\
				auto& nodeLinks = (links);															\
				for (auto it = nodeLinks.end(); it != nodeLinks.end(); ++it)						\
					if (it->getTargetIndex() == (end) && (!(costCheck) || it->getCost() == (cost)))	\
						it = nodeLinks.erase(it);													\
			} while(false)

		// On parcours les liens de la fin vers le début, pour obtenir une meilleure complexité en moyenne

		// Si costCheck == true : tous les liens de start à end sont supprimés, peu importe leur coût ;
		// sinon on ne supprime que les liens de start à end avec le coût spécifié.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end, costCheck, cost);
		REMOVE_LINKS_FROM_ITERABLE(nodes[end].incomingLinks, start, costCheck, cost);

		if (!oneDirection)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start, costCheck, cost);
			REMOVE_LINKS_FROM_ITERABLE(nodes[start].incomingLinks, end, costCheck, cost);
		}
#undef REMOVE_LINKS_FROM_ITERABLE
	}

	void setNodeFinal(IndexNoeud node, bool isFinal = true)
	{
		nodes[node].finalNode = isFinal;
	}
	void setNodeHeuristic(IndexNoeud node, Cout heuristic)
	{
		nodes[node].heuristic = heuristic;
	}

	// Détermine un ordre topologique inverse sur ce graphe :
	// les noeuds puits seront au début de ce vecteur, et les noeuds source seront à la fin.
	// Renvoit le vecteur vide s'il n'existe pas de tel ordre topologique.
	std::vector<IndexNoeud> getReverseTopologicalyOrderedNodes() const
	{
		IndexNoeud nodesCount = nodes.size();
		std::vector<IndexNoeud> v;
		v.reserve(nodesCount);

		std::vector<E_TOPOLOGICAL_ORDER_NODE_STATE> nodesState(nodesCount, ETONS_UNSEEN);
		for (IndexNoeud i = 0; i < nodesCount; i++)
		{
			if (nodesState[i] == ETONS_UNSEEN)
			{
				if (!addToTopologicalOrder(i, v, nodesState))
				{
					v.clear();
					break;
				}
			}
		}
		return v;
	}

	void addNodes(IndexNoeud nbOfNodesToAdd = 1)
	{
		nodes.reserve(nodes.size() + nbOfNodesToAdd);
		for (IndexNoeud i = 0; i < nbOfNodesToAdd; i++)
			nodes.push_back(Noeud(nodes.size()));
	}
	
	IndexNoeud size() const
	{
		return nodes.size();
	}
	Noeud& operator[](IndexNoeud index)
	{
		return nodes[index];
	}
	const Noeud& operator[](IndexNoeud index) const
	{
		return nodes[index];
	}
};

#endif
