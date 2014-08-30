// This file defines a generic handler for MPI messages.
//
// In a project, many tags may be user by different modules. If the order in
// which they are processed isn't important, the MPI could be probed by each
// module looking for each tag defined. However, if the order is relevant,
// probing in the wrong order of retrieval could lead to wrong results.
//
// If any tag is probed, then the module should relinquish its control of the
// processor to allow another module to run if a unknown tag arrives. This
// requires some repeated logic.
//
// Finally, it's easier to just define a callback function to be called when the
// required tag is received. This is the problem this class solves.
//
// There are two kinds of handlers:
// 1) handler for specific tags, which receive an integer describing the source
// of the message and returns true if the message was consumed.
// 2) generic handler, which can handle messages all other handlers ignored and
// receives the source and tag, returning true if the message was consumed.
//
// The handlers are called from specific to generic and in the order in which
// they were inserted.
//
// Messages are only processed when the method run() is called and there's no
// inherit support for threads, which can be implemented in the handler
// functions.
//
// Example:
// MPIHandler handler(world);
// handler.insert(2, [](int tag) { /*do stuff*/ return true; }
// handler.insert_generic([](int source, int tag)
//   { if (source == 2) { /*do stuff*/ return true; } return false; });
// handler.run();

#ifndef __MPI_HANDLER_HPP__
#define __MPI_HANDLER_HPP__

#include <boost/mpi/communicator.hpp>
#include <functional>
#include <map>
#include <list>

class MPIHandler {
  public:
    // Types of handlers allowed
    typedef std::function<bool (int)> handler_type;
    typedef std::function<bool (int, int)> generic_handler_type;

    MPIHandler(boost::mpi::communicator& world);

    // Inserts a generic handler
    void insert_generic(generic_handler_type handler);

    // Clears all generic handlers
    void clear_generic();

    // Inserts a specific handler for the given tag
    void insert(int tag, handler_type handler);

    // Clears all specific handlers
    void clear();

    // Clears all specific handlers for a given tag
    void clear(int tag);

    // Processes the MPI messages, calling the handlers when there's a possible
    // match
    void run();

  private:
    std::list<generic_handler_type> generic_handlers_;
    std::map<int, std::list<handler_type>> map_tags_to_handlers_;
    boost::mpi::communicator& world_;
};

#endif
