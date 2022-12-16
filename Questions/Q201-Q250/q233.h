#include <Header_Files/pch.h>
#include <cmath>
#include <Header_Files/functions.h>
#include <Header_Files/print.h>

//Lattice points on a circle
int global_total = 0;

void GeneratePrimes(long long maximum, std::vector<int>& pythagorean_primes, std::vector<int>& standard_primes)
{
    //creates and separates all primes from 2 - maximum into pythagorean and non-pythagorean vectors
    std::vector<int> sieve;
    for (int i = 0; i <= maximum; i++) sieve.push_back(1);
    sieve[0] = 0; sieve[1] = 0;
    for (int i = 2; i <= maximum; i++)
        if (sieve[i])
            for (int j = 2 * i; j <= maximum; j += i) sieve[j] = 0;

    for (int i = 2; i < sieve.size(); i++)
    {
        if (sieve[i])
        {
            if (!((i - 1) % 4)) pythagorean_primes.push_back(i);
            else standard_primes.push_back(i);
        }
    }
}

void recursive_prime_selection(std::vector<bool> used_primes, std::vector<int>& prime_exponents, int required_primes, int& unknown_value, int& known_value, int current_location = 0, int level = 0)
{
    if (level == required_primes)
    {
        //the base of the recursion
        bool unknown = false;
        int new_value = 1;
        if (used_primes[0]) unknown = true; //this will contribute to the 'x' part of equation

        //start at 1
        for (int i = 1; i < used_primes.size(); i++)
            if (used_primes[i]) new_value *= prime_exponents[i];

        if (unknown) unknown_value += MyPow(2, required_primes - 1) * new_value;
        else known_value += MyPow(2, required_primes - 1) * new_value;
        return;
    }

    for (int i = current_location; i < used_primes.size(); i++)
    {
        used_primes[i] = true;
        recursive_prime_selection(used_primes, prime_exponents, required_primes, unknown_value, known_value, i + 1, level + 1);
        used_primes[i] = false;
    }
}

bool recursive_create_exponents(std::vector<int> current_exponents, int goal, int& increment_level, std::vector<std::vector<int>>& equations, int increment_value = 1, int current_level = 1)
{
    //increment level must be set to 1 when calling this function
    if (current_level == current_exponents.size())
    {
        //base of the recursion, calcuate what the value of the missing exponent will be
        //based on the results obtained from another recursive function
        int known = 0, unknown = 0;
        std::vector<bool> used_primes;
        for (int i = 0; i < current_exponents.size(); i++) used_primes.push_back(false);
        for (int i = 1; i <= current_exponents.size(); i++)
            recursive_prime_selection(used_primes, current_exponents, i, unknown, known);

        /*for (int i = 1; i < current_exponents.size(); i++) std::cout << current_exponents[i] << " ";
        std::cout << std::endl;
        std::cout << "Known = " << known << ", Unknown = " << unknown << std::endl;*/

        //solve for the missing exponent based on the values of "known" and "unknown"
        double x = (goal - known) / unknown;
        
        //check to see if x is smaller than any of the other exponents, if it is then
        //return false because this isn't a viable solution
        for (int i = 1; i < current_exponents.size(); i++)
            if (current_exponents[i] > x) return false;

        //next check to see if x is an integer, if it is then we've found a viable equation
        if ((goal - known) % unknown == 0)
        {
            /*std::cout << "p0^" << (int)x << " ";
            for (int i = 1; i < current_exponents.size(); i++) std::cout << "p" << i << "^" << current_exponents[i] << " ";
            std::cout << std::endl;*/
            std::vector<int> successful_equation = {(int)x};
            for (int i = current_exponents.size() - 1; i >= 1 ; i--) successful_equation.push_back(current_exponents[i]);
            equations.push_back(successful_equation);
        }

        //even if we didn't find an integer solution, return true so that this recursive function
        //knows which exponent to change next
        return true; 
    }

    while (true)
    {
        current_exponents[current_level] = increment_value;
        if (!recursive_create_exponents(current_exponents, goal, increment_level, equations, increment_value, current_level + 1))
        {
            //the recursion returned false, we need to go back until we get one level higher than where the failure was caused
            if (current_level != (increment_level - 1)) return 0; //the increment on this level caused failure so go back
            else increment_value++; //we've reach the level prior to where failure happened
        }
        else increment_value++; //if the function returns true then we're at the base of the recursion, add 1 to the final number
        
        //If we're here then the recursion returned true (which is only possible on the lowest level of recursion) or the failure happened
        //at the level below this one. Either way, we've changed the increment value and the current level becomes the new increment level
        increment_level = current_level;
    }
}

std::vector<std::vector<int> > create_lattice_equations(int goal)
{
    //Returns acceptable equations based on the goal number and the limit. It will keep adding
    //more primes until the goal number is exceeded.
    std::vector<std::vector<int> > equations;
    int distinct_primes = 1;

    while (true)
    {
        int minimum = 0;
        for (int i = 1; i <= distinct_primes; i++) minimum += MyPow(2, i - 1) * recursiveChoose(distinct_primes, i);
        if (minimum > goal) break;

        //we pick one of the distinct primes to have an unknown exponent value, we will choose the exponent value for 
        //the other distinct primes. Keep incrementing the known exponent values until the solved unknown value becomes 
        //less than the known ones. For example, with three distinct primes we start of solving for x in the equation
        //p0^x * p1^1 * p2^1 => 1 * (ways to choose 1 distinct prime) + 2 * (ways to choose 2 distinct primes) +
        //4 * (ways to choose 3 distinct primes) = goal number => 1 * (1 + 1 + x) + 2 * (2x + 1) + 4 * (x) = goal
        // => 2 + x + 4x + 2 + 4x = goal => x = (goal - 4) / 9. We see if x is an integer, and if so we add the exponents
        //to the list of useable equations
        int known = 0, unknown = 0, increment_level = 1;
        std::vector<int> equation;
        for (int i = 0; i < distinct_primes; i++) equation.push_back(0);
        recursive_create_exponents(equation, goal, increment_level, equations);

        //after returning from the recursive function, increase the number of distinct primes by 1
        distinct_primes++;
    }

    return equations;
}

void create_all_lattice_circles(long long limit, std::vector<int>& p_primes, unsigned long long* ways_to_make_n_or_less, unsigned long long* cumulative_sum_under_n,
    std::vector<int>& acceptable_lattice_factorization, std::vector<int> used_primes, unsigned long long& answer, unsigned long long& number_of_ways, unsigned long long current_num = 1, int current_level = 0)
{
    //recursively choose all combinations of Pythagorean Primes that fit the given equation and stay 
    //below the limit. For each successful combination we then see what the maximum product is for non-Pythagorean
    //primes and all of the ways to make this product or less.
    if (current_level == acceptable_lattice_factorization.size())
    {
        //We've reached the base of the recursion. Increment the recursion by 1 and then call another function
        //to see how many non-Pythagorean primes we can add before exceeding the limit
        answer += (cumulative_sum_under_n[limit / current_num] * current_num);
        number_of_ways += ways_to_make_n_or_less[limit / current_num];

        /*for (int i = 0; i < used_primes.size(); i++) std::cout << p_primes[used_primes[i]] << "^" << acceptable_lattice_factorization[i] << " ";
        std::cout << "yields a total sum of " << ways_to_make_n_or_less[limit / current_num] * current_num << std::endl;*/

        return;
    }

    for (int i = 0; i < p_primes.size(); i++)
    {
        //we start at index 0 each level of the recursion because we want to try each prime with each exponent
        //(i.e. 5^10 * 13^2 and 13^10 and 5^2 should both be tested). Because of this, another vector is used 
        //to keep track of which primes have been used already to make sure we don't pick the same one twice,
        //which would alter the prime factorization
        bool cont = true;
        for (int j = 0; j < used_primes.size(); j++)
        {
            if (used_primes[j] == i)
            {
                cont = false;
                break;
            }
        }

        if (!cont) continue; //go to next prime if this one's already been used

        //Figure out the highest exponent we could use for the current prime using logarithms. If the exponent
        //in the equation is larger than this, break out of the loop as we can no longer use primes to stay
        //under the limit.
        int max_allowable_exponent = log(limit / current_num) / log(p_primes[i]);
        if (max_allowable_exponent < acceptable_lattice_factorization[current_level]) break;

        //if we haven't broken out of this loop yet then we recursively call to the next level
        used_primes.push_back(i); //keep track of the index of the prime being used
        create_all_lattice_circles(limit, p_primes, ways_to_make_n_or_less, cumulative_sum_under_n, acceptable_lattice_factorization, used_primes, answer,
            number_of_ways, current_num * MyPow(p_primes[i], acceptable_lattice_factorization[current_level]), current_level + 1);
        used_primes.pop_back(); //remove this prime from the used vector after return from recursion
    }
}

std::pair<std::string, double> q233()
{
    auto run_time = std::chrono::steady_clock::now();

    /*
    The question prompt states that the maximum value of m is 200 which means that the largest goal value for lattice
    points is 4*200 = 800. Using 5 distinct primes, the smallest amount of lattice points we can get is 972 so we should
    never need to use more than 4 primes at a time. Using this fact, we first generate the governing equations based on
    the goal number. We then use these goal equations and the given limit of N to determine the largest Pythagorean and
    non-Pythagorean primes we will need. Since we know that we'll never need more than 4 distinct primes, we use the 
    first 4 pythagorean primes to figure out the largest values of primes we'll actually need.
    */

    unsigned long long answer = 0, limit = 100000000000, maximal_np_prime_product = 1, prime_limit = 0, number_of_ways = 0;
    int goal = 20;
    std::vector<int> p_primes = { 5, 13, 17, 29 }, np_primes;

    //Generate the Pythagorean Prime factorizations that can be used to reach the goal lattice points.
    //For example, when goal = 420 there are 5 possible equations we can use: p0^52, p0^17 * p1^1,
    //p0^10 * p1^2, p0^7 * p1^3 and p0^3 * p1^2 * p2^1
    std::vector<std::vector<int> > p_prime_factorizations = create_lattice_equations((goal - 4) / 8);
    for (int i = 0; i < p_prime_factorizations.size(); i++) vprint(p_prime_factorizations[i]);

    //We find the largest Pythagorean Prime needed by going through each of the factorizations and using the smallest known
    //primes until we get to the last exponent. We then figure out the largest prime that could go in this final position
    //while still staying under the limit.
    for (int i = 0; i < p_prime_factorizations.size(); i++)
    {
        unsigned long long product = 1;
        for (int j = 0; j < p_prime_factorizations[i].size() - 1; j++) product *= MyPow(p_primes[j], p_prime_factorizations[i][j]);
        long long largest_p_prime = pow(limit / product, 1.0 / p_prime_factorizations[i].back());
        if (largest_p_prime > prime_limit) prime_limit = largest_p_prime;

        //Now use the next known p_prime to calculate the largest possible product for np primes
        product *= MyPow(p_primes[p_prime_factorizations[i].size() - 1], p_prime_factorizations[i].back());
        if ((limit / product) > maximal_np_prime_product) maximal_np_prime_product = (limit / product);
    }

    /*std::cout << "Largest prime needed is " << prime_limit << std::endl;
    std::cout << "Largest np product is " << maximal_np_prime_product << std::endl;*/

    //You only get 4 lattice points when N is a number without any Pythagorean primes in its prime factorization
    if (goal == 4)
    {
        maximal_np_prime_product = limit;
        prime_limit = limit;
    }
    
    //Now that we know the largest prime we'll need we can generate all primes up to this limit
    //and then separate the Pythagorean Primes out
    p_primes.clear(); //first clear out the first four known p_primes
    GeneratePrimes(prime_limit, p_primes, np_primes);

    std::cout << p_primes.size() << " vs. " << np_primes.size() << std::endl;

    //The final step before calculating the answer is to memoize the number of ways we can create 
    //numbers using only non-Pythagorean primes under a given limit. 
    unsigned long long* ways_to_make_n_or_less = new unsigned long long[maximal_np_prime_product + 1]();
    unsigned long long* cumulative_sum_under_n = new unsigned long long[maximal_np_prime_product + 1]();
    ways_to_make_n_or_less[1] = 1;
    cumulative_sum_under_n[1] = 1;

    for (int i = 0; i < np_primes.size(); i++)
    {
        int current_location = 1;
        while ((maximal_np_prime_product / np_primes[i]) >= current_location)
        {
            if (ways_to_make_n_or_less[current_location])
            {
                ways_to_make_n_or_less[current_location * np_primes[i]] = 1;
                cumulative_sum_under_n[current_location * np_primes[i]] = current_location * np_primes[i];
            }
            current_location++;
        }
    }
    for (int i = 1; i <= maximal_np_prime_product; i++)
    {
        ways_to_make_n_or_less[i] += ways_to_make_n_or_less[i - 1];
        cumulative_sum_under_n[i] += cumulative_sum_under_n[i - 1];
    }

    //With everything prepared we can now count the ways to make 420 lattice points
    std::vector<int> used_primes;
    for (int i = 0; i < p_prime_factorizations.size(); i++)
    {
        create_all_lattice_circles(limit, p_primes, ways_to_make_n_or_less, cumulative_sum_under_n,
            p_prime_factorizations[i], used_primes, answer, number_of_ways);
    }
    
    if (goal == 4) number_of_ways = ways_to_make_n_or_less[limit];
    std::cout << "There are " << number_of_ways << " circles with " << goal << " lattice points" << std::endl;

    return { std::to_string(answer), std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - run_time).count() / 1000000000.0 };

    //the answer is 271204031455541309
    //ran in 0.117934 seconds
}

//NOTES
//See website