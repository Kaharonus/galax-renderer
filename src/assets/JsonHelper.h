//
// Created by tomas on 7.9.22.
//

#pragma once
#include <nlohmann/json.hpp>

using namespace nlohmann;

inline json get_untyped(const json & j, const char * property) {
    if (j.find(property) != j.end()) {
        return j.at(property).get<json>();
    }
    return json();
}

inline json get_untyped(const json & j, std::string property) {
    return get_untyped(j, property.data());
}