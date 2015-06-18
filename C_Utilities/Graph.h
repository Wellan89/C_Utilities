#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <deque>
#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Types n�cessaire de la classe Graphe :
//		typedef <> IndexNoeud;
//		typedef <> Cout;
//			Note :	Ces types doivent �tre des types entiers.
// Fonctions statiques constantes n�cessaires de la classe Graphe :
//		static IndexNoeud INVALID_NODE_INDEX();
//		static Cout INFINITE_COST();
//			Note :	Cette derni�re fonction constante doit �tre sup�rieure stricte � tout autre co�t possible.
// Fonctions n�cessaires de la classe Graphe :
//		IndexNoeud size() const;
//		const Noeud& operator[](Graphe::IndexNoeud index) const;
// Fonctions suppl�mentaires n�cessaire pour l'algorithme de Bellman :
//		std::vector<Graphe::IndexNoeud> getReverseTopologicalyOrderedNodes() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de unsigned int pouvant �tre it�r�.
// Type n�cessaire de la classe Graphe pour l'algorithme de Bellman-Ford-Yen :
//		typedef <> Lien;

// Fonction n�cessaire de la classe Noeud :
//		const std::vector<Lien*>& getLinks() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de Lien* pouvant �tre it�r�.
// Fonction suppl�mentaire n�cessaire pour les algorithmes A* et DFS :
//		bool isFinal() const;
// Fonction suppl�mentaire n�cessaire pour l'algorithme A* :
//		Graphe::Cout getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					Plus simplement, l'heuristique d'un noeud x doit toujours �tre inf�rieure
//					� la distance r�elle entre x et le point final le plus proche. Plus cette heuristique est proche
//					de cette distance r�elle, meilleure est l'heuristique, et l'algorithme est alors plus efficace.
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.
// Fonction suppl�mentaire n�cessaire pour l'algorithme de Bellman :
//		const std::vector<Lien*>& getIncomingLinks() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de Lien* pouvant �tre it�r�.

// Fonctions n�cessaires de la classe Lien :
//		Graphe::Cout getCost() const;
//		Graphe::IndexNoeud getTargetIndex() const;
// Fonction suppl�mentaire n�cessaire pour les algorithmes de Bellman et Bellman-Ford-Yen :
//		Graphe::IndexNoeud getFromIndex() const;



// Impl�mentation minimale de ces classes
template<class Cost = long>
class Graph
{
public:
	class Link;
	class Node;
	typedef Link Lien;
	typedef Node Noeud;
	typedef size_t IndexNoeud;
	typedef Cost Cout;

	// On est ici oblig� de d�finir des fonctions pour acc�der � ces constantes,
	// � cause du standard C++ qui d�finit la valeur max d'un type en tant que fonction.
	static IndexNoeud INVALID_NODE_INDEX()
	{
		return std::numeric_limits<IndexNoeud>::max();
	}
	static Cout INFINITE_COST()
	{
		return std::numeric_limits<Cout>::max();
	}

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
			: index(_index), finalNode(false), heuristic(INFINITE_COST()) { }

	public:
		IndexNoeud getIndex() const							{ return index; }
		const std::vector<Lien>& getLinks() const			{ return links; }
		const std::vector<Lien>& getIncomingLinks() const	{ return incomingLinks; }

		bool isFinal() const								{ return finalNode; }
		Cout getHeuristic() const							{ return heuristic; }
	};

protected:
	// Enum d�finissant l'�tat d'un noeud lors de la construction d'un ordre topologique
	enum E_TOPOLOGICAL_ORDER_NODE_STATE
	{
		ETONS_UNSEEN,
		ETONS_SEEN,
		ETONS_ADDED
	};

	std::vector<Noeud> nodes;

	// Ajoute un noeud et ses fils � un ordre topologique.
	// Renvoit false si on d�tecte une erreur lors de la construction de l'ordre topologique.
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
		for (IndexNoeud i = 0; i < nodesNb; i++)
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
	void removeLink(IndexNoeud start, IndexNoeud end, bool oneDirection = false)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target)								\
			do {																\
				auto& nodeLinks = (links);										\
				for (auto it = nodeLinks.begin(); it != nodeLinks.end(); ++it)	\
					if (it->getTargetIndex() == (end))							\
						it = nodeLinks.erase(it);								\
			} while(false)

		// Ici, tous les liens de start � end sont supprim�s, peu importe leur co�t.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end);
		REMOVE_LINKS_FROM_ITERABLE(nodes[end].incomingLinks, start);

		if (!oneDirection)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start);
			REMOVE_LINKS_FROM_ITERABLE(nodes[start].incomingLinks, end);
		}
#undef REMOVE_LINKS_FROM_ITERABLE
	}
	void removeLinkWithCostCheck(IndexNoeud start, IndexNoeud end, Cout cost, bool oneDirection = false)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target, cost)								\
			do {																	\
				auto& nodeLinks = (links);											\
				for (auto it = nodeLinks.begin(); it != nodeLinks.end(); ++it)		\
					if (it->getTargetIndex() == (end) && it->getCost() == (cost))	\
						it = nodeLinks.erase(it);									\
			} while(false)

		// Ici, on ne supprime que les liens de start � end avec le co�t sp�cifi�.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end, cost);
		REMOVE_LINKS_FROM_ITERABLE(nodes[end].incomingLinks, start, cost);

		if (!oneDirection)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start, cost);
			REMOVE_LINKS_FROM_ITERABLE(nodes[start].incomingLinks, end, cost);
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

	// D�termine un ordre topologique inverse sur ce graphe :
	// les noeuds puits seront au d�but de ce vecteur, et les noeuds source seront � la fin.
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
