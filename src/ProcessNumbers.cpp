#include <ProcessNumbers.h>

void process_numbers(FILE* c_file, const std::vector<int>& array) {

  fprintf(c_file, "%s", "{ \"numbers\" : ");
  for (const auto& i : array) {
    // TODO
    fprintf(c_file, "%d, ", i);
  }
  fprintf(c_file, "%s", "}");
}
