#include <iostream>

#include "EdgeDetector.h"
#include "min_tree.h"
#include "node.h"

EdgeDetector::EdgeDetector(QImage const& image)
	: height(image.height()), width(image.width())
{
	originalImage = QImage(image);
	grayedImage = grayScale(image);
	//grayedImage.save("Resources/grayed.jpg");

	H = (float**) malloc(width * sizeof(float*));
	for(int i = 0; i < width; ++i)
	{
		H[i] = (float*) malloc(height * sizeof(float));
	}

	prepare();

	mean = 0.0f;
	for(int i = 0; i < width; ++i)
		for(int j = 0; j < height; ++j)
		{
			mean += H[i][j];
		}
	mean /= height*width;
}


EdgeDetector::~EdgeDetector(void)
{
	for(int i = 0; i < width; ++i)
		free(H[i]);
	free(H);
}

void EdgeDetector::prepare(void)
{
	float A, B, C;
	A = B = C = 0.0f;
	int w = 1;
	for(int x = 0; x < width; ++x)
	//for(int x = w+1; x < width - w -1; ++x)
	{
		for(int y = 0; y < height; ++y)
		//for(int y = w+1; y < height - w -1; ++y)
		{
			A = B = C = 0.0f;
			//calculate the vecinity
			int min_x = std::max(x-w, 1); int max_x = std::min(x+w, width-1);
			int min_y = std::max(y-w, 1); int max_y = std::min(y+w, height-1);
			for(int i = min_x; i < max_x; ++i)
			//for(int i = x-w; i < x+w; ++i)
			{
				for(int j = min_y; j < max_y; ++j)
				//for(int j = y-w; i < y+w; ++y)
				{
					float X = dIx(grayedImage, i, j);
					float Y = dIy(grayedImage, i, j);

					A += pow(X, 2);
					B += pow(Y, 2);
					C += X * Y;
				}
			}
			H[x][y] = CornerResponse(A, B, C);
		}
	}
}

QImage EdgeDetector::apply(consts::mode mode)
{
	switch(mode)
	{
	case consts::mode::fast:
		return apply_fast();
	case consts::mode::slow:
		return apply_slow();
	}
}

QImage EdgeDetector::apply_fast(void)
{
	int w = 1;
	std::vector< min_tree > verticals;
	std::vector< node* > max_nodes;

	for(int y = 0; y < 2*w; ++y)
	{
		std::vector< node* > list;
		for(int x = 0; x < 2*w; ++x)
		{
			node* n = new node(H[x][y], x, y);
			list.push_back(n);
		}
		min_tree nodes(list);
		verticals.push_back(nodes);
	}
	
	for(int i = 0; i < verticals.size(); ++i)
	{
		max_nodes.push_back(verticals[i].get_max());
	}

	min_tree maxima = min_tree(max_nodes);

	//iterate the whole picture
	for(int y = w; y < height - w; y += 1)
	{
		for(int x = w; x < width - w; x += 1)
		{
			int o = 0;
			for(int k = y - w; k < y + w; ++k)
			{
				node* d = new node(H[x-w][k], x-w, k);
				node* n = new node(H[x][k], x, k);
				//delete the node that dropped out in
				//the left
				verticals[o].delete_node(d);
				//add the new node that showed up
				//in the window at the right
				verticals[o].add_node(n);
				o++;
			}
			node current(H[x][y], x, y);
			//
			//compare with local maxima
			//
			if(maxima.get_max()->get_value() <= current.get_value())
			{
				if(current.get_value() > mean)
				//std::cout << maxima.get_max()->get_value() << " <= " << current.get_value() << std::endl;
					originalImage.setPixel(x, y, qRgba(255, 0, 0, 255));
			}
		}
		std::vector< node*> list;
		for(int z = 0; z < 2*w; ++z)
		{
			node* n = new node(H[z][y+1], z, y+1);
			list.push_back(n);
		}
		verticals[0] = list;
		max_nodes.clear();
		for(int i = 0; i < verticals.size(); ++i)
			max_nodes.push_back(verticals[i].get_max());
		maxima = min_tree(max_nodes);
	}

	return originalImage;
}

QImage EdgeDetector::apply_slow(void)
{
	//window
	int w = 1;
	for(int x = w; x < width - w; x += 1)
	{
		for(int y = w; y < height - w; y += 1)
		{
			bool flag = true;
			float current = H[x][y];

			for(int m = x - w; m <= x + w; ++m)
			{
				for(int n = y - w; n <= y + w; ++n)
				{
					float that = H[m][n];
					if(that > current) {
						flag = false;
					}
				}
			}
			if(flag)
			{
				//reduce noise
				if(current > mean)
					originalImage.setPixel(x, y, qRgba(255, 0, 0, 255));
			}
		}
	}
	return originalImage;
}

