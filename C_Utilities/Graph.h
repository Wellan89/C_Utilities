#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <deque>
#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Types nécessaire de la classe Graphe :
//		typedef <> IndexNoeud;
//		typedef <> Cout;
//			Note :	Ces types doivent être des types entiers.
// Constantes statiques nécessaires de la classe Graphe :
//		static constexpr IndexNoeud INVALID_NODE_INDEX;
//		static constexpr Cout INFINITE_COST;
//			Note :	Cette dernière fonction constante doit être supérieure stricte à tout autre coût possible.
// Fonctions nécessaires de la classe Graphe :
//		IndexNoeud size() const;
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
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'au noeud final le plus proche.
//					Elle doit toujours être positive ou nulle.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tout noeud x et tout noeud final f : h(x) <= d(x, f).
//					Plus simplement, l'heuristique d'un noeud x doit toujours être inférieure
//					à la distance réelle entre x et le point final le plus proche. Plus cette heuristique est proche
//					de cette distance réelle, meilleure est l'heuristique, et l'algorithme est alors plus efficace.
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions nécessaires de la classe Lien :
//		Graphe::Cout getCost() const;
//		Graphe::IndexNoeud getTargetIndex() const;



// Implémentation minimale de ces classes
template<class Cost = int, class NodeIndex = unsigned int>
class Graph
{
public:
	class Link;
	class Node;
	typedef Link Lien;
	typedef Node Noeud;
	typedef NodeIndex IndexNoeud;
	typedef Cost Cout;

	static constexpr IndexNoeud INVALID_NODE_INDEX = std::numeric_limits<IndexNoeud>::max();
	static constexpr Cout INFINITE_COST = std::numeric_limits<Cout>::max();

	class Link
	{
		friend Graph;

	protected:
		IndexNoeud targetIndex;
		Cout cost;

		Link(IndexNoeud _targetIndex, Cout _cost)
			: targetIndex(_targetIndex), cost(_cost) { }

	public:
		IndexNoeud getTargetIndex() const	{ return targetIndex; }
		Cout getCost() const				{ return cost; }
	};
	class Node
	{
		friend Graph;

	protected:
		std::vector<Lien> links;
		IndexNoeud index;

		Cout heuristic;
		bool finalNode;

		Node(IndexNoeud _index)
			: index(_index), finalNode(false), heuristic(0)	{ }

	public:
		IndexNoeud getIndex() const							{ return index; }
		const std::vector<Lien>& getLinks() const			{ return links; }

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
		for (IndexNoeud i = 0; i < nodesNb; i++)
			nodes.push_back(Noeud(i));
	}

	void addLink(IndexNoeud start, IndexNoeud end, Cout cost, bool unidirectionnal = false)
	{
		nodes[start].links.push_back(Lien(end, cost));
		if (!unidirectionnal && start != end)
			nodes[end].links.push_back(Lien(start, cost));
	}
	void removeLink(IndexNoeud start, IndexNoeud end, bool oneDirection = false)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target)									\
			do {																	\
				auto& nodeLinks = (links);											\
				nodeLinks.erase(std::remove_if(nodeLinks.begin(), nodeLinks.end(),	\
					[target](const Link& l)											\
					{																\
						return (l.getTargetIndex() == target);						\
					}), nodeLinks.end());											\
			} while (false)

		// Ici, tous les liens de start à end sont supprimés, peu importe leur coût.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end);

		if (!oneDirection && start != end)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start);
		}
#undef REMOVE_LINKS_FROM_ITERABLE
	}
	void removeLinkWithCostCheck(IndexNoeud start, IndexNoeud end, Cout cost, bool oneDirection = false)
	{
#define REMOVE_LINKS_FROM_ITERABLE(links, target, cost)									\
			do {																		\
				auto& nodeLinks = (links);												\
				nodeLinks.erase(std::remove_if(nodeLinks.begin(), nodeLinks.end(),		\
					[target, cost](const Link& l)										\
					{																	\
						return (l.getTargetIndex() == target && l.getCost() == cost);	\
					}), nodeLinks.end());												\
			} while (false)

		// Ici, on ne supprime que les liens de start à end avec le coût spécifié.
		REMOVE_LINKS_FROM_ITERABLE(nodes[start].links, end, cost);

		if (!oneDirection && start != end)
		{
			REMOVE_LINKS_FROM_ITERABLE(nodes[end].links, start, cost);
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
		IndexNoeud nodesCount = (IndexNoeud)nodes.size();
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
		// Attention : ici on ne vérifie pas qu'on insère bien un nombre correct de noeuds :
		// on doit toujours avoir : nodes.size() + nbOfNodesToAdd <= INVALID_NODE_INDEX.
		nodes.reserve((IndexNoeud)nodes.size() + nbOfNodesToAdd);
		for (IndexNoeud i = 0; i < nbOfNodesToAdd; i++)
			nodes.push_back(Noeud(nodes.size()));
	}
	
	IndexNoeud size() const
	{
		return (IndexNoeud)nodes.size();
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
