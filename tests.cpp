#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <random>

#include "gtest/gtest.h"
#include "tuple2.h"

TEST(basic, constructor)
{
    tuple2<std::string, int, double> t;
    get<1>(t) = 4;
    get<0>(t) = "sta";
    assert(get<0>(t) == "sta");
    assert(get<1>(t) == 4);
    assert(get<int>(t) == 4);
    assert(get<std::string>(t) == "sta");


    tuple2<std::string, int, double> t2 = t;
    assert(get<0>(t) == get<0>(t2));
    assert(get<1>(t) == get<1>(t2));
    assert(get<2>(t) == get<2>(t2));
    assert(get<std::string>(t) == get<std::string>(t2));
    assert(get<int>(t) == get<int>(t2));
    assert(get<double>(t) == get<double>(t2));

    tuple2<std::string, int, double> t3 = std::move(t2);
    assert(get<std::string>(t2).empty());
    assert(!get<std::string>(t3).empty());
    
    t2 = std::move(t3);
    assert(get<std::string>(t3).empty());
    assert(!get<std::string>(t2).empty());

    tuple2<int, double, int> tt(1, 23.0, 4);
    assert(get<0>(tt) == 1);
    assert(get<1>(tt) - 23.0 < 1e-9);
    assert(get<2>(tt) == 4);
}

// Tests by Artem Yurchenko
// Author: https://gitlab.com/grepcake/tuple
template <typename ...Args>
using container = tuple2<Args...>;

TEST(correctness, empty)
{
    std::tuple<> t;
}

TEST(correctness, int)
{
    const container <int> t1{ 5 };
    assert(get<0>(t1) == 5);
    assert(get<int>(t1) == 5);
}

TEST(correcntess, int_and_string)
{
    container<int, std::string> t1{ 5, "hey" };
    EXPECT_EQ(get<0>(t1), 5);
    EXPECT_EQ(get<std::string>(t1), "hey");
}


TEST(correcntess, copy_ctor)
{
    container<int, long> t1{ 5, 10 };
    auto t2 = t1;
    EXPECT_EQ(get<0>(t2), 5);
    EXPECT_EQ(get<1>(t2), 10);
}

TEST(correcntess, ce)
{
    container<int, std::string, int> t1{ 5, "hey", 6 };
    EXPECT_EQ(get<0>(t1), 5);
    EXPECT_EQ(get<2>(t1), 6);
    EXPECT_EQ(get<std::string>(t1), "hey");
    get<std::string>(t1).clear();
    assert(get<std::string>(t1) != "hey");
}

#include <stdio.h>

GTEST_API_
int main(int argc, char **argv) {
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
//  testing::GTEST_FLAG(filter) = "constructors*";
    return RUN_ALL_TESTS();
}
