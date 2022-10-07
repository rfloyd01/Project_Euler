#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes gcd()
#include <iomanip>

//The Mouse on the Moon
struct GraphPoint
{
	long double x = 0; //x value of point
	long double y = 0; //y value of point
	long double slope_numerator = 0; //the slope between this point and the previous point on the graph
	long double slope_denominator = 1;
	long double distance = 0; //the distance between this point and the previous on the graph
	long double area = 0; //the are between this point and the previous on the graph
	int intersections = 0; //the number of grid points crossed before reaching previous point
};

std::vector<GraphPoint> findInbetweenPoints(std::vector<GraphPoint> line)
{
	//looks at "line" which is composed of different line segments and answers the question,
	//if an identical line was placed 1 space below this one on the Cartesean plane, what 
	//graph points would fall in between the two lines? Only segments of the line that 
	//have a slope with decimal value will be able to have points fall between them.

	std::vector<GraphPoint> inbetween_points;
	for (int i = 1; i < line.size() - 1; i++)
	{
		if (((int)line[i].slope_numerator % (int)line[i].slope_denominator) != 0)
		{
			//move forwards in increments of x = 1 from the previous point
			//on the line to the current point. There should be an inbetween
			//point at each of these x values.
			long double current_y = line[i - 1].y;

			for (int j = line[i - 1].x + 1; j < line[i].x; j++)
			{
				current_y += (line[i].slope_numerator / line[i].slope_denominator);
				inbetween_points.push_back({ (long double)j, floor(current_y) });
			}
		}
	}

	return inbetween_points;
}

void optimalLineRatio(std::vector<GraphPoint>& optimal_path_a, std::vector<GraphPoint>& inbetween_points, GraphPoint current_graph_point, long double upper_slope, long double lower_slope, std::vector<GraphPoint>& new_best_points,
	std::vector<GraphPoint>& new_inner_points, long double& best_ratio, long double current_total_distance, long double current_total_area, bool& better_path_found, int current_point = 1)
{
	//attempt to connect the current point to all points on path a
	for (int i = current_point; i < optimal_path_a.size() - 1; i++) //don't go to last point as it's imaginary
	{
		//if the current line segment from path a crosses over multiple grid points, we need to try and visit all of them
		for (int j = 0; j < optimal_path_a[i].intersections; j++)
		{
			//if line segment of a doesn't have multiple segments then a_x and a_y will just be the end point of the segment.
			//There are instances though where line segments of 'a' can pass through multiple grid points (i.e. a line with 
			//a slope of 2 travelling from (0, 0) to (2, 4) would also intersect with (1, 2) so we need to make sure we test
			//the (1, 2) point as well as the (2, 4) point.
			GraphPoint new_point_a = { optimal_path_a[i].x - ((long double)j * optimal_path_a[i].slope_denominator), optimal_path_a[i].y - ((long double)j * optimal_path_a[i].slope_numerator), 0, 0, 0, 0, 0 };

			//check to see if the slop of the current segment of 'a' is 1, it's possible for these lines to terminate in a spot that
			//isn't a grid point (because we reflect our final line of the y = grid_size - x axis). If this case, make sure 
			if (optimal_path_a[i].slope_numerator == 1)
			{
				if ((int)(new_point_a.x + .5) != (int)new_point_a.x)
				{
					new_point_a.x -= .5;
					new_point_a.y -= .5;
				}
			}

			if (new_point_a.x <= current_graph_point.x) continue; //we wouldn't travel to any points behind the current one or directly above (TODO: this may be unneccessary by setting current point correctly)

			//The slope of the newly formed segment must be less than the previous segment, but greater than or equal to the current segment slope
			long double new_segment_slope = (new_point_a.y - current_graph_point.y) / (new_point_a.x - current_graph_point.x); //delta_y / delta_x
			//if ((new_segment_slope >= upper_slope) || (new_segment_slope < (optimal_path_a[i].slope_numerator / optimal_path_a[i].slope_denominator))) continue;
			if ((new_segment_slope >= upper_slope) || (new_segment_slope < lower_slope)) continue;
			if (new_segment_slope == 1) continue; //slope needs to be greater than 1
			new_point_a.slope_numerator = new_point_a.y - current_graph_point.y;
			new_point_a.slope_denominator = new_point_a.x - current_graph_point.x;

			//make sure slope is made up of co-prime numbers
			int slope_gcd = gcd((int)new_point_a.slope_numerator, (int)new_point_a.slope_denominator);
			if (slope_gcd > 1)
			{
				new_point_a.slope_numerator /= slope_gcd;
				new_point_a.slope_denominator /= slope_gcd;
			}

			//The new line segment meets the slope requirements. We now caclulate all the other aspects of this new line segment
			//so that we can potentially update optimal_path_b
			new_point_a.distance = sqrt((new_point_a.y - current_graph_point.y) * (new_point_a.y - current_graph_point.y) + (new_point_a.x - current_graph_point.x) * (new_point_a.x - current_graph_point.x));
			new_point_a.area = (new_point_a.x - current_graph_point.x) * (current_graph_point.y + (new_point_a.y - current_graph_point.y) / 2.0);
			new_point_a.intersections = new_point_a.distance / (sqrt(new_point_a.slope_numerator * new_point_a.slope_numerator + new_point_a.slope_denominator * new_point_a.slope_denominator));

			//Increment the current total distance and area to reflect the addition of the new line segment
			long double new_total_distance = current_total_distance + new_point_a.distance;
			long double new_total_area = current_total_area + new_point_a.area;

			//Add distance and area from partial point a_x and a_y to the actual point on line 'a'.
			new_total_distance += sqrt((optimal_path_a[i].y - new_point_a.y) * (optimal_path_a[i].y - new_point_a.y) + (optimal_path_a[i].x - new_point_a.x) * (optimal_path_a[i].x - new_point_a.x));
			new_total_area += (optimal_path_a[i].x - new_point_a.x) * (new_point_a.y + (optimal_path_a[i].y - new_point_a.y) / 2.0);

			//Then tak on area and distance measurements for the rest of the points on line 'a'.
			for (int k = i + 1; k < optimal_path_a.size() - 1; k++)
			{
				new_total_distance += optimal_path_a[k].distance;
				new_total_area += optimal_path_a[k].area;
			}

			//don't forget to add the final area at the end from the imaginary point
			new_total_area += optimal_path_a.back().area;

			//compare the new ratio to the existing best and if it's better, update it
			long double new_ratio = new_total_area / new_total_distance;
			if (new_ratio > best_ratio)
			{
				best_ratio = new_ratio;
				better_path_found = true;

				//clear necessary points from the new_best_points vector and replace with this new point
				if (!new_inner_points.size()) new_best_points.clear();
				else new_best_points = new_inner_points; //Will this cause anything wacky if passing by reference?

				new_best_points.push_back(new_point_a); //add the connecting location on line a
			}
		}
	}

	//After checking all points on line a, attempt to connect to one of the points in between lines a and b to find a better route.
	//Since the line won't be complete by connecting to one of these points, we recursively call this function to connect to 'a' from 
	//the inner point
	for (int i = 0; i < inbetween_points.size(); i++)
	{
		//make sure that we don't accidentally divide by 0 (I don't think this is possible here, but check anyway for now)
		//or go back to any inner points that have already been checked
		if (inbetween_points[i].x <= current_graph_point.x) continue;

		//Find the first point on line 'a' that's after the inbetween point, calculate the lower slope bound off of this point
		long double lower_slope_bound = 0;
		for (int j = 0; j < optimal_path_a.size(); j++)
		{
			if (optimal_path_a[j].x > inbetween_points[i].x)
			{
				lower_slope_bound = (optimal_path_a[j].slope_numerator / optimal_path_a[j].slope_denominator);
				break;
			}
		}

		long double new_segment_slope = (inbetween_points[i].y - current_graph_point.y) / (inbetween_points[i].x - current_graph_point.x);
		if ((new_segment_slope >= (current_graph_point.slope_numerator / current_graph_point.slope_denominator)) || (new_segment_slope < lower_slope_bound)) continue;

		//Since the slope is in the allowable range, we create a new graph point that has the same coordinates as the current in-between point,
		//but also add the necessary slope, area, distance and increment data in case this point becomes an actual location on the optimal line.
		//Since the in-between points aren't actually on the graph, they don't feature any of this data
		GraphPoint new_inner_point = { inbetween_points[i].x, inbetween_points[i].y, 0, 0, 0, 0, 0 };
		new_inner_point.slope_numerator = new_inner_point.y - current_graph_point.y;
		new_inner_point.slope_denominator = new_inner_point.x - current_graph_point.x;

		//make sure the slope of new point is comprised of co-prime numbers
		int slope_gcd = gcd((int)new_inner_point.slope_numerator, (int)new_inner_point.slope_denominator);
		if (slope_gcd > 1)
		{
			new_inner_point.slope_numerator /= slope_gcd;
			new_inner_point.slope_denominator /= slope_gcd;
		}

		new_inner_point.distance = sqrt((new_inner_point.y - current_graph_point.y) * (new_inner_point.y - current_graph_point.y) + (new_inner_point.x - current_graph_point.x) * (new_inner_point.x - current_graph_point.x));
		new_inner_point.area = (new_inner_point.x - current_graph_point.x) * (current_graph_point.y + (new_inner_point.y - current_graph_point.y) / 2.0);
		new_inner_point.intersections = new_inner_point.distance / (sqrt(new_inner_point.slope_numerator * new_inner_point.slope_numerator + new_inner_point.slope_denominator * new_inner_point.slope_denominator));

		//If we can go to the inbetween point, then we recursively call this function with updated parameters. When calling recursively set the current_point
		//variable to 1 as the order of the inbetween points won't necessarily line up with the optimal line points and we want to try all possible options
		new_inner_points.push_back(new_inner_point);
		optimalLineRatio(optimal_path_a, inbetween_points, new_inner_point, new_inner_point.slope_numerator / new_inner_point.slope_denominator, lower_slope_bound, new_best_points, new_inner_points, best_ratio, current_total_distance + new_inner_point.distance,
			current_total_area + new_inner_point.area, better_path_found, 1);
		new_inner_points.pop_back();
	}
}

bool linesEqual(std::vector<GraphPoint> line_a, std::vector<GraphPoint> line_b)
{
	if (line_a.size() != line_b.size()) return false;

	for (int i = 0; i < line_a.size(); i++)
	{
		if (line_a[i].x != line_b[i].x) return false;
		if (line_a[i].y != line_b[i].y) return false;
	}

	return true;
}

std::pair<std::string, long double> q314()
{
	auto run_time = std::chrono::steady_clock::now();

	int grid_size = 250;

	std::vector<GraphPoint> optimal_path_a, optimal_path_b, inbetween_points, new_best_points, new_inner_points;
	bool cont = true;
	int y = grid_size - 2;

	//each graph point has the form {x, y, m_n, m_d, distance, area, intersections}
	optimal_path_a.push_back({ 0, grid_size - 1.0, (long double)grid_size, 1, 0, 0, 0 });
	optimal_path_a.push_back({ 0.5, grid_size - 0.5, 1.0, 1.0, sqrt(2) / 2.0, (grid_size - 1) / 2.0 + 0.125, 1 });
	optimal_path_a.push_back({ (long double)grid_size, (long double)grid_size, 0, 0, 0, ((grid_size - .5) * (grid_size - .5)) / 2.0, 0 }); //an imaginary point to hold the any area not under the other points

	optimal_path_b.push_back({ 0, grid_size - 2.0, (long double)grid_size, 1, 0, 0, 0 });
	optimal_path_b.push_back({ 1, grid_size - 1.0, 1.0, 1.0, sqrt(2), grid_size - 1.5, 1 });
	optimal_path_b.push_back({ (long double)grid_size, (long double)grid_size, 0, 0, 0, ((grid_size - 1) * (grid_size - 1)) / 2.0, 0 }); //an imaginary point to hold the any area not under the other points

	long double best_ratio = (optimal_path_a[0].area + optimal_path_a[1].area + optimal_path_a[2].area) / (grid_size - 1 + optimal_path_a[0].distance + optimal_path_a[1].distance + optimal_path_a[2].distance);

	while (cont)
	{
		cont = false; //set to false every iteration, if it isn't set to true in below function then we're done

		std::vector<GraphPoint> base, temp_optimal_b;

		//calculate the current ratio for line b (make sure to include y for the perimeter, which isn't included by default)
		long double current_ratio_numerator = 0, current_ratio_denominator = y, current_ratio = 0;
		for (int i = 1; i < optimal_path_b.size(); i++)
		{
			current_ratio_numerator += optimal_path_b[i].area;
			current_ratio_denominator += optimal_path_b[i].distance;
		}
		current_ratio = current_ratio_numerator / current_ratio_denominator;

		//before trying to create a new path, see if path b is better than path a outright.
		if (current_ratio > best_ratio)
		{
			cont = true;
			best_ratio = current_ratio;
			temp_optimal_b = optimal_path_b;
		}

		//the last point is imaginary so we don't check it, the last real point will always be on the line Y = grid_size - X
		//so we also don't need to check it, this is why below loop only goes to size() - 2
		long double current_total_distance = y, current_total_area = 0;
		for (int i = 0; i < optimal_path_b.size() - 2; i++)
		{
			bool better_path_found = false;
			base.push_back(optimal_path_b[i]); //add current point to base vector, which is used to construct new optimal_b vector

			current_total_distance += optimal_path_b[i].distance;
			current_total_area += optimal_path_b[i].area;

			optimalLineRatio(optimal_path_a, inbetween_points, optimal_path_b[i], optimal_path_b[i].slope_numerator / optimal_path_b[i].slope_denominator,
				optimal_path_b[i + 1].slope_numerator / optimal_path_b[i + 1].slope_denominator, new_best_points, new_inner_points, current_ratio,
				current_total_distance, current_total_area, better_path_found, i + 1);

			if (better_path_found)
			{
				//"current_ratio" keeps track of the best ratio found so far for line b while "best_ratio" is the best ratio that exists so far between
				//both lines a and b. If current_ratio is more than best_ratio then we've proven line b is better than line a so we set the cont variable 
				//to true which will perpetuate the algorithm
				if (current_ratio > best_ratio)
				{
					cont = true;
					best_ratio = current_ratio;
				}

				//regardless of whether or not current_ratio is better than best_ratio, if the better_path_found variable is set 
				//to true it means we've improved on our existing best for line b so we update the temp_optimal_b vector
				temp_optimal_b = base;
				for (int j = 0; j < new_best_points.size(); j++) temp_optimal_b.push_back(new_best_points[j]); //adding these points will connect us to line a
				for (int j = 1; j < optimal_path_a.size(); j++)
				{
					//add on points from line a that are further up and to the right than last point in temp_optimal_b
					if ((optimal_path_a[j].x > temp_optimal_b.back().x) && (optimal_path_a[j].y > temp_optimal_b.back().y))
					{
						//we need to make sure we didn't connect to the previous line in the middle of a segment, if we did, then we
						//need to lower the length and area of the segment that got intersected.
						if (optimal_path_a[j - 1].x != temp_optimal_b.back().x)
						{
							//keep the same endpoint and slope as optimal_path a[j], but alter the distance, area and increment values
							GraphPoint fractional_point = { optimal_path_a[j].x , optimal_path_a[j].y, optimal_path_a[j].slope_numerator, optimal_path_a[j].slope_denominator, 0, 0, 0 };
							fractional_point.distance = sqrt((optimal_path_a[j].x - temp_optimal_b.back().x) * (optimal_path_a[j].x - temp_optimal_b.back().x)
								+ (optimal_path_a[j].y - temp_optimal_b.back().y) * (optimal_path_a[j].y - temp_optimal_b.back().y));
							fractional_point.area = (optimal_path_a[j].x - temp_optimal_b.back().x) * (temp_optimal_b.back().y + (optimal_path_a[j].y - temp_optimal_b.back().y) / 2.0);
							fractional_point.intersections = optimal_path_a[j].distance / sqrt(optimal_path_a[j].slope_numerator * optimal_path_a[j].slope_numerator + optimal_path_a[j].slope_denominator * optimal_path_a[j].slope_denominator);
							temp_optimal_b.push_back(fractional_point);
						}
						else temp_optimal_b.push_back(optimal_path_a[j]);
					}
				}
			}

			//clear out best point vectors before next iteration
			new_best_points.clear();
			new_inner_points.clear();
		}

		if (cont)
		{
			if (!linesEqual(temp_optimal_b, optimal_path_b))
			{
				//it was more efficient for path b to merge back into path a, this means we need to look at 
				//the newly shaped line and calculate whether or not any vertices will fall in between successive lines.
				inbetween_points = findInbetweenPoints(temp_optimal_b);
			}
			else
			{
				//the optimal line ended up being the same as line a but just shifted down one, this means we need 
				//to shift the y-axis values for all of inbetween points
				for (int i = 0; i < inbetween_points.size(); i++) inbetween_points[i].y--;
			}

			//set optimal path a to be equal to the new optimal path b and then make optimal path b the same as
			//optimal path a but with all y-axis values lowered.
			optimal_path_a = temp_optimal_b;
			optimal_path_b = optimal_path_a;
			for (int i = 0; i < optimal_path_b.size() - 1; i++) optimal_path_b[i].y--;
			for (int i = 1; i < optimal_path_b.size() - 1; i++) optimal_path_b[i].area -= (optimal_path_b[i].x - optimal_path_b[i - 1].x);

			//the final point in the optimal path of b needs to get extended so that it reaches the y = graph_size - x line. There are two 
			//ways to do this, either the slope of the final line segment is 1 and we just extend it, or, the final line segment slope is 
			//something other than 1 so we need to add a new point that DOES have a slope of 1.
			if (optimal_path_b[optimal_path_b.size() - 2].slope_numerator == 1)
			{
				optimal_path_b[optimal_path_b.size() - 2].x += .5;
				optimal_path_b[optimal_path_b.size() - 2].y += .5;
				optimal_path_b[optimal_path_b.size() - 2].distance += sqrt(2) / 2.0;
				optimal_path_b[optimal_path_b.size() - 2].area += .5 * (optimal_path_b[optimal_path_b.size() - 2].y - .5 + .25);

				//if we extended the line onto a grid point make sure to increment the intersections variable
				if ((int)(optimal_path_b[optimal_path_b.size() - 2].x + .5) == (int)optimal_path_b[optimal_path_b.size() - 2].x) optimal_path_b[optimal_path_b.size() - 2].intersections++;
			}
			else
			{
				GraphPoint extra_point = { optimal_path_b[optimal_path_b.size() - 2].x + .5, optimal_path_b[optimal_path_b.size() - 2].y + .5, 1.0, 1.0, sqrt(2) / 2.0,
					.5 * optimal_path_b[optimal_path_b.size() - 2].y + .125, 0 };
				optimal_path_b.insert(optimal_path_b.end() - 1, extra_point);
			}
			optimal_path_b.back().area = (optimal_path_b[optimal_path_b.size() - 2].y * optimal_path_b[optimal_path_b.size() - 2].y) / 2.0;
		}

		y--; //decrement y to the next value
	}

	//manage precision of answer before returning
	std::ostringstream best_ratio_with_precision;
	best_ratio_with_precision << std::fixed;
	best_ratio_with_precision << std::setprecision(8);
	best_ratio_with_precision << best_ratio;
	std::string answer = best_ratio_with_precision.str();

	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 132.52756426
	//ran in 0.198797 seconds
}

//NOTES:
//Older more brute force attempts
//
//int pathOptions = 0;
//
//void bruteForceCheck(int gridSize, double slope, std::vector<std::pair<int, int>> currentPoints,
//	double currentArea, double currentPerimeter, std::vector<std::pair<int, int>>& currentBestPoints, double& currentBestRatio)
//{
//	//for the brute force check we force the fence to only 
//	//move up and to the right. Every time we move to the right
//	//we add to the current area
//
//	//std::cout << "(" << currentPoints.back().first << ", " << currentPoints.back().second << ")" << std::endl;
//	//the slope variable should be equal to gridSize when this function is first called
//
//	//if ((currentPoints.back().first == gridSize) && (currentPoints.back().second == gridSize))
//	if (currentPoints.back().second == (gridSize - currentPoints.back().first))
//	{
//		pathOptions++;
//		//we're at the top right of the grid so we're done with this set
//		if ((currentArea / (2 * currentPerimeter)) > currentBestRatio)
//		{
//			currentBestRatio = (currentArea / (2 * currentPerimeter));
//			currentBestPoints = currentPoints;
//
//			/*for (int i = 1; i < currentBestPoints.size(); i++) std::cout << ((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) / ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first) << ", ";
//			std::cout << std::endl;
//			for (int i = 1; i < currentBestPoints.size(); i++) std::cout << sqrt(((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) * ((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) + ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first) * ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first))  << ", ";
//			std::cout << std::endl;*/
//		}
//		return;
//	}
//
//	//x always must move by 1, i.e. we can't move straight up. 
//	for (int x = 1; x <= (gridSize - currentPoints.back().second - currentPoints.back().first) / 2; x++)
//	{
//		//the value we choose for y is limited by three things. First, it must be 
//		//small enough that the slope of the new segment is less than the slope
//		//of the previous segment. Second, it must be below the line Y = X. Third,
//		//it must make the slope if the current segment greater than 45 degrees.
//
//		//Calculate maximum y value depending on the lesser limiting value, Y = X 
//		//line or previous segment slope. The -.000001 is to ensure that stay below
//		//the previous slope, whole numbers will get rounded down to the next integer
//		//while decimal numbers should get floored to the current integer.
//		int max_y = (gridSize - (currentPoints.back().first + x)) > (slope * x + currentPoints.back().second) ? slope * x + currentPoints.back().second - .000001 : gridSize - (currentPoints.back().first + x);
//		for (int y = x + 1; y <= (max_y - currentPoints.back().second); y++)
//		{
//			//if (gcd(x, y) > 1) continue;
//			currentPoints.push_back({ currentPoints.back().first + x, currentPoints.back().second + y });
//
//			bruteForceCheck(gridSize, (double)y / (double)x, currentPoints, currentArea - x * (x + y + (2 * (gridSize - (currentPoints.back().first + currentPoints.back().second)))),
//				currentPerimeter + sqrt(x * x + y * y), currentBestPoints, currentBestRatio);
//
//			currentPoints.pop_back();
//		}
//	}
//
//	//After trying all slopes greater than 45 degrees, we do one final check from the current spot and make a 45
//	//degree line to the same point on the other side of the line Y = X
//	double slope45X = (gridSize - (currentPoints.back().second - currentPoints.back().first)) / 2.0, slope45Y = gridSize - slope45X;
//	double slope45Area = currentArea - (slope45X - currentPoints.back().first) * ((slope45X - currentPoints.back().first) + (slope45Y - currentPoints.back().second) + (2 * (gridSize - (slope45X + slope45Y))));
//	double slope45Perimeter = 2 * currentPerimeter + sqrt(2 * (gridSize - currentPoints.back().first - currentPoints.back().second) * (gridSize - currentPoints.back().first - currentPoints.back().second));
//	pathOptions++;
//	if (slope45Area / slope45Perimeter > currentBestRatio)
//	{
//		currentBestRatio = slope45Area / slope45Perimeter;
//		currentBestPoints = currentPoints;
//
//		/*for (int i = 1; i < currentBestPoints.size(); i++) std::cout << ((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) / ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first) << ", ";
//		std::cout << 1 << std::endl;
//		for (int i = 1; i < currentBestPoints.size(); i++) std::cout << sqrt(((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) * ((double)currentBestPoints[i].second - (double)currentBestPoints[i - 1].second) + ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first) * ((double)currentBestPoints[i].first - (double)currentBestPoints[i - 1].first)) << ", ";
//		std::cout << (sqrt(2 * (gridSize - currentPoints.back().first - currentPoints.back().second) * (gridSize - currentPoints.back().first - currentPoints.back().second))) / 2.0 << std::endl;
//		*/
//		currentBestPoints.push_back({ -1, -1 }); //this will signify to me that a reflection occured for the last point
//
//	}
//
//	//improvements to this algorithm
//	//1. Keep track of current slope, when choosing the next point it must have a slope less than the slope of previous segment
//	//2. The slope of the new segment must be greater than 45 degrees (not greater than or equal).
//	//3. After testing all slopes between previous segment slope and 45 degrees, a finisher at 45 degrees is tested
//}
//
//void quickPathFind(int gridSize, double slope, int current_x, int current_y, double currentArea, double currentPerimeter, double& currentBestRatio)
//{
//	//Same as the function above but doesn't keep track of the physical points. This is faster and takes less memory
//	if (current_y == (gridSize - current_x))
//	{
//		//we're at the top right of the grid so we're done with this set
//		if ((currentArea / (2 * currentPerimeter)) > currentBestRatio)
//		{
//			currentBestRatio = (currentArea / (2 * currentPerimeter));
//			//std::cout << "yeet" << std::endl;
//		}
//		return;
//	}
//
//	//x always must move by 1, i.e. we can't move straight up. 
//	for (int x = 1; x <= (gridSize - current_y - current_x) / 2; x++)
//	{
//		//the value we choose for y is limited by three things. First, it must be 
//		//small enough that the slope of the new segment is less than the slope
//		//of the previous segment. Second, it must be below the line Y = X. Third,
//		//it must make the slope if the current segment greater than 45 degrees.
//
//		//Calculate maximum y value depending on the lesser limiting value, Y = X 
//		//line or previous segment slope. The -.000001 is to ensure that stay below
//		//the previous slope, whole numbers will get rounded down to the next integer
//		//while decimal numbers should get floored to the current integer.
//		int max_y = (gridSize - (current_x + x)) > (slope * x + current_y) ? slope * x + current_y - .000001 : gridSize - (current_x + x);
//		for (int y = x + 1; y <= (max_y - current_y); y++)
//		{
//			if (gcd(x, y) > 1) continue;
//			quickPathFind(gridSize, (double)y / (double)x, current_x + x, current_y + y, currentArea - x * (x + y + (2 * (gridSize - (current_x + x + current_y + y)))),
//				currentPerimeter + sqrt(x * x + y * y), currentBestRatio);
//
//		}
//	}
//
//	//After trying all slopes greater than 45 degrees, we do one final check from the current spot and make a 45
//	//degree line to the same point on the other side of the line Y = X
//	double slope45X = (gridSize - (current_y - current_x)) / 2.0, slope45Y = gridSize - slope45X;
//	double slope45Area = currentArea - (slope45X - current_x) * ((slope45X - current_x) + (slope45Y - current_y) + (2 * (gridSize - (slope45X + slope45Y))));
//	double slope45Perimeter = 2 * currentPerimeter + sqrt(2 * (gridSize - current_x - current_y) * (gridSize - current_x - current_y));
//	if (slope45Area / slope45Perimeter > currentBestRatio)
//	{
//		currentBestRatio = slope45Area / slope45Perimeter;
//		//std::cout << "yeet" << std::endl;
//	}
//}
//
//void quickerPathFind(int gridSize, std::vector<fraction>& allowable_slopes, int slope_index, int current_x, int current_y, double currentArea, double currentPerimeter, double& currentBestRatio)
//{
//	//Same as the function above but finds the next point by looking at coprime pairs representing allowable slopes
//	if (current_y == (gridSize - current_x))
//	{
//		//we're at the top right of the grid so we're done with this set
//		if ((currentArea / (2 * currentPerimeter)) > currentBestRatio)
//		{
//			currentBestRatio = (currentArea / (2 * currentPerimeter));
//			//std::cout << "yeet" << std::endl;
//		}
//		return;
//	}
//
//	//slope must be less than current slope and more than 1
//	for (int m = slope_index + 2; m < allowable_slopes.size(); m++)
//	{
//		for (int gcd = 1; gcd <= 3; gcd++)
//		{
//			//in previous testing I found that for the optimal path, GCD(y1 - y0, x1 - x0) was a maximum
//			//of 2 for any given line segment making up the path
//			int x = allowable_slopes[m].numerator * gcd, y = allowable_slopes[m].denominator * gcd;
//			if ((current_y + y) > (gridSize - (current_x + x))) break; //we're over the y = gridSize - x line
//			quickerPathFind(gridSize, allowable_slopes, m, current_x + x, current_y + y, currentArea - x * (x + y + (2 * (gridSize - (current_x + x + current_y + y)))),
//				currentPerimeter + sqrt(x * x + y * y), currentBestRatio);
//		}
//	}
//
//	//After trying all slopes greater than 45 degrees, we do one final check from the current spot and make a 45
//	//degree line to the same point on the other side of the line Y = X
//	double slope45X = (gridSize - (current_y - current_x)) / 2.0, slope45Y = gridSize - slope45X;
//	double slope45Area = currentArea - (slope45X - current_x) * ((slope45X - current_x) + (slope45Y - current_y) + (2 * (gridSize - (slope45X + slope45Y))));
//	double slope45Perimeter = 2 * currentPerimeter + sqrt(2 * (gridSize - current_x - current_y) * (gridSize - current_x - current_y));
//	if (slope45Area / slope45Perimeter > currentBestRatio)
//	{
//		currentBestRatio = slope45Area / slope45Perimeter;
//		//std::cout << "yeet" << std::endl;
//	}
//}

//std::pair<std::string, double> q314()
//{
//	auto run_time = std::chrono::steady_clock::now();
//
//	long long answer = 0;
//
//	std::vector<std::pair<int, int> > startingPoints, bestPoints;
//	startingPoints.push_back({ 0, 0 });
//
//	int grid_size = 100, start_point = grid_size / 2 - 15, end_point = grid_size / 2 + 4;
//	if (start_point < 0) start_point = 0;
//	if (end_point > grid_size) end_point = grid_size;
//	double bestRatio = 0;
//
//	double maximumExpectedSlope = grid_size; //TODO: Try and find the actual equation for this, this holds true when grid_size = 150
//
//	//find all coprime pairs up to the square root of our grid size
//	std::vector<fraction> coprimePairs;
//	orderedFaraySequence(maximumExpectedSlope, coprimePairs);
//
//	for (int i = 100; i >= 50; i--)
//	{
//		std::cout << i << std::endl;
//		startingPoints[0].second = i;
//		bruteForceCheck(grid_size, maximumExpectedSlope, startingPoints, grid_size * grid_size, i, bestPoints, bestRatio);
//		//quickPathFind(grid_size, maximumExpectedSlope, 0, i, grid_size * grid_size, i, bestRatio);
//		//quickerPathFind(grid_size, coprimePairs, -2, 0, i, grid_size * grid_size, i, bestRatio);
//		for (int i = 0; i < bestPoints.size(); i++) std::cout << "(" << bestPoints[i].first << ", " << bestPoints[i].second << ")" << std::endl;
//		std::cout << bestRatio << std::endl;
//		std::cout << std::endl;
//	}
//
//	std::cout << pathOptions << " paths were checked." << std::endl;
//
//	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
//}