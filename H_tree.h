#pragma once
#include"H_node.h"
#include"data_instance.h"
#include<stack>
#include"Path.h"
class H_tree {

public:
	H_node Root;//子结点
	H_tree();
	vector<cuboid> Path;
	
	double Space_Cost=0.0;
	bool isMateralize = false;
	bool  isMateralize_sketch = false;
	void Read_stream_instance(Data_instance t);//读入instance
	void Read_stream_instance_new_path(Data_instance t);//读入instance的函数
	void Materialize_Tree();//物化函数
	void Materialize_Tree_H();
	void Materialize_Tree_by_sketch();//物化函数,用sketch
	void Materialize_Tree_by_PWS();//基于PWS的聚合
	void query_pmf();
	void query_sketch();
	void quety_sketch_BFS();
	void query_pmf_BFS();
	void Bulid_path();//构建popular-path的函数
	vector<double>  Con_materzation_with_path();//查看卷积物化与popular-path层数之间关系的函数
	vector<double>  Sketch_materzation_with_path();////查看sketch物化与popular-path层数之间关系的函数
	void  agg_con();////卷积聚合的函数
	void test_Monitorng(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length);//测试吞吐量的函数
	void Materalize_for_time();
	void test_MonitorngUS(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length);
	void test_MonitorngClimate(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length);
	void Read_stream_instance2(Data_instance t, int M);
	void test_Monitorng_for_3w(Data_instance t, int mounth, int x, double EM, double VarM, int length);
	void test_MonitorngTPCH_pmf_for_3w(Data_object o, int mounth, int x, int length);
	void test_MonitorngClimatefor3W(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length);
	void test_MonitorngClimatefor3Wpmf(Data_object o, int mounth, int x, int length);

	void test_Online_MA_TPCH_pmf_only_at_o_and_m(Data_object o, int mounth, int x, int length);
	void test_batch_materialization_from_m_to_o();
	void test_Online_MA_Climate_pmf_only_at_o_and_m(Data_object o, int mounth, int x, int length);
	void test_regression_in_a_all_cell(Data_object o, int mounth, int x);
	void test_TPCH_Online_ma_Sketch(Data_object o, int mounth, int x);
	void test_Online_MA_TPCH_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length);
	void test_batch_materialization_from_m_to_o_Sketch();
	void test_Climate_Online_ma_Sketch(Data_object o, int mounth, int x);
	void test_Online_MA_Climate_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length);
	double caculate_space();
	double sketch_caculate_space(double& space2);
	void test_MonitorngDROP_pmf_for_TKDE(Data_object o, int mounth, int x, int length);
	void test_Regression_regression_DROP_TKDE(Data_object o, int mounth, int x,int length);
	void test_MonitorngDROP_sketch_for_TKDE(Data_object o, int mounth, int x, int length);
	void test_Online_MA_DROP_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length);

};
H_tree::H_tree() {
	Root.Dimension.push_back("*");
	Root.IsRoot = true;
};
void H_tree::Read_stream_instance(Data_instance t) {
	H_node *Now_node;
	Now_node= &this->Root;
	
	//先时间维度,遍历时间维
	int k =0 ;
	for (auto &d : t.Time_Dimension)
	{
		bool search = false;
		for(int i=0;i<Now_node->Children_node_list.size();i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}
			
		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			if (k == 0)
			{
				New_node.IsTime = true;
				
			}
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			
			Now_node = &Now_node->Children_node_list.back();
			
			
		}
		k++;
	}
	//标准维度
	for (auto &d : t.Standard_Dimension)
	{
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			
			Now_node = &Now_node->Children_node_list.back();
		}
		
	}
	Now_node->IsLeaf = true;
	if (Now_node->T < t.Data_instance_x)
	{
		Now_node->T = t.Data_instance_x;
	}
	//Now_node->T = 30;//叶子结点全部设置为30
	instance_pmf pmf(t.Data_object_id,t.Data_instance_x,t.Data_instance_Measure,t.Data_instance_prob);
	Now_node->add_pmf(pmf);
};
void H_tree::Materialize_Tree()
{

	this->Root.Materialize_node();
	this->isMateralize = true;


}
void H_tree::Materialize_Tree_H()
{

	this->Root.Materialize_node_Histogram();
	this->isMateralize = true;


}
void H_tree::Materialize_Tree_by_sketch()
{

	this->Root.Materialize_node_by_sketch();
	this->isMateralize_sketch = true;

}
void H_tree::Materialize_Tree_by_PWS()
{

	this->Root.Materialize_node_by_pws();

}
void H_tree::query_pmf()
{
	if (this->isMateralize == false)
	{
		cout << "*****物化未完成*****" << endl;
		return;
	}
	
	this->Root.query_pmf_DFS();






}
void H_tree::query_sketch()
{
	if (this->isMateralize_sketch == false)
	{
		cout << "*****物化未完成*****" << endl;
		return;
	}

	this->Root.query_sketch_DFS();
}
void H_tree::quety_sketch_BFS()
{
	if (this->isMateralize_sketch == false)
	{
		cout << "*****物化未完成*****" << endl;
		return;
	}
	vector<H_node> list;
	list.push_back(this->Root);
	while (list.empty()==false)
	{
		auto Now_node = list[0];
		Now_node.query_sketch_BFS();
		vector<H_node>::iterator k = list.begin();
		list.erase(k);//删除第一个元素
		if (Now_node.IsLeaf == false)
		{
			for (auto child : Now_node.Children_node_list)
			{
				list.push_back(child);
			}
		}
	}
	
}
void H_tree::query_pmf_BFS()
{
	if (this->isMateralize== false)
	{
		cout << "*****物化未完成*****" << endl;
		return;
	}
	vector<H_node> list;
	list.push_back(this->Root);
	while (list.empty() == false)
	{
		auto Now_node = list[0];
		Now_node.query_pmf_BFS();
		vector<H_node>::iterator k = list.begin();
		list.erase(k);//删除第一个元素
		if (Now_node.IsLeaf == false)
		{
			for (auto child : Now_node.Children_node_list)
			{
				list.push_back(child);
			}
		}
	}

}
void H_tree::Bulid_path() {
	cuboid root_cuboid;
	root_cuboid.cells.push_back(&this->Root);
	this->Path.push_back(root_cuboid);
	while (true)
	{
		cuboid now_cuboid = Path.back();
		cuboid new_cuboids;
		bool isjump=false;
		for(auto Nowcells : now_cuboid.cells)
		{
			
			for(int i=0;i<Nowcells->Children_node_list.size();i++)
			{

				new_cuboids.cells.push_back(&Nowcells->Children_node_list[i]);
				isjump = Nowcells->Children_node_list[i].IsLeaf;
			}
			
			//Path.push_back(new_cuboids);
		}
		this->Path.push_back(new_cuboids);
		if (isjump == true)
			return;
	}
}
double H_tree::caculate_space() {
	double space = 0.0;
	cuboid root_cuboid;
	root_cuboid.cells.push_back(&this->Root);
	this->Path.push_back(root_cuboid);
	space += this->Root.F_C.pmf.size();
	while (true)
	{
		cuboid now_cuboid = Path.back();
		cuboid new_cuboids;
		bool isjump = false;
		for (auto Nowcells : now_cuboid.cells)
		{

			for (int i = 0; i < Nowcells->Children_node_list.size(); i++)
			{

				new_cuboids.cells.push_back(&Nowcells->Children_node_list[i]);
				space += Nowcells->Children_node_list[i].F_C.pmf.size();
				isjump = Nowcells->Children_node_list[i].IsLeaf;
			}

			//Path.push_back(new_cuboids);
		}
		this->Path.push_back(new_cuboids);
		if (isjump == true)
			break;
	}
	return double(space * 3 * 8 / 1024);
}
double H_tree::sketch_caculate_space(double &space2) {
	double space = 0.0;
	cuboid root_cuboid;
	root_cuboid.cells.push_back(&this->Root);
	this->Path.push_back(root_cuboid);
	space += 5;
	space2 += 5;
	while (true)
	{
		cuboid now_cuboid = Path.back();
		cuboid new_cuboids;
		bool isjump = false;
		for (auto Nowcells : now_cuboid.cells)
		{

			for (int i = 0; i < Nowcells->Children_node_list.size(); i++)
			{

				new_cuboids.cells.push_back(&Nowcells->Children_node_list[i]);
				space += 5;
				space2 += 5;
				isjump = Nowcells->Children_node_list[i].IsLeaf;
				if (isjump == true) {
					space2 += 5;
				}
			}

			//Path.push_back(new_cuboids);
		}
		this->Path.push_back(new_cuboids);
		if (isjump == true)
			break;
	}
	space2 = space2*8 / 1024;
	return double(space * 8 / 1024);
}

vector<double>  H_tree::Con_materzation_with_path()
{
	vector<double> time_list;
	int length = Path.size() - 1;
	double start_t = clock();
	int i = length;
	{
		for (auto &cells : Path[i].cells)
		{
			cells->Use_object_pmf_to_get_line_pmf_on_leaf_node();
			cells->isMaterialize = true;
		}
	}
	//int path_length = 1;
	
	
	for (int i = length - 1; i > 0; i--)
	{

		for (auto &cells : Path[i].cells)
		{
			cells->Materialize_Not_leaf();
		}
		double end_t = clock();   //结束时间
		double time = double(end_t - start_t) / CLOCKS_PER_SEC;
		time_list.push_back(time);
	}
	
	return time_list;



}
vector<double>  H_tree::Sketch_materzation_with_path()
{
	vector<double> time_list;
	int length = Path.size() - 1;
	double start_t = clock();
	int i = length;
	{
		for (auto& cells : Path[i].cells)
		{
			cells->get_sketch_on_leaf_node();
			cells->isMaterialize = true;
		}
	}
	//int path_length = 1;
	
	double start_2 = clock();
	for (int i = length - 1; i > 0; i--)
	{

		for (auto& cells : Path[i].cells)
		{
			cells->Materialize_Not_leaf_by_sketch();
		}
		double end_t = clock();   //结束时间
		double time = double(end_t - start_2) / CLOCKS_PER_SEC;
		time_list.push_back(time);
	}
	time_list.push_back(double(start_2 - start_t) / CLOCKS_PER_SEC);
	return time_list;



}
void H_tree::agg_con()
{
	this->Root.agg_cell();
	this->isMateralize = true;
}
/*void H_tree::Read_stream_instance(Data_instance t) {
	H_node *Now_node;
	Now_node= &this->Root;
	
	//先时间维度,遍历时间维
	for (auto &d : t.Time_Dimension)
	{
		bool search = false;
		for(int i=0;i<Now_node->Children_node_list.size();i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}
			
		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			
		}
		
	}
	//标准维度
	for (auto &d : t.Standard_Dimension)
	{
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			
			Now_node = &Now_node->Children_node_list.back();
		}
		
	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	instance_pmf pmf(t.Data_object_id,t.Data_instance_x,t.Data_instance_Measure,t.Data_instance_prob);
	Now_node->add_pmf(pmf);
};*/
void H_tree::test_Monitorng(Data_instance t,int mounth,int x,double EM,double VarM,int popular_length) {
	H_node* Now_node;
	Now_node = &this->Root;

	//先时间维度,遍历时间维
	for(int j=0;j<t.Time_Dimension.size();j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (popular_length == 7)//说明此时1912是m
				{
					if (j == 0)//进行处理
					{
						Now_node->on_line_sketch(EM, VarM, mounth * x);
					}
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (popular_length == 7)//说明此时1912是m
			{
				if (j == 0)//进行处理
				{
					Now_node->on_line_sketch(EM, VarM, mounth * x);
					break;
				}
			}
		}

	}
	//标准维度
	for(int j=0;j< t.Standard_Dimension.size();j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if(popular_length==4)
				{
					if (j == 1)
					{
						Now_node->on_line_sketch(EM, VarM, x);
					}
					
				}
				else if (popular_length == 1)
				{
					if (j == 4)
					{
						Now_node->on_line_sketch(EM, VarM, x);
					}
				}
				break;
				
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();

			if (popular_length == 4)
			{
				if (j == 1)
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}

			}
			else if (popular_length == 1)
			{
				if (j == 4)
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
			}
		}

	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(EM, VarM, x);
	//instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	//Now_node->add_pmf(pmf);
};
void H_tree::Materalize_for_time() {

	this->Root.Materialize_node_for_time();

	this->isMateralize = true;









}

void H_tree::Read_stream_instance_new_path(Data_instance t) {
	H_node* Now_node;
	Now_node = &this->Root;
	for (int i = 0; i <2; i++)
	{
		string d = t.Standard_Dimension[i];

		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
		}




	}
	
		string d = t.Time_Dimension[0];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();

		}
		{
			d = t.Time_Dimension[1];
			bool search = false;
			for (int i = 0; i < Now_node->Children_node_list.size(); i++)
			{
				//存在当前维度
				if (Now_node->Children_node_list[i].Now_Dimenson == d)
				{
					Now_node = &Now_node->Children_node_list[i];
					search = true;
					break;
				}

			}
			if (search == false)
			{
				//新加一个cell
				H_node New_node;
				New_node.Now_Dimenson = d;
				New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
				New_node.Dimension.push_back(d);
				Now_node->Children_node_list.push_back(New_node);

				Now_node = &Now_node->Children_node_list.back();

			}

		}
	
	//标准维度
	for (int i = 2; i < 5; i++)
	{
		d = t.Standard_Dimension[i];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
		}

	}
	//后时间维度,遍历时间维
	//for (auto& d : t.Time_Dimension)
	
	for (int i = 5; i < t.Standard_Dimension.size(); i++)
	{
		string d = t.Standard_Dimension[i];

		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
		}




	}
	Now_node->IsLeaf = true;
	/*if (Now_node->T < t.Data_instance_x)
	{
		Now_node->T = t.Data_instance_x;
	}*/
	Now_node->T = 30;//叶子结点全部设置为30
	instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	Now_node->add_pmf(pmf);
};
void H_tree::test_MonitorngUS(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length) {
	H_node* Now_node;
	Now_node = &this->Root;

	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (popular_length == 4)//说明此时年
				{
					if (j == 0)//进行处理
					{
						Now_node->on_line_sketch(EM, VarM, mounth * x);
					}
				}
				else if (popular_length == 3)
				{
					if (j == 1)//进行处理
					{
						Now_node->on_line_sketch(EM, VarM, x);
					}
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (popular_length == 3)//说明此时年
			{
				if (j == 0)//进行处理
				{
					Now_node->on_line_sketch(EM, VarM, mounth * x);
				}
			}
			else if (popular_length == 2)
			{
				if (j == 1)//进行处理
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
			}
		}

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (popular_length==1)
				{
					if(j==0)
					Now_node->on_line_sketch(EM, VarM, x);

				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (popular_length == 1)
			{
				if (j == 0)
					Now_node->on_line_sketch(EM, VarM, x);

			}
		}

	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(EM, VarM, x);
	//instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	//Now_node->add_pmf(pmf);
};

void H_tree::test_MonitorngClimate(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length) {
	H_node* Now_node;
	Now_node = &this->Root;

	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (popular_length == 4)//
				{
					if (j == 0)//进行处理
					{
						Now_node->on_line_sketch(EM, VarM, mounth * x);
						//Now_node->on_line_sketch(o_i, mounth * x);
						
					}
				}
				else if (popular_length == 3)
				{
					if (j == 1)//进行处理
					{
						Now_node->on_line_sketch(EM, VarM, x);
						//Now_node->on_line_sketch(o_i, x);
					}
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (popular_length == 4)//说明此时年
			{
				if (j == 0)//进行处理
				{
					Now_node->on_line_sketch(EM, VarM, mounth * x);
				
					//Now_node->on_line_sketch(o_i, mounth * x);
				}
			}
			else if (popular_length == 3)
			{
				if (j == 1)//进行处理
				{
					Now_node->on_line_sketch(EM, VarM, x);
					//Now_node->on_line_sketch(o_i, x);
				}
			}
		}

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d =t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (popular_length == 2)
				{
					if (j == 0)
					{
						Now_node->on_line_sketch(EM, VarM, x);
					}
						//Now_node->on_line_sketch(EM, VarM, x);
						//Now_node->on_line_sketch(o_i, x);
						//Now_node->on_line_sketch(EM, VarM, x);

				}
				if (popular_length == 1)
				{
					if (j == 1)
					{
						Now_node->on_line_sketch(EM, VarM, x);
					}
						//Now_node->on_line_sketch(o_i, x);
						//Now_node->on_line_sketch(EM, VarM, x);

				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (popular_length == 2)
			{
				if (j == 0)
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
					//Now_node->on_line_sketch(o_i, x);
					//Now_node->on_line_sketch(EM, VarM, x);

			}
			if (popular_length == 1)
			{
				if (j == 1)
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
					//Now_node->on_line_sketch(o_i, x);
					//Now_node->on_line_sketch(EM, VarM, x);

			}
		}

	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(EM, VarM, x);
	//instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	//Now_node->add_pmf(pmf);
};

void H_tree::Read_stream_instance2(Data_instance t,int M) {
	H_node* Now_node;
	Now_node = &this->Root;
	int Now_length= 0;
	//先时间维度,遍历时间维
	int k = 0;
	for (auto& d : t.Time_Dimension)
	{
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			if (k == 0)
			{
				New_node.IsTime = true;

			}
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();


		}
		k++;
		Now_length++;
		if (Now_length > M)
		{
			Now_node->IsLeaf = true;
			if (Now_node->T < t.Data_instance_x)
			{
				Now_node->T = t.Data_instance_x;
			}
			instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
			Now_node->add_pmf(pmf);
			return;
		}
			//return;
	}
	//标准维度
	for (auto& d : t.Standard_Dimension)
	{
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
		}
		Now_length++;
		if (Now_length > M)
		{
			Now_node->IsLeaf = true;
			if (Now_node->T < t.Data_instance_x)
			{
				Now_node->T = t.Data_instance_x;
			}
			instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
			Now_node->add_pmf(pmf);
			return;
		}
	}
	Now_node->IsLeaf = true;
	if (Now_node->T < t.Data_instance_x)
	{
		Now_node->T = t.Data_instance_x;
	}
	//Now_node->T = 30;//叶子结点全部设置为30
	instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	Now_node->add_pmf(pmf);
};

void H_tree::test_Monitorng_for_3w(Data_instance t, int mounth, int x, double EM, double VarM,int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if(j==0)
				Now_node->on_line_sketch(EM, VarM, mounth * x);
				else
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
				Now_node->on_line_sketch(EM, VarM, mounth * x);
			else
			{
				Now_node->on_line_sketch(EM, VarM, x);
			}
		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			Now_node->T = 30;
			return;
		}

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_sketch(EM, VarM, mounth * x);
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->on_line_sketch(EM, VarM, x);
		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			Now_node->T = 30;
			return;
		}
	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(EM, VarM, x);
};
void H_tree::test_MonitorngDROP_pmf_for_TKDE(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	//Now_node->T = 2;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	
	//先时间维度,遍历时间维
	int year = 0;
	
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					this->Root.now_T++;
					Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
					//Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				//return;
				//如果是只测o则加return下同，否则注释return
				}
				else
				{
					Now_node->now_T = 2;
					Now_node->on_line_pmf(o, x);
				}



				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				Now_node->T = 360;
				Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				//return;
				//Now_node->T++;
			}
			else
			{
				Now_node->now_T = 2;
				Now_node->on_line_pmf(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
			}

		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}

	}
	//标准维度
	int size = t.Standard_Dimension.size();
	for (int ik = 0; ik < t.Standard_Dimension.size(); ik++)
	{
		//int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[ik];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_pmf(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
				break;
				if (ik == size - 1)
				{
					Now_node->IsLeaf = true;
				}
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->now_T = 24;
			Now_node->on_line_pmf(o, x);
			if (x > Now_node->now_T)Now_node->now_T = x;
			if (ik == size - 1)
			{
				Now_node->IsLeaf = true;
			}
		}
	}
	Now_node->IsLeaf = true;
	//Now_node->T = 30;//叶子结点全部设置为30
	//Now_node->on_line_pmf(o, x);
};

void H_tree::test_MonitorngTPCH_pmf_for_3w(Data_object o, int mounth, int x,int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	//int O_layer = 7 - length;
	/*if (length == 1)
	{
		for (int j = t.Standard_Dimension.size()-2; j < t.Standard_Dimension.size(); j++)
		{
			bool search = false;
			string d = t.Standard_Dimension[j];
			for (int i = 0; i < Now_node->Children_node_list.size(); i++)
			{
				//存在当前维度
				if (Now_node->Children_node_list[i].Now_Dimenson == d)
				{
					Now_node = &Now_node->Children_node_list[i];
					search = true;
					Now_node->on_line_pmf(o, x);
					break;

				}

			}
			if (search == false)
			{
				H_node New_node;
				New_node.Now_Dimenson = d;
				New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
				New_node.Dimension.push_back(d);
				Now_node->Children_node_list.push_back(New_node);

				Now_node = &Now_node->Children_node_list.back();
				Now_node->on_line_pmf(o, x);
			}
			now_length++;
			
		}
		Now_node->IsLeaf = true;
		Now_node->T = 30;//叶子结点全部设置为30
		Now_node->on_line_pmf(o, x);
		return;
	}
	else if (length == 3)
	{
		for (int j = t.Standard_Dimension.size() - 4; j < t.Standard_Dimension.size(); j++)
		{
			bool search = false;
			string d = t.Standard_Dimension[j];
			for (int i = 0; i < Now_node->Children_node_list.size(); i++)
			{
				//存在当前维度
				if (Now_node->Children_node_list[i].Now_Dimenson == d)
				{
					Now_node = &Now_node->Children_node_list[i];
					search = true;
					Now_node->on_line_pmf(o, x);
					break;

				}

			}
			if (search == false)
			{
				H_node New_node;
				New_node.Now_Dimenson = d;
				New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
				New_node.Dimension.push_back(d);
				Now_node->Children_node_list.push_back(New_node);

				Now_node = &Now_node->Children_node_list.back();
				Now_node->on_line_pmf(o, x);
			}
		}
		Now_node->IsLeaf = true;
		Now_node->T = 30;//叶子结点全部设置为30
		Now_node->on_line_pmf(o, x);
		return;
	}*/
	//先时间维度,遍历时间维
	int year = 0;
	for (int j = 0; j <t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					this->Root.now_T++;
					Now_node->on_line_pmf(o, (mounth - 1)*30 +x);
					//Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				//return;
				//如果是只测o则加return下同，否则注释return
				}
				else
				{
					Now_node->on_line_pmf(o,  x);
				}
				

				
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				Now_node->T = 360;
				Now_node->on_line_pmf(o, (mounth-1)* 30 + x);
				//return;
				//Now_node->T++;
			}
			else
			{
				//Now_node->now_T = 2;
				Now_node->on_line_pmf(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
			}

		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}

	}
	//标准维度
	//vector<int> l = { 0,3,2,5,4 };
	//vector<int> l = { 0,1,3,2,5,4 };
	vector<int> l = { 0,1,3,2,5,4 };
	for(int ik =0; ik < l.size();ik++)
	{
		int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_pmf(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
				break;
				if (ik == l.size() - 1)
				{
					Now_node->IsLeaf = true;
				}
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->now_T = 24;
			Now_node->on_line_pmf(o, x);
			if (x > Now_node->now_T)Now_node->now_T = x;
			if (ik == l.size() - 1)
			{
				Now_node->IsLeaf = true;
			}
		}
	}
	Now_node->IsLeaf = true;
	//Now_node->T = 30;//叶子结点全部设置为30
	//Now_node->on_line_pmf(o, x);
};
void H_tree::test_MonitorngClimatefor3W(Data_instance t, int mounth, int x, double EM, double VarM, int popular_length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int Now_length = 0;
	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					Now_node->on_line_sketch(EM, VarM, mounth * x);
				
				}	
				else
				{
					Now_node->on_line_sketch(EM, VarM, x);
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				Now_node->on_line_sketch(EM, VarM, mounth * x);
				
			}
			else
			{
				Now_node->on_line_sketch(EM, VarM, x);
			}
			break;
		}
		Now_length++;
		if (Now_length > popular_length)
			return;
	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_sketch(EM, VarM, mounth * x);
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->on_line_sketch(EM, VarM, mounth * x);
			break;
		}
		Now_length++;
		if (Now_length > popular_length)
			return;
	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(EM, VarM, x);
	Now_length++;
	if (Now_length > popular_length)
		return;
	//instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	//Now_node->add_pmf(pmf);
};

void H_tree::test_MonitorngClimatefor3Wpmf(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int Now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
					Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				else
				{
					Now_node->on_line_pmf(o, x);
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				Now_node->now_T = 360;
			}
				
		}
		Now_length++;
		if (Now_length > length)
			return;
	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_pmf(o, x);
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->on_line_pmf(o, x);
			//break;
		}
		Now_length++;
		if (Now_length > length)
			return;
	}
	Now_node->IsLeaf = true;
	Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_pmf(o, x);
	Now_length++;
	/*if (Now_length > length)
		return;*/
	//instance_pmf pmf(t.Data_object_id, t.Data_instance_x, t.Data_instance_Measure, t.Data_instance_prob);
	//Now_node->add_pmf(pmf);
};
void H_tree::test_Online_MA_TPCH_pmf_only_at_o_and_m(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	
	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 1)
				{
					
					Now_node->on_line_pmf(o,x);//一会别忘了改回来
					Now_node->isOlayer = true;
					if (x > Now_node->now_T)Now_node->now_T = x;
					
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 1)
			{
				Now_node->now_T = 2;
				Now_node->on_line_pmf(o, x);
				Now_node->isOlayer = true;
				if (x > Now_node->now_T)Now_node->now_T = x;
			}
			break;
		}
		/*now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}*/

	}
	//标准维度
	vector<int> l = { 0,1,3,2,5,4 };
	for (int ik = 0;ik < l.size(); ik++)
	{
		int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[j];
		bool isLeaf_now = false;
		if (ik + 1 == l.size())isLeaf_now = true;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (isLeaf_now) {
					Now_node->on_line_pmf(o, x);
					Now_node->isMaterialize = true;
					Now_node->IsLeaf = true;
					Now_node->isOnlineMazation = true;
				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (isLeaf_now) {
				Now_node->on_line_pmf(o, x);
				Now_node->isMaterialize = true;
				Now_node->IsLeaf = true;
				Now_node->isOnlineMazation = true;
			}
		}
	}
	
};
void H_tree::test_batch_materialization_from_m_to_o() {
	this->Root.Materialize_node_from_m_to_o();
	this->isMateralize = true;
}

void H_tree::test_Online_MA_Climate_pmf_only_at_o_and_m(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	//int O_layer = 7 - length;
	/*if (length == 1)
	{
		for (int j = t.Standard_Dimension.size()-2; j < t.Standard_Dimension.size(); j++)
		{
			bool search = false;
			string d = t.Standard_Dimension[j];
			for (int i = 0; i < Now_node->Children_node_list.size(); i++)
			{
				//存在当前维度
				if (Now_node->Children_node_list[i].Now_Dimenson == d)
				{
					Now_node = &Now_node->Children_node_list[i];
					search = true;
					Now_node->on_line_pmf(o, x);
					break;

				}

			}
			if (search == false)
			{
				H_node New_node;
				New_node.Now_Dimenson = d;
				New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
				New_node.Dimension.push_back(d);
				Now_node->Children_node_list.push_back(New_node);

				Now_node = &Now_node->Children_node_list.back();
				Now_node->on_line_pmf(o, x);
			}
			now_length++;

		}
		Now_node->IsLeaf = true;
		Now_node->T = 30;//叶子结点全部设置为30
		Now_node->on_line_pmf(o, x);
		return;
	}
	else if (length == 3)
	{
		for (int j = t.Standard_Dimension.size() - 4; j < t.Standard_Dimension.size(); j++)
		{
			bool search = false;
			string d = t.Standard_Dimension[j];
			for (int i = 0; i < Now_node->Children_node_list.size(); i++)
			{
				//存在当前维度
				if (Now_node->Children_node_list[i].Now_Dimenson == d)
				{
					Now_node = &Now_node->Children_node_list[i];
					search = true;
					Now_node->on_line_pmf(o, x);
					break;

				}

			}
			if (search == false)
			{
				H_node New_node;
				New_node.Now_Dimenson = d;
				New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
				New_node.Dimension.push_back(d);
				Now_node->Children_node_list.push_back(New_node);

				Now_node = &Now_node->Children_node_list.back();
				Now_node->on_line_pmf(o, x);
			}
		}
		Now_node->IsLeaf = true;
		Now_node->T = 30;//叶子结点全部设置为30
		Now_node->on_line_pmf(o, x);
		return;
	}*/
	//先时间维度,遍历时间维
	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					
					Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
					Now_node->isOlayer = true;
					//return;
					//如果是只测o则加return下同，否则注释return
				}
				//else
				{
					//Now_node->on_line_pmf(o, x);
				}



				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				
				Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				Now_node->isOlayer = true;
				//return;
			}
			//else
			{
				//Now_node->on_line_pmf(o, x);
			}

		}
		/*now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}*/

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		bool isLeaf_now = false;
		if (j == t.Standard_Dimension.size() -1)isLeaf_now = true;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (isLeaf_now) {
					Now_node->on_line_pmf(o, x);
					Now_node->isMaterialize = true;
					Now_node->IsLeaf = true;
					Now_node->isOnlineMazation = true;
				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (isLeaf_now) {
				Now_node->on_line_pmf(o, x);
				Now_node->isMaterialize = true;
				Now_node->IsLeaf = true;
				Now_node->isOnlineMazation = true;
			}
		}
	}

};
void H_tree::test_regression_in_a_all_cell(Data_object o, int mounth, int x) {
	//this->Root.now_T = 180;
	this->Root.on_line_pmf(o,x);
	//this->Root.on_line_sketch(o, x);
}

void H_tree::test_TPCH_Online_ma_Sketch(Data_object o, int mounth, int x) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	//先时间维度,遍历时间维
	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					this->Root.now_T++;
					//Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				//return;
				//如果是只测o则加return下同，否则注释return
				}
				else
				{
					Now_node->on_line_sketch(o, x);
				}



				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				//Now_node->T = 360;
				//Now_node->on_line_pmf(o, (mounth-1)* 30 + x);
				//return;
				Now_node->T++;
			}
			else
			{
				Now_node->on_line_sketch(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
			}

		}

	}
	//标准维度
	//vector<int> l = { 0,3,2,5,4 };
	//vector<int> l = { 0,1,3,2,5,4 };
	vector<int> l = { 0,1,3,2,5,4 };
	for (int ik = 0; ik < l.size(); ik++)
	{
		int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_sketch(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
				break;
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			Now_node->on_line_sketch(o, x);
			if (x > Now_node->now_T)Now_node->now_T = x;
		}
	}
	Now_node->IsLeaf = true;
	//Now_node->T = 30;//叶子结点全部设置为30
	//Now_node->on_line_pmf(o, x);
};

void H_tree::test_Online_MA_TPCH_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];

	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 1)
				{

					Now_node->on_line_sketch(o, x);
					Now_node->isOlayer = true;
					if (x > Now_node->now_T)Now_node->now_T = x;

				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 1)
			{
				//Now_node->now_T = 2;
				Now_node->on_line_pmf(o, x);
				Now_node->isOlayer = true;
				if (x > Now_node->now_T)Now_node->now_T = x;
			}
			break;
		}
	}
	//标准维度
	vector<int> l = { 0,1,3,2,5,4 };
	for (int ik = 0; ik < l.size(); ik++)
	{
		int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[j];
		bool isLeaf_now = false;
		if (ik + 1 == l.size())isLeaf_now = true;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (isLeaf_now) {
					Now_node->on_line_sketch(o, x);
					Now_node->isMaterialize = true;
					Now_node->IsLeaf = true;
					Now_node->isOnlineMazation = true;
				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (isLeaf_now) {
				Now_node->on_line_sketch(o, x);
				Now_node->isMaterialize = true;
				Now_node->IsLeaf = true;
				Now_node->isOnlineMazation = true;
			}
		}
	}

};

void H_tree::test_batch_materialization_from_m_to_o_Sketch() {
	this->Root.Materialize_node_from_m_to_o_Sketch();
	this->isMateralize = true;
}

void H_tree::test_Climate_Online_ma_Sketch(Data_object o, int mounth, int x) {
	H_node* Now_node;
	Now_node = &this->Root;
	int Now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	//先时间维度,遍历时间维
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
					Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
				else
				{
					Now_node->on_line_sketch(o, x);
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();

			if (j == 0)
				Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
			else
			{
				Now_node->on_line_sketch(o, x);
			}
		}
		
	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_sketch(o, x);
				if (j == t.Standard_Dimension.size() - 1)Now_node->IsLeaf = true;
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			if (j == t.Standard_Dimension.size() - 1)Now_node->IsLeaf = true;
			Now_node = &Now_node->Children_node_list.back();
			Now_node->on_line_sketch(o, x);
			//break;
		}
		Now_length++;

	}
};

void H_tree::test_Online_MA_Climate_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{

					Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
					Now_node->isOlayer = true;
					//return;
					//如果是只测o则加return下同，否则注释return
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{

				Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
				Now_node->isOlayer = true;
				//return;
			}

		}

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		bool isLeaf_now = false;
		if (j == t.Standard_Dimension.size() - 1)isLeaf_now = true;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (isLeaf_now) {
					Now_node->on_line_sketch(o, x);
					Now_node->isMaterialize = true;
					Now_node->IsLeaf = true;
					Now_node->isOnlineMazation = true;
				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (isLeaf_now) {
				Now_node->on_line_sketch(o, x);
				Now_node->isMaterialize = true;
				Now_node->IsLeaf = true;
				Now_node->isOnlineMazation = true;
			}
		}
	}

};

void H_tree::test_Regression_regression_DROP_TKDE(Data_object o, int mounth, int x, int length = 3) {
	H_node* Now_node;
	Now_node = &this->Root;
	//Now_node->T = 2;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];

	//先时间维度,遍历时间维
	int year = 0;

	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();

		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}

	}
	//标准维度
	int size = t.Standard_Dimension.size();
	for (int ik = 0; ik < t.Standard_Dimension.size(); ik++)
	{
		//int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[ik];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_pmf(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
				break;
				if (ik == size - 1)
				{
					Now_node->IsLeaf = true;
				}
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->now_T = 24;
			Now_node->on_line_pmf(o, x);
			if (x > Now_node->now_T)Now_node->now_T = x;
			if (ik == size - 1)
			{
				Now_node->IsLeaf = true;
			}
		}
	}
};

void H_tree::test_MonitorngDROP_sketch_for_TKDE(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	//Now_node->T = 2;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];

	//先时间维度,遍历时间维
	int year = 0;

	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{
					this->Root.now_T++;
					Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
					//Now_node->on_line_pmf(o, (mounth - 1) * 30 + x);
				//return;
				//如果是只测o则加return下同，否则注释return
				}
				else
				{
					Now_node->now_T = 2;
					Now_node->on_line_sketch(o, x);
				}



				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{
				Now_node->T = 360;
				Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
				//return;
				//Now_node->T++;
			}
			else
			{
				Now_node->now_T = 2;
				Now_node->on_line_sketch(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
			}

		}
		now_length++;
		if (now_length > length)
		{
			Now_node->IsLeaf = true;
			//Now_node->T = 30;
			return;
		}

	}
	//标准维度
	int size = t.Standard_Dimension.size();
	for (int ik = 0; ik < t.Standard_Dimension.size(); ik++)
	{
		//int j = l[ik];
		bool search = false;
		string d = t.Standard_Dimension[ik];
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				Now_node->on_line_sketch(o, x);
				if (x > Now_node->now_T)Now_node->now_T = x;
				break;
				if (ik == size - 1)
				{
					Now_node->IsLeaf = true;
				}
			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			Now_node->now_T = 24;
			Now_node->on_line_sketch(o, x);
			if (x > Now_node->now_T)Now_node->now_T = x;
			if (ik == size - 1)
			{
				Now_node->IsLeaf = true;
			}
		}
	}
	Now_node->IsLeaf = true;
	//Now_node->T = 30;//叶子结点全部设置为30
	Now_node->on_line_sketch(o, x);
};


void H_tree::test_Online_MA_DROP_sketch_only_at_o_and_m(Data_object o, int mounth, int x, int length) {
	H_node* Now_node;
	Now_node = &this->Root;
	int now_length = 0;
	Data_instance t = o.Data_instance_list[0];
	int year = 0;
	for (int j = 0; j < t.Time_Dimension.size(); j++)
	{
		string& d = t.Time_Dimension[j];
		bool search = false;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (j == 0)
				{

					Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
					Now_node->isOlayer = true;
					//return;
					//如果是只测o则加return下同，否则注释return
				}
				break;
			}

		}
		if (search == false)
		{
			//新加一个cell
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.IsTime = true;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);
			Now_node = &Now_node->Children_node_list.back();
			if (j == 0)
			{

				Now_node->on_line_sketch(o, (mounth - 1) * 30 + x);
				Now_node->isOlayer = true;
				//return;
			}

		}

	}
	//标准维度
	for (int j = 0; j < t.Standard_Dimension.size(); j++)
	{
		bool search = false;
		string d = t.Standard_Dimension[j];
		bool isLeaf_now = false;
		if (j == t.Standard_Dimension.size() - 1)isLeaf_now = true;
		for (int i = 0; i < Now_node->Children_node_list.size(); i++)
		{
			//存在当前维度
			if (Now_node->Children_node_list[i].Now_Dimenson == d)
			{
				Now_node = &Now_node->Children_node_list[i];
				search = true;
				if (isLeaf_now) {
					Now_node->on_line_sketch(o, x);
					Now_node->isMaterialize = true;
					Now_node->IsLeaf = true;
					Now_node->isOnlineMazation = true;
				}
				break;

			}

		}
		if (search == false)
		{
			H_node New_node;
			New_node.Now_Dimenson = d;
			New_node.Dimension.assign(Now_node->Dimension.begin(), Now_node->Dimension.end());
			New_node.Dimension.push_back(d);
			Now_node->Children_node_list.push_back(New_node);

			Now_node = &Now_node->Children_node_list.back();
			if (isLeaf_now) {
				Now_node->on_line_sketch(o, x);
				Now_node->isMaterialize = true;
				Now_node->IsLeaf = true;
				Now_node->isOnlineMazation = true;
			}
		}
	}

};
