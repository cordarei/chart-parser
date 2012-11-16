//      Copyright Joseph Irwin <joseph.irwin.gt@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
//            http://www.boost.org/LICENSE_1_0.txt

#include "chart.h"

#include <iostream>
#include <algorithm>
#include "boost/lambda/lambda.hpp"

/**
 * parser - executable runs the Earley chart parsing algorithm on its input
 *
 * input: accepts a sentence as input, with one token on each line
 * output: outputs all parse trees found
 */
int main()
{
    std::vector<std::string> input;
    std::string line;
    //read input; 1 token per line
    while(std::getline(std::cin, line)) {
        input.push_back(line);
    }
    std::cout << "Input: ";
    std::for_each(input.begin(), input.end(), std::cout << boost::lambda::_1 << " ");
    std::cout << std::endl;

    //run Earley algorithm
    jhi::constituent_vector parses
        = jhi::earley(jhi::grammar(jhi::get_default_rules()), "$sentence", input);
    std::cout << "# parses: " << parses.size() << std::endl;
    //dump parse trees
    for(int i = 0; i < parses.size(); ++i)
        jhi::print_constituent(parses[i]);
    return 0;
}
