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

    future_result_t insert(std::string table_name, int id, std::string name);
    future_result_t truncate(std::string table_name);
    future_result_t intersection(std::vector<std::string> table_names);
    future_result_t symmetric_difference(std::vector<std::string> table_names);

private:
    DataBase() = default;

    ThreadPool m_thread_pool{std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1};

    std::mutex m_mutex{};

    using table_t = std::unordered_map<int, std::string>;
    using db_t    = std::unordered_map<std::string, table_t>;

    db_t m_db{};
};

