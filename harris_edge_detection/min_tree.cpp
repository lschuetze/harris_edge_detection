#include <math.h>
#include "min_tree.h"
#include "node.h"


min_tree::min_tree(std::vector<node *> nodes)
{
	
	cnt_ = nodes.size();
	height_ = std::log(cnt_) / std::log(2);
	nodes_.push_back(nodes);

	for(int i = 0; i < height_; ++i)
	{
		std::vector< node* > level;
		int width = cnt_ / pow(2, i);
		for(int j = 0; j < width; j += 2)
		{
			node* max = (nodes_[i][j]->get_value() >= nodes_[i][j+1]->get_value()) ? nodes_[i][j] : nodes_[i][j+1]; 
			level.push_back(max);
		}
		nodes_.push_back(level);
	}
	
	/*
	nodes_ = std::vector< node* >(nodes.size());
	for(int i = 0; i < nodes.size(); ++i)
	{
		add_node(nodes[i]);
	}
	*/
}

min_tree::~min_tree(void)
{
}

void min_tree::add_node(node* p_node)
{
	for(int j = 0; j < cnt_; ++j)
	{
		if(nodes_[0][j] == NULL)
		{
			nodes_[0][j] = p_node;
			propagate(p_node, 0, j);
			break;
		}
	}
}

void min_tree::propagate(node* p_node, int i, int j)
{
	//std::cout << "propagate" << std::endl;

	int k = (j % 2 == 0) ? j + 1 : j - 1;
	node* max = (nodes_[i][j]->get_value() >= nodes_[i][k]->get_value()) ? nodes_[i][j] : nodes_[i][k];
	if(max->is_same(p_node))
	{
		nodes_[i+1][j/2] = max;
		if(i+1 < height_) propagate(max, i+1, j/2);
	}
}

void min_tree::delete_node(node* p_node)
{
	//std::cout << "delete node" << std::endl;
	for(int i = 0; i < height_; ++i)
	{
		for(int j = 0; j < cnt_ / pow(2, i); ++j)
		{
			node* other = nodes_[i][j];
			if(p_node->is_same(other))
			{
				//std::cout << "propagate" << std::endl;
				nodes_[i][j] = NULL;
				break;
			}
		}
	}
}

node* min_tree::get_max(void)
{
	//get heightest level in the hierarchy
	std::vector< node* > inner = nodes_[nodes_.size() -1];
	//this should be just one value
	return inner[inner.size() -1];
}
