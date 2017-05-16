#include <iostream>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string>  
#include <regex.h>  
#include <assert.h>  
#include <string.h>  
#include <list>
#include <vector>
using namespace std;  
  
int find_first(string input, string pattern, string &out){  
    regex_t reg;  
    regmatch_t pm[1];  
    int  iret = 0;  
    out = "";  
    /*编译正则表达式*/  
    iret = regcomp(&reg, pattern.c_str(), REG_EXTENDED|REG_NEWLINE);  
    if (iret != 0){  
        return -1;  
    }  
    iret = regexec(&reg, input.c_str(), 1, pm, 0);  
    if (iret == REG_NOMATCH){  
        out = "";  
        iret = input.length();  
    }else if (iret != 0) {  
        return -2;  
    }else{  
        out = input.substr(pm[0].rm_so,pm[0].rm_eo-pm[0].rm_so);  
        iret = pm[0].rm_eo;  
    }  
    regfree(&reg);  
    return iret;  
}  
  
int find_first(char *buff, char *pattern, char *outdata){  
    regex_t reg;  
    regmatch_t pm[1];  
    int  status = 0;  
    /*编译正则表达式*/  
    status = regcomp(&reg, pattern, REG_EXTENDED|REG_NEWLINE);  //扩展正则表达式和识别换行符  
    if (status != 0){    //成功返回0  
        return -1;  
    }  
    status = regexec(&reg, buff, 1, pm, 0);  
    if (status == REG_NOMATCH){  
        printf("no match!\n");  
        status = -1;  
    }  
    else if (status != 0) {  
        return -2;  
    }  
    else if (status == 0) {  
        int i, j;  
        for (i = pm[0].rm_so, j = 0; i < pm[0].rm_eo; i++, j++) {  
            outdata[j] = buff[i];  
        }  
        outdata[i] = '\0';  
    }  
    regfree(&reg);  
    return status;  
}  
  
int find_all(char *buff, char *pattern, char result[][20]){   //返回匹配个数  
    regex_t reg;  
    regmatch_t pm[1];  
    int  status = 0;  
    char * p = buff;  
    int count = 0;  
    /*编译正则表达式*/  
    status = regcomp(&reg, pattern, REG_EXTENDED|REG_NEWLINE);  //扩展正则表达式和识别换行符  
    if (status != 0){    //成功返回0  
        return -1;  
    }  
    int i = 0, j, k;  
    while((status = regexec(&reg, p, 1, pm, 0)) == 0) {  
        for(j = pm[0].rm_so, k = 0; j < pm[0].rm_eo; j++) {  
            result[i][k++] = p[j];  
        }  
        result[i][k] = '\0';  
        i++;  
        p += pm[0].rm_eo;  
        count++;  
        if (*p == '\0')  break;  
    }  
    regfree(&reg);  
    return count;  
} 
int find_num(string buff, string pattern,vector<string> &vec,int num){ 
	char* p  = (char*)malloc(sizeof(char)*buff.length());
	memcpy(p,buff.c_str(),sizeof(char)*buff.length());
    regex_t reg;  
    regmatch_t pm[1];  
    int  status = 0;  
    int count = 0;   
	int pos = 0;
    status = regcomp(&reg, pattern.c_str(), REG_EXTENDED|REG_NEWLINE);
    if (status != 0){
        return -1;  
    }  
    int i = 0, j, k;  
    while((status = regexec(&reg, p, 1, pm, 0)) == 0) {  
		char result[20];
		if(count >= num)
			break;
        for(j = pm[0].rm_so, k = 0; j < pm[0].rm_eo; j++) {  
            result[k++] = p[j];  
        }  
        result[k] = '\0';
		string tmp = result;
		vec.push_back(tmp);
        i++;  
        p += pm[0].rm_eo;
		pos += pm[0].rm_eo;
        count++;  
        if (*p == '\0')  
			break;  
    }
    regfree(&reg);
	p = p-pos;
	if(p!=NULL){
		free(p);
		p = NULL;
	}
    return count;
}  
int main()  
{  
	/*
    char result[20][20] = {0};  
    char buf[] = "1231a4568b789c234";
    char pattern[] = "[0-9]{1}";  
  
    char resultfirst[20] = {0};  
    find_first(buf, pattern, resultfirst);  
    cout<<strlen(resultfirst) << ":" << resultfirst<<endl;  
    cout << "***************************" <<endl;  
  
    int count = find_all(buf, pattern, result);  
    for (int i = 0; i < count; i++) {  
        cout<<"result:"<<"i="<<i+1<<"----"<<result[i]<<endl;  
    }  
    cout << "***************************" <<endl;  
	*/
	vector<string> vec;
	//string buf1 = "1231a4568b789c234";
	//string pattern1 = "[0-9]{2}"; 
	string buf1 = "\"huge\":\"1001\",\"linzhiling\":\"1002\"";
	cout<<"string = "<<buf1.c_str()<<endl;
	string pattern1 = "(hu)*(\")"; 
	int count1 = find_num(buf1, pattern1,vec,10);
	cout<<"getsize = "<<vec.size()<<endl;
	for(int i =0;i< vec.size();i++){
		cout<<"vec="<<vec[i].c_str()<<endl;
	}
	
    return 0;  
}  