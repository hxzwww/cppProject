#include <ProcessNumbers.h>
#include "MergeSort.h"
#include <TbbSort.h>

void process_numbers(FILE* c_file, std::vector<int>& array, int onCuda) {

  if (onCuda) {
    merge_sort_cuda(array.data(), array.size());
  } else {
    sort_array(array);
  }

  fprintf(c_file, "%s", "{\"numbers\": [");
  int x = array.size();
  int cnt = 0;
  for (const auto& i : array) {
    fprintf(c_file, "%d", i);
    cnt += 1;
    if (cnt != x) {
      fprintf(c_file, "%s", ",");
    }
  }
  fprintf(c_file, "%s", "]}");
}
