#include "gtest/gtest.h"
#include "ParseCmdLine.h"

TEST(parse_cmd_line, argc_valid) {
    std::vector<std::string> params{"path", "1234"};
    char *argv[]{};
    int argc{};

    for(auto &param:params) {
        argv[argc] = const_cast<char *>(param.c_str());
        ++argc;
    }

    ParseCmdLine parser{argc, argv};
    EXPECT_TRUE(parser.is_valid());
}

TEST(parse_cmd_line, argc_invalid) {
    std::vector<std::string> params{"path"};
    char *argv[]{};
    int argc{};

    for(auto &param:params) {
        argv[argc] = const_cast<char *>(param.c_str());
        ++argc;
    }

    ParseCmdLine parser{argc, argv};
    EXPECT_FALSE(parser.is_valid());
    EXPECT_EQ(std::string{"Неверное число аргументов"}, parser.error());
}

TEST(parse_cmd_line, port_valid) {
    std::vector<std::string> params{"path", "9000"};
    char *argv[]{};
    int argc{};

    for(auto &param:params) {
        argv[argc] = const_cast<char *>(param.c_str());
        ++argc;
    }

    ParseCmdLine parser{argc, argv};
    EXPECT_TRUE(parser.is_valid());
    EXPECT_EQ(9000, parser.port());
}

TEST(parse_cmd_line, port_invalid_convertation) {
    std::vector<std::string> params{"path", "90s00"};
    char *argv[]{};
    int argc{};

    for(auto &param:params) {
        argv[argc] = const_cast<char *>(param.c_str());
        ++argc;
    }

    ParseCmdLine parser{argc, argv};
    EXPECT_FALSE(parser.is_valid());
}

TEST(parse_cmd_line, port_invalid_range) {
    std::vector<std::string> params{"path", "65536"};
    char *argv[]{};
    int argc{};

    for(auto &param:params) {
        argv[argc] = const_cast<char *>(param.c_str());
        ++argc;
    }

    ParseCmdLine parser{argc, argv};
    EXPECT_FALSE(parser.is_valid());
}