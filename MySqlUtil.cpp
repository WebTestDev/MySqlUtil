#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>// ������������Ҫwinsock2.h 
#include <mysql.h>
#pragma comment(lib,"libmysql")

//��������IP��ַ,���host��NULL��"localhost"���ǵ�����������һ������
#define HOST "localhost"
//��¼�û���
#define USERNAME "root"
//��¼����
#define PASSWORD "123456"
//���ݿ�����
#define DATEBASE "test"
//���port����0,��ӦMYSQL��������TCP����˿�
#define PORT 3306
//�������NULL���ַ���ָ���׽��ֻ�Ӧ�ñ�ʹ�õ������ܵ�
#define UNIX_SOCKET NULL
/*
	Ĭ����0
	CLIENT_FOUND_ROWS --�����ҵ���(ƥ���)����,�����ܵ�Ӱ�������
	CLIENT_NO_SCHEMA  --������db_name.tbl_name.col_name�﷨
	CLIENT_COMPRESS   --ʹ��ѹ��Э��
	CLIENT_ODBC       --�ͻ���һ��ODBC�ͻ�
*/
#define CLIENT_FLAG 0
//�Ƿ��ӡ������Ϣ
#define DEBUG 1

/************************************************************************/
/* �������ݿ�                                                           */
/************************************************************************/
void connect(MYSQL * conn){
	//��ʼ��mysql����
	mysql_init(conn);
	if(conn==NULL){
		printf("Error mysql_init:%s !!!\n",mysql_error(conn));
		perror("Error mysql_init");
		exit(-1);
	}else if(DEBUG){
		printf("mysql_init successfully!!!\n");
	}
	//�������ݿ�
	if(mysql_real_connect(conn,HOST,USERNAME,PASSWORD,DATEBASE,PORT,UNIX_SOCKET,CLIENT_FLAG)!=NULL){
		if(DEBUG) printf("mysql_real_connect successfully!!!\n");
		//ѡ��Ĭ��ȱʡ�����ݿ�
		if(mysql_select_db(conn,DATEBASE)==0){
			if(DEBUG) printf("mysql_select_db successfully!!!\n");
			//�����Զ�����
			conn->reconnect=1;
		}else{
			perror("mysql_select_db");
			exit(-1);
		}
	}else{
		printf("Error mysql_real_connect:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}

/************************************************************************/
/* �Ͽ����ݿ�                                                           */
/************************************************************************/
void disconnect(MYSQL * conn){
	mysql_close(conn);
	if(DEBUG) printf("mysql_close successfully!!!\n");
}


void execSql(MYSQL * conn,const char* sqlStr){
	if(DEBUG) printf("sql exec cmd:%s\n",sqlStr);
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}

/************************************************************************/
/* ��ȡ���ݣ��򵥴�ӡ���ݣ������߸���ʵ�������д�洢��                 */
/************************************************************************/
void getData(MYSQL * conn, const char* sqlStr){
	if(DEBUG) printf("sql query cmd:%s\n",sqlStr);
	//ִ�в�ѯ���
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
		//�洢�����
		MYSQL_RES *res=mysql_store_result(conn);
		if(res==NULL){
			printf("Error mysql_store_result:%s !!!\n",mysql_error(conn));
			exit(-1);
		}else{
			if(DEBUG) printf("mysql_store_result successfully!!!\n");
		}
		MYSQL_ROW row;
		int index;
		while((row=mysql_fetch_row(res))){//������
			for(index=0;index<(int)mysql_num_fields(res);index++){
				printf("%s	",row[index]);
			}
			printf("*****************************\n");
		}
		//�ͷŽ���� 
		mysql_free_result(res);
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);		
	}
}

int main(){
	MYSQL conn;//����������MYSQL *conn��
	connect(&conn);
	//const char* str="insert into student values('3','lucy',19);";
	//execSql(&conn,str);
	const char* str="select * from student;";
	getData(&conn,str);
	disconnect(&conn);
	getchar();
	return 0;
}