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
        //cout<<line;
        if(line.find("%%")!=-1){}
    }

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
    for(int i=0;i<reList.size();i++){
        pair<string,string> temp=reList[i];
        cout<<temp.first<<"\t"<<temp.second<<endl;
    }
}
