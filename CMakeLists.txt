# thread-pool-cpp build script

cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

project(thread-pool-cpp VERSION 1.0.0)

ADD_DEFINITIONS(-std=c++1y -Wall -Werror -O3)

option(THREAD_POOL_CPP_BUILD_TESTS "Build tests." OFF)
option(THREAD_POOL_CPP_BUILD_BENCHMARKS "Build benchmarks." OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thread_pool)

# http://stackoverflow.com/a/11583676
add_library(thread-pool-cpp INTERFACE)

# If true C++11 thread_local support exists, we will use it:
include(thread_pool_has_thread_local_storage)
thread_pool_has_thread_local_storage(THREAD_POOL_HAS_THREAD_LOCAL_STORAGE)
message("THREAD_POOL_HAS_THREAD_LOCAL_STORAGE: ${THREAD_POOL_HAS_THREAD_LOCAL_STORAGE}")

if(NOT THREAD_POOL_HAS_THREAD_LOCAL_STORAGE)
  # Else, we will check for backups
  include(thread_pool_has_thread_storage)
  thread_pool_has_thread_storage(THREAD_POOL_HAS_THREAD_STORAGE)
  message("THREAD_POOL_HAS_THREAD_STORAGE: ${THREAD_POOL_HAS_THREAD_STORAGE}")
  if(NOT THREAD_POOL_HAS_THREAD_STORAGE)
    message(SEND_ERROR "Compiler does not support: thread_local, __thread, or declspec(thread)")
  endif()
endif()

if(THREAD_POOL_HAS_THREAD_LOCAL_STORAGE)
  target_compile_definitions(thread-pool-cpp PUBLIC THREAD_POOL_HAS_THREAD_LOCAL_STORAGE=1)
endif()

if(THREAD_POOL_CPP_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(THREAD_POOL_CPP_BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()

#### install

set(TARGET_NAME thread-pool-cpp)

set(config_install_dir "lib/cmake/${TARGET_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${TARGET_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${TARGET_NAME}Config.cmake")
set(targets_export_name "${TARGET_NAME}Targets")
set(namespace "${TARGET_NAME}::")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${version_config}" COMPATIBILITY SameMajorVersion
  )

configure_package_config_file(
  "cmake/Config.cmake.in"
  "${project_config}"
  INSTALL_DESTINATION "${config_install_dir}"
  )

install(
  TARGETS thread-pool-cpp
  EXPORT "${targets_export_name}"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
  INCLUDES DESTINATION "${include_install_dir}"
  )

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool/ # Trailing slash is required
  DESTINATION "${include_install_dir}/thread_pool" # match local layout
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
  )

install(
  FILES "${project_config}" "${version_config}"
  DESTINATION "${config_install_dir}"
  )

install(
  EXPORT "${targets_export_name}"
  NAMESPACE "${namespace}"
  DESTINATION "${config_install_dir}"
  )
