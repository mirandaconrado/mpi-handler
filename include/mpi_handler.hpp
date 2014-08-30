#ifndef __MPI_HANDLER_HPP__
#define __MPI_HANDLER_HPP__

#include <boost/mpi/communicator.hpp>
#include <functional>
#include <map>
#include <list>

class MPIHandler {
  public:
    typedef std::function<bool (int)> handler_type;
    typedef std::function<bool (int, int)> default_handler_type;

    MPIHandler(boost::mpi::communicator& world);

    void set_default(default_handler_type handler);
    void clear_default();

    void insert(int tag, handler_type handler);

    void clear();
    void clear(int tag);

    void run();

  private:
    default_handler_type default_handler_;
    std::map<int, std::list<handler_type>> map_tags_to_handlers_;
    boost::mpi::communicator& world_;
};

#endif
