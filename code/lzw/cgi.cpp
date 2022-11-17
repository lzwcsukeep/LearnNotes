#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include "../comm/tsys_comm.h"
#include "../t_sys_api/tsysjobmgr.h"
#define MAXLEN 4*1024 
#define EXTRA 0 
#define MAXINPUT MAXLEN+EXTRA+2 
using namespace std;
using namespace tsys;

void unencode(char *src, char *last, char *dest) 
{ 
    for(; src != last; src++, dest++) 
    {
        if(*src == '+') 
            *dest = ' '; 
        else if(*src == '%') 
        { 
            int code; 
            if(sscanf(src+1, "%2x", &code) != 1) code = '?'; 
            *dest = code; 
            src +=2; 
        } 
        else 
            *dest = *src; 
    }
    *dest=0;
/*
    *dest = ' '; 
    *++dest = 0; 
*/
} 
int main()
{
/*
    cout<<"tcna="<<tsys::EncryptRtagenPassword("tcna")<<endl;
    exit(0);
*/

    char *lenstr; 
    char input[MAXINPUT], data[MAXINPUT]; 
    long len; 

//    printf("%s%c%c","Content-Type:text/html;charset=gb2312",13,10);
    printf("Content-Type:text/html;charset=gb2312\n\n");
    printf("<html>\n");
    lenstr = getenv("CONTENT_LENGTH"); 
    if(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN) 
        printf("<P>form submit error,perhaps post data exceed 4096 bytes!</P>\n"); 
    else 
    { 
        FILE *f; 
        while (fgets(input, len+1, stdin))
        { 
/*
            endrntrim(input);
            len=strlen(input);       
*/
/*
            char *input;
            input = getenv("QUERY_STRING"); 
            len=strlen(input);
*/
            unencode(input+EXTRA, input+len, data); 

            string inpost(data,strlen(data));
            list<string> passwdlist;
//            Extract2ListByExpr("PASSWD=[^\r\n]{0,50}(\r\n|\n)",inpost,passwdlist);
            Extract2ListByExpr("PASSWD=<\\*<\\*<.{0,50}>\\*>\\*>",inpost,passwdlist);
            for (list<string>::iterator it=passwdlist.begin();it!=passwdlist.end();++it)
            {
                string token=*it;
                int pos=token.find("PASSWD=");
                string pwd=token.substr(strlen("PASSWD="),token.length()-strlen("PASSWD="));
                pwd=pwd.substr(5,pwd.length()-10);
                if (pwd!="")
                    pwd=tsys::EncryptRtagenPassword(pwd);
                printf("[%s]",pwd.c_str());
            }
        }

    } 
    printf("</html>\n");
    return 0;
}
