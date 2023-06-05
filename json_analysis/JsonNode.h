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
    //��������ģʽ
    /*static JsonNode* get_instance() {
        return &json_instance;
    }*/
    
    //��ȡ�ж����ӽڵ�
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
    //�ӽڵ�
    JsonNode* child = nullptr;
    //ͬ���ڵ�
    JsonNode* next=nullptr, * prev=nullptr;

    int type = json_null;

    //JsonNode�ļ����������ֵ���ͽ�Ϊstring number Ϊ�˸���ѡ�񣬷�Ϊint��double
    string value_string;
    int value_int;
    double value_double;
    bool value_bool;

    //�ڵ������
    string node_name;

    //new ������޲ι��캯����������ֶ�����Ϊnullptr��������߰���ĵ�ַ
    JsonNode() = default;

    //����ģʽstatic����
    //static JsonNode json_instance;
};
//��������ģʽstatic����
//JsonNode JsonNode::json_instance;


#endif // JsonNode_H
