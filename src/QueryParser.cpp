#include <iostream>
#include "QueryParser.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include <vector>
#include <unordered_map>
#include "DataBase.h"

std::string QueryParser::parse(std::string data) {

    std::vector<std::string> params{};
    boost::split(params, data, boost::is_any_of(" "));

    if (params.empty()) {
        return "ERR Неверный формат: нет команды";
    }

    auto cmd = params.at(0);

    if (cmd == "INSERT") {
        if (params.size() != 4) {
            return "ERR Неверный формат: неверное число параметров";
        }

        auto id_str = params.at(2);
        if (id_str.size() > std::numeric_limits<int>::digits10 + 1 ||
            !std::all_of(id_str.cbegin(), id_str.cend(), ::isdigit)) {
            return "ERR Неверный формат: id должно быть int";
        }

        auto id = strtoll(id_str.c_str(), nullptr, 0);

        if (id > std::numeric_limits<int>::max()) {
            return "ERR Неверный формат: id должно быть int";
        }

        insert(params.at(1), static_cast<int>(id), params.at(3));
    } else if (cmd == "TRUNCATE") {
        if (params.size() != 2) {
            return "ERR Неверный формат: неверное число параметров";
        }
        truncate(params.at(1));
    } else if (cmd == "INTERSECTION") {
        intersection();
    } else if (cmd == "SYMMETRIC_DIFFERENCE") {
        symmetric_difference();
    }

    return "OK";
}

void QueryParser::insert(std::string table_name, int id, std::string name) {
    std::cout << "insert " << table_name << " " << id << " " << name << std::endl;
}

void QueryParser::truncate(std::string table_name) {
    std::cout << "truncate " << table_name << std::endl;
}

void QueryParser::intersection() {
    std::cout << "intersection" << std::endl;
}

void QueryParser::symmetric_difference() {
    std::cout << "symmetric_difference" << std::endl;
}
