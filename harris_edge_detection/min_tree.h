#pragma once
#include <vector>
#include "node.h"

class min_tree
{
public:
	min_tree(std::vector<node *> nodes);
	~min_tree(void);

	void delete_node(node* p_node);
	void add_node(node* p_node);
	node* get_max(void);
private:
	int cnt_;
	int height_;
	std::vector< std::vector < node* > > nodes_;
	void propagate(node* p_node, int i, int j);
};

