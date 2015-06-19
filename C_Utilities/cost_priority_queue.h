#ifndef DEF_COST_PRIORITY_QUEUE
#define DEF_COST_PRIORITY_QUEUE

#include <utility>
#include <vector>
#include <queue>

// File de priorité où à chaque élément est associé un coût :
// Les éléments les plus prioritaires sont ceux avec le coût le plus faible.
// En cas d'égalité, les éléments insérés les plus tard sont rendus plus prioritaires.
template<class EltType, class Cost = int, class EltCount = unsigned int>
class cost_priority_queue
{
protected:
	struct pq_element
	{
		EltType elt;
		Cost cost;
		EltCount eltId;

		pq_element(EltType _elt, Cost _cost, EltCount _eltId)
			: elt(_elt), cost(_cost), eltId(_eltId)
		{ }
	};

	class cost_priority_queue_comp
	{
	public:
		// left est moins prioritaire que right <=> left a un coût supérieur à right
		// Lorsque left et right ont des coûts identiques, on compare leurs identificateurs :
		// l'élément avec l'ID le plus grand (l'élément le plus récent) est rendu plus prioritaire.
		// Cette optimisation permet à l'algorithme A* de trancher sur les cas d'égalité
		// et d'être bien plus efficace sur les grands graphes où de nombreux plus courts chemins existent.
		bool operator()(const pq_element& left, const pq_element& right) const
		{
			// On ne teste pas directement le cas d'égalité
			// au cas où le type Cout serait un type personnalisé (ou par exemple dans le cas de flottants).
			if (left.cost < right.cost)
				return true;
			else if (right.cost < left.cost)
				return false;
			else
				return (left.eltId < right.eltId);
		}
	};

	std::priority_queue<pq_element, std::vector<pq_element>, cost_priority_queue_comp> pq;
	EltCount currentCount;

public:
	cost_priority_queue() : currentCount(0)	{ }

	bool empty() const
	{
		return pq.empty();
	}
	void push(const EltType& elt, const Cost& cost)
	{
		pq.push(pq_element(elt, cost, currentCount++));
	}
	const EltType& top() const
	{
		return pq.top().elt;
	}
	const Cost& top_cost() const
	{
		return pq.top().cost;
	}
	void pop()
	{
		pq.pop();
	}
};

#endif
