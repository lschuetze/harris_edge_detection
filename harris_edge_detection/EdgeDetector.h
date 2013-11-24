#pragma once

#include <qimage.h>
#include <qrgb.h>
#include <vector>
#include <math.h>

namespace consts
{
	enum mode {fast, slow};
}

class EdgeDetector
{
public:
	EdgeDetector(QImage const& img);
	virtual ~EdgeDetector(void);

	QImage apply(consts::mode mode);
protected:
	void prepare(void);
	QImage apply_fast(void);
	QImage apply_slow(void);
private:
	QImage originalImage;
	QImage grayedImage;
	int height;
	int width;
	float** H;
	float mean;
};

//grays an image
static QImage grayScale(QImage const& in)
{
	QImage out(in);
	int pixels = out.width() * out.height();
	if(pixels * (int)sizeof(QRgb) <= out.byteCount())
	{
		QRgb* data = (QRgb*)out.bits();
		for(int i = 0; i < pixels; i++)
		{
			int v = qGray(data[i]);
			data[i] = qRgba(v, v, v, qAlpha(data[i]));
		}
	}
	return out;
}

//corner response function as stated in the paper
static float CornerResponse(float A, float B, float C, const float k = 0.04f)
{
	float tr = A + B;
	float det = A * B - pow(C, 2);

	return det + k * tr;
}

static float dIx(QImage const& image, int x, int y)
{
	int left = qRed(image.pixel(x - 1, y));
	int right = qRed(image.pixel(x + 1, y));
	return (right + left) / 2;
}

static float dIy(QImage const& image, int x, int y, const int w = 1)
{
	int above = qRed(image.pixel(x, y + 1));
	int down = qRed(image.pixel(x, y -1));
	return (above + down) / 2;
}

