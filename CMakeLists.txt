cmake_minimum_required(VERSION 3.25.2 FATAL_ERROR)


option(BUILD_TESTS_ONLY "Build only tests" OFF)

if(NOT BUILD_TESTS_ONLY)
  project(SortServer LANGUAGES CXX CUDA)
else()
  project(SortServer LANGUAGES CXX)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

if(NOT BUILD_TESTS_ONLY)
  find_package(CUDA REQUIRED)
  find_package(cpprestsdk REQUIRED)
  find_package(OpenSSL REQUIRED)
endif()


# Intel TBB package
find_package(TBB REQUIRED)
include_directories(${TBB_INCLUDE_DIRS})

# Google Test package
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if(NOT BUILD_TESTS_ONLY)
  # Define source files for backend code
  set(SRC_FILES
	  src/backend.cpp 
	  src/ProcessNumbers.cpp 
	  src/MergeSort.cpp
  )
  set_source_files_properties(
	src/ProcessNumbers.cpp 
	src/backend.cpp 
	src/MergeSort.cpp 
	PROPERTIES LANGUAGE CUDA
	)
  

  # Add the main executable
  add_executable(${PROJECT_NAME} ${SRC_FILES})
endif()

# Define source files for preprocessing tests
set(TEST_FILES
    src/ProcessNumbers.cpp
    src/SortTest.cpp
    src/MergeSort.cpp
    )

# Add preprocessing tests
add_executable(SortTest ${TEST_FILES})

# Add subdirectory for src
add_subdirectory(src)

if(NOT BUILD_TESTS_ONLY)
  # Link libraries to the main executable
  target_link_libraries(${PROJECT_NAME} PRIVATE 
      ${CUDA_LIBRARIES} 
      cpprestsdk::cpprest 
      OpenSSL::SSL 
      OpenSSL::Crypto 
      TBB::tbb
      pthread
  )
endif()

# Link libraries to the test executable
target_link_libraries(SortTest PRIVATE 
    ${GTEST_LIBRARIES} 
    TBB::tbb  # Add TBB for preprocessing
    gtest_main 
    pthread
)

# Include necessary directories
if(NOT BUILD_TESTS_ONLY)
  include_directories(${CUDA_INCLUDE_DIRS})
endif()

include_directories(include)

# Enable testing
enable_testing()

# Add test target
add_test(
    NAME SortTest
    COMMAND SortTest
)
