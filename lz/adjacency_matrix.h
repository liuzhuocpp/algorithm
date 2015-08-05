#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <vector>
#include "graph_utility.h"
#include "multi_array.h"

namespace lz {


	namespace ns_AdjacencyMatrix{ // for implement class AdjacencyMatrix

	template<typename EP>
	struct  E
	{
		MultiArray<bool, 2> e;
		MultiArray<EP, 2> ep;
		const EP& getEP(int a, int b) const { return ep[a][b]; }
		void setEP(int a, int b, const EP &_ep) { ep[a][b] = _ep; }
		void setEP(int a, int b, EP &&_ep) { ep[a][b] = _ep; }

		void resizeEP(const Extent &ex) { ep.resize(ex); }
	};
	template<>
	struct E<NoProperty>
	{
		MultiArray<int, 2> e;
		NoProperty getEP(int a, int b) const { return NoProperty(); }
		void setEP(int a, int b, const NoProperty &_ep) {}
		void setEP(int a, int b, NoProperty &&_ep) {}

		void resizeEP(const Extent &ex) {}
	};

	template<typename VP, typename EP>
	struct EV: public E<EP>
	{
		vector<VP> vp;
		void assginVP(int n) { vp.assgin(n); }
	};
	template<typename EP>
	struct EV<NoProperty, EP>: public E<EP>
	{
		void assginVP(int n) {}
	};

	template<typename VP, typename EP, typename GP>
	struct EVG: public EV<VP, EP>
	{
		GP gp;
	};
	template<typename VP, typename EP>
	struct EVG<VP, EP, NoProperty>: public EV<VP, EP>
	{
	};



	} // namespace ns_AdjacencyMatrix



template<typename VP = NoProperty, 
		 typename EP = NoProperty, 
		 typename GP = NoProperty >
class AdjacencyMatrix: public ns_AdjacencyMatrix::EVG<VP, EP, GP>
{
typedef ns_AdjacencyMatrix::EVG<VP, EP, GP> D;
public:
	typedef VP VertexProperty;
	typedef EP EdgeProperty;
	typedef GP GraphProperty;
	explicit AdjacencyMatrix(int n = 0)
	{
		clear(n);
	}
	void clear(int n) 
	{
		D::e.resize(extents[n][n]);
		D::resizeEP(extents[n][n]);
		D::assginVP(n);
	}
	const VP& vertexProperty(int i) const { return D::v[i]; }
	void setVertexProperty(int i, const VP &vp) { D::v[i] = vp; }
	void setVertexProperty(int i, VP &&vp) { D::v[i] = vp; }

	const GP& graphProperty() const { return D::gp; }	
	void setGraphProperty(int i, const GP &gp) { D::gp = gp; }
	void setGraphProperty(int i, GP &&gp) { D::gp = gp; }
	
	void add(int a, int b, const EP & ep = EP())
	{
		D::e[a][b] = 1;
		D::setEP(a, b, ep);
	}
	void add(int a, int b, EP && ep)
	{
		D::e[a][b] = 1;
		D::setEP(a, b, ep);
	}
	void addBidirection(int a, int b, const EP & ep)
	{
		add(a, b, ep);
		add(b, a, ep);
	}
	inline int vertexNumber() const { return D::e.dimension(0); }

	// adjacency edge const iterator const_iterator
	class const_iterator
	{
		friend class AdjacencyMatrix<VP, EP, GP>;
		const AdjacencyMatrix<VP, EP, GP> *g;
		int u;
		AdjacencyEdge<EP> to;
		const_iterator(const AdjacencyMatrix<VP, EP, GP> *_g, int _u, const AdjacencyEdge<EP> & _to):
			g(_g), u(_u), to(_to) {}
	public:
		const_iterator():g(0), u(-1), to(-1, EP()) {}
		const_iterator& operator++() 
		{
			int nt = to.end;
			for(nt ++; nt < g->vertexNumber(); nt ++)
			{
				if(g->D::e[u][nt])
				{
					break;
				}
			}
			to.end = nt;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator it = *this;
			++*this;
			return it;
		}	
		bool operator==(const const_iterator &o)const 
			{ return g == o.g && u == o.u && to.end == o.to.end; }
		bool operator!=(const const_iterator &o)const
			{ return !(*this == o); }
		const AdjacencyEdge<EP>& operator*() const { return to; }
		const AdjacencyEdge<EP>* operator->() const { return &to; }
	};	
	const_iterator begin(int u) const 
	{
		int nt;
		for(nt = 0; nt < vertexNumber(); ++ nt)
		{
			if(D::e[u][nt])
			{
				break;
			}
		}
		return const_iterator(this, u, AdjacencyEdge<EP>(nt, D::getEP(u, nt)  ) );
	}
	const_iterator end(int u) const 
		{ return const_iterator(this, u, AdjacencyEdge<EP>(vertexNumber(), EP()) ); }
	typedef const_iterator iterator;	
	typedef const_iterator Iterator;
	typedef const_iterator ConstIterator;

	MultiArray<bool, 2>::reference edge(int a, int b) { return D::e[a][b]; }
	MultiArray<bool, 2>::const_reference edge(int a, int b) const { return D::e[a][b]; }

	typename MultiArray<EP, 2>::reference edgeProperty(int a, int b) { return D::ep[a][b]; }
	typename MultiArray<EP, 2>::const_reference edgeProperty(int a, int b) const { return D::ep[a][b]; }








};






} // namespace lz




#endif // ADJACENCY_MATRIX_H