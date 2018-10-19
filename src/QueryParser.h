#pragma once

#include <string>
#include <tuple>
#include <future>
#include "DataBase.h"

namespace query_parser {
    using future_result_t = DataBase::future_result_t;

    std::string parse(std::string data, DataBase::future_result_t &result);
};
