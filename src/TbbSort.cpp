#include <TbbSort.h>
#include <tbb/tbb.h>

void sort_array(std::vector<int>& array) {
  tbb::parallel_sort(array.begin(), array.end());
}
