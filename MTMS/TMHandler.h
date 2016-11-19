#ifndef   __TMHandler__H__
#define   __TMHandler__H__
 
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"  
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include "MTCP_Rosaline_v12.h"

using namespace rapidjson;
using namespace std;

void req_json_to_mtcp(const char *data, char *type, char *ID);
int mtcp_to_json(char *data, char *result);
int mtcp_to_json(int &errc, char *errs, char *result);
void Getaddress(char *type, char *_ip, int &_port);

int val_isRawData(const char* data);
//int AnalyseValue(const char *data, float a[], float b[]);
int AnalyseValue(const char *data, float a[], float b[],int max_count);
int pub_Check(const char *data, char *type, char *ID);
int pub_json_to_mtcp(const char *data, char *type, char *ID, char *Val);
void pub_json_to_mtcp(const char *data, char *type, char *ID, char **Val);
int analyseADIP(const char *data, tMTCP_data_ADIP ADIP[], int adip_count);
int ArrayCount(const char *address);
int AnalyseArray(const char* address, float a[]);
int ADIPCount(const char *data);
int ALPHCount(const char *data, char **address);
//string PortAdapter(char* type, char* ctrl);
char* PortAdapter(char* type, char* ctrl);

#endif