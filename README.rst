=====
Top-Down Chart Parser
=====

Author
-------
Joseph Irwin

Overview
---------
A top-down chart parser.
I wrote this parser for a class in Computational Linguistics.

Design
-------

Parts:

* parsing algorithm
* grammar
* tokenizer?

Data Structures:

* rule
* arc
* chart

Grammar
--------
(a) sentence --> np, vp.
(b) np --> det, noun.
(c) np --> det, adj, noun.
(d) vp --> verb, np.
(e) vp --> vp, pp.
(f) pp --> prep, np.
(g) det --> [the].
(h) det --> [a].
(i) noun --> [boy].
(j) noun --> [dog].
(k) noun --> [rod].
(l) adj --> [smart].
(m) adj --> [long].
(n) verb --> [hits].
(o) prep --> [with].

 Note: to distinguish between terminal and non-terminals,
 in the actual grammar we add '$' to the beginning of non-terminal
 symbols. (Ex. "$sentence --> $np $vp", "$noun --> boy")

Build
------
To build, follow the following steps from this directory:

::

    mkdir build
    cd build
    cmake ..
    make

Requirements:

* Boost C++ libraries (shared_ptr, lambda)
* CMake

License
--------

Released under the `Boost Software License, Version 1.0 <http://www.boost.org/LICENSE_1_0.txt>`.

`UnitTest++ <http://unittest-cpp.sourceforge.net/>` is copyright its
authors, and included under the terms of the MIT License (see
`unittestcpp/COPYING`).

References
-----------
James Allen. Natural Language Understanding. Chapter 3. The Benjamin/Cummings Publishing Company, 1995.

Daniel Jurafsky and James H. Martin. Speech and Language Processing. 377-385. Prentice Hall, 2000.
