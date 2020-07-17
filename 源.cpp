#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<iomanip>
#include<fstream>
const char* PATH = "D:\\gra.txt";

using namespace std;
const int MAX = 100;


string Grammar[MAX];		//�ķ�����ʽ
int Gra_count=0;			//�ķ��ľ���

vector<char> VN;				//���ս��
vector<char> VT;				//�ս��

set<char> First[MAX];
set<char> Follow[MAX];

int AnalysisList[MAX][MAX] ;		//������	,������Ϊ���ս���±꣬������Ϊ�ս���±ֵ꣬Ϊ�ڼ�������ʽ






bool judgeRepeatVN(char c) {			//�ж��ڱ����Ƿ����ظ��ķ��ս��
	if (VN.size() == 0)
		return false;
	for (int i = 0; i < VN.size(); i++) {
		if (c == VN[i])
			return true;
	}
	return false;
}

void getVN(string* Gra) {		//��÷��ս��
	for (int i = 0; i < Gra_count; i++) {
		if (!judgeRepeatVN(Gra[i][0])) {	//���ظ�
			VN.push_back(Gra[i][0]);
		}
	}
	cout << "���ս��Ϊ��" << endl;
	for (int i = 0; i < VN.size(); i++)
		cout << VN[i] << " ";
	cout << endl;
}
	
bool judgeVT(char c) {			//�ж�c�Ƿ�Ϊ�ս��
	for (int i = 0; i < VN.size(); i++) {
		if (c == VN[i])
			return false;
	}
	return true;
}
bool judgeRepeatVT(char c) {		//�ж��ڱ����Ƿ����ظ����ս��
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
			if (judgeVT(Gra[i][j])) {			//���ս���������ظ��������ս����
				if (!judgeRepeatVT(Gra[i][j])) {
					VT.push_back(Gra[i][j]);
				}
			}
		}
	}
	cout << "�ս��Ϊ��" << endl;
	for (int i = 0; i < VT.size(); i++)
		cout << VT[i] << " ";
	cout << endl;
}



int getVTIndex(char x) {		//�����ս���ڼ����е��±�
	for (int i = 0; i < VT.size(); i++) {
		if (VT[i] == x)
			return i;
	}
	return -1;
}
int getVNIndex(char x) {		//���ط��ս���ڼ����е��±�
	for (int i = 0; i < VN.size(); i++) {
		if (VN[i] == x)
			return i;
	}
	return -1;
}






void getFirst(char x) {
	bool flag = 0;     //�жϷ��ս�����Ƿ��пմ�
	int tot = 0;		//��¼���ս������ʽ���пմ��Ĳ���ʽ����

	for (int i = 0; i < Gra_count; i++) {
		if (Grammar[i][0] == x) {
			if (judgeVT(Grammar[i][3])) {		//����ʽ��ʼΪ�ս��
				First[getVNIndex(x)].insert(Grammar[i][3]);
			}
			else {			//���ս��
				//�����ұ����Ҳ�����ʽ
				for (int j = 3; j < Grammar[i].length(); j++) {
					//�����ս���ͽ���
					if (judgeVT(Grammar[i][j])) {
						First[getVNIndex(x)].insert(Grammar[i][j]);
						break;
					}
					//�����ս������÷��ս����First����ʹ���˵ݹ�
					getFirst(Grammar[i][j]);
					set<char>::iterator it;
					int ind = getVNIndex(Grammar[i][j]);
					for (it = First[ind].begin(); it != First[ind].end(); it++) {
						if (*it == '$')
							flag = 1;
						else
							First[getVNIndex(x)].insert(*it);
					}
					//�޿մ�����Ѱ��
					if (flag == 0)
						break;
					else {
						flag = 0;
						tot++;
					}
				}
				//���ȫ�����пմ���ɣ���մ�ҲҪ�ӵ���ʼ�ķ��ս����First����
				if (tot == (Grammar[i].length() - 3))
					First[getVNIndex(x)].insert('$');
			}
		}

	}
}

void getFollow(char x) {
	//�ҵ����ս��x���ֵ�λ��
	for (int i = 0; i < Gra_count; i++) {
		int index = -1;
		for (int j = 3; j < Grammar[i].length(); j++) {
			if (Grammar[i][j] == x) {
				index = j;
				break;
			}
		}
		//����ҵ���x���Ҳ������һ���ַ�
		if (index != -1 && index < Grammar[i].length() - 1) {
			char next = Grammar[i][index + 1];
			//��һ���ַ�Ϊ�ս������ӽ�x��follow��
			if (judgeVT(next)) {
				Follow[getVNIndex(x)].insert(next);
			}
			//Ϊ���ս��
			else {
				bool flag = 0;
				//������һ���ַ���first��
				for (set<char>::iterator it = First[getVNIndex(next)].begin(); it != First[getVNIndex(next)].end(); it++) {
					//�пմ������
					if (*it == '$')
						flag = 1;
					else
						Follow[getVNIndex(x)].insert(*it);
				}
				//����пմ�����߲�Ϊ��������ֹ��ѭ��������ͷ����follow����x��follow��
				char temp = Grammar[i][0];
				if (flag && temp != x) {
					getFollow(temp);
					for (set<char>::iterator it = Follow[getVNIndex(temp)].begin(); it != Follow[getVNIndex(temp)].end(); it++) 
						Follow[getVNIndex(x)].insert(*it);
				}
			}
		}
		//�ڲ���ʽ��β���������ʽ�󲿵�Follow��Ӧ�ü��뵽x��Follow����
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
		//����ұߵ�һ������Ϊ�ս��
		if (judgeVT(temp)) {
			//��Ϊ�մ���ֱ�Ӹ��·�����
			if (temp != '$') {
				AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(temp)] = i;
			}
			//Ϊ�մ�����follow�����룬���·�����
			else {
				for (set<char>::iterator it = Follow[getVNIndex(Grammar[i][0])].begin(); it != Follow[getVNIndex(Grammar[i][0])].end(); it++) {
					//if (*it == '#') {
					//	//���һ��+1������Ϊ#��
					//	AnalysisList[getVNIndex(Grammar[i][0])][VT.size()] = i;
					//	continue;
					//}
					AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
				}
			}
		}
		//�ұߵ�һ��Ϊ���ս��
		else {
			for (set<char>::iterator it = First[getVNIndex(temp)].begin(); it != First[getVNIndex(temp)].end(); it++) {
				AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
			}
			//first�����пմ�����follow������
			if (First[getVNIndex(temp)].count('$') != 0) {
				for (set<char>::iterator it = Follow[getVNIndex(Grammar[i][0])].begin(); it != Follow[getVNIndex(Grammar[i][0])].end(); it++) {
					AnalysisList[getVNIndex(Grammar[i][0])][getVTIndex(*it)] = i;
				}
			}
		}
	}
}

void Analysis(string s) {
	vector<char> left;	//ʣ�����봮
	vector<char> stack;//����ջ

	cout << std::left << setw(8)<<"����" << std::left << setw(15) << "����ջ" << std::left << setw(15) << "ʣ�����봮" << std::left << setw(20) << "���ò���ʽ" << endl;
	//��������push��ʣ�����left
	
	for (int i =0; i < s.length() ; i++) {
		left.push_back(s[i]);
	}
	left.push_back('#');
	//��#�Ϳ�ʼ��push������ջ
	stack.push_back('#');
	stack.push_back(Grammar[0][0]);
	//���ʣ�����봮���Ȳ�Ϊ0����һֱѭ��
	int step = 0;
	while (left.size() > 0) {
		//�������
		cout << std::left <<setw(8) << step++;
		//�������ջ����
		string outputs = "";
		for (int i = 0; i < stack.size(); i++) {
			outputs += stack[i];
		}
		cout << std::left << setw(15) << outputs;
		outputs = "";
		//���ʣ�����봮����
		for (int i = 0; i < left.size(); i++) {
			outputs += left[i];
		}
		cout << std::left << setw(15) << outputs;
		char f1 = stack[stack.size() - 1];
		char f2 = left[0];
		//�������ƥ�䣬���Ҷ�Ϊ# 
		if (f1 == f2 && f1 == '#') {
			cout << std::left << setw(21) << "Accept!" << endl;
			return;
		}
		//�������ƥ�䣬���Ҷ�Ϊ�ս�� 
		if (f1 == f2) {
			stack.pop_back();
			left.erase(left.begin());
			cout << std::left << setw(15) << f1 << " ƥ��" << endl;
		}
		else if (AnalysisList[getVNIndex(f1)][getVTIndex(f2)] != -1&&f1!='#') {
			//�����Ԥ�����������ֵ
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
	//cout << "�������ķ�������S->T,#�Ž�����:" << endl;
	//string t;
	//cin >> t;
	//for (int i = 0; t != "#"; ) {
	//	Grammar[i++] = t;
	//	Gra_count++;
	//	cin >> t;
	//}
	ifstream file;
	file.open(PATH, ios::in);						//��Ҫ������ļ�
	if (!file) {									//û�д򿪸��ļ�
		cout << "�򿪴���" << endl;
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
	cout << "First��Ϊ��" << endl;
	for (int i = 0; i < VN.size(); i++) {
		cout << VN[i] << "  ";
		for (set<char>::iterator it=First[i].begin();it!=First[i].end();it++)
			cout << *it << ",";
		cout << endl;
	}
	cout << endl;


	//follow����ʼ���Ų���#
	Follow[getVNIndex(Grammar[0][0])].insert('#');
	for (int j = 0; j < Gra_count; j++) {
		getFollow(Grammar[j][0]);
	}
	cout << "Follow��Ϊ��" << endl;
	for (int i = 0; i < VN.size(); i++) {
		cout << VN[i] << "  ";
		for (set<char>::iterator it = Follow[i].begin(); it != Follow[i].end(); it++)
			cout << *it << ",";
		cout << endl;
	}



	memset(AnalysisList, -1, sizeof(AnalysisList));		//���
	cout  << endl;
	getAnalysisList();



	cout << "������Ϊ��" << endl;
	cout << " ";
	for (int j = 0; j < VT.size(); j++) {
		//�ų��մ�
		if (VT[j] == '$')
			continue;
		cout << "\t" << VT[j];
	}
	cout << endl;

	for (int i = 0; i < VN.size(); i++) {
		cout  << VN[i];
		for (int j = 0; j < VT.size(); j++) {
			//�ų��մ�
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
	cout << "������������Ĵ���";

	string s; cin >> s;
	cout << endl;
	Analysis(s);

	return 0;
}
