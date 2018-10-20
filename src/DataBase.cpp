#include <algorithm>
#include "DataBase.h"
#include <set>
#include <map>

void DataBase::init() {
    m_db.emplace("A", table_t{});
    m_db.emplace("B", table_t{});
}

DataBase::future_result_t DataBase::insert(std::string table_name, int id, std::string name) {

    return m_thread_pool.enqueue([this, table_name, id, name] {
        std::unique_lock<std::mutex> lock(m_mutex);

        auto db_it = m_db.find(table_name);
        if (db_it == m_db.cend()) {
            return std::make_tuple(true, std::string{"Нет такой таблицы: "} + table_name);
        }

        auto &table   = db_it->second;
        auto table_it = table.find(id);

        if (table_it != table.cend()) {
            return std::make_tuple(true, std::string{"Дублирование id: "} + std::to_string(id));
        }

        table.emplace(id, name);

        return std::make_tuple(false, std::string{""});
    });
}

DataBase::future_result_t DataBase::truncate(std::string table_name) {
    return m_thread_pool.enqueue([this, table_name] {
        std::unique_lock<std::mutex> lock(m_mutex);

        auto db_it = m_db.find(table_name);
        if (db_it == m_db.cend()) {
            return std::make_tuple(true, std::string{"Нет такой таблицы: "} + table_name);
        }

        m_db.erase(db_it);

        return std::make_tuple(false, std::string{""});
    });
}

template<typename F>
auto DataBase::match(F func, const std::string &table1_name, const std::string &table2_name) {
    return m_thread_pool.enqueue([this, table1_name, table2_name, func] {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::vector<std::string> tables_name{table1_name, table2_name};
        std::vector<table_t>     tables{};

        for (const auto &name:tables_name) {
            auto db_it = m_db.find(name);
            if (db_it == m_db.cend()) {
                return std::make_tuple(true, std::string{"Нет такой таблицы: "} + name);
            }
            tables.emplace_back(db_it->second);
        }

        return std::make_tuple<bool, std::string>(false, func(tables[0], tables[1]));
    });
}

template <int N> auto DataBase::match(const table_t &table1, const table_t &table2) {
    std::multimap<int, std::string> multimap{};
    std::set<int>                   set;

    for (const auto &it:table1) {
        set.insert(it.first);
        multimap.insert(it);
    }

    for (const auto &it:table2) {
        set.insert(it.first);
        multimap.insert(it);
    }

    std::string     result{};
    for (const auto &key:set) {
        if (multimap.count(key) == N) {
            auto range = multimap.equal_range(key);

            result += std::to_string(range.first->first) + ",";
            for (auto it = range.first; it != range.second; ++it) {
                result += it->second + (std::next(it) == range.second ? "" : ",");
            }
            result += "\n";
        }
    }
    return result;
}

DataBase::future_result_t DataBase::intersection(const std::string &table1_name, const std::string &table2_name) {
    return match([this](const table_t &table1, const table_t &table2) { return match<2>(table1, table2); },
                 table1_name, table2_name);
}

DataBase::future_result_t
DataBase::symmetric_difference(const std::string &table1_name, const std::string &table2_name) {
    return match([this](const table_t &table1, const table_t &table2) { return match<1>(table1, table2); },
                 table1_name, table2_name);
}


