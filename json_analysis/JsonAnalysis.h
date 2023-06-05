#pragma once
#ifndef JSON_ANALYSIS_H
#define JSON_ANALYSIS_H
#include"JsonNode.h"

class JsonAnalysis {
public:
	//单例懒汉模式 未考虑多线程，否则需要两次判断以便加锁 较为繁琐
	static JsonAnalysis* get_instance() {
		if (single_instance == nullptr)
			single_instance = new JsonAnalysis;
		return single_instance;
	}
	//垃圾回收类
	class GarbageRecycle {
	public:
		~GarbageRecycle() {
			if (JsonAnalysis::single_instance != nullptr)
				delete single_instance;
		}
	};
	static GarbageRecycle gr;
public:
	//根据value的值与类型，将其转换为JsonNode对象
	JsonNode* parse(string& value);
	
	//将JsonNode解析成str
	string node_to_string(JsonNode* node);
	
	//返回错误的位置
	string error_position();
public:
	//创建一个JsonNode节点，并设置节点类型为json_object
	JsonNode* create_object(void);

	JsonNode* create_array(void);

	JsonNode* create_string(string& str);

	JsonNode* create_string(string&& str);

	JsonNode* create_int(int num);

	JsonNode* create_double(double num);

	JsonNode* create_bool(bool flag);
	
	JsonNode* create_null(void);
public:
	//将节点item加到obj上
	JsonAnalysis& add_item_to_obj(JsonNode* obj, string item_name, JsonNode* item);
	
	//将节点item加到array上
	JsonAnalysis& add_item_to_array(JsonNode* ary, JsonNode* item);
	
	//回收结构体
	//如果这个节点不是自己new_node出来的，就delete否则，调用clear进行清除
	void recycle_node(JsonNode* item);
private:
	//新建一个JsonNode对象
	JsonNode* new_node();
private:
	//新建一个JsonNode对象，根据value的值对其进行构造，也即解析value，将其转换成JsonNode
	//ret_parse_end最后返回的位置  require_null_terminate选择解析错误是否退出
	JsonNode* parse_opts(string& value, string& ret_parse_end, bool require_null_terminate);
	
	string parse_value(JsonNode* item, string value);
	
	string parse_string(JsonNode* item, string value);
	
	string parse_number(JsonNode* item, string value);
	
	string parse_object(JsonNode* item, string value);

	string parse_array(JsonNode* item, string value);
	
	//跳过空格 换行符等符号
	string skip(string value);

	//将结构体转化为字符串
	string print_json(JsonNode* obj);
	
	string print_json(JsonNode* obj,int depth);
	
	string print_array(JsonNode* obj,int depth);
	
	string print_object(JsonNode* obj,int depth);

	//使双向链表首尾相连
	void suffix_object(JsonNode* end, JsonNode* first);
public:
	//析构时回收内存空间
	~JsonAnalysis();
private:
	string error_pos;
	JsonNode* root = nullptr;
	vector<JsonNode*> create_nodes;
	string create_nodes_used;
private:
	JsonAnalysis() = default;
	static JsonAnalysis* single_instance;
};
//JsonAnalysis* JsonAnalysis::single_instance = nullptr;
//JsonAnalysis::GarbageRecycle gr;
//这是为什么捏，重复引用头文件了吗，可是没有啊。。。
//搞不懂，还是头文件声明源文件定义好使啊

#endif