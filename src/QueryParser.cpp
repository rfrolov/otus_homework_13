#include "QueryParser.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include <vector>
#include "DataBase.h"

namespace query_parser {
    std::string parse(std::string data, DataBase::future_result_t &result) {

        std::vector<std::string> params{};
        boost::split(params, data, boost::is_any_of(" "));

        if (params.empty()) {
            return "Неверный формат: нет команды";
        }

        auto cmd = params.at(0);

        if (cmd == "INSERT") {
            if (params.size() != 4) {
                return "Неверный формат: неверное число параметров";
            }

            auto id_str = params.at(2);
            if (id_str.size() > std::numeric_limits<int>::digits10 + 1 ||
                !std::all_of(id_str.cbegin(), id_str.cend(), ::isdigit)) {
                return "Неверный формат: id должно быть int";
            }

            auto id = strtoll(id_str.c_str(), nullptr, 0);
            if (id > std::numeric_limits<int>::max()) {
                return "Неверный формат: id должно быть int";
            }
            result = DataBase::getInstance().insert(params.at(1), static_cast<int>(id), params.at(3));
            return {};
        } else if (cmd == "TRUNCATE") {
            if (params.size() != 2) {
                return "Неверный формат: неверное число параметров";
            }
            result = DataBase::getInstance().truncate(params.at(1));
            return {};
        } else if (cmd == "INTERSECTION") {
            if (params.size() != 1) {
                return "Неверный формат: неверное число параметров";
            }
            result = DataBase::getInstance().intersection("A", "B");
            return {};
        } else if (cmd == "SYMMETRIC_DIFFERENCE") {
            if (params.size() != 1) {
                return "Неверный формат: неверное число параметров";
            }
            result = DataBase::getInstance().symmetric_difference("A", "B");
            return {};
        }

        return "Неизвестная команда";
    }
}