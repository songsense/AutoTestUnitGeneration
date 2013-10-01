//
//  main.cpp
//  AutoTestGeneration
//
//  Created by 宋思明 on 10/1/13.
//  Copyright (c) 2013 Siming Song. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int insert2File(const string &filename, const string &insertingContent,
                 const string &afterkeyword) {
  fstream finout;
  string token;
  finout.open(filename,std::fstream::in | std::fstream::out);
  if (!finout.is_open())
    return 0;
  while (getline(finout, token)) {
    if (token.find(afterkeyword) != string::npos) {
      streampos pos = finout.tellg();
      stringstream restContents;
      while (getline(finout, token))
        restContents<<token<<'\n';
      finout.clear();
      finout.seekg(pos);
      finout<<insertingContent<<restContents.str();
      finout.close();
      return 1;
    }
  }
  finout.close();
  return -1;
}

int main(int argc, const char * argv[])
{
  string filename("./config.txt");
  ifstream fin;
  fin.open(filename,std::ofstream::in);
  if (fin.is_open()) {
    stringstream ss;
    string solutionFileName, TestFileName, newFunctionName, outputParameter, comments,    SavedTestFileName;
    vector<string> inputParameters;
    
    const int lineLength(256);
    char line[lineLength];
    string token;
    // get the solution file name
    fin.getline(line, lineLength);
    ss<<line;
    ss>>solutionFileName>>solutionFileName;
    // get the test file name
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    ss>>TestFileName>>TestFileName;
    // get the new function name
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    ss>>newFunctionName>>newFunctionName;
    // get input parameters
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    ss>>token; // tmp string, no use
    while (getline(ss, token, ',')) {
      inputParameters.push_back(token);
    }
    // get the output parameter
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    while (getline(ss, outputParameter, ':'));
    // get the comments for the solution
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    while (getline(ss, comments, ':'));
    // get the saved test file name
    memset(line, '\0', lineLength);
    ss.clear();
    ss.str("");
    fin.getline(line, lineLength);
    ss<<line;
    ss>>SavedTestFileName>>SavedTestFileName;
    
    fin.close();
    
    // Now write codes in files
    // Solution.h
    stringstream keyStream;
    ss.clear(); ss.str("");
    int ret;
    ss<<"\t//"<<comments<<"\n\t"<<outputParameter<<" "<<newFunctionName<<"(";
    for (size_t i = 0; i < inputParameters.size()-1; ++i)
      ss<<inputParameters[i]<<", ";
    ss<<inputParameters[inputParameters.size() - 1];
    ss<<");\n";
    keyStream<<"public:";
    ret = insert2File(solutionFileName+".h", ss.str(), keyStream.str());
    if (ret == 1)
      cout<<solutionFileName<<".h "<<"has been added successfully"<<endl;
    else if (ret == 0)
      cout<<"cannot open "<<solutionFileName<<".h"<<endl;
    else
      cout<<"fail to find the keyword: public"<<endl;
    
    // Solution.cpp
    ss.clear(); ss.str("");
    keyStream.clear(); keyStream.str("");
    ss<<"//"<<comments<<"\n"<<outputParameter<<" "<<solutionFileName<<"::"<<newFunctionName<<"(";
    for (size_t i = 0; i < inputParameters.size()-1; ++i)
      ss<<inputParameters[i]<<", ";
    ss<<inputParameters[inputParameters.size() - 1];
    ss<<"){\n\n}";
    keyStream<<"#include \""<<solutionFileName<<".h\"";
    ret = insert2File(solutionFileName+".cpp", ss.str(), keyStream.str());
    if (ret == 1)
      cout<<solutionFileName<<".cpp "<<"has been added successfully"<<endl;
    else if (ret == 0)
      cout<<"cannot open "<<solutionFileName<<".cpp"<<endl;
    else
      cout<<"fail to find the keyword: "<<keyStream.str()<<endl;
    
    // Test for Solution header
    ss.clear();    ss.str("");
    keyStream.clear(); keyStream.str("");
    ss<<"\t void Test_"<<solutionFileName<<"();\n";
    keyStream<<"public:";
    ret = insert2File(TestFileName+".h", ss.str(), keyStream.str());
    if (ret == 1)
      cout<<TestFileName<<".h "<<"has been added successfully"<<endl;
    else if (ret == 0)
      cout<<"cannot open "<<TestFileName<<".h"<<endl;
    else
      cout<<"fail to find the keyword: "<<keyStream.str()<<endl;
    
    // Test for solution cpp
    ss.clear(); ss.str("");
    keyStream.clear(); keyStream.str("");
    ss<<"void "<<TestFileName<<"::Test_"<<outputParameter<<" "<<solutionFileName<<"::"<<newFunctionName<<"() {\n";
    ss<<"\tofstream fout;\n";
    ss<<"\tOpenLogFile(fout, \""<<comments<<".html\", \""<<comments<<"\");\n\n\n";
    ss<<"\tCloseLogFile(fout);\n}\n";
    keyStream<<"#include \""<<TestFileName<<".h\"";
    ret = insert2File(TestFileName+".cpp", ss.str(), keyStream.str());
    if (ret == 1)
      cout<<TestFileName<<".cpp "<<"has been added successfully"<<endl;
    else if (ret == 0)
      cout<<"cannot open "<<TestFileName<<".cpp"<<endl;
    else
      cout<<"fail to find the keyword: "<<keyStream.str()<<endl;
  }
  else
    cout<<"fail to open the config.txt file"<<endl;
  
  return 0;
}

