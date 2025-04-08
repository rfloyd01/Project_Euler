#pragma once

#include <Header_Files/pch.h>
#include <Functions/functions.h>
#include <Header_Files/print.h>

//Peerless Trees
int global_call_counter = 0, global_call_max = 0;

bool partitionUsabilityCheck(std::vector<int>& original_partition)
{
	//Check to see whether making a tree from the given list of nodes is possible.
	//The goal is to make sure that no two nodes with the same number of neighbors are connected
	//directly to each other.
	std::vector<int> partition_copy = original_partition;

	for (int i = 0; i < original_partition.size(); i++)
	{
		if (partition_copy[i] == 0) continue;

		bool success = false;
		for (int j = i + 1; j < original_partition.size(); j++)
		{
			if (original_partition[i] != original_partition[j] && partition_copy[j] > 0)
			{
				partition_copy[i]--;
				partition_copy[j]--;

				if (partition_copy[i] == 0)
				{
					success = true;
					break;
				}
			}
		}

		if (!success) return false;
	}

	return true;
}

bool recursivePartitionCheck(std::vector<int>& original_partition, std::vector<int>& current_partition, int connections_remaining, int level = 0, int loc = 0)
{
	global_call_counter++; //TODO: Test variable remove when done

	if (connections_remaining == 0) return true;
	if (current_partition[level] == 0) return recursivePartitionCheck(original_partition, current_partition, connections_remaining, level + 1, level + 1);

	//in this block we're trying to find neighbors for the node located at original_partition[level]
	for (int i = loc + 1; i < original_partition.size(); i++)
	{
		if (current_partition[i] > 0 && original_partition[i] != original_partition[level])
		{
			current_partition[level]--;
			current_partition[i]--;

			if (recursivePartitionCheck(original_partition, current_partition, connections_remaining - 2, level, i))
			{
				return true;
			}

			//if we get here then a working tree couldn't be found, skip over the current node
			//and try to create a different set of neighbors that works
			current_partition[level]++;
			current_partition[i]++;
		}
	}

	//After trying all sets of neighbors for the current node, return false if no working set
	//could be found
	return false;
}

bool recursivePartitionCheckWithoutOnes(std::vector<int>& original_partition, std::vector<int>& current_partition, int connections_remaining, int ones_left,
	int level = 0, int loc = 0)
{
	global_call_counter++; //TODO: Test variable remove when done

	if (connections_remaining == 0) return true;
	if (current_partition[level] == 0) return recursivePartitionCheckWithoutOnes(original_partition, current_partition, connections_remaining, ones_left, level + 1, level + 1);

	//in this block we're trying to find neighbors for the node located at original_partition[level]
	for (int i = loc + 1; i < original_partition.size(); i++)
	{
		if (original_partition[i] == 1)
		{
			//There are only ones left, meaning the node at current_partition[level] must be filled 
			//up with leaf nodes. See if there are enough 1 nodes remaining to do this, if so, 
			//subtract them from the current count and iterate to the next level
			int ones_needed = current_partition[level];
			if (ones_left >= ones_needed)
			{
				current_partition[level] = 0;
				if (recursivePartitionCheckWithoutOnes(original_partition, current_partition, connections_remaining - 2 * ones_needed, ones_left - ones_needed, level, i))
				{
					return true;
				}

				current_partition[level] += ones_needed;
			}

			return false; //No need to iterate over other 1 nodes
		}
		else if (current_partition[i] > 0 && original_partition[i] != original_partition[level])
		{
			current_partition[level]--;
			current_partition[i]--;

			if (recursivePartitionCheckWithoutOnes(original_partition, current_partition, connections_remaining - 2, ones_left, level, i))
			{
				return true;
			}

			//if we get here then a working tree couldn't be found, skip over the current node
			//and try to create a different set of neighbors that works
			current_partition[level]++;
			current_partition[i]++;
		}
	}

	//After trying all sets of neighbors for the current node, return false if no working set
	//could be found
	return false;
}

std::vector<std::vector<int> > separatePartition(std::vector<int>& original_partition)
{
	//Takes an integer partition and separates it into separate groups of integers. For example,
	//the partition [5, 4, 3, 3, 2, 2, 2, 1] would become [[5], [4], [3, 3], [2, 2, 2], [1]].
	//The numbers are kept intact (as opposed to only keeping a count of each number) so we 
	//can keep track of which individual nodes still have connections available. Also, to keep 
	//things easier we don't include the 1 nodes at the end. These represent leaves of the tree
	//and are easy enough to deal with that they don't need to be included.
	std::vector<std::vector<int> > separated_partition = { {original_partition[0]} };

	for (int i = 1; i < original_partition.size(); i++)
	{
		if (original_partition[i] == 1) break;

		if (original_partition[i] == separated_partition.back().back()) separated_partition.back().push_back(original_partition[i]);
		else separated_partition.push_back({ original_partition[i] });
	}

	return separated_partition;
}

void partialPartitionCheck(std::vector<std::vector<int>>& original_partition, std::vector<std::vector<int>>& current_partition, int connections_remaining,
	int ones_remaining, int& trees_found, std::pair<int, int> current_node = { 0, 0 }, std::pair<int, int> loc = { 1, 0 })
{
	//An integer partition is passed into this function which represents a potential tree.
	//To make iterating over different possibilities of node connection easier, the partition
	//has been split into separate groups for each number. This method is a little tricky as 
	//it uses a combination of iteration and recursion to try all possible ways of connecting
	//the nodes in an efficient manner.
	global_call_counter++;
	if (connections_remaining == 0)
	{
		//std::cout << "Found a working tree: " << std::endl;
		trees_found++;
		return;
	}

	if (current_partition[current_node.first][current_node.second] == 0)
	{
		//The current node has been successfully connected to the specified number of neighboring
		//nodes. Move onto the next one.
		std::pair<int, int> next_node = (current_node.second == (original_partition[current_node.first].size() - 1))
			? std::make_pair(current_node.first + 1, 0)
			: std::make_pair(current_node.first, current_node.second + 1);

		partialPartitionCheck(original_partition, current_partition, connections_remaining, ones_remaining, trees_found, next_node, { next_node.first + 1, 0 });
		return;
	}

	if (current_node.first == original_partition.size())
	{
		//Reaching this block means that we've exhausted all possibilities
		//so simply return until all recursions are complete
		return;
	}

	if (loc.first == original_partition.size())
	{
		//This is where the leaf nodes would normally be. Take as many 1 nodes as possible/necessary
		//and then continue on.
		int ones_needed = current_partition[current_node.first][current_node.second];
		if (ones_remaining >= ones_needed)
		{
			//subtract out the 1 nodes needed and recurse down to next level
			current_partition[current_node.first][current_node.second] = 0;
			
			partialPartitionCheck(original_partition, current_partition, connections_remaining - (2 * ones_needed), ones_remaining - ones_needed,
				trees_found, current_node, loc);

			current_partition[current_node.first][current_node.second] = ones_needed;
		}
		
		return; //If there aren't enough ones then this current tree won't work, return to previous level
	}

	//First, calculate the maximum number of nodes of the current index that we want to connect
	//to. It's a function of how many openings the current node still has, and the number of existing
	//nodes of the current index. We iterate from this maximum number down to 0.
	int max = original_partition[current_node.first][current_node.second] < original_partition[loc.first].size() ?
		original_partition[current_node.first][current_node.second] : original_partition[loc.first].size();

	for (int i = max; i >= 0; i--)
	{
		int connections_made = 0;
		std::vector<int> partial_partition_copy = current_partition[loc.first]; //used to reset the current part of the partition after each iteration
		for (int j = 0; j < original_partition[loc.first].size(); j++)
		{
			//Attempt to connect to each of the nodes. To do so requires two things. First,
			//the node must have some connections still available (can't be 0). Second is a little
			//harder to track. If the node at 'j' is already connected to the graph (it's value is
			//less than what's depicted in the original_partition array) then we can only connect
			//to it if the current node isn't connected into the graph yet (connected parameter 
			//is false). If the current node IS connected into the graph already, we can only connect
			//to a node that ISN"T in the graph yet, otherwise the graph will form a loop and no 
			//longer be considered a tree. It would be impossible to connect all nodes if we create
			//a loop, but we can save ourselves a lot of recursion levels by eliminating them 
			//entirely.
			if (i > 0)
			{
				if (current_partition[loc.first][j] == 0) continue;
				if (((current_partition[current_node.first][current_node.second] != original_partition[current_node.first][current_node.second]) &&
					(current_partition[loc.first][j] != original_partition[loc.first][j]))) continue;

				//If we make it here than we can connect to the node at j
				current_partition[current_node.first][current_node.second]--;
				current_partition[loc.first][j]--;
				connections_made++;
			}

			if (connections_made == i)
			{
				//Enough connections have been made at the current part of the partition to move on to the next
				//part. If the current node has been fully connected then move onto the next node.
				partialPartitionCheck(original_partition, current_partition, connections_remaining - (2 * connections_made), ones_remaining,
					trees_found, current_node, { loc.first + 1, 0 });

				break;
			}
		}

		current_partition[current_node.first][current_node.second] += connections_made; //undo connections to neighbors from above loop
		current_partition[loc.first] = partial_partition_copy;
	}

	return;
}

void createPartionsOfNWithLengthL(int n, int total_connections,int l, int loc, std::vector<int>& current_partition, int& total_trees, bool print = false)
{
	//First calculate the maximum number that can go in the current location
	int max = n - l + 1;

	//If this maximum is a 1 it means that there are only leaf nodes left so 
	//the partition is functionally created. Call a method on the current partition
	//to see if it's possible to create a working tree.
	if (max == 1)
	{
		//std::vector<int> current_partition_copy = current_partition;
		std::vector<std::vector<int> > partial_partition = separatePartition(current_partition);
		std::vector<std::vector<int> > partial_partition_copy = partial_partition;
		global_call_counter = 0;

		//if (recursivePartitionCheckWithoutOnes(current_partition, current_partition_copy, total_connections, n))
		/*{
			total_partitions++;
			if (print) vprint(current_partition);
		}*/
		/*std::cout << "Attempting to make trees from: ";
		vprint(current_partition);*/
		int new_trees = 0;
		partialPartitionCheck(partial_partition, partial_partition_copy, total_connections, n, new_trees);

		total_trees += new_trees;
		if (new_trees == 0 && print)
		{
			std::cout << "No trees found for: ";
			vprint(current_partition);
		}
		else
		{
			//std::cout << "Did find trees for: ";
			//vprint(current_partition);
		}

		if (global_call_counter > global_call_max)
		{
			//std::cout << "New Call Record: " << global_call_counter << std::endl;
			global_call_max = global_call_counter;
		}
		return;
	}

	//We only want to create ordered partitions (to avoid double counting). If the maximum 
	//value that can go in the current spot is greater than the previous spot, lower it.
	if ((loc > 0) && (max > current_partition[loc - 1])) max = current_partition[loc - 1];

	//Recurse down to the next level to finish creating the partition.
	for (int i = max; i > 0; i--)
	{
		//if i * l < n then break loop
		if (i * l < n) break;

		current_partition[loc] = i;
		createPartionsOfNWithLengthL(n - i, total_connections, l - 1, loc + 1, current_partition, total_trees, print);
	}

	current_partition[loc] = 1; //reset the current digit

	return;
}

std::pair<std::string, long double> q936 ()
{
	auto run_time = std::chrono::steady_clock::now();
	int answer = 0;

	for (int nodes = 30; nodes <= 31; nodes++)
	{
		std::cout << "N = " << nodes << std::endl;
		std::vector<int> current_partition(nodes, 1);
		createPartionsOfNWithLengthL(2 * (nodes - 1), 2 * (nodes - 1), nodes, 0, current_partition, answer, true);
	}

	/*std::vector<std::vector<int> > partial_partition = { {5}, {4}, { 2, 2, 2 } };
	std::vector<std::vector<int> > partial_partition_copy = partial_partition;
	partialPartitionCheck(partial_partition, partial_partition_copy, 22, 7);*/

	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
}

//NOTES
//The answer is xxx
//Program ran in xxx seconds

/*
Notes:

I've been tussling with the best way to represent these trees (without actually creating them using nodes) and what I've come up with for now is to use a sorted
array of integers with length N, where each entry in the array represents the number of neighbors for a given node. For example, the six graphs shown in the
problem statement would be represented as:

[6, 1, 1, 1, 1, 1, 1]
[5, 2, 1, 1, 1, 1, 1]
[4, 2, 2, 1, 1, 1, 1]
[4, 3, 1, 1, 1, 1, 1]
[3, 2, 2, 2, 1, 1, 1]
[3, 3, 2, 1, 1, 1, 1]

Looking at these representations, a few things jump out. First, they all have a length of 7. This is obvious since we're looking at trees with seven nodes.
Second, the sum for each of these arrays is 12. A little less obvious, but it also makes sense due to how trees are defined in graph theory. Since all nodes
in the tree must be connected to each other, and cycles aren't allowed it follows that there will be N-1 edges in the graph and since each edge connects two 
nodes we would expect the total number of connections in the tree to equal 2(N - 1) which in this case is 2 * 6 = 12. Since the sum is always going to be 
12 and the length is always going to be 7, we're essentially just looking at partitions of 12 with length 7. Sure enough, using an existing method I have to 
generate these partitions, all of the above are present as well as one other partition: [2, 2, 2, 2, 2, 1, 1]. Examining this partition and it's fairly easy to 
see that there's no way to connect the nodes without some of the 2 nodes being connected to each other so therefore it doesn't count towards the final answer.

So one possible approach to this problem is to just generate integer partitions of the appropriate length and examine the numbers in each partition to see if 
they can be connected in such a way that no two numbers of the same value need to be connected to each other. A little brute force code reveals that for 
N = 3 through N = 50 there are 918219 integer partitions of 2(N - 1) that have a length of N. This number isn't all that large so in theory we can go over 
each of these partitions and perform a quick check fairly easily. Once partitions that aren't possible candidates have been weeded out, the question now 
becomes, is it possible to make more than one tree with each working set of nodes? None of the partitions of 7 above can create distinct trees, however,
7 isn't a very large number. What happens if we look at something larger, like N = 12? A little pencil and paper work shows that for the partition
[5,4,2,2,2,1,1,1,1,1,1,1] there are indeed multiple different trees. Here's my attempt to "draw" two of them:

  1 1   1 - 2                1 1   1
   \|       |                 \|   |
1 - 5 - 2 - 4 - 1          1 - 5 - 4 - 2 - 1
    |       |                  |   |
	1   1 - 2              1 - 2   2 - 1

Maybe not the easiest pictures to understand but clearly these two trees are inherently different even though they use the same numbers. For example,
in the first tree there's a 2 between the 4 and 5 nodes, however, in the second tree the 4 and 5 are directly connected to one another. Another difference 
is that in the first tree the 5 node is attached to four 1 nodes while in the second tree it's only connected to three 1 nodes. All this is to say, that yes,
there are multiple trees that can be created from the same partition. I'm sure that depending on what the current partition is there can be many ways
to create distinct trees. I think what I'll do is first try and get a sense of how many partitions of the ~1,000,000 I can weed out. If it ends up that 
there's less than 100,000 total options then maybe I'll write an algorithm which recursively builds different trees for each partition. If I don't end up 
weeding out that many partitions then I'll probably need a different approach to have a reasonable run time.

My first attempt at writing an (iterative) algorithm to see if a given partition could have a working tree built from it proved to not work unfortunately.
Out of the ~1,000,000 total partitions it said that only 8099 of them could be converted into trees which felt very low. A little digging and I realized that 
I wasn't checking all possibilities before deeming the given partition as a failure. After this I came up with a recursive algorithm which tries all combos
and appears to work correctly, however, it's not a very efficeint algorithm and I can't get past partitions of 16 before breaking the 1 minute rule. This 
recursive approach is brute-force in nature, so I think if I clean it up it'll work much better. The main issue is that it tries the same combinations over
and over again. For example, given the partition [6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1] it's fairly easy to see there's no working combos as the 6's can't combine
with each other and there aren't enough 1's to fill up both of the 6 nodes. Despite this, my algorithm will match the first six 1 nodes to the first 6 node
which will ultimately lead to failure. When it does it will then match the first five 1 nodes and the seventh 1 node to the first 6 node, and so on and so on.
All in all it will try choose(10, 6) = 210 different cominations of 1 nodes before realizing there's no working combination. This is obviously bad, since all 
of these combinations are in essence identical to each other.

Before attempting to come up with a different approach all together, I think I can get some kind of a speedup by looking at the 1 nodes. We know that these 
nodes will always make up the leaves of the tree (I think that's the right term) so will always be handled last in a recursive approach. Also, based on the 
partitions of seen with my experimenting so far, they also make up the majority of nodes. Instead of including them in the recursion it might just be easier 
to keep track of the number of 1's that are still availble to use. Using the above partition with all of the 6's and 1's, we start at the first 6 node, iterate
over the next (since nodes of the same index can't be neighbors) and then hit the one nodes. As soon as we reach the ones, we look at the total number of nodes 
still needed to make the current node complete and subtract that from the pool of 1's. So in this case we remove six 1's from the pool, leaving 4 more. We 
then iterate to the next 6 node and see that there aren't enough 1's to fill it up anymore.

After removing recursion down to the 1 nodes there was a noticeable speedup, however, still no where near quick enough. I'm going to have to implement some 
kind of combinatoric limit by separating digits of the partition into separate groups. For example, the partition 11344: [3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1]
will try choose(5, 3) = 10 different combinations of 2 nodes before reaching the conclusion that there are no viable options. I want to craft the algorithm
in such a way that it will only try to connect a 3 node to three different 2 nodes a single time as opposed to ten times considering it will reach the same 
conclusion each time.

With some difficulty I implemented an algorithm to get rid of redundant checks. It works much quicker than my existing attempts, however, is still too slow.
For reference it only takes 0.0001 seconds to find trees with 3 through 10 nodes but finding trees with 3 through 25 nodes takes 4 seconds. It seems there's
an exponential increase, so attempting to find trees with 50 nodes would probably take years. I've also found that I'm counting too many trees. The problem 
statement says that S(10) is 74 but I'm calculating it as 81. I'll work on tweaking my current algorithm to see why I'm getting a higher answer, and then I'll 
need to find another speedup. I wonder if there's some kind of memoization that I can implement?
*/