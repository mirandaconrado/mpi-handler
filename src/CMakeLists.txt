add_library(mpi_handler SHARED
  mpi_handler
)

target_link_libraries(mpi_handler
  ${Boost_LIBRARIES}
  ${MPI_LIB}
)
