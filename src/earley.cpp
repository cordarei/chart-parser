#include "chart.h"

namespace {

    typedef boost::shared_ptr<jhi::arc> arc_ptr;
    typedef std::vector<arc_ptr> chart_cell_type;
    typedef std::vector<chart_cell_type> chart_type;

    /**
     * pretty print the chart
     */
    void print_chart(chart_type &chart) {
        std::cout << "Chart:\n";
        for(int i = 0; i < chart.size(); ++i) {
            std::cout << "Cell: ";
            for(int j = 0; j < chart[i].size(); ++j) {
                std::cout << "[ "
                          << chart[i][j]->rule()
                          << " ("
                          << chart[i][j]->next_symbol()
                          << ") ("
                          << chart[i][j]->start()
                          << " "
                          << chart[i][j]->end()
                          << ") "
                          << (chart[i][j]->complete() ? "c" : "i")
                          << "] ";
            }
            std::cout << "\n";
        }
    }

    /**
     * add arc to chart
     *
     * adds arc only if the new arc doesn't match any arc already in the chart
     */
    void add_to_chart(chart_type &chart, arc_ptr arc) {
        for(int k = 0; k < chart[arc->end()].size(); ++k)
            if (arc->matches(chart[arc->end()][k]))
                return;
        chart[arc->end()].push_back(arc);
    }
}

namespace jhi {
    /**
     * earley
     *
     * runs Earley algorithm using the given grammar and input
     *
     * verbose -- optionally dump state of chart after each step
     */
    constituent_vector earley(
            jhi::grammar const& g,
            std::string const& start_symbol,
            std::vector<std::string> input,
            bool verbose)
    {
        typedef boost::shared_ptr<jhi::arc> arc_ptr;
        typedef std::vector<arc_ptr> chart_cell_type;
        typedef std::vector<chart_cell_type> chart_type;
        chart_type chart(input.size() + 1);

        //init chart
        std::vector<jhi::rule> s_rules(g.rules_with_head(start_symbol));
        for(int i = 0; i < s_rules.size(); ++i)
            chart[0].push_back(arc_ptr(new arc(0, 0, s_rules[i])));

        //fill chart
        for(int i = 0; i < chart.size(); ++i) {
            for(int j = 0; j < chart[i].size(); ++j) {
                if (verbose) print_chart(chart);
                arc_ptr a = chart[i][j];
                if (a->complete()) {
                    chart_cell_type cell = chart[a->start()];
                    chart_cell_type::iterator k = chart[a->start()].begin();
                    for( ; k != chart[a->start()].end(); ++k ) {
                        //extend incomplete arcs that match current constituent
                        if (!(*k)->complete() && (*k)->next_symbol() == a->constituent()->head()) {
                            add_to_chart(chart, (*k)->extend(a->constituent()));
                        }
                    }
                } else {
                    std::vector<jhi::rule> new_rules(g.rules_with_head(a->next_symbol()));

                    typedef std::vector<jhi::rule>::iterator rule_iter;
                    for(rule_iter k = new_rules.begin(); k != new_rules.end(); ++k) {
                        add_to_chart(chart, arc_ptr(new arc(i, i, *k)));
                        if (k->is_pos()) { //for rules that take terminals
                            //add arc if rule matches word at current position
                            if (i < input.size() && input[i] == k->rhs().front()) {
                                //create child constituent for word itself
                                constituent_ptr word(new constituent(i, i+1, input[i]));
                                constituent_vector v; v.push_back(word);
                                //then add arc to complete the POS rule
                                add_to_chart(chart,
                                        arc_ptr(new arc(i,
                                                i+1,
                                                boost::shared_ptr<jhi::constituent>(
                                                    new constituent(i, i+1, k->head(), v)
                                                ),
                                                *k)));
                            }
                        }
                    }
                }
            }
        }


        //gather successful parse trees
        std::vector<boost::shared_ptr<jhi::constituent> > parses;
        for(int i = 0; i < chart.back().size(); ++i) {
            if (chart.back()[i]->complete()
                && start_symbol == chart.back()[i]->constituent()->head()) {
                parses.push_back(chart.back()[i]->constituent());
            }
        }
        return parses;

    }//earley
}
