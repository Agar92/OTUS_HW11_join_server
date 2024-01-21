#include <table.h>

#include <gtest/gtest.h>
#include <string>


TEST(TableOperations, Insert)
{
    Table A;
    bool different_succes = true;
    const int num_records = 6;
    for (auto id = 0; id < num_records; id++)
        different_succes &= A.Insert(id, std::to_string(id));
    auto dublicate_success = A.Insert(0, "lean");
    ASSERT_TRUE(different_succes);
    ASSERT_FALSE(dublicate_success);
}

void FillA(Table& A)
{
    A.Insert(0, "lean");
    A.Insert(1, "sweater");
    A.Insert(2, "frank");
    A.Insert(3, "violation");
    A.Insert(4, "quality");
    A.Insert(5, "precision");
}

void FillB(Table& B)
{
    B.Insert(3, "proposal");
    B.Insert(4, "example");
    B.Insert(5, "lake");
    B.Insert(6, "flour");
    B.Insert(7, "wonder");
    B.Insert(8, "selection");
}

TEST(TableOperations, Intersection)
{
    Table A, B;
    FillA(A);
    FillB(B);
    auto result = Intersect(A, B);
    ASSERT_EQ(result.m_records.size(), 3UL);
    for(auto i = 0, id = 3; id <=5 ; i++, id++)
    {
        ASSERT_EQ(result.m_records[i].m_left->id, id);
        ASSERT_EQ(result.m_records[i].m_right->id, id);
        ASSERT_NE(result.m_records[i].m_left->name, "");
        ASSERT_NE(result.m_records[i].m_right->name, "");
        ASSERT_NE(result.m_records[i].m_left->name, result.m_records[i].m_right->name);
    }
}

TEST(TableOperations, ViewRetention)
{
    Table A, B;
    FillA(A);
    FillB(B);
    auto result = Intersect(A, B);
    A.Truncate();
    B.Truncate();
    ASSERT_EQ(result.m_records.size(), 3UL);
    for(auto i = 0, id = 3; id <=5 ; i++, id++)
    {
        ASSERT_EQ(result.m_records[i].m_left->id, id);
        ASSERT_EQ(result.m_records[i].m_right->id, id);
        ASSERT_NE(result.m_records[i].m_left->name, "");
        ASSERT_NE(result.m_records[i].m_right->name, "");
        ASSERT_NE(result.m_records[i].m_left->name, result.m_records[i].m_right->name);
    }
}

TEST(TableOperations, SymmetricDifference)
{
    Table A, B;
    FillA(A);
    FillB(B);
    auto result = SymmetricDifference(A, B);
    ASSERT_EQ(result.m_records.size(), 6);
    for(auto i = 0, id = 0; id <=2 ; i++, id++)
    {
        ASSERT_EQ(result.m_records[i].m_left->id, id);
        ASSERT_EQ(result.m_records[i].m_right->id, id);
        ASSERT_NE(result.m_records[i].m_left->name, "");
        ASSERT_EQ(result.m_records[i].m_right->name, "");
    }
    for(auto i = 3, id = 6; id <=8 ; i++, id++)
    {
        ASSERT_EQ(result.m_records[i].m_left->id, id);
        ASSERT_EQ(result.m_records[i].m_right->id, id);
        ASSERT_EQ(result.m_records[i].m_left->name, "");
        ASSERT_NE(result.m_records[i].m_right->name, "");
    }
}

TEST(TableOperations, Truncate)
{
    Table A;
    FillA(A);
    A.Truncate();
    auto intersection = Intersect(A, A);
    auto diff = SymmetricDifference(A, A);
    ASSERT_EQ(intersection.m_records.size(), 0UL);
    ASSERT_EQ(diff.m_records.size(), 0UL);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}