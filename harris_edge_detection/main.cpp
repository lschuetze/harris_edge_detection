#include <iostream>
#include <string>
#include <vector>

#include "EdgeDetector.h"

int main(int argc, char *argv[])
{
	QImage image("Resources/input.png");
	EdgeDetector ed(image);
	QImage out = ed.apply();
	out.save("Resources/output.png");
	return 0;
}
