#include<UnitTest++.h>
#include "grammar.h"

#include <boost/shared_ptr.hpp>

SUITE(GrammarTests)
{
    TEST(CanInitializeGrammarWithRules)
    {
        std::vector<jhi::rule> rules;
        rules.push_back(jhi::rule("$test", "test"));
        jhi::grammar g(rules);
        CHECK_EQUAL(1, g.rules_with_head("$test").size());
    }

    struct grammar_fixture {
        boost::shared_ptr<jhi::grammar> g;
        grammar_fixture() {
            g = boost::shared_ptr<jhi::grammar>(
                    new jhi::grammar(
                            jhi::get_default_rules()));
        }
    };

    TEST_FIXTURE(grammar_fixture, CanGetRulesMakingSentence)
    {
        //using namespace boost::lambda;
        //std::vector<jhi::rule> r(g->rules_with_head("$noun"));
        //std::for_each(r.begin(), r.end(), std::cout << _1 << "\n");
        CHECK_EQUAL(1, g->rules_with_head("$sentence").size());
    }
}
