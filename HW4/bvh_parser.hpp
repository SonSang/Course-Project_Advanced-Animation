#ifndef __BVH_PARSER_H__
#define __BVH_PARSER_H__

#include "body.hpp"
#include "live_body.hpp"
#include "animation.hpp"

std::string					read_text_file(const std::string &filename);
std::vector<std::string>    read_text_file_multiline(const std::string &filename);
std::vector<std::string>    tokenize(const std::string &line, const char delimiter = ' ');
std::vector<std::string>    tokenize(const std::string &line, const std::vector<char> delimiters);

class bvh_parser {
public:
    // parse a [.bvh] file and generate a model out of it.
    static body parse(const std::string &filename);

    // parse a [.bvh] file and generate an animation out of it.
    static animation parse_anim(const std::string &filename);

    // parse a [.bvh] file and generate a live body out of it.
    static live_body parse_live_body(const std::string &filename);
};

#endif