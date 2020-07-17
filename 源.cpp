#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<iomanip>
#include<fstream>
const char* PATH = "D:\\gra.txt";

using namespace std;
const int MAX = 100;


string Grammar[MAX];		//文法产生式
int Gra_count=0;			//文法的句数

vector<char> VN;				//非终结符
vector<char> VT;				//终结符

set<char> First[MAX];
set<char> Follow[MAX];

int AnalysisList[MAX][MAX] ;		//分析表	,横坐标为非终结符下标，纵坐标为终结符下标，值为第几个产生式






bool judgeRepeatVN(char c) {			//判断在表中是否有重复的非终结符
	if (VN.size() == 0)
		return false;
	for (int i = 0; i < VN.size(); i++) {
		if (c == VN[i])
			return true;
	}
	return false;
}

void getVN(string* Gra) {		//获得非终结符
	for (int i = 0; i < Gra_count; i++) {
		if (!judgeRepeatVN(Gra[i][0])) {	//无重复
			VN.push_back(Gra[i][0]);
		}
	}
	cout << "非终结符为：" << endl;
	for (int i = 0; i < VN.size(); i++)
		cout << VN[i] << " ";
	cout << endl;
}
	
bool judgeVT(char c) {			//判断c是否为终结符
	for (int i = 0; i < VN.size(); i++) {
		if (c == VN[i])
			return false;
	}
	return true;
}
bool judgeRepeatVT(char c) {		//判断在表中是否有重复的终结符
	if (VT.size() == 0)
		return false;
	for (int i = 0; i < VT.size(); i++) {
		if (c == VT[i])
			return true;
	}
	return false;
}

void getVT(string* Gra) {
	for (int i = 0; i < Gra_count; i++) {
		for (int j = 3; j < Gra[i].length(); j++) {
			if (judgeVT(Gra[i][j])) {			//是终结符，且无重复，加入终结符表
				if (!judgeRepeatVT(Gra[i][j])) {
					VT.push_back(Gra[i][j]);
				}
			}
		}
	}
	cout << "终结符为：" << endl;
	for (int i = 0; i < VT.size(); i++)
		cout << VT[i] << " ";
	cout << endl;
}



int getVTIndex(char x) {		//返回终结符在集合中的下标
	for (int i = 0; i < VT.size(); i++) {
		if (VT[i] == x)
			return i;
	}
	return -1;
}
int getVNIndex(char x) {		//返回非终结符在集合中的下标
	for (int i = 0; i < VN.size(); i++) {
		if (VN[i] == x)
			return i;
	}
	return -1;
}






void getFirst(char x) {
	bool flag = 0;     //判断非终结符中是否有空串
	int tot = 0;		//记录非终结符产生式含有空串的产生式个数

	for (int i = 0; i < Gra_count; i++) {
		if (Grammar[i][0] == x) {
			if (judgeVT(Grammar[i][3])) {		//产生式开始为终结符
				First[getVNIndex(x)].insert(Grammar[i][3]);
			}
			else {			//非终结符
				//从左到右遍历右部产生式
				for (int j = 3; j < Grammar[i].length(); j++) {
					//遇到终结符就结束
					if (judgeVT(Grammar[i][j])) {
						First[getVNIndex(x)].insert(Grammar[i][j]);
						break;
					}
					//不是终结符，求该非终结符的First集，使用了递归
					getFirst(Grammar[i][j]);
					set<char>::iterator it;
					int ind = getVNIndex(Grammar[i][j]);
					for (it = First[ind].begin(); it != First[ind].end(); it++) {
						if (*it == '$')
							flag = 1;
						else
							First[getVNIndex(x)].insert(*it);
					}
					//无空串不必寻找
					if (flag == 0)
						break;
					else {
						flag = 0;
						tot++;
					}
				}
				//如果全部都有空串组成，则空串也要加到开始的非终结符的First集中
				if (tot == (Grammar[i].length() - 3))
					First[getVNIndex(x)].insert('$');
			}
		}

	}
}

void getFollow(char x) {
	//找到非终结符x出现的位置
	for (int i = 0; i < Gra_count; i++) {
		int index = -1;
		for (int j = 3; j < Grammar[i].length(); j++) {
			if (Grammar[i][j] == x) {
				index = j;
				break;
			}
		}
		//如果找到了x并且不是最后一个字符
		if (index != -1 && index < Grammar[i].length() - 1) {
			char next = Grammar[i][index + 1];
			//下一个字符为终结符，添加进x的follow集
			if (judgeVT(next)) {
				Follow[getVNIndex(x)].insert(next);
			}
			//为非终结符
			else {
				bool flag = 0;
				//遍历下一个字符的first集
				for (set<char>::iterator it = First[getVNIndex(next)].begin(); it != First[getVNIndex(next)].end(); it++) {
					//有空串做标记
					if (*it == '$')
						flag = 1;
					else
						Follow[getVNIndex(x)].insert(*it);
				}
				//如果有空串且左边不为它本身（防止死循环），则头部的follow集是x的follow集
				char temp = Grammar[i][0];
				if (flag && temp != x) {
					getFollow(temp);
					for (set<char>::iterator it = Follow[getVNIndex(temp)].begin(); it != Follow[getVNIndex(temp)].end(); it++) 
						Follow[getVNIndex(x)].insert(*it);
				}
			}
		}
		//在产生式的尾部，则产生式左部的Follow集应该加入到x的Follow集中
		else if (index != -1 && index == Grammar[i].length() - 1 && x != Grammar[i][0]) {
			char temp = Grammar[i][0];
			getFollow(temp);
			for (set<char>::iterator it = Follow[getVNIndex(temp)].begin(); it != Follow[getVNIndex(temp)].end(); it++) {
				Follow[getVNIndex(x)].insert(*it);
			}
		}
	}
}








void getAnalysisList() {
	for (int i = 0; i < Gra_count; i++) {
		char temp = Grammar[i][3];
		//如果右边第一个符号为终结符
		if (judgeVT(temp)) {
			//不为空串，直接更新分析表
			if (temp != '$') {
				AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(temp)] = i;
			}
			//为空串，将follow集加入，更新分析表
			else {
				for (set<char>::iterator it = Follow[getVNIndex(Grammar[i][0])].begin(); it != Follow[getVNIndex(Grammar[i][0])].end(); it++) {
					//if (*it == '#') {
					//	//最后一个+1的索引为#号
					//	AnalysisList[getVNIndex(Grammar[i][0])][VT.size()] = i;
					//	continue;
					//}
					AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
				}
			}
		}
		//右边第一个为非终结符
		else {
			for (set<char>::iterator it = First[getVNIndex(temp)].begin(); it != First[getVNIndex(temp)].end(); it++) {
				AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
			}
			//first集中有空串，把follow集加入
			if (First[getVNIndex(temp)].count('$') != 0) {
				for (set<char>::iterator it = Follow[getVNIndex(Grammar[i][0])].begin(); it != Follow[getVNIndex(Grammar[i][0])].end(); it++) {
					AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
				}
			}
		}
	}
}

void Analysis(string s) {
	vector<char> left;	//剩余输入串
	vector<char> stack;//符号栈

	cout << std::left << setw(8)<<"步骤" << std::left << setw(15) << "符号栈" << std::left << setw(15) << "剩余输入串" << std::left << setw(20) << "所用产生式" << endl;
	//把整个串push进剩余符号left
	
	for (int i =0; i < s.length() ; i++) {
		left.push_back(s[i]);
	}
	left.push_back('#');
	//把#和开始符push进符号栈
	stack.push_back('#');
	stack.push_back(Grammar[0][0]);
	//如果剩余输入串长度不为0，就一直循环
	int step = 0;
	while (left.size() > 0) {
		//输出步数
		cout << std::left <<setw(8) << step++;
		//输出分析栈内容
		string outputs = "";
		for (int i = 0; i < stack.size(); i++) {
			outputs += stack[i];
		}
		cout << std::left << setw(15) << outputs;
		outputs = "";
		//输出剩余输入串内容
		for (int i = 0; i < left.size(); i++) {
			outputs += left[i];
		}
		cout << std::left << setw(15) << outputs;
		char f1 = stack[stack.size() - 1];
		char f2 = left[0];
		//如果可以匹配，并且都为# 
		if (f1 == f2 && f1 == '#') {
			cout << std::left << setw(21) << "Accept!" << endl;
			return;
		}
		//如果可以匹配，并且都为终结符 
		if (f1 == f2) {
			stack.pop_back();
			left.erase(left.begin());
			cout << std::left << setw(15) << f1 << " 匹配" << endl;
		}
		else if (AnalysisList[getVNIndex(f1)][getVTIndex(f2)] != -1&&f1!='#') {
			//如果在预测分析表中有值
			int index = AnalysisList[getVNIndex(f1)][getVTIndex(f2)];
			stack.pop_back();
			if (Grammar[index][3] != '$') {
				for (int i = Grammar[index].length() - 1; i >= 3; i--) {
					stack.push_back(Grammar[index][i]);
				}
			}
			cout << std::left << setw(15) << Grammar[index] << endl;
		}
		else {
			cout << std::left << setw(15)<< "error" << endl;
			return;
		}
	}
}

int main() {
	//cout << "请输入文法（形如S->T,#号结束）:" << endl;
	//string t;
	//cin >> t;
	//for (int i = 0; t != "#"; ) {
	//	Grammar[i++] = t;
	//	Gra_count++;
	//	cin >> t;
	//}
	ifstream file;
	file.open(PATH, ios::in);						//打开要读入的文件
	if (!file) {									//没有打开该文件
		cout << "打开错误！" << endl;
		return 0;
	}
	string instring;
	while (getline(file, instring)) {
		Grammar[Gra_count++] = instring;
	}





	getVN(Grammar);
	getVT(Grammar);
	VT.push_back('#');

	for (int i = 0; i < Gra_count; i++) {
		getFirst(Grammar[i][0]);
	}
	cout << endl;
	cout << "First集为：" << endl;
	for (int i = 0; i < VN.size(); i++) {
		cout << VN[i] << "  ";
		for (set<char>::iterator it=First[i].begin();it!=First[i].end();it++)
			cout << *it << ",";
		cout << endl;
	}
	cout << endl;


	//follow集开始符号插入#
	Follow[getVNIndex(Grammar[0][0])].insert('#');
	for (int j = 0; j < Gra_count; j++) {
		getFollow(Grammar[j][0]);
	}
	cout << "Follow集为：" << endl;
	for (int i = 0; i < VN.size(); i++) {
		cout << VN[i] << "  ";
		for (set<char>::iterator it = Follow[i].begin(); it != Follow[i].end(); it++)
			cout << *it << ",";
		cout << endl;
	}



	memset(AnalysisList, -1, sizeof(AnalysisList));		//清空
	cout  << endl;
	getAnalysisList();



	cout << "分析表为：" << endl;
	cout << " ";
	for (int j = 0; j < VT.size(); j++) {
		//排除空串
		if (VT[j] == '$')
			continue;
		cout << "\t" << VT[j];
	}
	cout << endl;

	for (int i = 0; i < VN.size(); i++) {
		cout  << VN[i];
		for (int j = 0; j < VT.size(); j++) {
			//排除空串
			if (VT[j] == '$')
				continue;
			if (AnalysisList[i][j] == -1) {
				cout << '\t';
			}
			else {
				cout<<'\t'<<Grammar[AnalysisList[i][j]];
			}
		}
		cout << endl;
	}

	cout << endl;
	cout << "请输入待分析的串：";

	string s; cin >> s;
	cout << endl;
	Analysis(s);

	return 0;
}
