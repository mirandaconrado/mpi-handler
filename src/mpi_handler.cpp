#include "mpi_handler.hpp"

MPIHandler::MPIHandler(boost::mpi::communicator& world):
  world_(world) {
    clear_default_handler();
}

void MPIHandler::set_default_handler(default_handler_type handler) {
  default_handler_ = handler;
}

void MPIHandler::clear_default_handler() {
  default_handler_ = [](int, int) { return false; };
}

void MPIHandler::insert(int tag, handler_type handler) {
  map_tags_to_handlers_[tag].push_back(handler);
}

void MPIHandler::clear() {
  map_tags_to_handlers_.clear();
}

void MPIHandler::clear(int tag) {
  auto it = map_tags_to_handlers_.find(tag);
  if (it != map_tags_to_handlers_.end())
    map_tags_to_handlers_.erase(it);
}

void MPIHandler::run() {
  bool stop = false;
  while (!stop) {
    auto status_opt = world_.iprobe();
    if (status_opt) {
      auto status = status_opt.get();

      bool processed = false;

      auto it = map_tags_to_handlers_.find(status.tag());
      if (it != map_tags_to_handlers_.end()) {
        for (auto& handler : it->second)
          if (handler(status.source())) {
            processed = true;
            break;
          }
      }

      if (!processed)
        processed = default_handler_(status.source(), status.tag());

      if (!processed)
        stop = true;
    }
    else
      stop = true;
  }
}
