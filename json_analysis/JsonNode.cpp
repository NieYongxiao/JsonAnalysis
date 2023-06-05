#include"JsonNode.h"
#include <stdexcept>

size_t JsonNode::size() {
    size_t c_size = 0;
    auto tmp = this->child;
    while (tmp) {
        tmp = tmp->child;
        ++c_size;
    }
    return c_size;
}

int JsonNode::get_type() {
    return this->type;
}
string JsonNode::get_node_name() {
    if (this->node_name.empty()) return {};
    return this->node_name;
}
int JsonNode::get_int() {
    if (this->type != json_int) throw std::runtime_error("type error!");
    return this->value_int;
}
double JsonNode::get_double() {
    if (this->type != json_double) throw std::runtime_error("type error!");
    return this->value_double;
}
bool JsonNode::get_bool() {
    if (this->type != json_bool) throw std::runtime_error("type error!");
    return this->value_bool;
}
string JsonNode::get_string() {
    if (this->type != json_string) throw std::runtime_error("type error!");
    return this->value_string;
}
bool JsonNode::is_array() {
    return this->type == json_array;
}
bool JsonNode::is_object() {
    return this->type == json_object;
}
bool JsonNode::is_null() {
    return this->type == json_null;
}
vector<JsonNode*> JsonNode::childs() {
    vector<JsonNode*> res;
    auto tmp = this->child;
    while (tmp) {
        res.push_back(tmp);
        tmp = tmp->next;
    }
    return res;
}

void JsonNode::clear() {
    child = nullptr, next = nullptr, prev = nullptr;
    type = json_null;
    node_name.clear();
    value_string.clear();
}