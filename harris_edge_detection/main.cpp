#include <iostream>
#include <string>
#include <vector>

#include "EdgeDetector.h"

#ifndef DEBUG
#define DEBUG
#endif

int main(int argc, char *argv[])
{
	consts::mode mode = consts::mode::fast;
	if(argc > 2)
	{
		if(std::string(argv[2]) == "--slow")
		{
			mode = consts::mode::slow;
		}
		else
		{
			if(std::string(argv[2]) == "--fast")
			{
				mode = consts::mode::fast;
			}
		}
	}

#ifdef DEBUG_
	else
	{
		std::cerr << "Usage: EdgeDetector --slow or --fast" << std::endl;
		return -1;
	}
#endif

	QImage image("Resources/input.png");
	EdgeDetector ed(image);
	QImage out = ed.apply(mode);
	out.save("Resources/output.png");
	return 0;
}
