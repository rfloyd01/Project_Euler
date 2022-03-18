#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes polynomial_multiply(), polcoeff() and <vector>
#include <iostream>
#include <queue>

//Squares under a hyperbola
struct gap {
	double start_x, start_y, max_square;

	gap() {
		start_x = 0.0;
		start_y = 0.0;
		max_square = 0.0;
	}

	friend bool operator==(const gap& num1, const gap& num2);
	friend bool operator!=(const gap& num1, const gap& num2);
	friend bool operator<(const gap& num1, const gap& num2);
	friend bool operator>(const gap& num1, const gap& num2);
	friend bool operator<=(const gap& num1, const gap& num2);
	friend bool operator>=(const gap& num1, const gap& num2);
};

double biggestSquare(gap g)
{
	//calculates the side length of a square with its bottom left point at (point start_x, start_y) and
	//its top right point on the line y = 1/x

	//first, use y = mx + b to calculate equation of line going from bottom left to top right of square.
	//in this case the slope is always 1 so the equation just becomes y = x + b. b can be calcualted
	//by subtracting the y-start point from the x-startpoint
}

std::pair<std::string, double> q247()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;

	//create a priority queue of gap elements
	std::priority_queue<int> yeezus;

	yeezus.push(5);
	yeezus.push(7);
	yeezus.push(2);
	yeezus.push(5);

	while (!yeezus.empty()) {
		std::cout << yeezus.top() << std::endl;
	}

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
/*
* First Thoughts:
* A pretty interesting looking problem we know that the curve will technically never reach the x axis (but it will approach it) because the y-value
* is constrained at 1/x. My first main thought is that adding a square will always divide a region into two more regions, some kind of ordered list
* can be created which sorts new regions created by their size (in decreasing order). Since we want to place the biggest squares first this list could
* be some kind of a priority queue where bigger regions are placed in the front.
* 
* Since the curve is infinite, so to are the numbers of regions that can be created. Since the problem wants us to find the greates n where S(n) is
* (3, 3) we can eleminate regions from contetion as soon as one of these indices is reached. For example, anything going above block number 5 in the
* given drawing will start with an S(n) of (4, 1). This is already higher than our limit for the first index so we can thus eliminate all regions that
* come to the right of the 5 block. The same can be done once a region has become too tall. In the drawing, whatever is placed above the 66 block
* will have a height of 3, but anything above whatever that block may be will have a height of 4, so there's no more reason to drill upwards. 
* 
* So finding the physical block location probably won't be too difficult. What will be a little more difficult though, is figuring out what the actual number
* is. I suggest cutting off routes once their S(n) values become too big, like stopping going to the right after block 5. This is all and good, however,
* if I do this then the next block to be placed would go directly over the 1 and would have a number of 6 instead of it's real number 12. I'm going to
* need to implement some kind of function that figures out how many blocks ahead I'm skipping once I elimnate a gap from contention. I'm sure that there 
* will be some kind of pattern here, but I think figuring out what that pattern will be is going to be the hardest part of this problem.
* 
* What I may do is first write code to figure out where the actual block is, once I've done that I can work on figuring out the pattern of how many 
* numbers I skip ahead each time I eliminate a gap from contention.
* 
* First Attempt:
* So first things first, I'm going to implement a priority queue that will sort the created gaps by their size. This way I can just keep pulling the
* current largest gap, and then add the new gaps into the queue. Before I can implement this, however, I need to actually calculate the areas of the gaps
* being created and figure out where the biggest square can go. The area under this curve is infinite, so the gap created to the right of squares
* placed on the bottom layer will always have an area of infinity. So unfortunately I can't just check by gap area. What I can do though, is record the
* starting location of each new gap and see how long a line with slop of +1 can be before hitting the original curve. The reason for a slope of 1 is
* because this will simulate going from the bottom left corner of the square to the top right.
* 
* starting with the original curve, our square will start at location (1, 0) and have a slope of 1 which can be signified with the equation:
*	y = x - 1. We also have the main equation for our curve, y = 1/x. By setting these two equations equal to eachother we can figure out where our
* square will intersect the curve. 0 = x^2 - x - 1. Solving this yields an x value of 1/2 + sqrt(5)/2 ~= 1.618034. This means that our first square will
* have a side length of 1.618034 - 1 = 0.618034. The drawing on the Project Euler page confirms this. Making this square will then create two more 
* gaps for us. One with a starting location of (1, 0.618034) and the other with a starting point of (1.618034, 0). We can then caclute using the same
* method how large the biggest square is for each of these gaps.
* 
* So this is all well and good, however, I can forsee some potential rounding issues once these squares start getting really small. Hopefully our
* answer can be found before this becomes a problem if we use doubles.
*/