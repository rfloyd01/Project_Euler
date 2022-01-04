#include <Header_Files/pch.h>
#include <Header_Files/functions.h> //includes primes(), is_prime()
#include <Header_Files/print.h> //includes vprint()
#include <Header_Files/int_64x.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

//Twenty-Two Foolish Primes
std::pair<std::string, double> q239()
{
    auto run_time = std::chrono::steady_clock::now();
    std::string answer;

    int maximum = 100;
    std::vector<int> prims = primes(maximum);
    std::vector<std::vector<int_64x> > permutations;

    int correct_primes = 3;
    int_64x non_primes(maximum - prims.size()), non_foolish_primes(prims.size() - correct_primes), foolish_primes(0), adder, remove; //remove 2, 3 and 5 from overall prime number count

    for (int i = 0; i <= non_foolish_primes; i++)
    {
        std::vector<int_64x> yo;
        for (int j = 0; j <= i; j++) yo.push_back(0);
        permutations.push_back(yo);
    }

    permutations[0][0] = 5550; //the number of ways to fill the vector [x, 2, 3, x, 5] where x's are non-prime numbers below 100
    permutations[1][0] = 3300; //the number of ways to fill the vector [x, 2, 3, x, 5] where one x is a non-prime number below 100 and the other is a prime below 100 (but not 2, 3 or 5)
    permutations[2][0] = 462;  //the number of ways to fill the vector [x, 2, 3, x, 5] where x's are prime numbers below 100 (but not 2, 3 or 5)

    /*std::cout << "1 Number used: " << std::endl;
    std::cout << non_foolish_primes << " Non-Foolish Primes used." << std::endl;
    std::cout << foolish_primes << " Foolish Primes used." << std::endl;
    for (int i = 0; i < permutations.size(); i++)
    {
        std::cout << '[';
        if (permutations[i].size() > 0)
        {
            for (int j = 0; j < permutations[i].size() - 1; j++) std::cout << permutations[i][j] << ", ";
            std::cout << permutations[i].back();
        }
        std::cout << ']' << std::endl;
    }
    std::cout << std::endl;*/

    //Our permutation array has been pre-set with all combos of the first 5 numbers, so we start at i = 6
    for (int i = 6; i <= maximum; i++)
    {
        //the way the algorithm works is that each vector of permutations needs to be updated 1 at a time starting from
        //the end and going towards the beginning. To fully update a row requires two passes. In the first pass, new
        //foolish primes will be addressed by shifting values over by one element and old foolish primes will be added to
        //each element accordingly. In the second pass, non-foolish primes and non-prime numbers will be added to each
        //element accordingly.

        bool fool = false; //this boolean keeps track of whether or not a new foolish prime is added in this iteration
        bool last_row = true;
        adder = 0;

        if (primeNumberTest(i)) fool = true;

        //j is the current row of permutations that we're looking at
        for (int j = permutations.size() - 1; j >= 0; j--)
        {
            if (j < permutations.size() - 1) last_row = false; //can't add any primes if we're in the last vector of permutations

            //first pass through the row addresses foolish primes, new and old
            for (int k = permutations[j].size() - 1; k >= 0; k--)
            {
                if (permutations[j][k] == 0) continue;
                remove = 0;
                if (fool)
                {
                    //if there's a new foolish prime being added in this iteration then we need to find anywhere that
                    //this prime currently exists within the permutations vector and retroactively "shift" those permutations
                    //over by 1 element.

                    //first we need to check and see if any shifting can be done on this square, as long as the current element is greater than 0
                    //and isn't the last element of the vector (i.e. number of foolish primes = total number of primes in permutation) then shifting
                    //will be possible
                    if (k < permutations[j].size() - 1)
                    {
                        remove = (j - k) * permutations[j][k] / non_foolish_primes;
                        permutations[j][k + 1] += remove;
                        permutations[j][k] -= remove;
                    }

                    //If there are permutations getting shifted, then any already discovered  foolish primes added to these permutations
                    //will cause a shift by two elements instead of just 1, this effect only happens during this "shift". We can only add
                    //more primes if we aren't currently in the last vector of permutations.
                    if ((remove > 0) && !last_row) permutations[j + 1][k + 2] += remove * (foolish_primes - k);
                }

                //add already discovered foolish primes to any permutations that haven't been shifted, but only if we aren't currently in 
                //the last vector of permutations
                if (!last_row) permutations[j + 1][k + 1] += (permutations[j][k] * (foolish_primes - k));
            }

            //we now iterate through the row again and add any non_foolish primes and non_primes that we can
            if (fool) adder = -1; //don't want to add any primes which are about to become foolish
            for (int k = permutations[j].size() - 1; k >= 0; k--)
            {
                if (permutations[j][k] == 0) continue; //skip any elements with no value
                if (!last_row) permutations[j + 1][k] += permutations[j][k] * ((non_foolish_primes + adder) - (j - k));
                permutations[j][k] *= (non_primes - (i - 1 - j - correct_primes)); //TODO:This is currently removing too many non_primes
            }
        }

        //with all of the rows addressed we can now officially increment the foolish primes by one and decrement the non-foolish
        //primes by one as well if a foolish prime was added
        if (fool)
        {
            foolish_primes++;
            non_foolish_primes--;
        }

        /*std::cout << i << " Numbers used: " << std::endl;
        std::cout << non_foolish_primes << " Non-Foolish Primes used." << std::endl;
        std::cout << foolish_primes << " Foolish Primes used." << std::endl;
        for (int i = 0; i < permutations.size(); i++)
        {
            std::cout << '[';
            if (permutations[i].size() > 0)
            {
                for (int j = 0; j < permutations[i].size() - 1; j++) std::cout << permutations[i][j] << ", ";
                std::cout << permutations[i].back();
            }
            std::cout << ']' << std::endl;
        }
        std::cout << std::endl;*/
    }

    //Currently there's only one value in the entire permutation array, at the very last element. This value represents the total number of ways to
    //line up the tiles 1-100 where 2, 3 and 5 are the only prime numbers in the correct place (i.e. the other 22 primes are foolish). Well there are
    //choose(25, 3) = 2,300 ways to pick which primes aren't foolish. Due to symmetry it doesn't matter which primes we choose to be non-foolish, there
    //will be the same number of ways to arrange the tiles with only 3 primes in the correct location. Therefore the total number of ways to have 22 
    //foolish primes is found by multiplying the current value by 2,300.
    permutations.back().back() *= 2300;

    //We now have the correct number of permutations with 22 foolish primes! In order to get our actual answer we need to take the first 13 digits of
    //the foolish permutations and divide it by the first 13 digits of the total possible permutations (first 13 digits so that we can have correct
    //precision to 12 decimal points). Since we can comfortably fit 19 digits into an unsigned long long just take the first 19 digits for added precision.
    std::string ans = permutations.back().back().getNumberString();

    int_64x fact(1);
    for (int i = 2; i <= maximum; i++) fact *= i; //100 factorial is the total amount of ways to shuffle the tiles

    std::string maximum_permutations = fact.getNumberString();
    std::string zeros(maximum_permutations.length() - ans.length(), '0'); //add zeros to the front of the answer string until it has the same number of characters as the 100! string
    ans = zeros + ans;

    //store the first 19 digits of each string into unsigned long longs so we can divide them
    unsigned long long total_perms   = stoull(std::string(maximum_permutations.begin(), maximum_permutations.begin() + 19));
    unsigned long long foolish_perms = stoull(std::string(ans.begin(), ans.begin() + 19));
    
    //it's actually pretty obnoxious what needs to be done here in order to get all 12 digits after the decimal point. Need to create an output string stream,
    //set its precision to 12, write the answer to it and then convert the string stream to a standard string.
    double ratio = (double)foolish_perms / (double)total_perms;
    std::ostringstream yoyo;
    yoyo << std::fixed << std::setprecision(12) << ratio;
    answer = yoyo.str();

    return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is 0.001887854841
    //ran in 1.80768 seconds
}

//NOTES
//There are only 25 prime numbers between 1 and 100, so if 22 are away from their natural positions that means 3 of them must be in their natural position. What we can do is pick 3 primes
//and force them to be in the correct position and then find out how many ways there are to put all the other numbers in place, making sure that not a single prime goes into its proper
//place. Once we get this answer, there are choose(25, 3) = 2,300 ways that we can select the 3 "correct" primes so just multiply the current answer by 2,300 to find the total number of ways
//to have exactly 22 Foolish primes. To get the final answer just divide the number of ways to have 22 foolish primes by 100! which is the total number of ways to shuffle the numbers 1 - 100.
//It'll be easiest to pick the first three primes (2, 3, 5) to be non-foolish and set up our permutations array starting from the 6-th iteration as opposed to the first. After the 5-th iteraion
//our number line would be set up like this [x, 2, 3, x, 5] where the x's can be any numbers other then 2, 3 or 5. To fill the x's there are 100 - 25 = 75 non-prime numbers and 25 - 3 = 22
//prime numbers to choose from. We can fill the x's in three different ways, by using two non-primes (choose(75, 2) * 2 = 5,550 ways), a non-prime and a prime (75 * 22 * 2 = 3,300 ways) or by
//using two primes (choose(22, 2) * 2 = 462 ways). We set up our permutation array to reflect these initial numbers. Since the numbers 2, 3 and 5 are cemented in place they shouldn't be used
//in further calculations so we make the permutation array shorter by 3. Here's what the starting array looks like:
//
//[5550]
//[3300, 0]
//[462, 0, 0]
//[0, 0, 0, 0]
//[0, 0, 0, 0, 0]
//..............
//................
//[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

//Original code for integers
/*
//std::pair<std::string, double> q239()
//{
//    auto run_time = std::chrono::steady_clock::now();
//    int answer = 0;
//
//    int maximum = 10, remove;
//    //int_64x remove;
//    std::vector<int> prims = primes(maximum);
//    std::vector<std::vector<int> > permutations;
//    //std::vector<std::vector<int_64x> > permutations;
//
//    int non_primes = maximum - prims.size(), non_foolish_primes = prims.size(), foolish_primes = 0;
//    //int_64x non_primes(maximum - prims.size()), non_foolish_primes(prims.size()), foolish_primes(0), adder;
//
//    for (int i = 0; i <= prims.size(); i++)
//    {
//        std::vector<int> yo;
//        //std::vector<int_64x> yo;
//        for (int j = 0; j <= i; j++) yo.push_back(0);
//        permutations.push_back(yo);
//    }
//
//    permutations[0][0] = non_primes;
//    permutations[1][0] = non_foolish_primes;
//
//    std::cout << "1: " << std::endl;
//    for (int i = 0; i < permutations.size(); i++) vprint(permutations[i]);
//    std::cout << std::endl;
//
//    for (int i = 2; i <= maximum; i++)
//    {
//        //the way the algorithm works is that each vector in permutations needs to be updated 1 at a time starting from
//        //the end and going towards the beginning. To fully update a row requires to passes. In the first pass, new
//        //foolish primes will be addressed by shifting values over by one element and old foolish primes will be added to
//        //each element accordingly. In the second pass, non-foolish primes and non-prime numbers will be added to each
//        //element accordingly.
//
//        bool fool = false; //this boolean keeps track of whether or not a new foolish prime is added in this iteration
//        bool last_row = true;
//        int adder = 0;
//
//        if (primeNumberTest(i)) fool = true;
//
//        for (int j = permutations.size() - 1; j >= 0; j--)
//        {
//            if (j < permutations.size() - 1) last_row = false; //can't add any primes if we're in the last vector of permutations
//
//            //first pass through the row addresses foolish primes, new and old
//            for (int k = permutations[j].size() - 1; k >= 0; k--)
//            {
//                if (permutations[j][k] == 0) continue;
//                remove = 0;
//                if (fool)
//                {
//                    //if there's a new foolish prime being added in this iteration then we need to find anywhere that
//                    //this prime currently exists within the permutations vector and retroactively "shift" those permutations
//                    //over by 1 element.
//
//                    //first we need to check and see if any shifting can be done on this square, as long as the current element is greater than 0
//                    //and isn't the last element of the vector (i.e. number of foolish primes = total number of primes in permutation) then shifting
//                    //will be possible
//
//                    if (k < permutations[j].size() - 1)
//                    {
//                        remove = (j - k) * permutations[j][k] / non_foolish_primes;
//                        //Shift the necessary number of permutations over to the next element
//                        permutations[j][k] -= remove;
//                        permutations[j][k + 1] += remove;
//                    }
//
//                    //If there are permutations getting shifted, then any already discovered  foolish primes added to these permutations
//                    //will cause a shift by two elements instead of just 1, this effect only happens during this "shift". We can only add
//                    //more primes if we aren't currently in the last vector of permutations.
//                    if ((remove > 0) && !last_row) permutations[j + 1][k + 2] += remove * (foolish_primes - k);
//                }
//
//                //add already discovered foolish primes to any permutations that haven't been shifted, but only if we aren't currently in
//                //the last vector of permutations
//                if (!last_row) permutations[j + 1][k + 1] += (permutations[j][k] * (foolish_primes - k));
//            }
//
//            //we know iterate through the row again and add any non_foolish primes and non_primes that we can
//            if (fool) adder = -1; //don't want to add any primes which are about to become foolish
//            for (int k = permutations[j].size() - 1; k >= 0; k--)
//            {
//                if (permutations[j][k] == 0) continue; //skip any elements with no value
//                if (!last_row) permutations[j + 1][k] += permutations[j][k] * ((non_foolish_primes + adder) - (j - k));
//                permutations[j][k] *= (non_primes - (i - 1 - j));
//            }
//        }
//
//        //with all of the rows addressed we can now officially increment the foolish primes by one and decrement the non-foolish
//        //primes by one as well if a foolish prime was added
//        if (fool)
//        {
//            foolish_primes++;
//            non_foolish_primes--;
//        }
//
//        std::cout << i << ": " << std::endl;
//        for (int i = 0; i < permutations.size(); i++) vprint(permutations[i]);
//        std::cout << std::endl;
//    }
//
//    return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };
//
//    //the answer is xxx
//    //ran in xxx seconds
//}

//NOTES
*/

//Code for int_64x and all primes in wrong place
/*
std::pair<std::string, double> q239()
{
    auto run_time = std::chrono::steady_clock::now();
    std::string answer = "0";

    int maximum = 5;
    std::vector<int> prims = primes(maximum);
    std::vector<std::vector<int_64x> > permutations;

    int_64x non_primes(maximum - prims.size()), non_foolish_primes(prims.size()), foolish_primes(0), adder, remove;

    for (int i = 0; i <= prims.size(); i++)
    {
        std::vector<int_64x> yo;
        for (int j = 0; j <= i; j++) yo.push_back(0);
        permutations.push_back(yo);
    }

    permutations[0][0] = non_primes;
    permutations[1][0] = non_foolish_primes;

    std::cout << "1 Number used: " << std::endl;
    std::cout << non_foolish_primes << " Non-Foolish Primes used." << std::endl;
    std::cout << foolish_primes << " Foolish Primes used." << std::endl;
    for (int i = 0; i < permutations.size(); i++)
    {
        std::cout << '[';
        if (permutations[i].size() > 0)
        {
            for (int j = 0; j < permutations[i].size() - 1; j++) std::cout << permutations[i][j] << ", ";
            std::cout << permutations[i].back();
        }
        std::cout << ']' << std::endl;
    }
    std::cout << std::endl;

    for (int i = 2; i <= maximum; i++)
    {
        //the way the algorithm works is that each vector of permutations needs to be updated 1 at a time starting from
        //the end and going towards the beginning. To fully update a row requires to passes. In the first pass, new
        //foolish primes will be addressed by shifting values over by one element and old foolish primes will be added to
        //each element accordingly. In the second pass, non-foolish primes and non-prime numbers will be added to each
        //element accordingly.

        bool fool = false; //this boolean keeps track of whether or not a new foolish prime is added in this iteration
        bool last_row = true;
        adder = 0;

        if (primeNumberTest(i)) fool = true;

        //j is the current row of permutations that we're looking at
        for (int j = permutations.size() - 1; j >= 0; j--)
        {
            if (j < permutations.size() - 1) last_row = false; //can't add any primes if we're in the last vector of permutations

            //first pass through the row addresses foolish primes, new and old
            for (int k = permutations[j].size() - 1; k >= 0; k--)
            {
                if (permutations[j][k] == 0) continue;
                remove = 0;
                if (fool)
                {
                    //if there's a new foolish prime being added in this iteration then we need to find anywhere that
                    //this prime currently exists within the permutations vector and retroactively "shift" those permutations
                    //over by 1 element.

                    //first we need to check and see if any shifting can be done on this square, as long as the current element is greater than 0
                    //and isn't the last element of the vector (i.e. number of foolish primes = total number of primes in permutation) then shifting
                    //will be possible
                    if (k < permutations[j].size() - 1)
                    {
                        remove = (j - k) * permutations[j][k] / non_foolish_primes;
                        permutations[j][k + 1] += remove;
                        permutations[j][k] -= remove;
                    }

                    //If there are permutations getting shifted, then any already discovered  foolish primes added to these permutations
                    //will cause a shift by two elements instead of just 1, this effect only happens during this "shift". We can only add
                    //more primes if we aren't currently in the last vector of permutations.
                    if ((remove > 0) && !last_row) permutations[j + 1][k + 2] += remove * (foolish_primes - k);
                }

                //add already discovered foolish primes to any permutations that haven't been shifted, but only if we aren't currently in
                //the last vector of permutations
                if (!last_row) permutations[j + 1][k + 1] += (permutations[j][k] * (foolish_primes - k));
            }

            //we now iterate through the row again and add any non_foolish primes and non_primes that we can
            if (fool) adder = -1; //don't want to add any primes which are about to become foolish
            for (int k = permutations[j].size() - 1; k >= 0; k--)
            {
                if (permutations[j][k] == 0) continue; //skip any elements with no value
                if (!last_row) permutations[j + 1][k] += permutations[j][k] * ((non_foolish_primes + adder) - (j - k));
                permutations[j][k] *= (non_primes - (i - 1 - j));
            }
        }

        //with all of the rows addressed we can now officially increment the foolish primes by one and decrement the non-foolish
        //primes by one as well if a foolish prime was added
        if (fool)
        {
            foolish_primes++;
            non_foolish_primes--;
        }

        std::cout << i << " Numbers used: " << std::endl;
        std::cout << non_foolish_primes << " Non-Foolish Primes used." << std::endl;
        std::cout << foolish_primes << " Foolish Primes used." << std::endl;
        for (int i = 0; i < permutations.size(); i++)
        {
            std::cout << '[';
            if (permutations[i].size() > 0)
            {
                for (int j = 0; j < permutations[i].size() - 1; j++) std::cout << permutations[i][j] << ", ";
                std::cout << permutations[i].back();
            }
            std::cout << ']' << std::endl;
        }
        std::cout << std::endl;
    }

    answer = permutations.back().back().getNumberString();
    int_64x fact(1);

    for (int i = 2; i <= maximum; i++) fact *= i;
    //std::cout << answer << std::endl;
    std::cout << "Total ways to shuffle " << maximum << " numbers is " << fact << std::endl;

    return { answer, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is xxx
    //ran in 2.35781 seconds
}
*/