#include "mpi_handler.hpp"

#include <gtest/gtest.h>

#include "test_mpi.hpp"

TEST(MPIHandler, ClearGenericHandler) {
  int received_tag = 0;

  MPIHandler handler(world);
  handler.insert_generic(
      [&](int source, int tag) { received_tag = tag; return false; });
  handler.clear_generic();
  world.barrier();

  if (world.rank() == 1) {
    int val = 3;
    world.send(0, 2, val);
  }
  world.barrier();

  handler.run();
  if (world.rank() == 0)
    EXPECT_EQ(0, received_tag);
  world.barrier();
}

TEST(MPIHandler, ClearHandler) {
  bool received_specific = false;

  MPIHandler handler(world);
  handler.insert(2,
      [&](int source) { received_specific = true; return false; });
  handler.clear(2);
  world.barrier();

  if (world.rank() == 1) {
    int val = 3;
    world.send(0, 2, val);
  }
  world.barrier();

  handler.run();
  if (world.rank() == 0)
    EXPECT_FALSE(received_specific);
  world.barrier();
}

TEST(MPIHandler, InsertGenericHandler) {
  int received_tag = 0;

  MPIHandler handler(world);
  handler.insert_generic(
      [&](int source, int tag) { received_tag = tag; return false; });
  world.barrier();

  if (world.rank() == 1) {
    int val = 3;
    world.send(0, 2, val);
  }
  world.barrier();

  handler.run();
  if (world.rank() == 0)
    EXPECT_EQ(2, received_tag);
  world.barrier();
}

TEST(MPIHandler, InsertHandler) {
  bool received_specific = false;

  MPIHandler handler(world);
  handler.insert(2,
      [&](int source) { received_specific = true; return false; });
  world.barrier();

  if (world.rank() == 1) {
    int val = 3;
    world.send(0, 2, val);
  }
  world.barrier();

  handler.run();
  if (world.rank() == 0)
    EXPECT_TRUE(received_specific);
  world.barrier();
}

