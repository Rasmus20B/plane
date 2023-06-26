include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(queue_test queue_test.cc)
add_executable(vector_test vector_test.cc)
add_executable(queue_bench queue_benchmark.cc)

target_link_libraries(queue_test GTest::gtest_main)
target_link_libraries(vector_test GTest::gtest_main raylib)

include(GoogleTest)
