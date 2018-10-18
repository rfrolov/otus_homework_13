#pragma once

#include <string>

struct QueryParser {

    std::string parse(std::string data);

private:
    void insert(std::string table_name, int id, std::string name);
    void truncate(std::string table_name);
    void intersection();
    void symmetric_difference();
};
