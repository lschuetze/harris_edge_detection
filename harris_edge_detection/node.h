#pragma once

#include <iostream>

class node
{
public:
	node(float value, int x, int y);
	~node(void);
/*
	//friend bool operator>(node& left, node& other);
	friend bool operator>(node left, node other);
	//friend bool operator<(node& left, node& other);
	friend bool operator<(node left, node other);
	friend bool operator<=(node& left, node& other);
	friend bool operator>=(node& left, node& other);
*/
	bool is_same(node* other);

	inline float get_value(void) const { return value_; }
private:
	float value_;
	int x_;
	int y_;
};