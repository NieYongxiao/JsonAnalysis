#pragma once
#ifndef JSON_ANALYSIS_H
#define JSON_ANALYSIS_H
#include"JsonNode.h"

class JsonAnalysis {
public:
	//��������ģʽ δ���Ƕ��̣߳�������Ҫ�����ж��Ա���� ��Ϊ����
	static JsonAnalysis* get_instance() {
		if (single_instance == nullptr)
			single_instance = new JsonAnalysis;
		return single_instance;
	}
	//����������
	class GarbageRecycle {
	public:
		~GarbageRecycle() {
			if (JsonAnalysis::single_instance != nullptr)
				delete single_instance;
		}
	};
	static GarbageRecycle gr;
public:
	//����value��ֵ�����ͣ�����ת��ΪJsonNode����
	JsonNode* parse(string& value);
	
	//��JsonNode������str
	string node_to_string(JsonNode* node);
	
	//���ش����λ��
	string error_position();
public:
	//����һ��JsonNode�ڵ㣬�����ýڵ�����Ϊjson_object
	JsonNode* create_object(void);

	JsonNode* create_array(void);

	JsonNode* create_string(string& str);

	JsonNode* create_string(string&& str);

	JsonNode* create_int(int num);

	JsonNode* create_double(double num);

	JsonNode* create_bool(bool flag);
	
	JsonNode* create_null(void);
public:
	//���ڵ�item�ӵ�obj��
	JsonAnalysis& add_item_to_obj(JsonNode* obj, string item_name, JsonNode* item);
	
	//���ڵ�item�ӵ�array��
	JsonAnalysis& add_item_to_array(JsonNode* ary, JsonNode* item);
	
	//���սṹ��
	//�������ڵ㲻���Լ�new_node�����ģ���delete���򣬵���clear�������
	void recycle_node(JsonNode* item);
private:
	//�½�һ��JsonNode����
	JsonNode* new_node();
private:
	//�½�һ��JsonNode���󣬸���value��ֵ������й��죬Ҳ������value������ת����JsonNode
	//ret_parse_end��󷵻ص�λ��  require_null_terminateѡ����������Ƿ��˳�
	JsonNode* parse_opts(string& value, string& ret_parse_end, bool require_null_terminate);
	
	string parse_value(JsonNode* item, string value);
	
	string parse_string(JsonNode* item, string value);
	
	string parse_number(JsonNode* item, string value);
	
	string parse_object(JsonNode* item, string value);

	string parse_array(JsonNode* item, string value);
	
	//�����ո� ���з��ȷ���
	string skip(string value);

	//���ṹ��ת��Ϊ�ַ���
	string print_json(JsonNode* obj);
	
	string print_json(JsonNode* obj,int depth);
	
	string print_array(JsonNode* obj,int depth);
	
	string print_object(JsonNode* obj,int depth);

	//ʹ˫��������β����
	void suffix_object(JsonNode* end, JsonNode* first);
public:
	//����ʱ�����ڴ�ռ�
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
//����Ϊʲô���ظ�����ͷ�ļ����𣬿���û�а�������
//�㲻��������ͷ�ļ�����Դ�ļ������ʹ��

#endif