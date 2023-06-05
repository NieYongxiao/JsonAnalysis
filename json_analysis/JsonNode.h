#pragma once
#ifndef JsonNode_H
#define JsonNode_H

#include<string>
#include<vector>

using std::string;
using std::vector;

static const char* rp;

enum json_type {
    json_bool,
    json_null,
    json_int,
    json_double,
    json_string,
    json_array,
    json_object
};

class JsonAnalysis;
class JsonNode {
    friend JsonAnalysis;
public:
    //单例饿汉模式
    /*static JsonNode* get_instance() {
        return &json_instance;
    }*/
    
    //获取有多少子节点
    size_t size();

    int get_type();

    string get_node_name();

    int get_int();

    double get_double();

    bool get_bool();

    string get_string();

    bool is_array();

    bool is_object();

    bool is_null();

    vector<JsonNode*> childs();
private:
    void clear();
private:	
    //子节点
    JsonNode* child = nullptr;
    //同级节点
    JsonNode* next=nullptr, * prev=nullptr;

    int type = json_null;

    //JsonNode文件中有意义的值类型仅为string number 为了更多选择，分为int和double
    string value_string;
    int value_int;
    double value_double;
    bool value_bool;

    //节点的名字
    string node_name;

    //new 会调用无参构造函数，如果不手动设置为nullptr则会是乱七八糟的地址
    JsonNode() = default;

    //懒汉模式static对象
    //static JsonNode json_instance;
};
//定义懒汉模式static对象
//JsonNode JsonNode::json_instance;


#endif // JsonNode_H
