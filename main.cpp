#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
void readInput();
void generateProgram();
void printInclude(ofstream& output);
void printReList();
string findRE(string schema);

string declare="";
vector<pair<string,string> > reList;
vector<pair<string,string> > actionList;

int main()
{
    readInput();
    generateProgram();
    printReList();
    return 0;
}


void readInput()
{
    ifstream input("input.l");
    char c;
    bool isInDeclare=false;
    while(input>>c){
        if(isInDeclare){
            if(c=='%'){
                input>>c;
                break;
            }else{
                declare+=c;
            }
        }else{
            if(c=='%'){
                input>>c;
                isInDeclare=true;
            }
        }
    }

    //处理正则表达式
    string line="";
    while(getline(input,line)){
        if(line.size()==0){continue;}

        if(line.find("%%")==string::npos){
            stringstream ss(line);
            string id;
            if(ss>>id){
               string re="";
               char c;

               while(ss>>c){
                    if(c=='\t'||c==' '){continue;}
                    if(c=='\\'){
                        ss>>c;
                        switch (c){
                        case 't':
                            re+='\t';
                            break;
                        case 'n':
                            re+='\n';
                            break;
                        default:
                            re+='\\';
                            re+=c;
                        }
                    }else{
                        re+=c;
                    }
               }
                reList.push_back(make_pair(id,re));
            }else{
                continue;
            }
        }else{
           break;
        }
    }

    //处理动作
    line="";
    while(getline(input,line)){
        cout<<line<<endl;
        if(line.size()==0){continue;}
        if(line.find("%%")==string::npos){
            stringstream ss(line);
            string schema;
            string re;
            if(ss>>schema){
                 if(*(schema.begin())=='{'){
                    string temp=schema.substr(1,schema.size()-2);
                    re=findRE(temp);
                 }else if(*(schema.begin())=='\''){
                    string temp=schema.substr(1,schema.size()-2);
                    re=temp;
                 }else{
                     re=schema;
                 }

                string action="";
                char c;
                bool isInAction=false;
                while(ss>>c){
                    if(c=='\t'||c==' '){continue;}
                    if(c=='{'){isInAction=true;}
                    if(c=='}'){isInAction=false;}
                    if(isInAction){
                        action+=c;
                        actionList.push_back(make_pair(re,action));
                        break;
                    }
                }
            }
        }else{
            break;
        }
    }

}

string findRE(string schema){
    for(int i=0;i<reList.size();i++){
        pair<string,string> temp=reList[i];
        if(schema.compare(temp.first)==0){
            return temp.second;
        }
    }
    return "";
}

void generateProgram(){
    ofstream output("lex.yy.c");
    printInclude(output);
    output<<declare<<endl;
}

void printInclude(ofstream& output){
    output<<"include <iostream>"<<endl;
}

void printReList(){
    for(int i=0;i<actionList.size();i++){
        pair<string,string> temp=reList[i];
        cout<<temp.first<<"\t"<<temp.second<<endl;
    }
}
