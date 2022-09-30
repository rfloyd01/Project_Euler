#pragma once

#include <Header_Files/pch.h>
#include <Header_Files/print.h>
#include <Header_Files/functions.h> //includes coprime
#include <algorithm>

//Digit Sum Numbers
long long mod = 1000000007;

long long waysToShuffle(std::vector<int> &partition, long long* factorials)
{
	long long digitSum = 0;
	for (int i = 0; i < partition.size(); i++) digitSum += partition[i];

	long long ways = factorials[digitSum];
	for (int i = 0; i < partition.size(); i++) ways /= factorials[partition[i]];
	return ways % mod;
}
void createPartitionForms(std::vector<std::vector<std::vector<std::pair<long long, int> > > >& partitionFormsShuffle, 
	int maximum, long long* factorials)
{
	//Create a complex vector that stores the number of different partition forms
	//for all numbers 1 - 9 .For example 9 has the following 3-digit partitions:
	//[7, 1, 1], [6, 2, 1], [5, 3, 1], [5, 2, 2], [4, 4, 1], [4, 3, 2], [3, 3, 3],
	//these partitions could be separated into two different groups (there are 3
	//in the 3!/(2!1!) group and 4 in the 3!/(1!1!1!) group.
	
	//the maximum factorial amount we need here is 9!, do all modular arithmetic at the end
	//long long facties[10] = { 1 };
	//for (long long i = 1; i < 10; i++) facties[i] = i * facties[i - 1];

	//Create structure to hold the number of partition forms for all numbers 1-9
	std::vector<std::vector<std::vector<std::pair<std::vector<int>, int> > > > allPartitionForms;

	//Each DS-Number needs to have all of the digts equal another digit, therefore
	//we need to add i to the front of each partition once all partitions are made
	for (int i = 0; i <= maximum; i++)
	{
		std::vector<std::vector<std::pair<std::vector<int>, int> > > iPartitionForms;
		std::vector<std::vector<std::pair<long long, int> > > iPartitionShuffle;
		std::vector<std::vector<std::vector<int> > > partitions; //holds the standard partitions of i
		getPartitions(i, partitions); //partitions where i/2 is the max value

		for (int j = 0; j <= i; j++)
		{
			std::vector<std::pair<std::vector<int>, int> > iPartitionsLengthJ;
			std::vector<std::pair<long long, int> > iPartitionsLengthJShuffle;

			for (int k = 0; k < partitions[j].size(); k++)
			{
				std::vector<int> currentPartitionForm;
				int currentNumber = i, currentAmount = 1; //"add" an i to the front of each partition
				//std::cout << i << " + ";
				//vprint(partitions[j][k]);

				for (int l = 0; l < partitions[j][k].size(); l++)
				{
					if (partitions[j][k][l] == currentNumber) currentAmount++;
					else
					{
						currentPartitionForm.push_back(currentAmount);
						currentAmount = 1;
						currentNumber = partitions[j][k][l];
					}
				}
				currentPartitionForm.push_back(currentAmount); //add the final digit amount
				std::sort(currentPartitionForm.begin(), currentPartitionForm.end()); //sort the partition form
				//std::cout << "Form is ";
				//vprint(currentPartitionForm);

				//see if this form already exists in the iPartitionsLengthJ vector yet,
				//if so then just increment the current total, otherwise create a new pair and 
				//set current total at 1.
				bool placed = false;
				for (int l = 0; l < iPartitionsLengthJ.size(); l++)
				{
					if (iPartitionsLengthJ[l].first == currentPartitionForm)
					{
						placed = true;
						iPartitionsLengthJ[l].second++;
						iPartitionsLengthJShuffle[l].second++;
						break;
					}
				}
				if (!placed)
				{
					std::pair<std::vector<int>, int> newPartitionForm = { currentPartitionForm, 1 };
					std::pair<long long, int> newPartitionFormShuffle = { waysToShuffle(currentPartitionForm, factorials), 1 };
					//std::pair<long long, int> newPartitionFormShuffle = { waysToShuffle(currentPartitionForm, facties), 1 };
					iPartitionsLengthJ.push_back(newPartitionForm);
					iPartitionsLengthJShuffle.push_back(newPartitionFormShuffle);
				}
			}
			iPartitionForms.push_back(iPartitionsLengthJ);
			iPartitionShuffle.push_back(iPartitionsLengthJShuffle);
		}
		allPartitionForms.push_back(iPartitionForms);
		partitionFormsShuffle.push_back(iPartitionShuffle);
	}

	//Uncomment the below code to print out the forms
	/*for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < allPartitionForms[i].size(); j++)
		{
			for (int k = 0; k < allPartitionForms[i][j].size(); k++)
			{
				std::cout << "There are " << allPartitionForms[i][j][k].second << " partitions of " << 2 * i << " that have length of " << j << " with the form ";
				vprint(allPartitionForms[i][j][k].first);
				std::cout << "These partitions can be shuffled in " << partitionFormsShuffle[i][j][k].first << " ways." << std::endl;
			}
		}
		std::cout << std::endl;
	}*/
}

std::pair<std::string, double> q725()
{
	auto run_time = std::chrono::steady_clock::now();
	long long answer = 0;
	const int maximumDigits = 7;



	//Memoize modular multiplicative inverses all the way up to the limit. Since the value of 
	//mod isn't a prime
	//(we need at least up to 18! in all cases though)
	long long* mmInverses , *facts;
	if (maximumDigits < 18)
	{
		facts = new long long[19]();
		mmInverses = new long long[19]();
	}
	else
	{
		facts = new long long[maximumDigits + 1]();
		mmInverses = new long long[maximumDigits + 1]();
	}
	facts[0] = 1;

	//There's a faster way to find the modular multiplicative inverses, but since we're going up to a
	//maximum of only 2020 I decided to just brute force this
	int actualMax = (maximumDigits < 18 ? 18 : maximumDigits);
	for (long long i = 1; i <= actualMax; i++)
	{
		facts[i] = ModMult(i, facts[i - 1], mod);
		mmInverses[i] = ModularMultiplicativeInverse(i, mod);
	}

	//std::cout << mmInverses[2] << std::endl;

	//Create structure to hold the number of partition forms for all numbers 1-9 and the ways to shuffle them
	std::vector<std::vector<std::vector<std::pair<long long, int> > > > partitionFormsShuffle;
	createPartitionForms(partitionFormsShuffle, 9, facts);
	//createPartitionForms(partitionFormsShuffle, 9);

	//create a complex vector of similar form to the partitionFormsShuffle vector, but instead of the innermost
	//entity being a pair, it will be a long integer which represents the sum of numbers DS-numbers (including numbers with 
	//leading zeros) that can be made where the sum is 2*n. All values get initialized to 0.
	std::vector<std::vector<std::vector<long long> > > DSNumberSums;
	for (int i = 0; i < partitionFormsShuffle.size(); i++)
	{
		std::vector<std::vector<long long> > DSSumsI;
		for (int j = 0; j < partitionFormsShuffle[i].size(); j++)
		{
			std::vector<long long> DSSumsILength;
			for (int k = 0; k < partitionFormsShuffle[i][j].size(); k++) DSSumsILength.push_back(0);
			DSSumsI.push_back(DSSumsILength);
		}
		DSNumberSums.push_back(DSSumsI);
	}
	
	for (int n = 3; n <= 3; n++)
	{
		//A note on the partitionFormsShuffle vector
		//The partitionFormsShuffle vector tells us how many different forms of partitions exist
		//for n with different lengths, as well as the number of ways to shuffle each of these partition forms. For example,
		//the number 8 has the following partitions that are two numbers long: [7, 1], [6, 2], [5, 3] and [4, 4].
		//There are two different types of partitions here, one of the form [1, 1] which because they feature two
		//distinct numbers and one of the form [2], because there are two of the same number. These partitions have
		//have 2!/(1!*1!) = 2 ways and 2!/2! = 1 way to shuffle them respectively. Because of this, there will be a two
		//different pairs located at partitionsFormsShuffle[8][2]. One will look like this <2, 3> and the other will 
		//look like this <1, 1>. These pairs represent <ways to shuffle the partition form, number of existing partitions of this form>.
		
		long long baseWays = 2 * n * 11; //11 because it's the second repunit number
		
		for (long long i = 2; i <= maximumDigits; i++) //i represents the current length of DS-numbers being looked at
		{
			//Note: Due to the way I created the partitions, they all have a length that's 1 longer than the 
			//index they're stored at in the vector.
			//if (n == 3)
			//{
			//	for (int j = 1; j < partitionFormsShuffle[3].size(); j++)
			//	{
			//		for (int k = 0; k < partitionFormsShuffle[3][j].size(); k++)
			//		{
			//			/*std::cout << "There are " << allPartitionForms[i][j][k].second << " partitions of " << 2 * i << " that have length of " << j << " with the form ";
			//			vprint(allPartitionForms[i][j][k].first);*/
			//			std::cout << "These partitions can be shuffled in " << partitionFormsShuffle[3][j][k].first << " ways." << std::endl;
			//		}
			//	}
			//	std::cout << std::endl;
			//}
			long long stop = (i > (n + 1) ? n : i - 2); //we only want to add 0's to partitions that
			for (long long j = 1; j <= stop; j++) //NOTE: Will take longer but can physically calculate each n!/(i!j!...) to avoid 
			{
				//note: j represents the actual partition length
				
				//all partitions of length 1 through i will get a 0 "added" to them. This will have the effect of 
				//increasing the total number of ways of shuffling the partition form by a factor of i / (i - (j + 1)).
				//To avoid decimals, we multiply by i first, then separately divide by (i - (j + 1)).
				for (long long k = 0; k < partitionFormsShuffle[n][j].size(); k++)
				{
					partitionFormsShuffle[n][j][k].first = ModMult(partitionFormsShuffle[n][j][k].first, i, mod);
					/*if (n == 3)
					{
						std::cout << i << " " << j << std::endl;
						std::cout << ModMult(partitionFormsShuffle[n][j][k].first, mmInverses[i - (j + 1)], mod) << " vs. " << 
							partitionFormsShuffle[n][j][k].first / (i - (j + 1)) << std::endl;
					}*/
					partitionFormsShuffle[n][j][k].first = ModMult(partitionFormsShuffle[n][j][k].first, mmInverses[i - (j + 1)], mod);
				}
			}
			
			//std::cout << std::endl;

			if (stop != n) stop++; //this allows us to get partitions with a length of i that have no zeros in them

			//once zeros have been added, go through all partitions from length = 1 to stop+1 and add their values to the total
			//long long debugCounter = 0;
			for (long long j = 1; j <= stop; j++)
			{
				for (long long k = 0; k < DSNumberSums[n][j].size(); k++)
				{
					//long long waysToShuffleWithLeadZeros = (baseWays * partitionFormsShuffle[n][j][k].first) / i;
					long long waysToShuffleWithLeadZeros = ModMult(baseWays, partitionFormsShuffle[n][j][k].first, mod);
					waysToShuffleWithLeadZeros = ModMult(waysToShuffleWithLeadZeros, mmInverses[i], mod);
					/*if (waysToShuffleWithLeadZeros * i != baseWays * partitionFormsShuffle[n][j][k].second)
					{
						std::cout << "n = " << n << std::endl;
						std::cout << "i = " << i << std::endl;
						std::cout << "j = " << j << std::endl;
						std::cout << "stop = " << stop << std::endl;
						std::cout << "(" << baseWays << " * " << partitionFormsShuffle[n][j][k].second << ") / " <<
							i << " = " << waysToShuffleWithLeadZeros << std::endl << std::endl;
					}*/
					//multiply the sum from this form by the number of this form that exist and add to answer
					answer = (answer + ModMult((long long)partitionFormsShuffle[n][j][k].second, (((waysToShuffleWithLeadZeros - DSNumberSums[n][j][k]) % mod) + mod), mod)) % mod;
					//debugCounter += (partitionFormsShuffle[n][j][k].first * (waysToShuffleWithLeadZeros - DSNumberSums[n][j][k]));
					
					DSNumberSums[n][j][k] = waysToShuffleWithLeadZeros;
				}
			}
			//once all partitions for this length have been added, increase the base value to the next repunit
			baseWays = ModMult(baseWays, (long long)10, mod);
			baseWays = (baseWays + (2 * n)) % mod;

			//for debugging
			//std::cout << "Sum of " << i << "-digit DS-numbers that sum to " << n << " is " << debugCounter << std::endl;
		}
		//std::cout << std::endl;
	}
	

	//delete[]facts;
	delete[]mmInverses;
	return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is xxx
	//ran in xxx seconds
}

//NOTES
