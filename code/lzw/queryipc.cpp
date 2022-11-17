#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include "../comm/tc_thread.h"
#include "../comm/tsys_comm.h"
#include "../comm/wbl_mysql.h"
#include "../comm/mysql_util.h"
#include "../t_sys_api/tsysjobmgr.h"
#define MAXLEN 64*1024 
#define EXTRA 0 
#define MAXINPUT MAXLEN+EXTRA+2 
using namespace std;
using namespace tsys;

CCluster2MySql g_mysql;

bool IPisNetworkx(const string& ip,const string& network)
{
    size_t pos=network.find("/");
    if (pos==string::npos)
        return false;
    string netid=network.substr(0,pos);
    uint32_t unet=inet_addr(netid.c_str());

    uint32_t uip=inet_addr(ip.c_str());

    unet=ntohl(unet);
    uip=ntohl(uip);
    uint32_t yanma=0;
    if (network.find("/32")!=string::npos)
        yanma=0xFFFFFFFF;
    else if (network.find("/31")!=string::npos)
        yanma=0xFFFFFFFE;
    else if (network.find("/30")!=string::npos)
        yanma=0xFFFFFFFC;
    else if (network.find("/29")!=string::npos)
        yanma=0xFFFFFFF8;
    else if (network.find("/28")!=string::npos)
        yanma=0XFFFFFFF0;
    else if (network.find("/27")!=string::npos)
        yanma=0XFFFFFFE0;
    else if (network.find("/26")!=string::npos)
        yanma=0XFFFFFFC0;
    else if (network.find("/25")!=string::npos)
        yanma=0XFFFFFF80;
    else if (network.find("/24")!=string::npos)
        yanma=0xFFFFFF00;
    else if (network.find("/23")!=string::npos)
        yanma=0xFFFFFE00;
    else if (network.find("/22")!=string::npos)
        yanma=0xFFFFFC00;
    else if (network.find("/21")!=string::npos)
        yanma=0xFFFFF800;
    else if (network.find("/20")!=string::npos)
        yanma=0xFFFFF000;
    else if (network.find("/19")!=string::npos)
        yanma=0xFFFFE000;
    else if (network.find("/18")!=string::npos)
        yanma=0xFFFFC000;
    else if (network.find("/17")!=string::npos)
        yanma=0xFFFF8000;
    else if (network.find("/16")!=string::npos)
        yanma=0xFFFF0000;
    else if (network.find("/15")!=string::npos)
        yanma=0xFFFE0000;
    else if (network.find("/14")!=string::npos)
        yanma=0xFFFC0000;
    else if (network.find("/13")!=string::npos)
        yanma=0xFFF80000;
    else if (network.find("/12")!=string::npos)
        yanma=0xFFF00000;
    else if (network.find("/11")!=string::npos)
        yanma=0xFFE00000;
    else if (network.find("/10")!=string::npos)
        yanma=0xFFC00000;
    else if (network.find("/9")!=string::npos)
        yanma=0xFF800000;
    else if (network.find("/8")!=string::npos)
        yanma=0xFF000000;
    else  //not stanard netmask  
        return false;
    if ((yanma&uip)==unet)
        return true;
    else
        return false;
}


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

/*
            endrntrim(input);
            len=strlen(input);       
*/
/*
            char *input;
            input = getenv("QUERY_STRING"); 
            len=strlen(input);
*/

char input[MAXINPUT]={0}, data[MAXINPUT]={0};
vector<string> g_confs;

bool Process1IPDelete(const string& ip)
{
#ifdef __SYNCCMDB__
    char xsql[1024];
    sprintf(xsql,"insert into t_syncagent(ip,status,instime,remark) values('%s',0,now(),'SYNCBYREGIPINDEX')",ip.c_str());
    try
    {
        g_mysql.query(xsql); //second really delete jobsvr in t_jobsvr
    }
    catch (exception &ex)
    {
        printf("<P>forcibly sync agent cmdb info :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }
    return true;
//  forbidden in regip2  
//    printf("<P>Delete function is forbidden.</P>\n");
//    exit(0);
    char cmd[4096];
    sprintf(cmd,"cd tsc_cmdbsync;./tsc_cmdbsync ./tsc_cmdbsync.conf sync1ip %s",ip.c_str());
    system(cmd);
//    printf("<P>IP has be synced.</P>\n");
//    exit(0);
    return true;
//  end forbidden ip regip2
#endif
    char sql[4096];
    sprintf(sql," \
delete from r_job_rtagent where exists \
( \
select 1 from  \
( \
select t2.t_jobsvr_id,t2.t_rtagent_id from t_rtagent t,r_job_rtagent t2 where t.t_rtagent_id=t2.t_rtagent_id and t.ip='%s' \
) tmain where r_job_rtagent.t_jobsvr_id=tmain.t_jobsvr_id and r_job_rtagent.t_rtagent_id=tmain.t_rtagent_id \
)",ip.c_str());
    try 
    {
        g_mysql.query(sql); //first delete relations in r_job_rtagent
    }   
    catch (exception &ex)
    {   
        printf("<P>route del jobsvr :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }       
    sprintf(sql,"delete from t_rtagent where ip='%s'",ip.c_str());
    try         
    {           
        g_mysql.query(sql); //second really delete jobsvr in t_jobsvr
    }   
    catch (exception &ex)
    {
        printf("<P>route del jobsvr :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }
    printf("<P>Delete tsc agent IP registration successfully.</P>\n");
    exit(0);
}

void Process1IPReresh(const string& ip)
{
    char sql[1024];
//    sprintf(sql,"insert into t_pushagent(ip,status,instime,remark) values('%s',0,now(),'PUSHBYREGIPINDEX')",ip.c_str());
    sprintf(sql,"insert into t_pushagent(ip,status,pushtime,remark) values('%s',0,now(),'PUSHBYREGIPINDEX')",ip.c_str());
    try
    {
        g_mysql.query(sql); //second really delete jobsvr in t_jobsvr
    }
    catch (exception &ex)
    {
        printf("<P>forcibly refresh agent :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }
}

bool Process1IPSelect(const string& ip,set<string> &s_unreg,set<string>& s_offline)
{
    char sql[4096];
    bool isleafagent;
    string regeds;

//    sprintf(sql,"select t.*,cast(uncompress(failreason) AS char( 500 )) as failreason2,if(unix_timestamp(now())-unix_timestamp(timestamp)<2400,1,0) as alived2 from t_rtagent t where ip='%s'",ip.c_str());
    sprintf(sql,"select t.*,cast(uncompress(failreason) AS char( 500 )) as failreason2,alived as alived2 from t_rtagent t where ip='%s'",ip.c_str());
    wbl::MySqlData datai=g_mysql.query(sql);
    if (datai.num_rows()>0)
        regeds="REGED";
    else
    {
        regeds="NORED";    
        s_unreg.insert(ip);
    }

    if (regeds=="NORED")
    {
        printf("<tr><tr><td style=\"background-color: #ccffff;\">%s</td><td style=\"background-color: #ccffff;color: red\">NOREG</td></tr>",ip.c_str());
    }
    else
    {
        string onlines=datai[0]["alived2"]=="1"?"ONLINE":"OFFLINE";
        string noregcolor;
        if (onlines=="OFFLINE")
        {
            noregcolor="color: red";
            s_offline.insert(ip);
        }
        string reg_mode=datai[0]["parentip"]==""?"NORAML":"PARENT";
        string noregcolor2;
        if (reg_mode=="PARENT")
            noregcolor2="color: red";
        printf("<tr><td style=\"background-color: #ccffff;\">%s</td><td style=\"background-color: #ccffff;\">REGED</td><td style=\"background-color: #ccffff;%s\">%s</td><td style=\"background-color: #ccffff;\">%s</td><td style=\"background-color: #ccffff;\">%s</td><td style=\"background-color: #ccffff;\">%s</td><td style=\"background-color: #ccffff;%s\">%s</td></tr>",ip.c_str(),noregcolor.c_str(),onlines.c_str(),datai[0]["ver"].c_str(),datai[0]["timestamp"].c_str(),datai[0]["regtime"].c_str(),noregcolor2.c_str(),reg_mode.c_str());
    }
        
}

void DoTestTgwRun(const vector<string> &vecout)
{
    printf("<p>start runing tgw test shell...如果测试任务返回10000，表示测试成功，否则失败!（SPLITED TASK all run finished. all return value is same,so atomic task return the same value 10000.）</p>\n");

    string xmltpl;
    if (!filetostring("/data/tsc/tsc_kit/bin_tsc_cmd/bin_runtsc_xmltpl/bin_send2tsys/testtgw.xml",xmltpl))
    {
        printf("<p> open file /data/tsc/tsc_kit/bin_tsc_cmd/bin_runtsc_xmltpl/bin_send2tsys/testtgw.xml failed.</p>\n");
        return;        
    }
    xmltpl=StringReplace(xmltpl,"TARGETIP",vecout[0],1);
    
    string runtesttgwxml="/tmp/tmp_tgwrun_"+GetTimeWithUsecond()+".xml";
    if (!stringtofile(xmltpl,runtesttgwxml.c_str()))
    {
        printf("<p> save file %s failed.</p>\n",runtesttgwxml.c_str());
        return;
    }

    
    string cmd="cd /data/tsc/tsc_kit/bin_tsc_cmd/bin_runtsc_xmltpl/bin_send2tsys;./send2tsys ./send2tsys.conf "+runtesttgwxml+" STDOUT ''";
    FILE *fl=popen(cmd.c_str(),"r");
    if (!fl)
    {
        printf("<p> open error!</p>\n");
        return;
    }
    printf("<p>");
    char rdline[4096];
    while (fgets(rdline,4096,fl))
    {   
        endrntrim(rdline);
        printf("%s<br/>\n",rdline);
    }
    printf("</p>");
    fclose(fl);

}

bool DoRegisterAgents2(const vector<string> &vecout,const string& jobsvr1,const string& jobsvr2)
{
    if (vecout.size()==0)
    {
        printf("<P>At least 1 IP.</P>\n");
        exit(0);
    }
    string inssql="insert into t_regagent(ip,status,instime) values";
    for (vector<string>::const_iterator it=vecout.begin();it!=vecout.end();++it)
    {
        inssql=inssql+"('"+*it+"',0,now()),";
    
/*
        string cmd="cd /data/reg_fixip;./ping_client2 "+*it;
//        system(cmd.c_str());
        FILE *fl=popen(cmd.c_str(),"r");
        if (!fl)
        {
            printf("<P>popen failed.</P>\n");
            exit(0);
        }
        pclose(fl);
*/
        
    }
    inssql.erase(inssql.length()-1);
    try
    {
        g_mysql.query(inssql);
    }
    catch (exception &ex)
    {
        printf("<P>route register agent :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }
    printf("<P>Your ips has registered successfully.</P>\n");
    exit(0);
}

bool DoRegisterAgents(const vector<string> &vecout,const string& jobsvr1,const string& jobsvr2)
{
    char sql[4096];
    for (vector<string>::const_iterator it=vecout.begin();it!=vecout.end();++it)
    {
        sprintf(sql,"insert into t_rtagent(regtime,alived,ver,sha512ok,ip,ignorehb,activate,remark) values(now(),1,'Linux S 3.2.22 FAKEALIVE_NOT_DECTECTED',1,'%s',NULL,1,'regbycgi')",
        it->c_str());
        try
        {
            g_mysql.query(sql); //second really delete jobsvr in t_jobsvr
        }
        catch (exception &ex)
        {
            printf("<P>route register agent :ERROR:%s</P>\n",ex.what());
            exit(-1);
        }

        sprintf(sql,"\
insert into r_job_rtagent select (select t_jobsvr_id from t_jobsvr where ip='%s'),(select t_rtagent_id from t_rtagent where ip='%s')",
                jobsvr1.c_str(),it->c_str());
        try
        {
              g_mysql.query(sql);
        }
        catch (exception &ex)
        {
            printf("<P>route register agent :ERROR:%s</P>\n",ex.what());
            exit(-1);
        }


        sprintf(sql,"\
insert into r_job_rtagent select (select t_jobsvr_id from t_jobsvr where ip='%s'),(select t_rtagent_id from t_rtagent where ip='%s')",
                jobsvr2.c_str(),it->c_str());
        try
        {
              g_mysql.query(sql);
        }
        catch (exception &ex)
        {
            printf("<P>route register agent :ERROR:%s</P>\n",ex.what());
            exit(-1);
        }


    }

    sprintf(sql,"update t_generalpara set paravalue=1,last_paravalue=1 where paraname='routemodified'");
    try
    {
        g_mysql.query(sql);
    }
    catch (exception &ex)
    {
        printf("<P>route register agent :ERROR:%s</P>\n",ex.what());
        exit(-1);
    }

    printf("<P>Your ips has registered successfully.</P>\n");
    exit(0);
}

int main()
{
    char *lenstr; 
    long len; 

//    printf("%s%c%c","Content-Type:text/html;charset=gb2312",13,10);
    printf("Content-Type:text/html;charset=gb2312\n\n");
    printf("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\"/></head><body>\n");
    lenstr = getenv("CONTENT_LENGTH"); 

    if(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN||len==0) 
        printf("<P>form submit error,perhaps post data is zero or exceed %d bytes!</P>\n",MAXLEN); 
    else 
    { 
        while (fgets(input, len+1, stdin)) //in apache,will only read one line in cgi
        { 

            FILE *fcnf=fopen("/etc/queryips.conf","r");
            if (!fcnf)
            {
                printf("<P>/etc/queryips.conf not found</P>\n");
                exit(-1);
            }
            char line[255]={0};
            while (fgets(line, 255, fcnf))
            {
                endrntrim(line);
                g_confs.push_back(line);
            }       
            fclose(fcnf);
            if (g_confs.size()<7)
            {       
                printf("<P>/etc/queryips.conf has at least 5 items</P>\n");
                exit(-1);
            }  

            unencode(input+EXTRA, input+len, data); 

            string inpost(data,strlen(data));

            size_t pos=inpost.find("=");
            string key=inpost.substr(0,pos);
            string value=inpost.substr(pos+1,inpost.length()-pos-1);
            value=smarttrim(value);

            vector<string> vecout;
            ParseSentenceToWords(value,vecout," \r\n");
            if (vecout.size()==0||vecout.size()>10000)
            {
                printf("<P>You can't submit 0 or great 10000 IPs.</P>\n");
                exit(-1);
            }
#ifdef __SYNCCMDB__
            if ((key=="TextArea5")&&vecout.size()>1 ||(key=="TextArea2")&&vecout.size()>100 )
#else
            if ((key=="TextArea2"||key=="TextArea5")&&vecout.size()>1)
#endif
            {
                printf("<P>You can only delete or test 1 IP!</P>\n");
                exit(-1);
            }

            try
            {
                g_mysql.Init(g_confs[0],g_confs[0],
                    g_confs[1],g_confs[1],
                    g_confs[2],g_confs[2],NULL);
                g_mysql.use(g_confs[3],g_confs[3]);
                g_mysql.setCharSet(g_confs[4]);
            }
            catch (exception &ex)
            {
                printf("<P>mysql connection error!</P>\n");
                exit(-1);
            }

            if (key=="TextArea3")
            {
                DoRegisterAgents2(vecout,g_confs[5],g_confs[6]);
                exit(0);
            }
            if (key=="TextArea5")
            {
                DoTestTgwRun(vecout);
                exit(0);
            }
            if (key=="TextArea1")
            {
                printf("<h2>查询结果(公网IP已被过滤):</h2>\n");
		        printf("<h4>警告:此页面只适用于公共版agent(TsysAgent_S5.tar.gz)注册，如果您已安装了SNG版agent(运行vi /usr/local/TsysAgent/etc/proxy_ctrl.conf，确认listenport是否为61369，若为61369即为SNG版本，普通版本为48369)，请参考http://km.oa.com/group/17345/articles/show/129225注册!!!</h4>\n");
                printf("<h4>说明:若查询结果列表中有红色标记字段，表明此IP不正常；1.NOREG时请点下面按钮注册 2. OFFLINE可能是网络原因暂时标记为离线，如果想立即使用，请使用首页regipindex.htm[立即刷新IP状态]功能 3.PARENT时表示是用SNG的注册方式，正常情况(机器安装了SNG版agent)下是可以用的，只有在不可用而且确信错误安装了侦听48369端口的普通版本时，才可在主页中删除该IP的错误注册信息，然后重新注册</h4>\n");
                printf("<table>\n");
                printf("<tr><td style=\"background-color: #ccffff;\">agentip</td><td style=\"background-color: #ccffff;\">reged</td><td style=\"background-color: #ccffff;\">online</td><td style=\"background-color: #ccffff;\">ver</td><td style=\"background-color: #ccffff;\">timestamp</td><td style=\"background-color: #ccffff;\">regtime</td><td style=\"background-color: #ccffff;\">reg_mode</td></tr>\n");
            }
            set<string> s_unreg;
            set<string> s_offline;
            int validcount=0;
            for (vector<string>::iterator it=vecout.begin();it!=vecout.end();++it)
            {
                //printf("%s<br>\n",it->c_str());
                if (!CheckIPLegal(it->c_str())) continue;
                string ip=*it;
                if (!(IPisNetwork(ip,"100.64.0.0/10")||IPisNetwork(ip,"9.0.0.0/8")||ip.find("10.")==0||ip.find("172.")==0||ip.find("192.")==0))
                    continue;
                if (key=="TextArea2")
                    Process1IPDelete(*it);
                else if (key=="TextArea4")
                    Process1IPReresh(*it);
                else
                {
                    validcount++;
                    Process1IPSelect(*it,s_unreg,s_offline);
                }
            }
#ifdef __SYNCCMDB__
    if (key=="TextArea2")
    {
        printf("<P>IPs has be synced.</P>\n");
        exit(0);
    }
#endif
            if (key=="TextArea4")
            {
                printf("<P>Your ips has forcibly refresh successfully.</P>\n");
                exit(0);
            }
            if (key=="TextArea2")
            {
                printf("<p>The IP which deleted is illeagal or WAN IP.</p>\n");
                exit(-1);
            }

            printf("</table>\n");
            printf("<p style=\"color:red\">有效内网IP总数:%d</p>",validcount);

            printf("<h2>未注册IP列表</h2>\n");

            printf("<form action=\"../cgi-bin/queryips.cgi\" method=\"post\" id=\"form1\">\n");
            printf("<textarea name=\"TextArea3\" readonly=\"readOnly\" style=\"height: 224px; width: 650px;\">\n");
            for (set<string>::iterator it2=s_unreg.begin();it2!=s_unreg.end();++it2)
            {
                printf("%s\n",it2->c_str());
            }
            printf("</textarea>\n");
            printf("<input id=\"Submit1\" type=\"submit\" value=\"注册这些IP(SNG 版本agent千万别操作!!!)\" style=\"font-weight: bold; color: red\"/>\n");
            printf("</form>\n");
    

            printf("<h2>已注册但是离线的IP列表:</h2><p>(如果你怀疑使用了错误的注册方式，你可以粘贴一个IP到首页regipindex.htm红色框中，点[删除一个IP]按钮删除,反复操作,然后重新批量注册)</p>\n");
            printf("<textarea name=\"TextArea4\" readonly=\"readOnly\" style=\"height: 224px; width: 650px;\">\n");
            for (set<string>::iterator it3=s_offline.begin();it3!=s_offline.end();++it3)
            {
                printf("%s\n",it3->c_str());
            }
            printf("</textarea>\n"); 

/*
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
*/
            break;
        }//end while (fgets(input, len+1, stdin))

    } 
    printf("</body></html>\n");
    return 0;
}
