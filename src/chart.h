//      Copyright Joseph Irwin <joseph.irwin.gt@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
//            http://www.boost.org/LICENSE_1_0.txt

#ifndef __PARSER__CHART_H__
#define __PARSER__CHART_H__
#include "boost/shared_ptr.hpp"
#include "grammar.h"

namespace jhi {
    /**
     * encapsulates a constituent in a parse tree
     */
    class constituent {
        typedef boost::shared_ptr<constituent> pointer_type;
        typedef std::vector<pointer_type> child_vector_type;

            int _s;
            int _e;
            std::string _h;
            child_vector_type _children;
        public:
            constituent(int start, int end, std::string head)
                : _s(start), _e(end), _h(head) {}
            constituent(int start, int end, std::string head, child_vector_type const& children)
                : _s(start), _e(end), _h(head), _children(children) {}

            std::string head() const { return _h; }
            std::vector<boost::shared_ptr<jhi::constituent> > const& children() const {
                return _children;
            }
            int start() const { return _s; }
            int end() const { return _e; }
    };
    typedef boost::shared_ptr<constituent> constituent_ptr;
    typedef std::vector<constituent_ptr> constituent_vector;

    /**
     * prints the given constituent tree to the standard output
     */
    inline void print_constituent(constituent_ptr c, std::string indent = "") {
        if (c->children().empty()) {
            std::cout << indent << "'" << c->head() << "'" << std::endl;
        } else {
            std::cout << indent << "(" << c->head() << std::endl;
            std::for_each(c->children().begin(), c->children().end(),
                    boost::lambda::bind(print_constituent, boost::lambda::_1, (indent+"    ")));
            std::cout << indent << ")" << std::endl;
        }
    }

    /**
     * encapsulates an arc in the parse chart
     *
     * contains a rule to apply, and the constituents currently built
     */
    class arc {
            int _s;
            int _e;
            jhi::rule _r;
            int _next;
            boost::shared_ptr<jhi::constituent> _c;
            constituent_vector _parts;
        public:
            arc(int start, int end, jhi::rule const& r)
                : _s(start), _e(end), _r(r), _next(0), _c() {}
            arc(int start, int end, boost::shared_ptr<jhi::constituent> c, jhi::rule const& r)
                : _s(start), _e(end), _r(r), _next(r.rhs().size()), _c(c) {}

            bool complete() const { return _c && _next == _r.rhs().size(); }

            constituent_ptr constituent() const { return _c; }
            jhi::rule const& rule() const { return _r; }
            int start() const { return _s; }
            int end() const { return _e; }
            std::string next_symbol() const { return complete() ? "" : _r.rhs()[_next]; }

            /**
             * return true if the given arc matches this arc
             *
             * two arcs match if they apply the same rule over the same span
             */
            bool matches(boost::shared_ptr<arc> other) {
                return _s == other->_s
                    && _e == other->_e
                    && _r == other->_r
                    && _next == other->_next
                    && _c == other->_c;
            }

            /**
             * create a new arc by extending this arc with the given constituent
             */
            boost::shared_ptr<arc> extend(boost::shared_ptr<jhi::constituent> c) {
                constituent_vector partials(_parts);
                partials.push_back(c);

                //create new complete arc or partial arc
                if (partials.size() == _r.rhs().size()) {
                    return boost::shared_ptr<arc>(
                        new arc(_s,
                            c->end(),
                            constituent_ptr(
                                new jhi::constituent(_s, c->end(), _r.head(), partials)
                                ),
                            _r)
                    );
                } else {
                    return boost::shared_ptr<arc>(new arc(_s, c->end(), _r, partials));
                }
            }

        private:
            arc(int start, int end, jhi::rule const& r, constituent_vector const& partials)
                : _s(start), _e(end), _r(r), _next(partials.size()), _parts(partials) {}
    };

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
            bool verbose=false);

}//namespace jhi

#endif //__PARSER__CHART_H__
