# Add the source files in this directory
set(SOURCES 
      src/ProcessNumbers.cpp
      src/MergeSort.cu)
if(NOT BUILD_TESTS_ONLY)
# Add these sources to the project executable
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
endif()
