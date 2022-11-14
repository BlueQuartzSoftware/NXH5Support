find_package(Catch2 CONFIG REQUIRED)

include(Catch)

add_executable(NXH5Support_test)

target_link_libraries(NXH5Support_test
  PRIVATE
  NX::H5Support
  Catch2::Catch2

  # Catch2::Catch2WithMain
)

set_target_properties(NXH5Support_test
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:NXH5Support>
)

target_compile_definitions(NXH5Support_test
  PRIVATE
  NXH5Support_BUILD_DIR="$<TARGET_FILE_DIR:NXH5Support_test>"
)

target_compile_options(NXH5Support_test
  PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

set(TEST_SOURCE_DIR "${NXH5Support_SOURCE_DIR}/test")
set(NXH5Support_TestDataDir "${NXH5Support_BINARY_DIR}/test")

set(configured_filepath ${NXH5Support_BINARY_DIR}/generated/NXH5Support/TestGenConstants.hpp)
configure_file("${CMAKE_CURRENT_LIST_DIR}/cmake/TestGenConstants.hpp.in"
  "${NXH5Support_BINARY_DIR}/generated/NXH5Support/TestGenConstants.hpp"
)

target_sources(NXH5Support_test
  PRIVATE
  ${TEST_SOURCE_DIR}/h5support_test_main.cpp
  ${TEST_SOURCE_DIR}/test_readwrite.cpp
  ${TEST_SOURCE_DIR}/test_IO.cpp
  ${TEST_SOURCE_DIR}/test_IO_chunks.cpp
  ${configured_filepath}
)

add_custom_command(TARGET NXH5Support_test PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory

  "${CMAKE_SOURCE_DIR}/test/data" "${NXH5Support_TestDataDir}")

# ${CMAKE_SOURCE_DIR}/test/data $<TARGET_FILE_DIR:NXH5Support_test>)
target_include_directories(NXH5Support_test PRIVATE ${NXH5Support_GENERATED_DIR})

catch_discover_tests(NXH5Support_test)