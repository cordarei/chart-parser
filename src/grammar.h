//      Copyright Joseph Irwin <joseph.irwin.gt@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
//            http://www.boost.org/LICENSE_1_0.txt

#ifndef __PARSER__GRAMMAR_H__
#define __PARSER__GRAMMAR_H__

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"

namespace jhi {

    /**
     * class rule
     *
     * encapsulates a rule in the grammar
     * (allows up to three symbols on the right-hand side)
     */
    class rule {
        std::string _h;
        std::vector<std::string> _rhs;

        public:
        rule(std::string const& head, std::string const& child) 
            : _h(head) {
                    _rhs.push_back(child);
                }
        rule(std::string const& head, std::string const& child1, std::string const& child2) 
            : _h(head) {
                    _rhs.push_back(child1);
                    _rhs.push_back(child2);
                }
        rule(std::string const& head, std::string const& child1, std::string const& child2, std::string const& child3) 
            : _h(head) {
                    _rhs.push_back(child1);
                    _rhs.push_back(child2);
                    _rhs.push_back(child3);
                }

        std::string const& head() const { return _h; }
        std::vector<std::string> const& rhs() const { return _rhs; }

        /**
         * return true if this rule takes a terminal (lexical token) on the right side
         */
        bool is_pos() const {
            return '$' != rhs().front()[0];
        }

        friend bool operator==(rule const& left, rule const& right) {
            return (left.head() == right.head())
                && (std::equal(left.rhs().begin(), left.rhs().end(), right.rhs().begin()));
        }

        friend std::ostream& operator<<(std::ostream& out, rule const& r) {
            using namespace boost::lambda;
            out << r.head() << " -->";
            std::vector<std::string> const& rhs = r.rhs();
            std::for_each(rhs.begin(), rhs.end(), out << " " << _1);
            return out;
        }
    };

    /**
     * class grammar
     *
     * contains rules and allows searching for rules with a given lhs symbol
     */
    class grammar {
        std::vector<rule> _rules;

        public:
        grammar(std::vector<rule> const& rules) : _rules(rules) {}

        /**
         * get rules that create the given symbol
         */
        std::vector<rule> rules_with_head(std::string const& head) const
        {
            using namespace boost::lambda;
            std::vector<rule> ret;
            std::remove_copy_if(_rules.begin(), _rules.end(), std::back_inserter(ret),
                    bind(std::not_equal_to<std::string>(), head, bind(&rule::head, _1))
                    );
            return ret;
        }
    };

    /*
     *
     * (a) sentence --> np, vp.
     * (b) np --> det, noun.
     * (c) np --> det, adj, noun.
     * (d) vp --> verb, np.
     * (e) vp --> vp, pp.
     * (f) pp --> prep, np.
     * (g) det --> [the].
     * (h) det --> [a].
     * (i) noun --> [boy].
     * (j) noun --> [dog].
     * (k) noun --> [rod].
     * (l) adj --> [smart].
     * (m) adj --> [long].
     * (n) verb --> [hits].
     * (o) prep --> [with].
     *
     */
    inline std::vector<rule> get_default_rules()
    {
        std::vector<rule> rules;

        rules.push_back(rule("$sentence", "$np", "$vp"));
        rules.push_back(rule("$np", "$det", "$noun"));
        rules.push_back(rule("$np", "$det", "$adj", "$noun"));
        //uncomment next line to allow noun phrase pps
        //rules.push_back(rule("$np", "$np", "$pp"));
        rules.push_back(rule("$vp", "$verb", "$np"));
        rules.push_back(rule("$vp", "$vp", "$pp"));
        rules.push_back(rule("$pp", "$prep", "$np"));
        rules.push_back(rule("$det", "the"));
        rules.push_back(rule("$det", "a"));
        rules.push_back(rule("$noun", "boy"));
        rules.push_back(rule("$noun", "dog"));
        rules.push_back(rule("$noun", "rod"));
        rules.push_back(rule("$adj", "smart"));
        rules.push_back(rule("$adj", "long"));
        rules.push_back(rule("$verb", "hits"));
        rules.push_back(rule("$prep", "with"));

        return rules;
    }

}
#endif //__PARSER__GRAMMAR_H__
