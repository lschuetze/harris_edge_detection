#pragma once
template <typename T>
class MaxNode
{
public:
	MaxNode(T value, int x, int y);
	~MaxNode(void);

	friend bool operator > (MaxNode<T>& left, MaxNode<T>& right);
	friend bool operator < (MaxNode<T>& left, MaxNode<T>& right);
private:
	T value;
	int x;
	int y;
};

template <typename T>
bool operator > (MaxNode<T>& left, MaxNode<T>& right)
{
	return left.value > right.value;
}

template <typename T>
bool operator < (MaxNode<T>& left, MaxNode<T>& right)
{
	return left.value > right.value;
}

