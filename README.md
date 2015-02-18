<img style="display: block; margin: auto;" src="img/mbb.png" alt"Logo libmbb"/>

libmbb - Embedded Building Bricks
=================================

Synopsis
--------

*libmbb* is a C library assembling tools for the application development on
embedded platforms.

Features
--------

* [Hierarchical state machines](docs/HSM.md), including timers
* [Fixed-cacpacity queues](docs/Queue.md)
* [Debugging macros](docs/Debug.md)
* [Unit tests](docs/Test.md)

Building
--------

*libmbb* uses [scons](http://www.scons.org) for Building. Calling `scons` will
build the library (`mbb/libmbb.a`) along with the example programs and tests.

Call `scons test` to run all unit tests.

Examples
--------

* [debugging](examples/debugging.c): debugging macros
* [monostable](examples/monostable.c): multiple HSM instances, libev timers
* [pelican](examples/pelican.c): [Miro Samek's](http://www.state-machine.com/)
  [PEdestrian LIght CONtrolled (PELICAN) Crossing
  Example](http://www.state-machine.com/resources/AN_PELICAN.pdf), periodic
  timers

Tools
-----

The tools sub directory contains the following command line tools:

* `mhsm_scaffold` adds event processing function stubs to source files
* `munt_main` generates main functions for unit tests

Dependencies
------------

* The [libev](http://software.schmorp.de/pkg/libev.html) timers backend and the
  examples using it are only compiled if `libev` and its header files are
  installed on your system
* The tools are written in and thus depend on
  [Ruby](https://www.ruby-lang.org/)

License
-------

*libmbb* is [MIT licensed](LICENSE.txt).
