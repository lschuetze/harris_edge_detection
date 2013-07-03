#include <iostream>

#include "EdgeDetector.h"


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
	for(int x = w + 1; x < width - w - 1; ++x)
	{
		for(int y = w + 1; y < height - w - 1; ++y)
		{
			A = B = C = 0.0f;
			//calculate the vecinity 
			for(int i = x - w; i <= x + w; ++i)
			{
				for(int j = y - w; j <= y + w; ++j)
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

QImage EdgeDetector::apply(void)
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

