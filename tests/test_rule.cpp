#include <UnitTest++.h>
#include "chart.h"

SUITE(RuleTests)
{
    TEST(RuleConstructorSetsRhsCorrectly)
    {
        jhi::rule r1("$left", "$right");
        CHECK_EQUAL(1, r1.rhs().size());

        jhi::rule r2("l", "r1", "r2");
        CHECK_EQUAL(2, r2.rhs().size());

        jhi::rule r3("l", "r1", "r2", "r3");
        CHECK_EQUAL(3, r3.rhs().size());
    }

    TEST(RuleIsPosReturnsFalseForUnaryNonTerminal)
    {
        jhi::rule r("$left", "$right");
        CHECK_EQUAL(false, r.is_pos());
    }

    TEST(RuleIsEqualToSelf)
    {
        CHECK(jhi::rule("$np", "$np", "$noun") == jhi::rule("$np", "$np", "$noun"));
    }
}
