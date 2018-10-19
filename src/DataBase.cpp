#include "DataBase.h"

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

DataBase::future_result_t DataBase::intersection(std::vector<std::string> table_names) {
    return m_thread_pool.enqueue([this, table_names] {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::vector<table_t> tables{};
        for (const auto      &name:table_names) {
            auto db_it = m_db.find(name);
            if (db_it == m_db.cend()) {
                return std::make_tuple(true, std::string{"Нет такой таблицы: "} + name);
            }
            tables.emplace_back(db_it->second);
        }

        // TODO:
        return std::make_tuple(false, std::string{""});
    });
}

DataBase::future_result_t DataBase::symmetric_difference(std::vector<std::string> table_names) {
    return m_thread_pool.enqueue([this, &table_names] {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::vector<table_t> tables{};
        for (const auto      &name:table_names) {
            auto db_it = m_db.find(name);
            if (db_it == m_db.cend()) {
                return std::make_tuple(true, std::string{"Нет такой таблицы: "} + name);
            }
            tables.emplace_back(db_it->second);
        }

        // TODO:
        return std::make_tuple(false, std::string{""});
    });
}
