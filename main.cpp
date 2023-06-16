#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<ctime>
#include"data_instance.h"
#include"data_object_pmf.h"
#include"H_node.h"
#include"H_tree.h"
using namespace std;
double Space_Cost_all = 0;
Data_instance Read_into_Data_instance(string str, const const char split=',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[11]);
	double p = stod(in_str[10]);
	vector<string> SD = { in_str[1] ,in_str[2] ,in_str[7] ,in_str[6] ,in_str[9] ,in_str[8] };
	vector<string> TD = { in_str[5] ,in_str[4] };
	Data_instance s_i(id,x,y,p,SD,TD);
	return s_i;

}
Data_instance Read_into_Data_instance_US(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[6]);
	double p = stod(in_str[7]);
	vector<string> SD = { in_str[4] ,in_str[5]};
	vector<string> TD = { in_str[1] ,in_str[2] };
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
Data_instance Read_into_Data_instance_DROP(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[4]);
	double y = stod(in_str[5]);
	double p = stod(in_str[6]);
	vector<string> SD = { in_str[2] ,in_str[3] };
	vector<string> TD = { in_str[1]};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
Data_instance Read_into_Data_instance_agg(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[2]);
	double y = stod(in_str[3]);
	double p = stod(in_str[4]);
	vector<string> SD = { "*" };
	vector<string> TD =  {};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
void test1(H_tree Tree,int num,double &time)
{
	Space_Cost_all = 0.0;
	ifstream fp("E:\\linedata8.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= num)
		{
			cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = 1000 * double(end_t - start_t) / CLOCKS_PER_SEC;
}
void test_sketch(H_tree Tree,int num,double &time)
{
	Space_Cost_all = 0.0;
	ifstream fp("E:\\linedata8.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= num)break;
		Tree.Read_stream_instance(s_i);
		
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = 1000 * double(end_t - start_t) / CLOCKS_PER_SEC;

}
void test_pmf()
{
	
	vector<double> result;
	for (int i = 1; i <= 40; i++)
	{
		H_tree Tree;
		double time = 0;
		test1(Tree, 360*(i),time);
		result.push_back(Space_Cost_all);
		Space_Cost_all = 0.0;
	}
	int k = 21;
	for (auto time : result)
	{
		cout << 360 * k << "个object对象的聚合空间为        " << Space_Cost_all << endl;
		k++;
	}


}
void test_sketch()
{

	vector<double> result;
	for (int i = 1; i <= 40; i++)
	{
		H_tree Tree;
		double time = 0;
		test_sketch(Tree, 360 * (i + 0), time);
		result.push_back(time);
	}
	int k = 1;
	for (auto time : result)
	{
		cout << 360 * k << "个object对象的聚合时间为        " << time << endl;
		k++;
	}


}
void test_tpch(int number,double &time,double &space,double &space2)
{
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number +1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	 time =  double(end_t - start_t) / CLOCKS_PER_SEC;
	 space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	 space2 = space;
	 space2 += Tree.Root.Space_data / 1024.0;
	 


}
void test_sketch(int number, double& time, double& space)
{
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double start_1 = clock();
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >=30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double end_1 = clock();
	double read_time = double(end_1 - start_1) / CLOCKS_PER_SEC;
	//dataFile << double(number * 366) / 10000 << "   " << read_time << endl;
	//dataFile.close();
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	//Space_cost是Tree的所有的线的条数，每一条线由3个double类型决定
	//Space_cost_sketch是所有SKech变量*5以后的个数
	//
	space = Tree.Root.Space_cost_sketch*8.0/1024;
	//space += Tree.Root.Space_data / 1024.0;


}
void test_pmf_tpch()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t=0, s=0,spp=0;
	for (int i = 1000; i <= 11000;i+=1000)
	{

		test_tpch(i, t, s,spp);
		test_struct a(i, t, s);
		Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("TPCH_materzation_time20220219.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("TPCH_materzation_space20220219.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i) / 1000 << "   " << s << endl;
		dataFile2.close();
		//i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}
void test_sketch_tpch()
{


	double t, s;
	for (int i = 1; i <= 360;)
	{

		test_sketch(i, t, s);
		ofstream dataFile;
		dataFile.open("tpch_sketch_materzation_time_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_sketch_materzation_space_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
	}






}
void test_pws(int number, double& time, double& space)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化,pws" << endl;
	Tree.Materialize_Tree_by_PWS();
	double end_t = clock();   //结束时间
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	//Space_cost是Tree的所有的线的条数，每一条线由3个double类型决定
	//Space_cost_sketch是所有SKech变量*5以后的个数
	//
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_pws;


}
void test_pws_tpch()
{


	double t, s;
	for (int i = 160; i <=500;)
	{

		test_pws(i, t, s);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_pws_time10begin6_10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_pws_space10begin6_10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
	}






}
void test_pmf_query1(int number, double& time, double& space,double &query_time)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_data / 1024.0;
	
	cout << "物化完成，开始查询" << endl;
	start_t = clock();
	Tree.query_pmf();
	end_t = clock();   //结束时间
	 query_time = double(end_t - start_t) / CLOCKS_PER_SEC;


}
void test_pmf_query()
{

	double t, s,query_time;
	for (int i = 10; i <= 800;)
	{

		test_pmf_query1(i, t, s,query_time);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_pmf_time10begin6_16_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_pmf_space10begin6_16_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		ofstream dataFile3;
		dataFile3.open("tpch_pmf_query_time10begin6_16_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile3 << double(i * 366) / 10000 << "   " << query_time << endl;
		dataFile3.close();
		i += 10;
		t = 0;
		s = 0;
		query_time = 0;
	}








}
void test_sketch_query1

(int number, double& time, double& space, double& query_time)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_data / 1024.0;

	cout << "物化完成，开始查询" << endl;
	start_t = clock();
	Tree.query_sketch();
	end_t = clock();   //结束时间
	query_time = double(end_t - start_t) / CLOCKS_PER_SEC;


}
void test_sketch_query()
{

	double t, s, query_time;
	for (int i = 10; i <= 800;)
	{

		test_sketch_query1(i, t, s, query_time);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_time_sketch617.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_space_sketch617.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		ofstream dataFile3;
		dataFile3.open("tpch_query_sketch617.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile3 << double(i * 366) / 10000 << "   " << query_time << endl;
		dataFile3.close();
		i += 10;
		t = 0;
		s = 0;
		query_time = 0;
	}








}
void test_sketch_query_bfs(int number, double& time, double& space, double& query_time)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_data / 1024.0;

	cout << "物化完成，开始查询" << endl;
	start_t = clock();
	Tree.quety_sketch_BFS();
	end_t = clock();   //结束时间
	query_time = double(end_t - start_t) / CLOCKS_PER_SEC;


}
void test_sketch_query_BFS()
{

	double t, s, query_time;
	for (int i = 10; i <= 800;)
	{

		test_sketch_query_bfs(i, t, s, query_time);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_time_sketch622bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_space_sketch622bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		ofstream dataFile3;
		dataFile3.open("tpch_query_sketch622bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile3 << double(i * 366) / 10000 << "   " << query_time << endl;
		dataFile3.close();
		i += 10;
		t = 0;
		s = 0;
		query_time = 0;
	}








}
void test_pmf_query_bfs(int number, double& time, double& space, double& query_time)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_data / 1024.0;

	cout << "物化完成，开始查询" << endl;
	start_t = clock();
	Tree.query_pmf_BFS();
	end_t = clock();   //结束时间
	query_time = double(end_t - start_t) / CLOCKS_PER_SEC;


}
void test_query_pmf_BFS()
{

	double t, s, query_time;
	for (int i = 450; i <= 800;)
	{

		test_pmf_query_bfs(i, t, s, query_time);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_time_pmf617bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_space_pmf617bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		ofstream dataFile3;
		dataFile3.open("tpch_query_pmf617bfs.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile3 << double(i * 366) / 10000 << "   " << query_time << endl;
		dataFile3.close();
		i += 10;
		t = 0;
		s = 0;
		query_time = 0;
	}








}

void test_pmf_H_query2(int number, double& time, double& space, double& query_time)
{
	ifstream fp("TPCH_test1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 366 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_H();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space += Tree.Root.Space_data / 1024.0;

	cout << "物化完成，开始查询" << endl;
	start_t = clock();
	Tree.query_pmf();
	end_t = clock();   //结束时间
	query_time = double(end_t - start_t) / CLOCKS_PER_SEC;


}
void test_pmf_H()
{

	double t, s, query_time;
	for (int i = 10; i <= 800;)
	{

		test_pmf_H_query2(i, t, s, query_time);
		//test_struct a(i * 366, t, s);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("tpch_pmf_H_time10begin6_26_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 366) / 10000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_pmf_H_space10begin6_26_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 366) / 10000 << "   " << s << endl;
		dataFile2.close();
		ofstream dataFile3;
		dataFile3.open("tpch_pmf_H_query_time10begin6_26_2.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile3 << double(i * 366) / 10000 << "   " << query_time << endl;
		dataFile3.close();
		i += 10;
		t = 0;
		s = 0;
		query_time = 0;
	}








}
void test_sketch_agg_pws()
{

	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 10000)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		
	}















}
void test_materzation_with_con_popular_path_length()
{
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double start_1 = clock();
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >=10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Materialize_Tree();
	Tree.Bulid_path();

	//vector<double> time = Tree.Con_materzation_with_path();
	ofstream dataFile;
	dataFile.open("test_materzation_with_con_popular_path_length_new.txt", ofstream::app);
	// 朝TXT文档中写入数据
	int path_length = 1;
	for (auto cuboid : Tree.Path)
	{
		double sum = 0.0;
		for (auto cell : cuboid.cells)
		{
			sum += cell->Materalize_time;
		}
		path_length++;
		dataFile << path_length << "   " << sum << endl;
	}
	dataFile.close();

}
void test_materzation_with_sketch_popular_path_length()
{
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double start_1 = clock();
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >=10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Bulid_path();
	vector<double> time = Tree.Sketch_materzation_with_path();
	ofstream dataFile;
	dataFile.open("test_materzation_with_sketch_popular_path_length1233.txt", ofstream::app);
	// 朝TXT文档中写入数据
	int path_length = 1;
	for (auto t : time)
	{
		dataFile << path_length << "   " << t << endl;
		path_length++;
	}

	dataFile.close();

}
void test_agg_cell_con(int number, double& time, double& space)
{
	ifstream fp("TPCH_m10_0_to10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_agg(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.agg_con();
	//Tree.Materialize_Tree_by_PWS();
	//Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;



}
void test_agg_con()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 1; i <= 350;)
	{
		//int i = 340;
		test_agg_cell_con(i, t, s);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("agg_con_time_m10new720.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("agg_con_space_m10new720.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 1;
		t = 0;
		s = 0;
		spp = 0;
	}
}
void test_agg_cell_sketch(int number, double& time, double& space)
{
	ifstream fp("m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_agg(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	//Tree.Materialize_Tree();
	//Tree.Materialize_Tree_by_PWS();
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost_sketch * 8.0 * 3.0 / 1024;



}
void test_agg_sketch()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 1; i <= 350;)
	{
		//int i = 340;
		test_agg_cell_sketch(i, t, s);
		ofstream dataFile;
		dataFile.open("agg_sketch_timem10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("agg_sketch_spacem10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}
void test_agg_cell_pws(int number, double& time, double& space)
{
	ifstream fp("m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_agg(line);

		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	//Tree.Materialize_Tree();
	Tree.Materialize_Tree_by_PWS();
	//Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;



}
void test_agg_pws()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 1; i <= 10;)
	{

		test_agg_cell_pws(i, t, s);
		ofstream dataFile;
		dataFile.open("agg_pws_time1.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("agg_pws_space1.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 1;
		t = 0;
		s = 0;
		spp = 0;
	}
}
void test_Montoring(int  length,double &V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id > 10000)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id)&&(o_i.Data_object_id>0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{
				
				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_Monitorng(o_i.Data_instance_list[0],mounth, o_i.Data_instance_list[0].Data_instance_x,EM,VarM, length);
			double end_1 = clock();
			double t = (end_1 - start_1)/ CLOCKS_PER_SEC;
			Time_sum += t;
			/*ofstream dataFile;
			dataFile.open("test_Montoring7new721.txt", ofstream::app);
			// 朝TXT文档中写入数据
			if(o_i.Data_object_id%100==0)
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
			dataFile.close();*/
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}
		

	}
	V = double(10000) / Time_sum;
	/*Tree.Bulid_path();
	int n = 1;
	for (auto node : Tree.Path)
	{
		cout << n <<"    " <<node.cells.size() << endl;
		n++;
	}*/


}

void online_monitoring()
{
	double t, s;
	for (int i = 1; i <= 360;)
	{

		test_sketch(i, t, s);
		ofstream dataFile;
		dataFile.open("tpch_sketch_materzation_time.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("tpch_sketch_materzation_space.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
	}




}

void test_search()
{
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Materalize_for_time();
	Tree.Bulid_path();
	Tree.Sketch_materzation_with_path();
	int i =1 ;
	for (auto cuboid : Tree.Path)
	{
		ofstream outFile;
		string str = "Newcells";
		outFile.open(str + to_string(i) + ".csv", ios::app); //
		for (auto cells : cuboid.cells)
		{
			
			
			for (auto D : cells->Dimension)
			{
				outFile << D << ',';
			}
			
			outFile << cells->S_C.E_k << ',' << cells->S_C.Var_k << ',' << cells->Materalize_time << endl;;
		}
		i++;
	}
}


void query_time()
{

	int k = 3;
	string str = "cells";
	str += to_string(k);
	ifstream fp(str+".csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	double query_time=0.0;
	double t_s = clock();
	while (getline(fp, line)) { //循环读取每行数据
		{
			istringstream iss(line);	// 输入流
			string token;			// 接收缓冲区
			vector<string> in_str;
			while (getline(iss, token, ','))	// 以split为分隔符
			{
				//cout << token << "  "; // 输出
				in_str.push_back(token);
			}
			int length = in_str.size() - 1;
			double time = stod(in_str[length]);
			double VarK = stod(in_str[length - 1]);
			double E_K = stod(in_str[length - 2]);
			double k_m = 80, p_m = 0.5;//阈值
			double a = abs(E_K - k_m);
			double Upper_Bounds = (VarK) / (VarK + a * a);
			double Lower_Bounds = (a * a) / (VarK + a * a);
			cout << Upper_Bounds << "   " << Lower_Bounds << endl;
			if (Upper_Bounds<p_m || Lower_Bounds>p_m)
				continue;
			else
			{
				query_time += time;
			}



		}
		

	}
	double t_e = clock();
	query_time += (t_e - t_s) / CLOCKS_PER_SEC;


	cout << query_time << endl;
}


void test_US(int number, double& time, double& space, double& space2)
{
	ifstream fp("USdata2.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_US(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number+1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space2 = space;
	space2 += Tree.Root.Space_data / 1024.0;



}
void test_pmf_US()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 250; i <= 10000;)
	{

		test_US(i, t, s, spp);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("US_materzation_time_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i ) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("US_materzation_space_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i ) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 300;
		t = 0;
		s = 0;
		spp = 0;
	}
}

void test_sketch_US(int number, double& time, double& space, double& space2)
{
	ifstream fp("USdata2.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_US(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost_sketch * 8.0  / 1024;
	space2 = space;
	//space2 += Tree.Root.Space_data / 1024.0;



}
void test_sketch_US()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 250; i <= 11000;)
	{

		test_sketch_US(i, t, s, spp);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("US_materzation_time_sketch.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("US_materzation_space_sketch.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 300;
		t = 0;
		s = 0;
		spp = 0;
	}
}

void test_materzation_with_con_popular_path_length_US()
{
	ifstream fp("USdata2.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double start_1 = clock();
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_US(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 4001)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		//Tree.Read_stream_instance_new_path(s_i);
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Bulid_path();
	vector<double> time = Tree.Con_materzation_with_path();
	ofstream dataFile;
	dataFile.open("test_materzation_with_con_popular_path_length_US.txt", ofstream::app);
	// 朝TXT文档中写入数据
	int path_length = 1;
	for (auto t : time)
	{
		dataFile << path_length << "   " << t << endl;
		path_length++;
	}

	dataFile.close();

}

Data_instance Read_into_Data_instance_aggUS(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[6]);
	double p = stod(in_str[7]);
	vector<string> SD = { "*" };
	vector<string> TD = {};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
void test_agg_cell_conUS(int number, double& time, double& space)
{
	ifstream fp("USdata_New.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_aggUS(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number*30 + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.agg_con();
	//Tree.Materialize_Tree_by_PWS();
	//Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;



}
void test_agg_conUS()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 5; i <= 350;)
	{
		//int i = 340;
		test_agg_cell_conUS(i, t, s);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("USagg_con_time_m10new720.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i*30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("USagg_con_space_m10new720.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i*30 ) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}

Data_instance Read_into_Data_instance_agg_new(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[11]);
	double p = stod(in_str[10]);
	vector<string> SD = { "*" };
	vector<string> TD = {};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
void test_agg_cell_con1(int number, double& time, double& space)
{
	ifstream fp("TPCH_m10_0_to10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_agg_new(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.agg_con();
	//Tree.Materialize_Tree_by_PWS();
	//Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;



}
void test_agg_con1()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 10; i <= 350;)
	{
		//int i = 340;
		test_agg_cell_con1(i, t, s);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("agg_con_time_m10_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("agg_con_space_m10_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}


/*void test_agg_cell_sketchclimate(int number, double& time, double& space)
{
	ifstream fp("USdata3.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_aggUS(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	//Tree.Materialize_Tree();
	//Tree.Materialize_Tree_by_PWS();
	Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost_sketch * 8.0 * 3.0 / 1024;



}
void test_agg_sketchclimate()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 50; i <= 11000;)
	{
		//int i = 340;
		test_agg_cell_sketchUS(i, t, s);
		ofstream dataFile;
		dataFile.open("USagg_sketch_timem10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile <<double(i) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("USagg_sketch_spacem10.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 100;
		t = 0;
		s = 0;
		spp = 0;
	}
}*/
void UStest_Montoring()
{
	double Time_sum = 0.0;
	ifstream fp("USdata2.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);

	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_US(line);
		if (s_i.Data_object_id > 10001)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_MonitorngUS(o_i.Data_instance_list[0], mounth, o_i.Data_instance_list[0].Data_instance_x, EM, VarM, 3);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			ofstream dataFile;
			dataFile.open("UStest_Montoring3.txt", ofstream::app);
			// 朝TXT文档中写入数据
			if(o_i.Data_object_id%100==0)
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
			dataFile.close();
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	Tree.Bulid_path();
	int n = 1;
	for (auto node : Tree.Path)
	{
		cout << n << "    " << node.cells.size() << endl;
		n++;
	}


}

Data_instance Read_into_Data_instance_agg_Climate(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[6]);
	double p = stod(in_str[8]);
	vector<string> SD = { "*" };
	vector<string> TD = {};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}
void test_agg_cell_con_CLimete(int number, double& time, double& space)
{
	ifstream fp("climate_m10_04.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_agg_Climate(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 30 * number + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.agg_con();
	//Tree.Materialize_Tree_by_PWS();
	//Tree.Materialize_Tree_by_sketch();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;



}
void test_agg_con_CLimete()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i = 1; i <= 350;)
	{
		//int i = 340;
		test_agg_cell_con_CLimete(i, t, s);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("agg_con_time_climate0.4.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i * 30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("agg_con_space_climate0.4.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i * 30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}

Data_instance Read_into_Data_instance_Climate(string str, const const char split = ',')
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	vector<string> in_str;
	while (getline(iss, token, split))	// 以split为分隔符
	{
		//cout << token << "  "; // 输出
		in_str.push_back(token);
	}
	int id = stoi(in_str[0]);
	double x = stod(in_str[3]);
	double y = stod(in_str[6]);
	double p = stod(in_str[8]);
	vector<string> SD = { in_str[4],in_str[5],in_str[7] };
	vector<string> TD = {in_str[1],in_str[2]};
	Data_instance s_i(id, x, y, p, SD, TD);
	return s_i;

}

void test_Climate(int number, double& time, double& space, double& space2)
{
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	//getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= number*30 + 1)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	double start_t = clock();
	cout << "读取完成，开始物化" << endl;
	Tree.Materialize_Tree();
	double end_t = clock();   //结束时间
	//cout << "time = " << 1000 * double(end_t - start_t) / CLOCKS_PER_SEC << "ms" << endl;  //输出时间（单位：mｓ）
	//cout << "root pmf size  " << Tree.Root.F_C.pmf.size() << endl;
	time = double(end_t - start_t) / CLOCKS_PER_SEC;
	space = Tree.Root.Space_cost * 8.0 * 3.0 / 1024;
	space2 = space;
	space2 += Tree.Root.Space_data / 1024.0;



}
void test_pmf_Climate()
{
	vector<double> time, space;
	vector<test_struct> Test_1;
	double t = 0, s = 0, spp = 0;
	for (int i =10; i <= 350;)
	{

		test_Climate(i, t, s, spp);
		//test_struct a(i * 366, t, s);
		//Test_1.push_back(a);
		//cout << i * 366 << "object time is     " << t << "s,    space is    " << a.space << endl;
		ofstream dataFile;
		dataFile.open("climate_materzation_time_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile << double(i*30) / 1000 << "   " << t << endl;
		dataFile.close();
		ofstream dataFile2;
		dataFile2.open("climate_materzation_space_new.txt", ofstream::app);
		// 朝TXT文档中写入数据
		dataFile2 << double(i*30) / 1000 << "   " << s << endl;
		dataFile2.close();
		i += 10;
		t = 0;
		s = 0;
		spp = 0;
	}
}


void Climatetest_Montoring(int t, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	;

	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > 10001)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_MonitorngClimate(o_i.Data_instance_list[0], mounth, o_i.Data_instance_list[0].Data_instance_x, EM, VarM,4);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			ofstream dataFile;
			
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	V = double(10000) / Time_sum;


}
void TPCH_query()
{
	ifstream fp("Ncells4.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[4]);
		double Var = stod(in_str[5]);
		double time = stod(in_str[6]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	
	/*for (double k = 10.5; k <= 12.5;)
	{
		double t_k = k;
		for (double p = 0; p <= 1.01;)
		{
			double search_time = 0.0;
			double start_t = clock();




			for (int i = 0; i < EM.size(); i++)
			{
				double a = abs(t_k - EM[i]);
				double Upper = VarM[i] / (VarM[i] + a * a);
				double Lower = a * a / (VarM[i] + a * a);
				if (p<Upper && p>Lower)
				{
					search_time += Time[i];
				}
				//cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;


			}
			double end_t = clock();   //结束时间
			search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
			ofstream dataFile;
			string str = "Search_time_p,k=";
			str += to_string(k) + ".txt";
			dataFile.open(str, ofstream::app);
			// 朝TXT文档中写入数据

			dataFile << p << "   " << search_time << endl;
			dataFile.close();
			//if (search_time > 60)break;
			p += 0.05;
		}
		k += 0.1;
	}
	*/

	for (double t_p = 0.7; t_p <=0.9; t_p +=0.2)
	{
		double p = t_p;
		for (double k =0; k<= 30;k++)
		{
			double search_time = 0.0;
			double start_t = clock();


			double t_k = k;

			for (int i = 0; i < EM.size(); i++)
			{
				double a = abs(t_k - EM[i]);
				double Upper = VarM[i] / (VarM[i] + a * a);
				double Lower = a * a / (VarM[i] + a * a);
				if (p<Upper && p>Lower)
				{
					search_time += Time[i];
				}
				//cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;


			}
			double end_t = clock();   //结束时间
			search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
			ofstream dataFile;
			//string str = "Search_time_p,k=";
			string str = "Search_time_k,p=";
			str += to_string(p) + ".txt";
			dataFile.open(str, ofstream::app);
			// 朝TXT文档中写入数据

			dataFile << k << "   " << search_time << endl;
			dataFile.close();
			//if (search_time > 60)break;
			//p += 0.05;
		}
		//k += 0.1;
	}
}


void test_search_Climate()
{
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Materalize_for_time();
	Tree.Bulid_path();
	Tree.Sketch_materzation_with_path();
	int i = 1;
	for (auto cuboid : Tree.Path)
	{
		ofstream outFile;
		string str = "climatecells";
		outFile.open(str + to_string(i) + ".csv", ios::app); //
		for (auto cells : cuboid.cells)
		{


			for (auto D : cells->Dimension)
			{
				outFile << D << ',';
			}

			outFile << cells->S_C.E_k << ',' << cells->S_C.Var_k << ',' << cells->Materalize_time << endl;;
		}
		i++;
	}
}

void Climate_query()
{
	ifstream fp("climatecells3.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[3]);
		double Var = stod(in_str[4]);
		double time = stod(in_str[5]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	double t_p = 0.9;
	//double t_k = -7.5;
	for (double k = -50;k <= 50;)
	{
		double search_time = 0.0;
		double start_t = clock();




		for (int i = 0; i < EM.size(); i++)
		{
			double a = abs(k - EM[i]);
			//double a = 0.5;
			double Upper = VarM[i] / (VarM[i] + a * a);
			double Lower = a * a / (VarM[i] + a * a);
			if (t_p<Upper && t_p>Lower)
			{
				search_time += Time[i];
			}
			cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;
			cout << EM[i] << "   " << VarM[i] << endl;

		}
		double end_t = clock();   //结束时间
		search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
		ofstream dataFile;
		dataFile.open("1ClimateSearch_time_p=0.9.txt", ofstream::app);
		// 朝TXT文档中写入数据

		dataFile << k << "   " << search_time << endl;
		dataFile.close();
		//if (search_time > 60)break;
		k += 1;
	}
}



void test_MontoringTPCH_for_3W(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		//Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > 10000)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_Monitorng_for_3w(o_i.Data_instance_list[0], mounth, o_i.Data_instance_list[0].Data_instance_x, EM, VarM, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			/*ofstream dataFile;
			dataFile.open("test_Montoring7new721.txt", ofstream::app);
			// 朝TXT文档中写入数据
			if(o_i.Data_object_id%100==0)
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
			dataFile.close();*/
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	V = double(10000) / Time_sum;
	Tree.Bulid_path();
	int n = 1;
	for (auto node : Tree.Path)
	{
		cout << n <<"    " <<node.cells.size() << endl;
		n++;
	}


}
void test_MontoringTPCH_pmf_for_3W(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id > 10001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_MonitorngTPCH_pmf_for_3w(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x,length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			/*ofstream dataFile;
			dataFile.open("test_Montoring7new721.txt", ofstream::app);
			// 朝TXT文档中写入数据
			if(o_i.Data_object_id%100==0)
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
			dataFile.close();*/
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	V = double(10001) / Time_sum;
	Tree.Bulid_path();
	int n = 1;
	/*for (auto node : Tree.Path)
	{
		cout << n << "    " << node.cells.size() << endl;
		n++;
	}*/


}


void Climatetest_Montoring_for_3W(int t, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("climate_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	;

	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > 10001)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_MonitorngClimatefor3W(o_i.Data_instance_list[0], mounth, o_i.Data_instance_list[0].Data_instance_x, EM, VarM, 4);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			ofstream dataFile;

			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	V = double(10000) / Time_sum;
	Tree.Bulid_path();
	int n = 1;
	for (auto node : Tree.Path)
	{
		cout << n << "    " << node.cells.size() << endl;
		n++;
	}

}

void Climatetest_Montoring_for_3Wpmf_online_mazation(int length = 7, double V = 7)
{
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	;

	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > 10001)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			/*for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;*/
			Tree.test_MonitorngClimatefor3Wpmf(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			ofstream dataFile;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("Climate_online_Ma_0309.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
				dataFile.close();

				ofstream dataFile2;
				double space = Tree.caculate_space();
				dataFile2.open("Climate_online_Ma_space325.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << endl;
				dataFile2.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	V = double(1000) / Time_sum;


}
/*void test_TPCH_Online_materialization(int  length, double& V, int Number)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		//Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > Number)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			for (auto instance_si : o_i.Data_instance_list)
			{

				EM += instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
				EM2 += instance_si.Data_instance_Measure * instance_si.Data_instance_Measure * instance_si.Data_instance_prob;
			}
			VarM = EM2 - EM * EM;
			Tree.test_Monitorng_for_3w(o_i.Data_instance_list[0], mounth, o_i.Data_instance_list[0].Data_instance_x, EM, VarM, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TPCH_online_Ma.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}

}
*/
void test_TPCH_Online_materialization(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id >= 20001)break;
	
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_MonitorngTPCH_pmf_for_3w(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TimeTPCH_online_Ma316.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id/1000 << "   " << Time_sum << endl;
				dataFile.close();

				ofstream dataFile2;
				double space = Tree.caculate_space();
				dataFile2.open("TimeTPCH_online_Ma_space325.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << endl;
				dataFile2.close();


			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_TPCH_Online_materialization_at_m_and_o(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_TPCH_pmf_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TPCH_online_Ma_at_o_and_m.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_TPCH_bactch_materialization_from_m_to_o() {
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_TPCH_pmf_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, 7);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				double begin_2 = clock();
				//Tree.test_batch_materialization_from_m_to_o();
				double end_2 = clock();
				double time_2 = (end_2 - begin_2) / CLOCKS_PER_SEC;
				ofstream dataFile;
				dataFile.open("TPCH_bactch_materialization_only_at_m318.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id/1000 << "   " << Time_sum + 0*time_2 << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_Climate_Online_materialization_at_m_and_o(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_Climate_pmf_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("Climate_online_Ma_at_o_and_m.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_Climate_bactch_materialization_from_m_to_o() {
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_Climate_pmf_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, 7);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				double begin_2 = clock();
				Tree.test_batch_materialization_from_m_to_o();
				double end_2 = clock();
				double time_2 = (end_2 - begin_2) / CLOCKS_PER_SEC;
				ofstream dataFile;
				dataFile.open("Climate_bactch_materialization_path=2.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id/1000 << "   " << Time_sum + time_2*0 << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}
void test_TPCH_Online_materialization_sketch(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id >= 20001)break;

		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_TPCH_Online_ma_Sketch(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TimeTPCH_online_Ma_sketch317.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << Time_sum << endl;
				dataFile.close();

				ofstream dataFile2;
				double space2 = 0.0;
				double space = Tree.sketch_caculate_space(space2);
				dataFile2.open("TPCH_online_Ma_spacesketch325.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << "   " << space2 << endl;
				dataFile2.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}
void test_TPCH_bactch_materialization_from_m_to_oSKetch() {
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_TPCH_sketch_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, 7);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				double begin_2 = clock();
				Tree.test_batch_materialization_from_m_to_o_Sketch();
				double end_2 = clock();
				double time_2 = (end_2 - begin_2) / CLOCKS_PER_SEC;
				ofstream dataFile;
				dataFile.open("TPCH_bactch_materializationSketch317.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << Time_sum + time_2 << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}
void test_Climate_Online_materialization_sketch()
{
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id > 10001)break;
		//Data_instance s_i = Read_into_Data_instance(line);//获得第k行
		//k++;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			double EM = 0, VarM = 0, EM2 = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Climate_Online_ma_Sketch(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			ofstream dataFile;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("Climate_online_Ma_sketch_0318.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id << "   " << Time_sum << endl;
				dataFile.close();

				ofstream dataFile2;
				double space2 = 0.0;
				double space = Tree.sketch_caculate_space(space2);
				dataFile2.open("Climate_online_space_sketch325.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << "   " << space2 << endl;
				dataFile2.close();

			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
	


}
void test_Climate_bactch_sketch_materialization_from_m_to_o() {
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (s_i.Data_object_id >= 110001)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_Online_MA_Climate_sketch_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, 7);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				double begin_2 = clock();
				Tree.test_batch_materialization_from_m_to_o_Sketch();
				double end_2 = clock();
				double time_2 = (end_2 - begin_2) / CLOCKS_PER_SEC;
				ofstream dataFile;
				dataFile.open("Climate_bactch_sketch_materialization.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << Time_sum + time_2 * 0 << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}
void test_TPCH_regression_convolution_for_TKDE(int number) {
	double Time_sum = 0.0;
	ifstream fp("TPCH_m10.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	int count = 0;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance(line);
		if (count >= number + 1)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			count++;
			if(o_i.Data_object_id % 200 == 0)
			cout << o_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_regression_in_a_all_cell(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile1,dataFile2;
				dataFile1.open("time_TPCH_regression_convolutionsketch317.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile1 << double(o_i.Data_object_id)/1000 << "   " << Time_sum << endl;
				dataFile1.close();
				/*dataFile2.open("space_TPCH_regression_convolution_KB317.txt", ofstream::app);
				// 朝TXT文档中写入数据
				double cost_space = double(Tree.Root.F_C.pmf.size() )* (3 * 8) / 1024;
				dataFile2 << double(o_i.Data_object_id) /1000 << "   " << cost_space << endl;
				dataFile2.close();*/

				
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}
	}
}
void test_Climate_regression_convolution_for_TKDE(int number) {
	double Time_sum = 0.0;
	ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	int count = 0;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_Climate(line);
		if (count >= number + 1)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			count++;
			if (o_i.Data_object_id % 200 == 0)
				cout << o_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[1]);
			Tree.test_regression_in_a_all_cell(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile1, dataFile2;
				dataFile1.open("time_Climate_regression_convolutionsketch.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile1 << o_i.Data_object_id / 1000 << "   " << Time_sum << endl;
				dataFile1.close();
				dataFile2.open("space_Climate_regression_convolution_KB.txt", ofstream::app);
				// 朝TXT文档中写入数据
				double cost_space = double(Tree.Root.F_C.pmf.size() * (3 * 8)) / 1024.0;
				dataFile2 << o_i.Data_object_id / 1000 << "   " << cost_space << endl;
				dataFile2.close();


			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}
	}
}

void TPCH_query2()
{
	ifstream fp("Ncells4.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[4]);
		double Var = stod(in_str[5]);
		double time = stod(in_str[6]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	double sum_time = 0.0;
	for (double k = 10.5; k <= 12.5;)
	{
		double t_k = k;
		for (double p = 0; p <= 1.01;)
		{
			double search_time = 0.0;
			double start_t = clock();



			double exceptioncells = 0;
			for (int i = 0; i < EM.size(); i++)
			{
				sum_time += Time[i];
				double a = abs(t_k - EM[i]);
				double Upper = VarM[i] / (VarM[i] + a * a);
				double Lower = a * a / (VarM[i] + a * a);
				if (p<Upper && p>Lower)
				{
					//search_time += Time[i];
					exceptioncells++;
				}
				//cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;


			}

			cout << sum_time << endl;
			return;
			double end_t = clock();   //结束时间
			search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
			ofstream dataFile;
			string str = "Search_%_p,k=";
			str += to_string(k) + ".txt";
			dataFile.open(str, ofstream::app);
			// 朝TXT文档中写入数据
			double qurey = exceptioncells / double(EM.size());
			dataFile << p << "   " << 1 - qurey << endl;
			//dataFile << p << "   " << search_time << endl;
			dataFile.close();
			//if (search_time > 60)break;
			p += 0.05;
		}
		k += 0.1;
	}
	
	/*for (double t_p = 0.5; t_p <= 0.9; t_p += 0.2)
	{
		
		for (double k = 0; k <= 100; k++)
		{
			double p = t_p;
			double sumcells = 0, exceptioncells = 0;
			double search_time = 0.0;
			double start_t = clock();


			double t_k = k;

			for (int i = 0; i < EM.size(); i++)
			{
				double a = abs(t_k - EM[i]);
				double Upper = VarM[i] / (VarM[i] + a * a);
				double Lower = a * a / (VarM[i] + a * a);
				if (p<Upper && p>Lower)
				{
					//search_time += Time[i];
					exceptioncells++;
				}
				//cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;
				sumcells++;

			}
			double end_t = clock();   //结束时间
			search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
			ofstream dataFile;
			//string str = "Search_time_p,k=";
			string str = "Search_%_k,p=";
			str += to_string(p) + ".txt";
			dataFile.open(str, ofstream::app);
			// 朝TXT文档中写入数据
			double qurey = exceptioncells / double(EM.size());
			dataFile << k << "   " << 1 - qurey << endl;
			dataFile.close();
			//if (search_time > 60)break;
			//p += 0.05;
		}
		//k += 0.1;
	}*/
}

void Climate_query2()
{
	ifstream fp("climatecells3.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[3]);
		double Var = stod(in_str[4]);
		double time = stod(in_str[5]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	//double t_p = 0.9;
	double t_k = -7.5;
	//for (double k = -50; k <= 50;)
	for(double p = 0; p <= 1; p+=0.1 )
	{
		double search_time = 0.0;
		double start_t = clock();
		double t_p = p;
		double k = t_k;
		double exceptioncells = 0;


		for (int i = 0; i < EM.size(); i++)
		{
			double a = abs(k - EM[i]);
			//double a = 0.5;
			double Upper = VarM[i] / (VarM[i] + a * a);
			double Lower = a * a / (VarM[i] + a * a);
			if (t_p<Upper && t_p>Lower)
			{
				//search_time += Time[i];
				exceptioncells++;
			}
			cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;
			cout << EM[i] << "   " << VarM[i] << endl;

		}
		double end_t = clock();   //结束时间

		//search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
		ofstream dataFile;
		
		dataFile.open("1ClimateSearch_%_k=-7.5.txt", ofstream::app);
		// 朝TXT文档中写入数据
		double qurey = exceptioncells / double(EM.size());
		dataFile << t_p << "   " << 1 - qurey << endl;
		//dataFile << k << "   " << search_time << endl;
		dataFile.close();
		//if (search_time > 60)break;
		k++;
	}
}

void test_DROP_Online_materialization(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("DROPdataA15B15q1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		if (s_i.Data_object_id >= 20001)break;

		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[0]);
			Tree.test_MonitorngDROP_pmf_for_TKDE(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TimeDROP_online_time_1_11.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << Time_sum << endl;
				dataFile.close();

				ofstream dataFile2;
				double space = Tree.caculate_space();
				dataFile2.open("TimeDROP_online_Ma_space111.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << endl;
				dataFile2.close();


			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_DROP_regression_convolution_for_TKDE(int number) {
	double Time_sum = 0.0;
	ifstream fp("DROPdataA15B15q1.csv");//定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	int count = 0;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		if (count >= number + 1)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			count++;
			if (o_i.Data_object_id % 200 == 0)
				cout << o_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[0]);
			Tree.test_regression_in_a_all_cell(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile1, dataFile2;
				dataFile1.open("time_DROP_regression_convolution112.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile1 << double(o_i.Data_object_id) / 1000 << "   " << Time_sum << endl;
				dataFile1.close();
				dataFile2.open("space_DROP_regression_convolution112.txt", ofstream::app);
				// 朝TXT文档中写入数据
				double cost_space = double(Tree.Root.F_C.pmf.size() )* (3 * 8) / 1024;
				dataFile2 << double(o_i.Data_object_id) /1000 << "   " << cost_space << endl;
				dataFile2.close();


			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}
	}
}

void test_search_DROP()
{
	ifstream fp("DROPdataA15B15q1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Materalize_for_time();
	Tree.Bulid_path();
	Tree.Sketch_materzation_with_path();
	int i = 1;
	for (auto cuboid : Tree.Path)
	{
		ofstream outFile;
		string str = "2DROPcells";
		outFile.open(str + to_string(i) + ".csv", ios::app); //
		for (auto cells : cuboid.cells)
		{


			for (auto D : cells->Dimension)
			{
				outFile << D << ',';
			}

			outFile << cells->S_C.E_k << ',' << cells->S_C.Var_k << ',' << cells->Materalize_time*2.0 << endl;;
		}
		i++;
	}
}
void DROP_query2()
{
	ifstream fp("2DROPcells4.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[4]);
		double Var = stod(in_str[5]);
		double time = stod(in_str[6]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	//double t_p = 0.9;
	double t_k = -0.05;
	double p = 0.9;
	for (double k = -1; k <= 1;)
	//for (double p = 0; p <= 1.1; p += 0.05)
	{
		double search_time = 0.0;
		double start_t = clock();
		double t_p = p;
		//double k = t_k;
		double exceptioncells = 0;


		for (int i = 0; i < EM.size(); i++)
		{
			double a = abs(k - EM[i]);
			//double a = 0.5;
			double Upper = VarM[i] / (VarM[i] + a * a);
			double Lower = a * a / (VarM[i] + a * a);
			if (t_p<Upper && t_p>Lower)
			{
				search_time += Time[i];
				exceptioncells++;
			}
			cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;
			cout << EM[i] << "   " << VarM[i] << endl;

		}
		double end_t = clock();   //结束时间

		//search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
		ofstream dataFile;

		dataFile.open("DROPSearch_prun_k0.9.txt", ofstream::app);
		// 朝TXT文档中写入数据
		double qurey = exceptioncells / double(EM.size());
		dataFile << k << "   " << (1 - qurey)*100 << endl;
		//dataFile << t_p << "   " << search_time << endl;
		dataFile.close();
		dataFile.open("DROPSearch_time_k0.9.txt", ofstream::app);
		// 朝TXT文档中写入数据
		//double qurey = exceptioncells / double(EM.size());
		//dataFile << k << "   " << (1 - qurey) * 100 << endl;
		dataFile << k << "   " << search_time << endl;
		dataFile.close();
		//if (search_time > 60)break;
		k+=0.1;
	}
}
void DROP_query()
{
	ifstream fp("DROPcells4.csv");
	//ifstream fp("climate_m10_D.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	vector<double>EM, VarM, Time;
	while (getline(fp, line)) { //循环读取每行数据

		istringstream iss(line);	// 输入流
		string token;			// 接收缓冲区
		vector<string> in_str;
		while (getline(iss, token, ','))	// 以split为分隔符
		{
			//cout << token << "  "; // 输出
			in_str.push_back(token);
		}
		//int id = stoi(in_str[0]);
		double Means = stod(in_str[4]);
		double Var = stod(in_str[5]);
		double time = stod(in_str[6]);
		EM.push_back(Means);
		VarM.push_back(Var);
		Time.push_back(time);
	}
	//double t_k = 0, t_p = 0;
	double t_p = 0.3;
	//double t_k = -7.5;
	int exceptioncells = 0;
	for (double k = -50; k <= 100;)
	{
		double search_time = 0.0;
		double start_t = clock();




		for (int i = 0; i < EM.size(); i++)
		{
			double a = abs(k - EM[i]);
			//double a = 0.5;
			double Upper = VarM[i] / (VarM[i] + a * a);
			double Lower = a * a / (VarM[i] + a * a);
			if (t_p<Upper && t_p>Lower)
			{
				search_time += Time[i];
				exceptioncells++;
			}
			cout << Upper << "  " << Lower << "   " << Upper - Lower << endl;
			cout << EM[i] << "   " << VarM[i] << endl;

		}
		double end_t = clock();   //结束时间
		search_time += double(end_t - start_t) / CLOCKS_PER_SEC;
		ofstream dataFile;
		dataFile.open("DROP_time_p=0.3.txt", ofstream::app);
		// 朝TXT文档中写入数据

		dataFile << k << "   " << search_time*4 << endl;
		dataFile.close();
		//if (search_time > 60)break;
		k += 1;
	}
}


void test_DROP_Online_sketch_materialization(int  length, double& V)
{
	double Time_sum = 0.0;
	ifstream fp("DROPdataA15B15q1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		if (s_i.Data_object_id >= 20001)break;

		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{
			if (s_i.Data_object_id % 200 == 0)cout << s_i.Data_object_id << endl;
			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[0]);
			Tree.test_MonitorngDROP_sketch_for_TKDE(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, length);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				ofstream dataFile;
				dataFile.open("TimeDROP_online_sketch_timenew2.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << Time_sum*1.947712 << endl;
				dataFile.close();

				ofstream dataFile2;
				double space = Tree.caculate_space();
				dataFile2.open("TimeDROP_online_sketch_space.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile2 << o_i.Data_object_id / 1000 << "   " << space << endl;
				dataFile2.close();


			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}

void test_DROP_materzation_with_sketch_popular_path_length()
{
	ifstream fp("DROPdataA15B15q1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	//ofstream dataFile;
	//dataFile.open("stream_read_v.txt", ofstream::app);
	double start_1 = clock();
	while (getline(fp, line)) { //循环读取每行数据
		//Read_into_Data_instance(line);
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		//Read_into_Data_instance(line)
		int id = s_i.Data_object_id;
		if (id >= 10201)
		{
			//cout << "当前进行实验的对象个数为：" << id - 1 << endl;
			break;
		}
		Tree.Read_stream_instance(s_i);
		cout << endl;
		k++;

	}
	Tree.Bulid_path();
	vector<double> time = Tree.Sketch_materzation_with_path();
	ofstream dataFile;
	dataFile.open("test_materzation_with_sketch_DROP.txt", ofstream::app);
	// 朝TXT文档中写入数据
	int path_length = 1;
	for (auto t : time)
	{
		dataFile << path_length << "   " << t << endl;
		path_length++;
	}

	dataFile.close();

}
void test_DROP_bactch_materialization_from_m_to_oSKetch() {
	double Time_sum = 0.0;
	ifstream fp("DROPdataA15B15q1.csv"); //定义声明一个ifstream对象，指定文件路径
	string line;
	getline(fp, line); //跳过列名，第一行不做处理
	int k = 0;
	H_tree Tree;
	double t = 0.0;
	Data_object o_i;
	while (getline(fp, line)) { //循环读取每行数据
		Data_instance s_i = Read_into_Data_instance_DROP(line);
		//if (count >= number + 1)break;
		if ((s_i.Data_object_id == o_i.Data_object_id) || (o_i.Data_object_id < 1))
		{
			o_i.Add_new_instance(s_i);
		}
		if ((s_i.Data_object_id != o_i.Data_object_id) && (o_i.Data_object_id > 0))//说明获取完一个O_i,开始执行在线更新工作
		{

			//开始在线更新,此处开始计时
			double start_1 = clock();
			int mounth = 0;
			int x = 0;
			mounth = stod(o_i.Data_instance_list[0].Time_Dimension[0]);
			Tree.test_Online_MA_DROP_sketch_only_at_o_and_m(o_i, mounth, o_i.Data_instance_list[0].Data_instance_x, 7);
			double end_1 = clock();
			double t = (end_1 - start_1) / CLOCKS_PER_SEC;
			Time_sum += t;
			if ((o_i.Data_object_id) % 1000 == 0) {
				double begin_2 = clock();
				Tree.test_batch_materialization_from_m_to_o_Sketch();
				double end_2 = clock();
				double time_2 = (end_2 - begin_2) / CLOCKS_PER_SEC;
				ofstream dataFile;
				dataFile.open("DROP_bactch_materializationSketch317.txt", ofstream::app);
				// 朝TXT文档中写入数据
				dataFile << o_i.Data_object_id / 1000 << "   " << (Time_sum + time_2)*1.947712 << endl;
				dataFile.close();
			}
			o_i.Data_instance_list.clear();
			o_i.Data_object_id = -1;
			o_i.Add_new_instance(s_i);

		}


	}
}


int main()
{
	//double sum = 0;
	//Climatetest_Montoring_for_3W(4, sum);
	//TPCH_query();
	//test_search();
	//test_search_Climate();
	//test_search();
	//TPCH_query();
	//Climatetest_Montoring();
	//test_pmf_Climate();
	//UStest_Montoring();
	//online_monitoring();
	//test_agg_sketchUS();
	//test_agg_conUS();
	//test_pmf_tpch();//离线物化10k卷积聚合函数,m=10,0~10k
	//test_sketch_tpch();//离线物化10ksketch聚合函数,m=10,0~10k
	//test_materzation_with_con_popular_path_length();
//	test_materzation_with_sketch_popular_path_length();
	//double t, s;
	//test_agg_pws();
	//test_agg_sketch();
	//test_agg_con();
	//test_Montoring();
	//test_search();
	//query_time();
	//test_pmf_US();
	//double t, s;
	//test_sketch_US();
	//test_materzation_with_con_popular_path_length_US();
//	test_agg_con1();
	//test_agg_con_CLimete();
	//test_agg_conUS();
	//test_agg_conUS();
	//test_agg_con_CLimete();
//	Climate_query();
	//Climate_query();
	//test_search_DROP();
	//test_query_DPRO();
	/*double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
	for (int i = 0; i < 10; i++)
	{
		double V1 = 0.0, V2 = 0.0, V3 = 0.0,V4=0.0;
		Climatetest_Montoring(1, V1);
		Climatetest_Montoring(2, V2);
		Climatetest_Montoring(3, V3);

		Climatetest_Montoring(4, V4);
		cout << 1 << " " << V1 << endl;
		cout <<2 << " " << V3 << endl;
		cout << 3 << " " << V3 << endl;

		cout <<4 << " " << V4 << endl;
		sum1 += V1;
		sum2 += V2;
		sum3 += V3;

		sum4 += V4;
		
	}
	cout << sum1 / 10 << endl;
	cout << sum2 / 10 << endl;
	cout << sum3 / 10 << endl;

	cout << sum4 / 10 << endl;*/
	//DROP_query2();
	//DROP_query2();
	//test_agg_pws();
	//double sum = 0;
	//test_MontoringTPCH_for_3W(7, sum);
	/*double sum = 0;
	for (int i = 0; i < 10; i++)
	{
		double time = 0;
		Climatetest_Montoring_for_3W(4, time);
		sum += time;
	}
	sum = sum / 10;
	cout <<4<<"   " << sum << endl;
	 sum = 0;
	for (int i = 0; i < 10; i++)
	{
		double time = 0;
		Climatetest_Montoring_for_3W(3, time);
		sum += time;
	}
	sum = sum / 10;

	cout << 3 << "   " << sum << endl;
	sum = 0;
	for (int i = 0; i < 10; i++)
	{
		double time = 0;
		Climatetest_Montoring_for_3W(2, time);
		sum += time;
	}
	sum = sum / 10;

	cout << 2 << "   " << sum << endl;
 sum = 0;
	for (int i = 0; i < 10; i++)
	{
		double time = 0;
		Climatetest_Montoring_for_3W(1, time);
		sum += time;
	}
	sum = sum / 10;
	cout << 1 << "   " << sum << endl;*/
	//TPCH_query();
	//Climate_query();
double	time = 0.0;
//test_DROP_bactch_materialization_from_m_to_oSKetch();
//test_search_DROP();
//test_DROP_Online_sketch_materialization(3, time);
//test_DROP_Online_materialization(3, time);
//test_DROP_regression_convolution_for_TKDE(20001);
//test_TPCH_Online_materialization(7, time);
//test_Climate_regression_convolution_for_TKDE(20001);
//test_TPCH_Online_materialization(7, time);
//test_TPCH_regression_convolution_for_TKDE(20001);
	//return 0;
//test_TPCH_regression_convolution_for_TKDE(10001);
//test_TPCH_Online_materialization_sketch(7, time);
//test_TPCH_bactch_materialization_from_m_to_o();
// 
//test_TPCH_bactch_materialization_from_m_to_oSKetch();
//test_Climate_Online_materialization_sketch();
//test_Climate_bactch_sketch_materialization_from_m_to_o();
//test_Climate_regression_convolution_for_TKDE(10001);
//test_TPCH_regression_convolution_for_TKDE(11001);
//test_TPCH_Online_materialization(7, time);
//TPCH_query2();
//test_Climate_Online_materialization_sketch();
//test_DROP_materzation_with_sketch_popular_path_length();
test_DROP_bactch_materialization_from_m_to_oSKetch();
};