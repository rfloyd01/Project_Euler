#pragma once

#include <Header_Files/pch.h>
#include <iomanip>
#include <sstream>

//Licence Plates
std::pair<std::string, double> q371()
{
	auto run_time = std::chrono::steady_clock::now();

    double answer = 0.0, round_weight = 1.0;

    //we can't win in the first round so start in round 2
    //Vectors are initialized assuming we've already seen 1 license plate
    int current_round = 2;
    std::vector<double> wo_500 = { 0.001, 0.998 }, w_500 = { 0.0, 0.001 };

    while (round_weight > 1e-10)
    {
        round_weight = 0.0;
        wo_500.push_back(0.0);
        w_500.push_back(0.0);

        for (int j = current_round; j > 0; j--)
        {
            //First calculate the chances of finding a winning license plate this round
            round_weight += j * (wo_500[j] + w_500[j]); //multiply the odds of winning with the current round to add to the expected value

            //Then update each value with the odds of not finding a winning plate this round
            wo_500[j] = ((wo_500[j] * (j + 1)) + (wo_500[j - 1] * (1000 - 2 * j))) / 1000.0;
            w_500[j] = ((w_500[j] * j) + (w_500[j - 1] * (1002 - 2 * j)) + (wo_500[j - 1])) / 1000.0;
        }
        wo_500[0] /= 1000; //odds for picking nothing but zero
        answer += current_round++ * round_weight;
    }
    std::cout << current_round << std::endl;

    //Use a string stream to get the appropriate decimal precision
    std::stringstream stream;
    stream << std::fixed << std::setprecision(8) << answer / 1000.0; //the odds of picking any license plate is 1/1000 so factor that in now
	
	return { stream.str(), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

	//the answer is 40.66368097
	//ran in 0.0001538 seconds
}

//NOTES
/*
- Regardless of the starting license plate the odds should be the same (with the exception of 0). Meaning, I don't think you need to solve this for
  license plate 1, license plate 2 3... etc.
- If we think of this as a recursion, then any time we see a new license plate we go down to the next level
- There are 4 things that can take us down to the next level
    - We see a 0 plate, which can't lead us to a win
    - We find a totally new license plate that doesn't lead to a win
    - We see a plate that we've already seen before
    - We find a totally new license plate that does lead to a win
- We should have a variable that keeps track of the odds of us getting to a certain point in the function
- We should also have a variable that keeps track of the number of distinct non-zero plates that we've seen which will be needed to calculate the odds
- A recursive approach may look something like this

recursiveCalculateExpectedValue(double odds, int distinct_plates, int current_level = 0)
{
    //base case: in theory we could play until infinity and never win, but the odds of this are so low that they won't effect our
    //answer to 8 decimal places. Once the odds are low enough we shouldn't need to recurse any lower

    //Carry out scenario one:   we see a 0 license plate
    //distinct_plates doesn't chantge, 1/1000 chance of finding the 0 plate, level goes up by 1
    recursiveCalculateExpectedValue(odds / 1000, distinct_plates, current_level + 1);

    //Carry out scenario two: we find an already seen plate
    //distinct_plates doesn't change, distinct_plates/1000 chance of findingan already seen plate, level goes up by 1
    recursiveCalculateExpectedValue(distinct_plates * odds / 1000, distinct_plates, current_level + 1);

    //Carry out scenario three:   we find a new plate not leading to a win
    //distinct_plates increases by 1, (1000 - distinct_plates - 1)/1000 chance of finding a new plate, level goes up by 1
    //the reason for the - 1 is to remove the chances of seeing a 0 plate
    recursiveCalculateExpectedValue((1000 - distinct_plates -1) * odds / 1000, distinct_plates + 1, current_level + 1);

    //Carry out scenario four:  we find a new plate leading to a win, no need to recurse here, this is where we increase the expected value
    answer += current_level * distinct_plates/1000;
}

- The above function probably needs to be touched up a bit but is definitely a start in the right direction I think
- One thing that will throw a wrench into the equation is the license plate 500. It allows us to win with an already seen plate
    - Maybe add a boolean into the mix called seen_500 which will effect the odds ever so slightly (or could just use an integer which will be 0 or 1)
- The biggest question left is what to see as the base case. It should be very easy to get to a point where we're more than 8 decimal places in
   - For example, the odds of seeing a single plate three times in a row is 1/(1000^3) = 1e-9 which is a small number, however since there are 999 different
     plates we can do this with and not get a win (looking at you 500) there's really a 999 * 1e-9 = 9.99e-7 chance of seeing 3 plates which is statistically
     significant enough to care about.
- Maybe for now just assume that since there are 1000 different plates that any odds we calculated can be magnified by a factor of 1000 so set the exit
  threshold to be when the odds fall below 1e-8 / 1000

*/