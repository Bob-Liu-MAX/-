#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// 学生结构体
struct Student {
    string name;
    bool isCalled;
};

// 数据文件名
const string DATA_FILE = "T-students.txt";

// 全局变量
vector<Student> students;
vector<string> calledList;

// 初始化随机数种子
void init() {
    srand(time(nullptr));
}

// 从文件加载学生列表
void loadFromFile() {
    ifstream fin(DATA_FILE.c_str());
    if (!fin) {
        cout << "未找到数据文件，将使用默认学生列表。" << endl;
        return;
    }

    students.clear();
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) {
            Student s;
            s.name = line;
            s.isCalled = false;
            students.push_back(s);
        }
    }
    fin.close();
    cout << "已从 " << DATA_FILE << " 加载 " << students.size() << " 名学生。" << endl;
}

// 保存学生列表到文件
void saveToFile() {
    ofstream fout(DATA_FILE.c_str());
    if (!fout) {
        cout << "无法打开文件写入！" << endl;
        return;
    }

    for (int i = 0; i < (int)students.size(); i++) {
        fout << students[i].name << endl;
    }
    fout.close();
    cout << "已保存 " << students.size() << " 名学生到 " << DATA_FILE << endl;
}

// 添加学生
void addStudent(const string& name) {
    Student s;
    s.name = name;
    s.isCalled = false;
    students.push_back(s);
}

// 删除学生
void removeStudent(const string& name) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->name == name) {
            students.erase(it);
            for (auto it2 = calledList.begin(); it2 != calledList.end(); ++it2) {
                if (*it2 == name) {
                    calledList.erase(it2);
                    break;
                }
            }
            break;
        }
    }
}

// 显示所有学生
void displayStudents() {
    cout << "\n===== 学生列表 =====" << endl;
    for (int i = 0; i < (int)students.size(); i++) {
        cout << i + 1 << ". " << students[i].name;
        if (students[i].isCalled) {
            cout << " [已点名]";
        }
        cout << endl;
    }
    cout << "===================" << endl;
}

// 显示已点名学生
void displayCalledStudents() {
    cout << "\n===== 已点名学生 =====" << endl;
    for (int i = 0; i < (int)calledList.size(); i++) {
        cout << "- " << calledList[i] << endl;
    }
    cout << "共 " << calledList.size() << " 人" << endl;
    cout << "=====================" << endl;
}

// 重置点名状态
void reset() {
    for (int i = 0; i < (int)students.size(); i++) {
        students[i].isCalled = false;
    }
    calledList.clear();
}

// 随机点名
string randomCall() {
    if (students.empty()) {
        return "";
    }

    int notCalledCount = 0;
    for (int i = 0; i < (int)students.size(); i++) {
        if (!students[i].isCalled) {
            notCalledCount++;
        }
    }

    if (notCalledCount == 0) {
        return "all_called";
    }

    vector<int> notCalledIndices;
    for (int i = 0; i < (int)students.size(); i++) {
        if (!students[i].isCalled) {
            notCalledIndices.push_back(i);
        }
    }

    int randomPos = rand() % notCalledIndices.size();
    int selectedIndex = notCalledIndices[randomPos];

    students[selectedIndex].isCalled = true;
    calledList.push_back(students[selectedIndex].name);

    return students[selectedIndex].name;
}

// 获取总人数
int getTotalCount() {
    return students.size();
}

// 获取已点名人数
int getCalledCount() {
    return calledList.size();
}

// 获取未点名人数
int getNotCalledCount() {
    return getTotalCount() - getCalledCount();
}

// 检查列表是否为空
bool isEmpty() {
    return students.empty();
}

// 显示菜单
void showMenu() {
    cout << "\n===== 点名系统 =====" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 删除学生" << endl;
    cout << "3. 显示学生列表" << endl;
    cout << "4. 开始点名" << endl;
    cout << "5. 查看已点名学生" << endl;
    cout << "6. 重置点名记录" << endl;
    cout << "7. 刷新学生列表(从文件重新加载)" << endl;
    cout << "8. 保存学生列表到文件" << endl;
    cout << "0. 退出" << endl;
    cout << "===================" << endl;
    cout << "请选择: ";
}

int main() {
    init();
    loadFromFile();

    // 如果文件为空，添加默认学生
    if (students.empty()) {
        addStudent("张三");
        addStudent("李四");
        addStudent("王五");
        addStudent("赵六");
        addStudent("钱七");
        saveToFile();
    }

    int choice;
    string name;
    string result;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "请输入学生姓名: ";
                getline(cin, name);
                if (!name.empty()) {
                    addStudent(name);
                    cout << "已添加: " << name << endl;
                }
                break;

            case 2:
                cout << "请输入要删除的学生姓名: ";
                getline(cin, name);
                removeStudent(name);
                cout << "已删除: " << name << endl;
                break;

            case 3:
                displayStudents();
                break;

            case 4:
                if (isEmpty()) {
                    cout << "学生列表为空，请先添加学生！" << endl;
                } else {
                    result = randomCall();
                    if (result == "all_called") {
                        cout << "所有学生都已点过名了！" << endl;
                        cout << "总计: " << getTotalCount() << " 人" << endl;
                    } else {
                        cout << "==========>" << result << "<==========" << endl;
                        cout << "剩余未点名: " << getNotCalledCount() << " 人" << endl;
                    }
                }
                break;

            case 5:
                displayCalledStudents();
                break;

            case 6:
                reset();
                cout << "已重置点名记录！" << endl;
                break;

            case 7:
                loadFromFile();
                reset();
                break;

            case 8:
                saveToFile();
                break;

            case 0:
                cout << "退出系统！" << endl;
                return 0;

            default:
                cout << "无效选择，请重新选择！" << endl;
        }
    }

    return 0;
}
