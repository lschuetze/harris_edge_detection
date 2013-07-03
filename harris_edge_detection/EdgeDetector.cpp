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

	/*
	for(int i = 0; i < width; ++i)
		for(int j = 0; j < height; ++j)
			H[i][j] = 0.0f;
			*/

	prepare();


	mean = 0.0f;
	float min = 0.0f; float max = 0.0f;
	for(int i = 0; i < width; ++i)
		for(int j = 0; j < height; ++j)
		{
			mean += H[i][j];
			//max = std::max(max, H[i][j]);
			//min = std::min(min, H[i][j]);
		}
	mean /= height*width;
	//mean = (max + min) / 2;
}


EdgeDetector::~EdgeDetector(void)
{
	for(int i = 0; i < width; ++i)
		free(H[i]);
	free(H);
}

//calculate corner response for every pixel
/*
void EdgeDetector::prepare(void)
{
	float A, B, C;
	A = B = C = 0.0f;
	for(int x = 1; x < width - 1; ++x)
	{
		for(int y = 1; y < height - 1; ++y)
		{
			float X = dIx(grayedImage, x, y);
			float Y = dIy(grayedImage, x, y);

			A = pow(X, 2);
			B = pow(Y, 2);
			C = X * Y;

			H[x][y] = A * B - pow(C, 2) - 0.04f * (A + B);
		}
	}
}
*/

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
				if(current > mean)
					originalImage.setPixel(x, y, qRgba(255, 0, 0, 255));
			}
		}
	}
	return originalImage;
}

