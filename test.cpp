#include <iostream>
#include <vector>

#include "boost/multi_array.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>
#include <iostream>
#include <map>
#include <string>
#include <boost/property_map/property_map.hpp>

using namespace boost;
using namespace std;



int main()
{

	 typedef property<edge_weight_t, int, property<edge_index_t , int> > EdgeWeightProperty;
	 typedef boost::adjacency_list<listS, vecS, directedS, no_property, 		
 		EdgeWeightProperty > Graph;

 	Graph g;
 	add_edge(0, 1, 8, g);
 	add_edge (0, 3, 18, g);
 	add_edge (1, 2, 20, g);
    add_edge (2, 3, 2, g);
    add_edge (3, 1, 1, g);
    add_edge (1, 3, 7, g);




	// Graph g(5);
	// add_edge(0, 1, g);
	// add_edge(0, 2, g);
	// add_edge(0, 2, g);
	// add_edge(0, 2, g);
	// add_edge(0, 3, g);
	// add_edge(0, 4, g);
	graph_traits<Graph>::adjacency_iterator  ai, ai_end;

	for(tie(ai, ai_end) = adjacent_vertices(0, g); ai != ai_end; ++ ai)
	{
		// *ai = 1;
		cout << "JJJ  " << *ai << endl;;
	}

	graph_traits<Graph>::out_edge_iterator  oi, oi_end;
	cout << "IOIO**********" << endl;
	cout << sizeof (graph_traits<Graph>::out_edge_iterator) << endl;

	for(tie(oi, oi_end) = out_edges(0, g); oi != oi_end; ++ oi)
	{
		// *oi = 1;
		cout << "ZZZZZZZZZz  " << *oi << endl;;
		cout << "coaLL " << source(*oi, g) << endl;

		cout << "FUCK " << get(edge_weight_t(), g,  *oi) << endl;
	}



	// enum { A, B, C, D, E, F, N };
	// const char* name = "ABCDEF";

	// typedef boost::adjacency_matrix<directedS, no_property, no_property, no_property> Graph;
	// Graph g(N);
	// add_edge(B, C, g);
	// add_edge(B, F, g);
	// add_edge(C, A, g);
	// add_edge(C, C, g);
	// add_edge(D, E, g);
	// add_edge(E, D, g);
	// add_edge(F, A, g);


	// graph_traits<Graph>::adjacency_iterator  ai, ai_end;
	// for(tie(ai, ai_end) = adjacent_vertices(1, g); ai != ai_end; ++ ai)
	// {		
	// 	cout << "JJJ" << *ai << endl;;
	// }

	return 0;



	// graph_traits<Graph>::vertex_iterator vi, vi_end;
	// for(tie(vi, vi_end) = vertices(g); vi != vi_end; ++ vi)
	// {
	// 	cout << "KKK " << *vi << endl;
	// }

	// std::cout << "vertex set: ";
	// boost::print_vertices(g, name);
	// std::cout << std::endl;

	// std::cout << "edge set: ";
	// boost::print_edges(g, name);
	// std::cout << std::endl;

	// std::cout << "out-edges: " << std::endl;
	// boost::print_graph(g, name);
	// std::cout << std::endl;


	return 0;
}
