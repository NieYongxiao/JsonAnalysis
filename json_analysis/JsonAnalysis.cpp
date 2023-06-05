#include"JsonAnalysis.h"
#include <stdexcept>

//头文件声明源文件定义 
JsonAnalysis* JsonAnalysis::single_instance = nullptr;
JsonAnalysis::GarbageRecycle gr;

JsonAnalysis::~JsonAnalysis() {
	for (auto node : create_nodes) {
		delete node;
	}
	create_nodes.clear();
	create_nodes_used.clear();
}

string JsonAnalysis::error_position() {
	return error_pos;
}

JsonNode* JsonAnalysis::new_node() {
	int pos = create_nodes_used.find('0');
	if (pos != string::npos) {
		auto node = create_nodes[pos];
		create_nodes_used[pos] = '1';
		node->clear();
		return node;
	}
	auto node = new JsonNode;
	create_nodes_used.push_back('1');
	create_nodes.push_back(node);
	return node;
}



string JsonAnalysis::skip(string value) {
	int start_pos = 0;
	//ACSII码中小于等于32的部分都是一些空格制表符换行符等符号
	while (start_pos < value.size() && value[start_pos] <= 32) {
		start_pos++;
	}
	return value.substr(start_pos);
}

JsonNode* JsonAnalysis::parse(string& value) {
	string ret;
	return root = parse_opts(value, ret, false);
}

JsonNode* JsonAnalysis::parse_opts(string& value, string& ret_parse_end, bool require_null_terminate) {
	JsonNode* c = new_node();
	error_pos = {};
	auto end = parse_value(c, skip(value));
	if (!end.empty()) {
		recycle_node(c);
		return nullptr;
	}
	if (require_null_terminate) { //保留格式
		end = skip(end);
		if (end.size()==0) {  //如果返回字符串为空
			error_pos = end;
			return nullptr;
		}
	}
	ret_parse_end = end;
	return c;
}

string JsonAnalysis::parse_value(JsonNode* item, string value) {
	if (value.size()==0) return {};
	//首先判断类型是不是bool null 
	if (value.substr(0, 5) == "false") {
		item->type = json_bool;
		item->value_bool = false;
		return value.substr(5);
	}
	if (value.substr(0, 4) == "true") {
		item->type = json_bool;
		item->value_bool = true;
		return value.substr(4);
	}
	if (value.substr(0, 4) == "null") {
		item->type = json_null;
		return value.substr(4);
	}
	char front = value[0];
	if (front == '\"')
	{
		return parse_string(item, value);
	}
	if (front == '-' || (front >= '0' && front <= '9') || front == '+')
	{
		return parse_number(item, value);
	}
	if (front == '[')
	{
		return parse_array(item, value);
	}
	if (front == '{')
	{
		return parse_object(item, value);
	}
	error_pos = value;
	return {};
}

//按照""对string进行解析
string JsonAnalysis::parse_string(JsonNode* item,string value) {
	if (value[0] != '\"') { //字符串必须以""开头结尾
		error_pos = value;
		return {};
	}
	int size = value.size(); 
	int len = 0;//len是"..."里的长度
	if (auto pos = value.find('\"', 1) != string::npos) {  //跳过第一个"开始寻找
		len = pos - 1;
	}
	else {
		error_pos = value;
		throw std::overflow_error("parse_string error!");
		return NULL;
	}
	if (len < 1) return {}; 
	//本身为"hello","hi"  则复制之后会将其分割，变成hello"，删除"之后取得hello
	string str_v = value.substr(1, len); 
	string str(str_v.begin(), str_v.end());
	for (auto it = str.begin(); it != str.end(); ++it) { //会报错吧
		if (*it == '\"') it = str.erase(it);  
	}
	item->type = json_string;
	item->value_string = move(str);
	return value.substr(len + 2);
}

string JsonAnalysis::parse_number(JsonNode* item, string value) { 
	bool sign = true;
	int start_pos = 0;
	//首先判断正负，后续就不用考虑开头的正负了
	if (value[0] == '-') {
		sign = false;
		start_pos = 1;
	}
	while (start_pos<value.size()&&value[start_pos] == 0)
		start_pos++;
	string num_str;
	if (value[start_pos] >= '0' && value[start_pos] <= '9') {
		while ((value[start_pos] >= '0' && value[start_pos] <= '9') 
			|| value[start_pos] == 'e' || value[start_pos] == 'E' || value[start_pos] == '.'
			|| (value[start_pos - 1] == 'e' && (value[start_pos] == '+' || value[start_pos] == '-'))
			|| (value[start_pos - 1] == 'E' && (value[start_pos] == '+' || value[start_pos] == '-'))
			) {
			num_str += value[start_pos];
			start_pos++;
		}
	}
	if (num_str.size() == 0) return {};
	double num_float = std::stod(num_str);
	int num_int = std::stoi(num_str);
	if (num_float - num_int > 0.0) {
		item->type = json_double;
		item->value_double = num_float;
	}
	else {
		item->type = json_int;
		item->value_int = num_int;
	}
	return value.substr(start_pos);
}

string JsonAnalysis::parse_array(JsonNode* item, string value) {
	JsonNode* child;
	if (value[0] != '[') {
		error_pos = value[0];
		return {};  //string 可以返回这个吗
	}
	item->type = json_array;
	value = skip(value.substr(1));
	if (value[0] == ']') //[]   这返回值是 ] 吗
		return value.substr(1);
	item->child = child = new_node();
	value = parse_value(child, skip(value));
	if (value.empty()) {
		return {};
	}
	while (value[0] == ',' && value.size() > 1) {
		JsonNode* new_item = new_node();
		child->next = new_item;
		new_item->prev = child;
		child = new_item;
		value = skip(parse_value(child, skip(value.substr(1))));
		if (value.empty()) return {};
	}
	if (value[0] == ']')
		return value.substr(1);
	error_pos = value[0];
	return {};
}

string JsonAnalysis::parse_object(JsonNode* item, string value) {
	if (value[0] != '{') {
		error_pos = value[0];
		return {};
	}
	item->type = json_object;
	value = skip(value.substr(1));
	if (value.size() && value[0] == '}')
		return value.substr(1);
	JsonNode* child=new_node();
	item->child = child;
	value = parse_string(child, skip(value));
	if (value.empty())
		return {};
	child->node_name = child->value_string; //此处细想
	child->value_string.clear();
	if (skip(value)[0] != ':') {
		error_pos = value[0];
		return {};
	}
	value = parse_value(child, skip(value.substr(1)));
	while (value[0] == ',' && value.size() > 1) {
		JsonNode* new_item = new_node();
		child->next = new_item;
		new_item->prev = child;
		child = new_item;

		value = skip(parse_string(child, skip(value.substr(1))));
		if (value.empty())
			return {};
		child->node_name = child->value_string;
		child->value_string.clear();
		if (skip(value)[0] != ':') {
			error_pos = value[0];
			return {};
		}
		value = parse_value(child, skip(value.substr(1)));
		if (value.empty())
			return {};
	}
	if (value.empty())
		return {};
	if (value[0] == '}')
		return value.substr(1);
	error_pos = value[0];
	return {};
}

//深度遍历广度遍历进行删除，vector的值即JsonNode*执行clear，string的值改为0
//clear将所有指针即child ，prev ，next置为nullptr，vector和string里的值清空
void JsonAnalysis::recycle_node(JsonNode* c) {
	auto it = std::find_if(create_nodes.begin(), create_nodes.end(), [c](auto node) { return c == node; });
	if (it != create_nodes.end()) {
		//是自己new出来的，继续深度遍历广度遍历进行删除
		JsonNode* next;
		while (c) {
			next = c->next;
			if (c->child)
				recycle_node(c->child);
			auto pos = std::distance(create_nodes.begin(), it);

			c->clear();
			create_nodes_used[pos] = '0';
			c = next;
		}
	}
	else
		delete c;
}


 
JsonNode* JsonAnalysis::create_object() {
	JsonNode* item = new_node();
	item->type = json_object;
	return item;
}

JsonNode* JsonAnalysis::create_array() {
	JsonNode* item = new_node();
	item->type = json_array;
	return item;
}

JsonNode* JsonAnalysis::create_string(string& str) {
	JsonNode* item = new_node();
	item->type = json_string;
	item->value_string = str;
	return item;
}

JsonNode* JsonAnalysis::create_string(string&& str) {
	JsonNode* item = new_node();
	item->type = json_string;
	item->value_string = move(str);
	return item;
}

JsonNode* JsonAnalysis::create_int(int num) {
	JsonNode* item = new_node();
	item->type = json_int;
	item->value_int = num;
	return item;
}

JsonNode* JsonAnalysis::create_double(double num) {
	JsonNode* item = new_node();
	item->type = json_double;
	item->value_double = num;
	return item;
}

JsonNode* JsonAnalysis::create_bool(bool flag) {
	JsonNode* item = new_node();
	item->type = json_bool;
	item->value_bool = flag;
	return item;
}

JsonNode* JsonAnalysis::create_null(void) {
	JsonNode* item = new_node();
	item->type = json_null;
	return item;
}



void JsonAnalysis::suffix_object(JsonNode* end, JsonNode* first) {
	end->next = first;
	first->prev = end;
}

JsonAnalysis& JsonAnalysis::add_item_to_obj(JsonNode* obj, string item_name, JsonNode* item) {
	if (obj == nullptr) 
		return *this;
	if (item == nullptr)
		return *this;
	item->node_name = item_name;
	obj->type = json_object;
	return add_item_to_array(obj, item);
}

JsonAnalysis& JsonAnalysis::add_item_to_array(JsonNode* ary, JsonNode* item) {
	if (ary == nullptr)
		return *this;
	if (item == nullptr)
		return *this;
	auto child = ary->child;
	if (!child)
		ary->child = item;
	else {
		while (child && child->next) {
			child = child->next;
		}
		suffix_object(child, item);
	}
	return *this;
}

string JsonAnalysis::node_to_string(JsonNode* node) {
	return print_json(node);
 }

string JsonAnalysis::print_json(JsonNode* obj) {
	return print_json(obj, 0);
}

string JsonAnalysis::print_json(JsonNode* obj, int depth) {
	if (obj == nullptr)
		return {};
	string out;
	switch (obj->type) {
	case json_null:
		return out = { "null" }; 
		//out = { "null" };
		break;
	case json_bool:
		return out = obj->value_bool ? "true" : "false";
		//out = obj->value_bool ? "true" : "false";
		break;
	case json_int:
		return out = std::to_string(obj->value_int); 
		//out = std::to_string(obj->value_int);
		break;
	case json_double:
		return out = std::to_string(obj->value_double); 
		//out = std::to_string(obj->value_double);
		break;
	case json_string:
		return out = "\"" + obj->value_string + "\"";
		//out = "\"" + obj->value_string + "\"";
		break;
	case json_array:
		return out = print_array(obj, depth);
		//out = print_array(obj, depth);
		break;
	case json_object:
		return out = print_object(obj, depth);
		//out = print_object(obj, depth);
		break;
	default:
		break;
	}
	//return "\n"+out;
	return out;
}

string JsonAnalysis::print_array(JsonNode* obj, int depth) {
	if (obj == nullptr)
		return {};
	string out;
	auto child = obj->child;
	while(child){
		auto ret = print_json(child, depth + 1);
		if (ret.size()) 
			out += (ret + (child->next ? "," : ""));
		child = child->next;
	}
	return "[" + out + "]";
}

string JsonAnalysis::print_object(JsonNode* obj, int depth) {
	if (obj == nullptr)
		return {};
	string out;
	auto child = obj->child;
	while (child) {
		auto name = "\"" + child->node_name + "\"";
		auto ret = print_json(child, depth + 1);
		if (ret.size())
			out += name + ":" + ret + (child->next ? "," : "");
		child = child->next;
	}
	return "{" + out + "}";
}