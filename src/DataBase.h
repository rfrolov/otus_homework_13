#pragma once

#include "ThreadPool.h"
#include <unordered_map>

struct DataBase {
    using future_result_t = std::future<std::tuple<bool, std::string>>;

    static DataBase &getInstance() {
        static DataBase instance{};
        return instance;
    }

    DataBase(const DataBase &) = delete;
    DataBase(const DataBase &&) = delete;
    DataBase &operator=(const DataBase &) = delete;
    DataBase &operator=(const DataBase &&) = delete;

    void init();

    future_result_t insert(const std::string &table_name, int id, std::string &name);
    future_result_t truncate(const std::string &table_name);
    future_result_t intersection(const std::string &table1_name, const std::string &table2_name);
    future_result_t symmetric_difference(const std::string &table1_name, const std::string &table2_name);

private:
    using table_t = std::unordered_map<int, std::string>;
    using db_t    = std::unordered_map<std::string, table_t>;

    DataBase() = default;

    template<typename F>
    auto match(F func, const std::string &table1_name, const std::string &table2_name);

    template<int N>
    auto match(const std::vector<table_t> &tables);


    ThreadPool m_thread_pool{std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1};
    std::mutex m_mutex{};
    db_t       m_db{};
};

