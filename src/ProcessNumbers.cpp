#include <ProcessNumbers.h>
#include <tbb/tbb.h>

void process_numbers(FILE* c_file, std::vector<int>& array) {

  tbb::parallel_sort(array.begin(), array.end());

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
