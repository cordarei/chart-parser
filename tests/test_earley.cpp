#include <UnitTest++.h>
#include "chart.h"

SUITE(ChartParseAlgoTests)
{
    TEST(CanParseSimpleSentenceWithTestGrammar)
    {
        std::vector<jhi::rule> rules;
        rules.push_back(jhi::rule("$simple", "$left", "$right"));
        rules.push_back(jhi::rule("$left", "Hello"));
        rules.push_back(jhi::rule("$right", "World"));
        jhi::grammar g(rules);

        std::vector<std::string> input;
        input.push_back("Hello");
        input.push_back("World");

        jhi::constituent_vector parses =
            jhi::earley(g, "$simple", input);
        CHECK_EQUAL(1, parses.size());
    }

    TEST(CanParseSentenceWithNounPhrase)
    {
        std::vector<jhi::rule> rules;
        rules.push_back(jhi::rule("$sentence", "$np", "$verb"));
        rules.push_back(jhi::rule("$np", "$det", "$noun"));
        rules.push_back(jhi::rule("$det", "the"));
        rules.push_back(jhi::rule("$noun", "boy"));
        rules.push_back(jhi::rule("$verb", "eats"));
        jhi::grammar g(rules);

        std::vector<std::string> input;
        input.push_back("the");
        input.push_back("boy");
        input.push_back("eats");

        jhi::constituent_vector parses =
            jhi::earley(g, "$sentence", input);
        CHECK_EQUAL(1, parses.size());
    }

    TEST(CanParseLeftRecursiveRule)
    {
        std::vector<jhi::rule> rules;
        rules.push_back(jhi::rule("$np", "$np", "$noun"));
        rules.push_back(jhi::rule("$np", "$noun"));
        rules.push_back(jhi::rule("$noun", "apple"));
        rules.push_back(jhi::rule("$noun", "worm"));
        rules.push_back(jhi::rule("$noun", "gate"));
        jhi::grammar g(rules);

        std::vector<std::string> input;
        input.push_back("apple");
        input.push_back("worm");
        input.push_back("gate");

        jhi::constituent_vector parses =
            jhi::earley(g, "$np", input);
        CHECK_EQUAL(1, parses.size());
    }

    TEST(ReturnsMultipleParseTreesWhenAmbiguous)
    {
        std::vector<jhi::rule> rules;
        rules.push_back(jhi::rule("$np", "$np", "$noun"));
        rules.push_back(jhi::rule("$np", "$noun", "$np"));
        rules.push_back(jhi::rule("$np", "$noun"));
        rules.push_back(jhi::rule("$noun", "apple"));
        rules.push_back(jhi::rule("$noun", "worm"));
        rules.push_back(jhi::rule("$noun", "gate"));
        jhi::grammar g(rules);

        std::vector<std::string> input;
        input.push_back("apple");
        input.push_back("worm");
        input.push_back("gate");

        jhi::constituent_vector parses =
            jhi::earley(g, "$np", input);
        CHECK_EQUAL(2, parses.size());
    }

    TEST(CanParseSimpleSentenceWithDefaultGrammar)
    {
        jhi::grammar g(jhi::get_default_rules());

        std::vector<std::string> input;
        input.push_back("the");
        input.push_back("boy");
        input.push_back("hits");
        input.push_back("a");
        input.push_back("dog");

        jhi::constituent_vector parses =
            jhi::earley(g, "$sentence", input);
        CHECK_EQUAL(1, parses.size());
        //CHECK(false);
    }
}
