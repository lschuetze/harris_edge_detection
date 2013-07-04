#include "node.h"

node::node(float value, int x, int y)
{
	value_ = value;
	x_ = x;
	y_ = y;
}


node::~node(void)
{
}

bool node::is_same(node* other)
{
	return x_ == other->x_ && y_ == other->y_;
}
/*
bool operator>(node& left, node& other)
{
	return left.value_ > other.value_;
}


bool operator>(node left, node other)
{
	return left.value_ > other.value_;
}

bool operator<(node& left, node& other)
{
	return left.value_ < other.value_;
}

bool operator<(node left, node other)
{
	return left.value_ < other.value_;
}

bool operator<=(node& left, node& other)
{
	return left.value_ <+ other.value_;
}

bool operator>=(node& left, node& other)
{
	//std::cout << left.value_ << " >= " << other.value_ << std::endl;
	return left.value_ >= other.value_;
}
*/
