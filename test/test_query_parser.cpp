#include "gtest/gtest.h"
#include "QueryParser.h"

struct {
    std::string table1_name{};
    std::string table2_name{};
    int id{};
    std::string name{};

    void clear() {
        table1_name = {};
        table2_name = {};
        id = {0};
        name = {};
    }
} data;


DataBase::future_result_t DataBase::insert(const std::string &table_name, int id, std::string &name) {
    data.table1_name = table_name;
    data.id = id;
    data.name = name;
    return DataBase::future_result_t{};
}

DataBase::future_result_t DataBase::truncate(const std::string &table_name) {
    data.table1_name = table_name;
    return DataBase::future_result_t{};
}

DataBase::future_result_t DataBase::intersection(const std::string &table1_name, const std::string &table2_name) {
    data.table1_name = table1_name;
    data.table2_name = table2_name;
    return DataBase::future_result_t{};
}

DataBase::future_result_t
DataBase::symmetric_difference(const std::string &table1_name, const std::string &table2_name) {
    data.table1_name = table1_name;
    data.table2_name = table2_name;
    return DataBase::future_result_t{};
}


TEST(query_parser, empty) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("", fake_result);

    EXPECT_EQ(std::string{"Неизвестная команда"}, result);
}

TEST(query_parser, insert_wrong_params_num) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("INSERT", fake_result);

    EXPECT_EQ(std::string{"Неверный формат: неверное число параметров"}, result);
}

TEST(query_parser, insert_wrong_id) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("INSERT table_name id name", fake_result);

    EXPECT_EQ(std::string{"Неверный формат: id должно быть int"}, result);
}

TEST(query_parser, insert_called) {
    data.clear();

    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("INSERT table_name 1234 name", fake_result);

    EXPECT_EQ(std::string{""}, result);
    EXPECT_EQ(std::string{"table_name"}, data.table1_name);
    EXPECT_EQ(std::string{""}, data.table2_name);
    EXPECT_EQ(1234, data.id);
    EXPECT_EQ(std::string{"name"}, data.name);
}

TEST(query_parser, truncate_wrong_params_num) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("TRUNCATE", fake_result);

    EXPECT_EQ(std::string{"Неверный формат: неверное число параметров"}, result);
}

TEST(query_parser, truncate_called) {
    data.clear();

    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("TRUNCATE truncate_table_name", fake_result);

    EXPECT_EQ(std::string{""}, result);
    EXPECT_EQ(std::string{"truncate_table_name"}, data.table1_name);
    EXPECT_EQ(std::string{""}, data.table2_name);
    EXPECT_EQ(0, data.id);
    EXPECT_EQ(std::string{""}, data.name);
}

TEST(query_parser, intersection_wrong_params_num) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("INTERSECTION param", fake_result);

    EXPECT_EQ(std::string{"Неверный формат: неверное число параметров"}, result);
}

TEST(query_parser, intersection_called) {
    data.clear();

    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("INTERSECTION", fake_result);

    EXPECT_EQ(std::string{""}, result);
    EXPECT_EQ(std::string{"A"}, data.table1_name);
    EXPECT_EQ(std::string{"B"}, data.table2_name);
    EXPECT_EQ(0, data.id);
    EXPECT_EQ(std::string{""}, data.name);
}

TEST(query_parser, symmetric_difference_wrong_params_num) {
    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("SYMMETRIC_DIFFERENCE param", fake_result);

    EXPECT_EQ(std::string{"Неверный формат: неверное число параметров"}, result);
}

TEST(query_parser, symmetric_difference_called) {
    data.clear();

    DataBase::future_result_t fake_result;
    auto result = query_parser::parse("SYMMETRIC_DIFFERENCE", fake_result);

    EXPECT_EQ(std::string{""}, result);
    EXPECT_EQ(std::string{"A"}, data.table1_name);
    EXPECT_EQ(std::string{"B"}, data.table2_name);
    EXPECT_EQ(0, data.id);
    EXPECT_EQ(std::string{""}, data.name);
}
