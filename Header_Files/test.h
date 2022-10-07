#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <iostream>
#include <Header_Files/functions.h>
#include <iomanip>

//struct GraphPoint
//{
//	long double x = 0; //x value of point
//	long double y = 0; //y value of point
//	long double slope_numerator = 0; //the slope between this point and the previous point on the graph
//	long double slope_denominator = 1;
//	long double distance = 0; //the distance between this point and the previous on the graph
//	long double area = 0; //the are between this point and the previous on the graph
//	int intersections = 0; //the number of grid points crossed before reaching previous point
//};
//
////void optimalLineRatio(std::vector<GraphPoint> &optimal_path_a, std::vector<GraphPoint>& optimal_path_b,
////	std::vector<GraphPoint>& inbetween_points, GraphPoint current_graph_point, long double& best_ratio, long double current_total_distance, long double current_total_area, int y, bool& better_path_found, int current_point = 1)
////{
////	//attempt to connect the current point on path b all points on path a and inbetween the two
////	for (int i = current_point; i < optimal_path_b.size() - 1; i++) //don't go to last point as it's imaginary
////	{
////		//first try all points on path a, start search at the current point as we'd never pick a point behind the current one
////		for (int j = current_point; j < optimal_path_a.size() - 1; j++) //don't go to last point as it's imaginary
////		{
////			long double new_segment_slope = (optimal_path_a[j].y - optimal_path_b[i].y) / (optimal_path_a[j].x - optimal_path_b[i].x);
////			if (new_segment_slope >= optimal_path_a[j].slope || new_segment_slope < optimal_path_b[i].slope) continue;
////
////			//The new line segment meets the slope requirements. Since it connects to the original line it's path will no longer deviate.
////			//Iterate through the remaining points of the original line to see if the new ratio is better than the old one.
////
////			long double new_total_distance = current_total_distance + sqrt((optimal_path_a[j].y - optimal_path_b[i].y) * (optimal_path_a[j].y - optimal_path_b[i].y)
////			+ (optimal_path_a[j].x - optimal_path_b[i].x) * (optimal_path_a[j].x - optimal_path_b[i].x));
////			long double new_total_area = current_total_area + (optimal_path_a[j].x - optimal_path_b[i].x) * (optimal_path_b[i].y + optimal_path_a[j].y / 2.0);
////
////			for (int k = j + 1; k < optimal_path_a.size() - 1; k++)
////			{
////				new_total_distance += optimal_path_a[k].distance;
////				new_total_area += optimal_path_a[k].area;
////			}
////
////			//don't forget to add the final area at the end
////			new_total_area += optimal_path_a.back().distance;
////
////			//compare the new ratio to the existing best and if it's better, update it
////			long double new_ratio = new_total_area / new_total_distance;
////			if (new_ratio > best_ratio)
////			{
////				best_ratio = new_ratio;
////				better_path_found = true;
////			}
////		}
////
////		//after attempting to go to points on the previous line, attempt to go to points in between the two lines
////		for (int j = 0; j < inbetween_points.size(); j++)
////		{
////			//make sure that we don't accidentally divide by 0
////			if (current_graph_point.x == inbetween_points[j].x) continue;
////
////			long double new_segment_slope = (inbetween_points[j].y - optimal_path_b[i].y) / (inbetween_points[j].x - optimal_path_b[i].x);
////			if (new_segment_slope >= optimal_path_a[j].slope || new_segment_slope < optimal_path_b[i].slope) continue;
////
////			//If we can go to the inbetween point, then we recursively call this function with updated parameters
////			optimalLineRatio(optimal_path_a, optimal_path_b, inbetween_points)
////		}
////
////		//add the distance and area affect from the new point i of line b to the current values as from this point on
////		//the point won't change
////		current_distance += optimal_path_b[i].distance;
////		current_area += optimal_path_b[i].area;
////	}
////}
//
//std::vector<GraphPoint> findInbetweenPoints(std::vector<GraphPoint> line)
//{
//	//looks at "line" which is composed of different line segments and answers the question,
//	//if an identical line was placed 1 space below this one on the Cartesean plane, what 
//	//graph points would fall in between the two lines? Only segments of the line that 
//	//have a slope with decimal value will be able to have points fall between them.
//
//	std::vector<GraphPoint> inbetween_points;
//	for (int i = 1; i < line.size() - 1; i++)
//	{
//		if (((int)line[i].slope_numerator % (int)line[i].slope_denominator) != 0)
//		{
//			//move forwards in increments of x = 1 from the previous point
//			//on the line to the current point. There should be an inbetween
//			//point at each of these x values.
//			long double current_y = line[i - 1].y;
//
//			for (int j = line[i - 1].x + 1; j < line[i].x; j++)
//			{
//				current_y += (line[i].slope_numerator / line[i].slope_denominator);
//				inbetween_points.push_back({ (long double)j, floor(current_y) });
//			}
//		}
//	}
//
//	return inbetween_points;
//}
//
//void optimalLineRatio(std::vector<GraphPoint>& optimal_path_a,  std::vector<GraphPoint>& inbetween_points, GraphPoint current_graph_point, long double upper_slope, long double lower_slope, std::vector<GraphPoint>& new_best_points,
//	std::vector<GraphPoint>& new_inner_points, long double& best_ratio, long double current_total_distance, long double current_total_area, bool& better_path_found, int current_point = 1)
//{
//	//attempt to connect the current point to all points on path a
//	for (int i = current_point; i < optimal_path_a.size() - 1; i++) //don't go to last point as it's imaginary
//	{
//		//if the current line segment from path a crosses over multiple grid points, we need to try and visit all of them
//		for (int j = 0; j < optimal_path_a[i].intersections; j++)
//		{
//			//if line segment of a doesn't have multiple segments then a_x and a_y will just be the end point of the segment.
//			//There are instances though where line segments of 'a' can pass through multiple grid points (i.e. a line with 
//			//a slope of 2 travelling from (0, 0) to (2, 4) would also intersect with (1, 2) so we need to make sure we test
//			//the (1, 2) point as well as the (2, 4) point.
//			GraphPoint new_point_a = { optimal_path_a[i].x - ((long double)j * optimal_path_a[i].slope_denominator), optimal_path_a[i].y - ((long double)j * optimal_path_a[i].slope_numerator), 0, 0, 0, 0, 0 };
//
//			//check to see if the slop of the current segment of 'a' is 1, it's possible for these lines to terminate in a spot that
//			//isn't a grid point (because we reflect our final line of the y = grid_size - x axis). If this case, make sure 
//			if (optimal_path_a[i].slope_numerator == 1)
//			{
//				if ((int)(new_point_a.x + .5) != (int)new_point_a.x)
//				{
//					new_point_a.x -= .5;
//					new_point_a.y -= .5;
//				}
//			}
//
//			if (new_point_a.x <= current_graph_point.x) continue; //we wouldn't travel to any points behind the current one or directly above (TODO: this may be unneccessary by setting current point correctly)
//
//			//The slope of the newly formed segment must be less than the previous segment, but greater than or equal to the current segment slope
//			long double new_segment_slope = (new_point_a.y - current_graph_point.y) / (new_point_a.x - current_graph_point.x); //delta_y / delta_x
//			//if ((new_segment_slope >= upper_slope) || (new_segment_slope < (optimal_path_a[i].slope_numerator / optimal_path_a[i].slope_denominator))) continue;
//			if ((new_segment_slope >= upper_slope) || (new_segment_slope < lower_slope)) continue;
//			if (new_segment_slope == 1) continue; //slope needs to be greater than 1
//			new_point_a.slope_numerator = new_point_a.y - current_graph_point.y;
//			new_point_a.slope_denominator = new_point_a.x - current_graph_point.x;
//
//			//make sure slope is made up of co-prime numbers
//			int slope_gcd = gcd((int)new_point_a.slope_numerator, (int)new_point_a.slope_denominator);
//			if (slope_gcd > 1)
//			{
//				new_point_a.slope_numerator /= slope_gcd;
//				new_point_a.slope_denominator /= slope_gcd;
//			}
//
//			//The new line segment meets the slope requirements. We now caclulate all the other aspects of this new line segment
//			//so that we can potentially update optimal_path_b
//			new_point_a.distance = sqrt((new_point_a.y - current_graph_point.y) * (new_point_a.y - current_graph_point.y) + (new_point_a.x - current_graph_point.x) * (new_point_a.x - current_graph_point.x));
//			new_point_a.area = (new_point_a.x - current_graph_point.x) * (current_graph_point.y + (new_point_a.y - current_graph_point.y) / 2.0);
//			new_point_a.intersections = new_point_a.distance / (sqrt(new_point_a.slope_numerator * new_point_a.slope_numerator + new_point_a.slope_denominator * new_point_a.slope_denominator));
//
//			//Increment the current total distance and area to reflect the addition of the new line segment
//			long double new_total_distance = current_total_distance + new_point_a.distance;
//			long double new_total_area = current_total_area + new_point_a.area;
//
//			//Add distance and area from partial point a_x and a_y to the actual point on line 'a'.
//			new_total_distance += sqrt((optimal_path_a[i].y - new_point_a.y) * (optimal_path_a[i].y - new_point_a.y) + (optimal_path_a[i].x - new_point_a.x) * (optimal_path_a[i].x - new_point_a.x));
//			new_total_area += (optimal_path_a[i].x - new_point_a.x) * (new_point_a.y + (optimal_path_a[i].y - new_point_a.y) / 2.0);
//
//			//Then tak on area and distance measurements for the rest of the points on line 'a'.
//			for (int k = i + 1; k < optimal_path_a.size() - 1; k++)
//			{
//				new_total_distance += optimal_path_a[k].distance;
//				new_total_area += optimal_path_a[k].area;
//			}
//
//			//don't forget to add the final area at the end from the imaginary point
//			new_total_area += optimal_path_a.back().area;
//
//			//compare the new ratio to the existing best and if it's better, update it
//			long double new_ratio = new_total_area / new_total_distance;
//			if (new_ratio > best_ratio)
//			{
//				best_ratio = new_ratio;
//				better_path_found = true;
//
//				//clear necessary points from the new_best_points vector and replace with this new point
//				if (!new_inner_points.size()) new_best_points.clear();
//				else new_best_points = new_inner_points; //Will this cause anything wacky if passing by reference?
//
//				new_best_points.push_back(new_point_a); //add the connecting location on line a
//			}
//		}
//	}
//
//	//After checking all points on line a, attempt to connect to one of the points in between lines a and b to find a better route.
//	//Since the line won't be complete by connecting to one of these points, we recursively call this function to connect to 'a' from 
//	//the inner point
//	for (int i = 0; i < inbetween_points.size(); i++)
//	{
//		//make sure that we don't accidentally divide by 0 (I don't think this is possible here, but check anyway for now)
//		//or go back to any inner points that have already been checked
//		if (inbetween_points[i].x <= current_graph_point.x) continue;
//
//		//Find the first point on line 'a' that's after the inbetween point, calculate the lower slope bound off of this point
//		long double lower_slope_bound = 0;
//		for (int j = 0; j < optimal_path_a.size(); j++)
//		{
//			if (optimal_path_a[j].x > inbetween_points[i].x)
//			{
//				lower_slope_bound = (optimal_path_a[j].slope_numerator / optimal_path_a[j].slope_denominator);
//				break;
//			}
//		}
//
//		long double new_segment_slope = (inbetween_points[i].y - current_graph_point.y) / (inbetween_points[i].x - current_graph_point.x);
//		if ((new_segment_slope >= (current_graph_point.slope_numerator / current_graph_point.slope_denominator)) || (new_segment_slope < lower_slope_bound)) continue;
//
//		//Since the slope is in the allowable range, we create a new graph point that has the same coordinates as the current in-between point,
//		//but also add the necessary slope, area, distance and increment data in case this point becomes an actual location on the optimal line.
//		//Since the in-between points aren't actually on the graph, they don't feature any of this data
//		GraphPoint new_inner_point = { inbetween_points[i].x, inbetween_points[i].y, 0, 0, 0, 0, 0 };
//		new_inner_point.slope_numerator = new_inner_point.y - current_graph_point.y;
//		new_inner_point.slope_denominator = new_inner_point.x - current_graph_point.x;
//
//		//make sure the slope of new point is comprised of co-prime numbers
//		int slope_gcd = gcd((int)new_inner_point.slope_numerator, (int)new_inner_point.slope_denominator);
//		if (slope_gcd > 1)
//		{
//			new_inner_point.slope_numerator /= slope_gcd;
//			new_inner_point.slope_denominator /= slope_gcd;
//		}
//
//		new_inner_point.distance = sqrt((new_inner_point.y - current_graph_point.y) * (new_inner_point.y - current_graph_point.y) + (new_inner_point.x - current_graph_point.x) * (new_inner_point.x - current_graph_point.x));
//		new_inner_point.area = (new_inner_point.x - current_graph_point.x) * (current_graph_point.y + (new_inner_point.y - current_graph_point.y) / 2.0);
//		new_inner_point.intersections = new_inner_point.distance / (sqrt(new_inner_point.slope_numerator * new_inner_point.slope_numerator + new_inner_point.slope_denominator * new_inner_point.slope_denominator));
//
//		//If we can go to the inbetween point, then we recursively call this function with updated parameters. When calling recursively set the current_point
//		//variable to 1 as the order of the inbetween points won't necessarily line up with the optimal line points and we want to try all possible options
//		new_inner_points.push_back(new_inner_point);
//		optimalLineRatio(optimal_path_a, inbetween_points, new_inner_point, new_inner_point.slope_numerator / new_inner_point.slope_denominator, lower_slope_bound, new_best_points, new_inner_points, best_ratio, current_total_distance + new_inner_point.distance,
//			current_total_area + new_inner_point.area, better_path_found, 1);
//		new_inner_points.pop_back();
//	}
//}
//
//bool linesEqual(std::vector<GraphPoint> line_a, std::vector<GraphPoint> line_b)
//{
//	if (line_a.size() != line_b.size()) return false;
//
//	for (int i = 0; i < line_a.size(); i++)
//	{
//		if (line_a[i].x != line_b[i].x) return false;
//		if (line_a[i].y != line_b[i].y) return false;
//	}
//
//	return true;
//}

std::pair<std::string, long double> test()
{
	auto run_time = std::chrono::steady_clock::now();
	//
	//int grid_size = 250;

	//std::vector<GraphPoint> optimal_path_a, optimal_path_b, inbetween_points, new_best_points, new_inner_points;
	//bool cont = true;
	//int y = grid_size - 2;

	////each graph point has the form {x, y, m_n, m_d, distance, area, intersections}
	//optimal_path_a.push_back({ 0, grid_size - 1.0, (long double)grid_size, 1, 0, 0, 0 });
	//optimal_path_a.push_back({ 0.5, grid_size - 0.5, 1.0, 1.0, sqrt(2) / 2.0, (grid_size - 1) / 2.0 + 0.125, 1 });
	//optimal_path_a.push_back({ (long double)grid_size, (long double)grid_size, 0, 0, 0, ((grid_size - .5) * (grid_size - .5)) / 2.0, 0 }); //an imaginary point to hold the any area not under the other points

	//optimal_path_b.push_back({ 0, grid_size - 2.0, (long double)grid_size, 1, 0, 0, 0 });
	//optimal_path_b.push_back({ 1, grid_size - 1.0, 1.0, 1.0, sqrt(2), grid_size - 1.5, 1 });
	//optimal_path_b.push_back({ (long double)grid_size, (long double)grid_size, 0, 0, 0, ((grid_size - 1) * (grid_size - 1)) / 2.0, 0 }); //an imaginary point to hold the any area not under the other points

	//long double best_ratio = (optimal_path_a[0].area + optimal_path_a[1].area + optimal_path_a[2].area) / (grid_size - 1 + optimal_path_a[0].distance + optimal_path_a[1].distance + optimal_path_a[2].distance);

	//while (cont)
	//{
	//	cont = false; //set to false every iteration, if it isn't set to true in below function then we're done

	//	std::vector<GraphPoint> base, temp_optimal_b;

	//	//calculate the current ratio for line b (make sure to include y for the perimeter, which isn't included by default)
	//	long double current_ratio_numerator = 0, current_ratio_denominator = y, current_ratio = 0;
	//	for (int i = 1; i < optimal_path_b.size(); i++)
	//	{
	//		current_ratio_numerator += optimal_path_b[i].area;
	//		current_ratio_denominator += optimal_path_b[i].distance;
	//	}
	//	current_ratio = current_ratio_numerator / current_ratio_denominator;

	//	//before trying to create a new path, see if path b is better than path a outright.
	//	if (current_ratio > best_ratio)
	//	{
	//		
	//		cont = true;
	//		best_ratio = current_ratio;
	//		temp_optimal_b = optimal_path_b;
	//	}

	//	//the last point is imaginary so we don't check it, the last real point will always be on the line Y = grid_size - X
	//	//so we also don't need to check it, this is why below loop only goes to size() - 2
	//	long double current_total_distance = y, current_total_area = 0;
	//	for (int i = 0; i < optimal_path_b.size() - 2; i++)
	//	{
	//		bool better_path_found = false;
	//		base.push_back(optimal_path_b[i]); //add current point to base vector, which is used to construct new optimal_b vector

	//		current_total_distance += optimal_path_b[i].distance;
	//		current_total_area += optimal_path_b[i].area;

	//		optimalLineRatio(optimal_path_a, inbetween_points, optimal_path_b[i], optimal_path_b[i].slope_numerator / optimal_path_b[i].slope_denominator,
	//			optimal_path_b[i + 1].slope_numerator / optimal_path_b[i + 1].slope_denominator, new_best_points, new_inner_points, current_ratio,
	//			current_total_distance, current_total_area, better_path_found, i + 1);

	//		if (better_path_found)
	//		{
	//			//"current_ratio" keeps track of the best ratio found so far for line b while "best_ratio" is the best ratio that exists so far between
	//			//both lines a and b. If current_ratio is more than best_ratio then we've proven line b is better than line a so we set the cont variable 
	//			//to true which will perpetuate the algorithm
	//			if (current_ratio > best_ratio)
	//			{
	//				cont = true;
	//				best_ratio = current_ratio;
	//			}

	//			/*if (y == 164)
	//			{
	//				std::cout << "Test level reached" << std::endl;
	//			}*/

	//			//regardless of whether or not current_ratio is better than best_ratio, if the better_path_found variable is set 
	//			//to true it means we've improved on our existing best for line b so we update the temp_optimal_b vector
	//			temp_optimal_b = base;
	//			for (int j = 0; j < new_best_points.size(); j++) temp_optimal_b.push_back(new_best_points[j]); //adding these points will connect us to line a
	//			for (int j = 1; j < optimal_path_a.size(); j++)
	//			{
	//				//add on points from line a that are further up and to the right than last point in temp_optimal_b
	//				if ((optimal_path_a[j].x > temp_optimal_b.back().x) && (optimal_path_a[j].y > temp_optimal_b.back().y))
	//				{
	//					//we need to make sure we didn't connect to the previous line in the middle of a segment, if we did, then we
	//					//need to lower the length and area of the segment that got intersected.
	//					if (optimal_path_a[j - 1].x != temp_optimal_b.back().x)
	//					{
	//						//keep the same endpoint and slope as optimal_path a[j], but alter the distance, area and increment values
	//						GraphPoint fractional_point = { optimal_path_a[j].x , optimal_path_a[j].y, optimal_path_a[j].slope_numerator, optimal_path_a[j].slope_denominator, 0, 0, 0 };
	//						fractional_point.distance = sqrt((optimal_path_a[j].x - temp_optimal_b.back().x) * (optimal_path_a[j].x - temp_optimal_b.back().x)
	//							+ (optimal_path_a[j].y - temp_optimal_b.back().y) * (optimal_path_a[j].y - temp_optimal_b.back().y));
	//						fractional_point.area = (optimal_path_a[j].x - temp_optimal_b.back().x) * (temp_optimal_b.back().y + (optimal_path_a[j].y - temp_optimal_b.back().y) / 2.0);
	//						fractional_point.intersections = optimal_path_a[j].distance / sqrt(optimal_path_a[j].slope_numerator * optimal_path_a[j].slope_numerator + optimal_path_a[j].slope_denominator * optimal_path_a[j].slope_denominator);
	//						temp_optimal_b.push_back(fractional_point);
	//					}
	//					else temp_optimal_b.push_back(optimal_path_a[j]);
	//				}
	//			}
	//		}

	//		//clear out best point vectors before next iteration
	//		new_best_points.clear();
	//		new_inner_points.clear();
	//	}

	//	if (cont)
	//	{
	//		if (!linesEqual(temp_optimal_b, optimal_path_b))
	//		{
	//			//it was more efficient for path b to merge back into path a, this means we need to look at 
	//			//the newly shaped line and calculate whether or not any vertices will fall in between successive lines.
	//			inbetween_points = findInbetweenPoints(temp_optimal_b);
	//		}
	//		else
	//		{
	//			//the optimal line ended up being the same as line a but just shifted down one, this means we need 
	//			//to shift the y-axis values for all of inbetween points
	//			for (int i = 0; i < inbetween_points.size(); i++) inbetween_points[i].y--;
	//		}

	//		//set optimal path a to be equal to the new optimal path b and then make optimal path b the same as
	//		//optimal path a but with all y-axis values lowered.
	//		optimal_path_a = temp_optimal_b;
	//		optimal_path_b = optimal_path_a;
	//		for (int i = 0; i < optimal_path_b.size() - 1; i++) optimal_path_b[i].y--;
	//		for (int i = 1; i < optimal_path_b.size() - 1; i++) optimal_path_b[i].area -= (optimal_path_b[i].x - optimal_path_b[i - 1].x);

	//		/*std::cout << "[0, " << y << "]: " << std::endl;
	//		for (int i = 1; i < optimal_path_a.size() - 1; i++)
	//		{
	//			std::cout << "[" << optimal_path_a[i].x << ", " << optimal_path_a[i].y << "]: calculated p = " << optimal_path_a[i].distance <<
	//				", actual p = " << sqrt((optimal_path_a[i].x - optimal_path_a[i - 1].x) * (optimal_path_a[i].x - optimal_path_a[i - 1].x) +
	//					(optimal_path_a[i].y - optimal_path_a[i - 1].y) * (optimal_path_a[i].y - optimal_path_a[i - 1].y)) << std::endl;
	//		}
	//		std::cout << std::endl;*/
	//		//std::cout << "Start point [0, " << y << "]: Best ratio = " << best_ratio << std::endl;

	//		//the final point in the optimal path of b needs to get extended so that it reaches the y = graph_size - x line. There are two 
	//		//ways to do this, either the slope of the final line segment is 1 and we just extend it, or, the final line segment slope is 
	//		//something other than 1 so we need to add a new point that DOES have a slope of 1.
	//		if (optimal_path_b[optimal_path_b.size() - 2].slope_numerator == 1)
	//		{
	//			optimal_path_b[optimal_path_b.size() - 2].x += .5;
	//			optimal_path_b[optimal_path_b.size() - 2].y += .5;
	//			optimal_path_b[optimal_path_b.size() - 2].distance += sqrt(2) / 2.0;
	//			optimal_path_b[optimal_path_b.size() - 2].area += .5 * (optimal_path_b[optimal_path_b.size() - 2].y - .5 + .25);

	//			//if we extended the line onto a grid point make sure to increment the intersections variable
	//			if ((int)(optimal_path_b[optimal_path_b.size() - 2].x + .5) == (int)optimal_path_b[optimal_path_b.size() - 2].x) optimal_path_b[optimal_path_b.size() - 2].intersections++;
	//		}
	//		else
	//		{
	//			GraphPoint extra_point = { optimal_path_b[optimal_path_b.size() - 2].x + .5, optimal_path_b[optimal_path_b.size() - 2].y + .5, 1.0, 1.0, sqrt(2) / 2.0,
	//				.5 * optimal_path_b[optimal_path_b.size() - 2].y + .125, 0 };
	//			optimal_path_b.insert(optimal_path_b.end() - 1, extra_point);
	//		}
	//		optimal_path_b.back().area = (optimal_path_b[optimal_path_b.size() - 2].y * optimal_path_b[optimal_path_b.size() - 2].y) / 2.0;
	//	}

	//	y--; //decrement y to the next value
	//}

	///*long double area_total = 0;
	//long double perimeter_total = optimal_path_a[0].y;
	//std::cout << "The optimal route is: " << std::endl;
	//std::cout << "[0, " << optimal_path_a[0].y << "] a = " << optimal_path_a[0].area <<
	//	", p = " << optimal_path_a[0].y << std::endl;

	//for (int i = 1; i < optimal_path_a.size() - 1; i++)
	//{
	//	std::cout << "[" << optimal_path_a[i].x << ", " << optimal_path_a[i].y << "] a = " << optimal_path_a[i].area << 
	//		", p = sqrt(" << (optimal_path_a[i].x - optimal_path_a[i - 1].x) * (optimal_path_a[i].x - optimal_path_a[i - 1].x) +
	//		(optimal_path_a[i].y - optimal_path_a[i - 1].y) * (optimal_path_a[i].y - optimal_path_a[i - 1].y) << ") = " << optimal_path_a[i].distance
	//		<< " vs. " << sqrt((optimal_path_a[i].x - optimal_path_a[i - 1].x) * (optimal_path_a[i].x - optimal_path_a[i - 1].x) +
	//			(optimal_path_a[i].y - optimal_path_a[i - 1].y) * (optimal_path_a[i].y - optimal_path_a[i - 1].y)) << std::endl;
	//	area_total += optimal_path_a[i].area;
	//	perimeter_total += optimal_path_a[i].distance;
	//}
	//std::cout << std::endl;
	//std::cout << "Total area = " << area_total + optimal_path_a.back().area << std::endl;
	//std::cout << "Total perimeter = " << perimeter_total << std::endl;
	//std::cout << "Raw perimeter sum = " << 126 + sqrt(145) + sqrt(50) + sqrt(26) + sqrt(17) + sqrt(53) + sqrt(45) + sqrt(370) + sqrt(34) + sqrt(13) + sqrt(100) + sqrt(85) + sqrt(32) << std::endl;*/

	////manage precision of answer before returning
	//std::ostringstream best_ratio_with_precision;
	//best_ratio_with_precision << std::fixed;
	//best_ratio_with_precision << std::setprecision(8);
	//best_ratio_with_precision << best_ratio;
	std::string answer = "yeet";
	
	return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 132.52756426
	//ran in 0.198797 seconds
}

//current best ratio for N=250 is 133.00456453