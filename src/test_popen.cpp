#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

vector<string> split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

int main() {
    // string result = exec();
    // cout << "fini : " << result << endl;

    FILE *in;
    char buff[512];

    ostringstream output;

    if(!(in = popen("echo \"/\\n\" | sqlplus64 VINCMONO/VINCMONO17@\"(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.40.128.30)(PORT=1521))(CONNECT_DATA=(SID=emrepus)))\" @/home/agurato/dev/m2/indexation/src/comparison.sql", "r"))){
        return 1;
    }
    while(fgets(buff, sizeof(buff), in)!=NULL){
        output << buff;
    }
    pclose(in);

    string outputS = output.str();

    vector<string> arr;
    arr=split(outputS, "\n");
    for(size_t i=0;i<arr.size();i++) {
        if(arr[i].find("img=") != string::npos) {
            cout << arr[i].substr(9) << endl;
        }
        else if(arr[i].find("diff=") != string::npos) {
            cout << arr[i].substr(5) << endl;
        }
        else if(arr[i].find("time=") != string::npos) {
            cout << arr[i].substr(5) << endl;
        }
    }


    return 0;
}
