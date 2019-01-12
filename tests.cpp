#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <random>

#include "gtest/gtest.h"
#include "tuple2.h"

TEST(basic, default_con)
{
    tuple2<std::string, int, double> t;
    get<1>(t) = 4;
    get<0>(t) = "sta";
    assert(get<0>(t) == "sta");
    assert(get<1>(t) == 4);
    assert(get<int>(t) == 4);
    assert(get<std::string>(t) == "sta");
}

TEST(basic, c_con)
{
    tuple2<std::string, int, double> t;
    get<1>(t) = 4;
    get<0>(t) = "sta";

    tuple2<std::string, int, double> t2 = t;
    assert(get<0>(t) == get<0>(t2));
    assert(get<1>(t) == get<1>(t2));
    assert(get<2>(t) == get<2>(t2));
    assert(get<std::string>(t) == get<std::string>(t2));
    assert(get<int>(t) == get<int>(t2));
    assert(get<double>(t) == get<double>(t2));
}

TEST(basic, move_con)
{
    tuple2<std::string, int, double> t2;
    get<1>(t2) = 4;
    get<0>(t2) = "sta";

    tuple2<std::string, int, double> t3 = std::move(t2);
    assert(get<std::string>(t2).empty());
    assert(!get<std::string>(t3).empty());
}

TEST(basic, args_con)
{
    tuple2<std::string, int, double> t;
    get<1>(t) = 4;
    get<0>(t) = "sta";

    tuple2<int, double, int> tt(1, 23.0, 4);
    assert(get<0>(tt) == 1);
    assert(get<1>(tt) - 23.0 < 1e-9);
    assert(get<2>(tt) == 4);
}

TEST(basic, get_move)
{
    tuple2<std::string, int, double> t;
    get<1>(t) = 4;
    get<0>(t) = "sta";

    std::string s("123123123");
    get<0>(t) = std::move(s);
    EXPECT_EQ(get<0>(t), "123123123");
    EXPECT_EQ(s.size(), 0);
}

TEST(basic, cast_con)
{
    tuple2<std::string, int, double> t("asd", 12, 12);
    EXPECT_EQ(get<0>(t), "asd");
    EXPECT_EQ(get<std::string>(t), "asd");

    EXPECT_EQ(get<1>(t), 12);
    EXPECT_EQ(get<int>(t), 12);

    assert(std::abs(get<2>(t) - 12) < 1e-9);
    assert(std::abs(get<double>(t) - 12) < 1e-9);
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
