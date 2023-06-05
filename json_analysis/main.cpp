#include"JsonAnalysis.h"
#include <iostream>
#include <time.h>

using namespace std;

void test_depth_1() {
	auto json_root = JsonAnalysis::get_instance()->create_object();
	auto json_ary = JsonAnalysis::get_instance()->create_array();
	auto json_int = JsonAnalysis::get_instance()->create_int(1);
	auto json_double = JsonAnalysis::get_instance()->create_double(3.14);
	auto json_string = JsonAnalysis::get_instance()->create_string("hello world!");
	JsonAnalysis::get_instance()->add_item_to_array(json_ary, json_int).add_item_to_array(json_ary, json_double).add_item_to_array(json_ary, json_string);
	JsonAnalysis::get_instance()->add_item_to_obj(json_root, "array", json_ary);
	JsonAnalysis::get_instance()->add_item_to_obj(json_root, "EventTree", JsonAnalysis::get_instance()->create_int(10e2));
	clock_t start = clock();
	cout << JsonAnalysis::get_instance()->node_to_string(json_root) << endl;
	clock_t end = clock();
	cout << "解析成string的运行时间为：" << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;;
}



int main() {
	test_depth_1();
	return 0;
}