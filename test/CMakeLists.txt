add_executable(run_tests.bin EXCLUDE_FROM_ALL
  mpi_handler.cpp
  test_mpi_main.cpp
)

target_link_libraries(run_tests.bin gtest
  mpi_handler
  ${Boost_LIBRARIES}
  ${MPI_LIB}
)

add_custom_target(test COMMAND mpirun -np 2 run_tests.bin
                       DEPENDS run_tests.bin)
