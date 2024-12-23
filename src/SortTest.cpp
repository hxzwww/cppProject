#include <gtest/gtest.h>
#include "ProcessNumbers.h"

using vt = std::vector<int>;

TEST(SortTest, AbobaTest) {
    vt array{1, 2, 3, 4, 5};   
    const vt array_copy = array ;   

    sort_array(array);
    ASSERT_EQ(array, array_copy);
}

TEST(SortTest, AbobaTest2) {
    vt array2{1, 2, 9, 4, 5};   
    const vt array_copy2{1, 2, 4, 5, 9};

    sort_array(array2);
    ASSERT_EQ(array2, array_copy2);
}

