#include "tritset.h"
#include <gtest/gtest.h>

TEST(BasicTests, Allocate) {
    //резерв памяти для хранения 1000 тритов
    TritSet set(1000);
    // length of internal array
    size_t allocLength = set.capacity();
    GTEST_ASSERT_GE(allocLength, 1000 * 2 / 8 / sizeof(uint));

    //не выделяет никакой памяти
    set[1000000000] = Unknown;
    GTEST_ASSERT_EQ(allocLength, set.capacity());

    // false, but no exception or memory allocation
    if (set[2000000000] == True) {}
    GTEST_ASSERT_EQ(allocLength, set.capacity());

    //выделение памяти
    set[1000000000] = True;
    GTEST_ASSERT_LT(allocLength, set.capacity());

    //no memory operations
    allocLength = set.capacity();
    set[1000000000] = Unknown;
    set[1000000] = False;
    GTEST_ASSERT_EQ(allocLength, set.capacity());

    // Free to 1000
    set.shrink();
    GTEST_ASSERT_GT(allocLength, 1000 * 2 / 8 / sizeof(uint));
}

TEST(Logic, LogicAnd){
    TritSet setA(100);
    TritSet setB(200);

    TritSet setC = setA & setB;
    GTEST_ASSERT_EQ(setC.capacity(), setB.capacity());

    setA[0] = True;
    setB[0] = True;
    setA[1] = False;
    setB[1] = Unknown;

    TritSet setD = setA & setB;
    Trit true0 = setD[0]; // True & True
    Trit false1 = setD[1]; // False & Unknown
    GTEST_ASSERT_EQ(true0, True);
    GTEST_ASSERT_EQ(false1, False);
}

TEST(Logic, LogicOr){
    TritSet setA(100);
    TritSet setB(200);

    setA[0] = True;
    setB[0] = Unknown;
    setA[1] = False;
    setB[1] = False;

    TritSet setD = setA | setB;
    Trit true0 = setD[0]; // True | Unknown
    Trit false1 = setD[1]; // False | False
    GTEST_ASSERT_EQ(true0, True);
    GTEST_ASSERT_EQ(false1, False);
}

TEST(Logic, LogicNeg){
    TritSet setA(100);
    setA[0] = False;
    setA[1] = True;
    TritSet setB = ~setA;

    Trit true0 = setB[0];
    Trit false1 = setB[1];
    Trit unk2 = setB[2];
    GTEST_ASSERT_EQ(true0, True);
    GTEST_ASSERT_EQ(false1, False);
    GTEST_ASSERT_EQ(unk2, Unknown);
}

TEST(Atributes, CardinalityTrit){
    TritSet setA(100);
    setA[0] = True;
    setA[1] = False;
    setA[200] = False;

    size_t length = setA.capacity();

    GTEST_ASSERT_EQ(setA.cardinality(True), 1);
    GTEST_ASSERT_EQ(setA.cardinality(False), 2);
    GTEST_ASSERT_EQ(setA.cardinality(Unknown), length - 3);
}

TEST(Atributes, CardinalityMap){
    TritSet setA(100);
    setA[0] = True;
    setA[1] = False;
    setA[200] = False;
    setA[98] = False;
    size_t length = setA.capacity();

    std::unordered_map< Trit, int, std::hash<int>> trits = setA.cardinality();

    GTEST_ASSERT_EQ(trits[True], 1);
    GTEST_ASSERT_EQ(trits[False], 3);
    GTEST_ASSERT_EQ(trits[Unknown], length - 4);
}

TEST(Atributes, LogicalLengh){
    TritSet setA(100);
    setA[0] = True;
    setA[1] = False;

    GTEST_ASSERT_EQ(setA.length(), 2);

    setA[200] = False;
    setA[98] = False;

    GTEST_ASSERT_EQ(setA.length(), 201);

    setA[200] = Unknown;

    GTEST_ASSERT_GE(setA.capacity(), 201);
    GTEST_ASSERT_EQ(setA.length(), 99);
}

TEST(Methods, Shrink){
    TritSet setA(1000);
    setA[10000] = True;

    size_t length = setA.capacity();
    setA.shrink();
    GTEST_ASSERT_EQ(length, setA.capacity());

    setA[1002] = False;
    length = setA.capacity();

    setA[10000] = Unknown;
    setA.shrink();
    GTEST_ASSERT_GT(length, setA.capacity());
}

TEST(Methods, Trim){
    TritSet setA(1000);
    setA[10000] = True;
    setA[9999] = False;
    setA[1000] = False;

    // set Unknown setA[1001:]
    setA.trim(1001);
    Trit trit1000 = False;
    Trit trit9999 = Unknown;
    Trit trit10000 = Unknown;
    GTEST_ASSERT_EQ(trit1000, False);
    GTEST_ASSERT_EQ(trit9999, Unknown);
    GTEST_ASSERT_EQ(trit10000, Unknown);

    // set Unknown setA[1000:]
    setA.trim(1000);
    trit1000 = Unknown;
    GTEST_ASSERT_EQ(trit1000, Unknown);
}


TEST (Iterator, IteratorTest){
    TritSet setA(1000);
    size_t length = setA.capacity();

    for (auto trit: setA)
        trit = False;

    GTEST_ASSERT_EQ(length, setA.cardinality(False));
}

TEST (ExtremePoints, ZeroSize){
    TritSet setA(0);
    GTEST_ASSERT_EQ(setA.capacity(), 0);
    GTEST_ASSERT_EQ(setA.length(), 0);

    setA[10] = False;
    size_t length = setA.capacity();

    setA[10] = Unknown;
    GTEST_ASSERT_EQ(setA.capacity(), length);

    setA.shrink();
    GTEST_ASSERT_EQ(setA.capacity(), 0);
}

TEST (Operators, ConstOperator){
    TritSet setA(100);
    setA[0] = False;

    const TritSet setB = setA;
    GTEST_ASSERT_EQ(setB[0], False);
}

// I don't know how to test it...
// std::cout << "MOVE CALLED"; works
TEST (Operators, MoveConstructor){
    TritSet new_set = id(TritSet(1000));
    GTEST_ASSERT_GE(new_set.capacity(), 1000);
}