//��json��ʽתΪmtcp��ʽ���mtcp��ʽתΪjson��ʽ
#include "TMHandler.h"

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_iterators.hpp"
#include "rapidxml\rapidxml_print.hpp"
#include "rapidxml\rapidxml_utils.hpp"

#include "MTCP_Rosaline_v12.h"

using namespace rapidxml;


int pub_json_to_mtcp(const char *data, char *type, char *ID, char *Val) //ID=SOCKET_ID    Val��ID��value
{
	string str = data;
	string::size_type pos = 0;
	while ((pos = str.find_first_of('\\', pos)) != string::npos)
	{
		str.insert(pos, "\\");//����  
		pos = pos + 2;
	}
	data = str.c_str();

		char *head = new char[1024];
		memset(head, 0, 1024);

		Document document;
		document.Parse<0>(data);

		//ȡ���Լ���Ҫ��ֵ
		if (document.HasParseError()){
			delete[] head;
			head = nullptr;
			return -1;
		}

		if (!document.IsObject())
		{
			delete[] head;
			head = nullptr;
			return -1;
		}

		if (!document.HasMember("data"))
		{
			delete[] head;
			head = nullptr;
			return -1;
		}
		Value &node = document["data"];
		if (node.IsObject())
		{
			
				if (node.HasMember("tid") && node["tid"].IsString()) {
					//cout << "....................." << node["tid"].GetString() << endl;
					memcpy(head, node["tid"].GetString(), 4);
					int type_cn = 0, id_cn = 0;
					int checksum = 0;//��־�ڼ����������� 
					char *str = new char[1024];
					memcpy(str, node["tid"].GetString(), 1024);
					for (int i = 0; i < strlen(node["tid"].GetString()); i++){
						if (checksum == 0 && str[i] != '#'){
							type[type_cn] = str[i];
							type_cn++;
						}
						else if (checksum == 0 && str[i] == '#'){
							checksum++;
						}
						else if (checksum == 1 && str[i] != '#'){
							ID[id_cn] = str[i];
							id_cn++;
						}
						else if (checksum == 1 && str[i] == '#'){
							checksum++;
						}
					}

				}
				if (node.HasMember("value") && node["value"].IsString()) {
					memcpy(Val, node["value"].GetString(), strlen(node["value"].GetString()));
				}
				if (node.HasMember("value") && node["value"].IsInt())
				{
					_itoa(node["value"].GetInt(), Val, 10);
				}
				if (node.HasMember("value") && node["value"].IsFloat())
				{
					sprintf(Val, "%f", node["value"].GetFloat());
				}			
		}
		delete[] head;
		head = nullptr;

		int issend = 0;
		if (_stricmp(ID, "SEND") == 0)
		{
			if (document.HasMember("event")){
				if (document["event"] == 3){
					issend = 1;
				}
			}
		}
		return issend;
	
}


void pub_json_to_mtcp(const char *data, char *type, char *ID, char **Val)
{
	int event = -1;
	string str = data;
	string::size_type pos = 0;
	while ((pos = str.find_first_of('\\', pos)) != string::npos)
	{
		str.insert(pos, "\\");//����  
		pos = pos + 2;
	}
	data = str.c_str();
	string readdate;
	readdate = data;
	Document document;
	document.Parse<0>(readdate.c_str());
	if (document.HasParseError())
	{
		cout << "Illegal json data!" << endl;
	}
	//ȡ���Լ���Ҫ��ֵ
	if (document.HasMember("data"))
	{
		Value &node = document["data"];
		if (node.IsArray())
		{
			if (document.HasMember("event") && document["event"].IsInt()){
				event = document["event"].GetInt();
			}
			if (event == 3)//�ж��Ƿ�Ϊ��������
			{
				for (size_t i = 0; i < node.Size(); ++i)
				{
					Value & v = node[i];
					assert(v.IsObject());
					if (v.HasMember("tid") && v["tid"].IsString()) {
						int type_cn = 0, id_cn = 0;
						int checksum = 0;//��־�ڼ����������� 
						char *str1 = new char[1024];
						memcpy(str1, v["tid"].GetString(), 1024);
						for (int i = 0; i < strlen(v["tid"].GetString()); i++){
							if (checksum == 0 && str[i] != '#'){
								type[type_cn] = str[i];
								type_cn++;
							}
							else if (checksum == 0 && str[i] == '#'){
								checksum++;
							}
							else if (checksum == 1 && str[i] != '#'){
								ID[id_cn] = str[i];
								id_cn++;
							}
							else if (checksum == 1 && str[i] == '#'){
								checksum++;
							}
						}
						delete[] str1;
						str1 = nullptr;
					}
					if (v.HasMember("value") && v["value"].IsArray()) {
						for (int i = 0; i < v["value"].Size(); i++)
						{
							if (v["value"][i].IsString())
							{
								for (int j = 0; j <= v["value"][i].GetStringLength(); j++)
								{
									Val[i][j] = v["value"][i].GetString()[j];
								}
							}
						}
					}
				}
			}
		}
	}
}


int val_isRawData(const char *data)//��֤Val�д�ŵĶ�ά����
{
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(data, ifstream::in);
	if (!in.is_open())
	{
		cout << "RawData Can't find data!" << endl;
		return -1;
	}
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();
	// parse json from string.
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError()) {
		cout << "Illegal json data!" << endl;
		return -1;
	}

	return doc.Size();
}

//int AnalyseValue(const char *data, float a[], float b[])//����Val�д�ŵĶ�ά����
int AnalyseValue(const char *data, float a[], float b[],int max_count)//����Val�д�ŵĶ�ά����
{
	if (val_isRawData(data) == -1)
	{
		return -1;
	}
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(data, ifstream::in);
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	for (int i = 0; i<max_count && i < doc.Size(); i++)
	{
		if (doc[i].IsArray() && (doc[i][0].IsFloat() || doc[i][0].IsInt()) && (doc[i][1].IsFloat() || doc[i][1].IsInt()))
		{
			a[i] = doc[i][0].GetFloat();
			b[i] = doc[i][1].GetFloat();
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int ADIPCount(const char *data)
{
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(data, ifstream::in);

	if (!in.is_open())
	{
		cout << "ADIP Can't find data!" << endl;
		return -1;
	}
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();
	// parse json from string.
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError()) {
		cout << "Illegal json data!" << endl;
		return -1;
	}
	return doc.Size();
}

int analyseADIP(const char *data, tMTCP_data_ADIP ADIP[], int adip_size)
{
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(data, ifstream::in);
	if (!in.is_open())
	{
		//cout << "Can't find data!" << endl;
		return -1;
	}
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();

	printf("%s\n", stringFromStream.c_str());

	// parse json from string.
	Document doc;

	printf("====================================== adip file 1 ====================================================\n");

	doc.Parse<0>(stringFromStream.c_str());

	if (doc.HasParseError()) {
		cout << "Illegal json data!" << endl;
		return -1;
	}

	printf("====================================== adip file 2 ====================================================\n");

	if (adip_size != doc.Size())
	{
		printf("adip size error\n");
		return -1;
	}

	printf("====================================== adip file  3 ====================================================\n");

	// use values in parse result.
	for (int i = 0; i < adip_size; i++)
	{
		printf("====================================== adip file adip count(%d) ########====================================================\n", i);

		ADIP[i].I_pulse = doc[i]["I_pulse"].GetFloat();
		printf("I_pulse\r\n");
		ADIP[i].V_pulse = doc[i]["V_pulse"].GetFloat();
		printf("V_pulse\r\n");
		ADIP[i].optical_power = doc[i]["optical_power"].GetFloat();
		printf("Op_power\r\n");

		int raw_count = doc[i]["V_pulse_raw"].Size();
		raw_count = (raw_count<10) ? (raw_count) : (10);
		printf("V_Pulse_raw count : %d\r\n", raw_count);
		for (int j = 0; j < raw_count; j++)
		{
			ADIP[i].V_pulse_raw[j] = doc[i]["V_pulse_raw"][j].GetFloat();
		}

		int power_count = doc[i]["optical_power_row"].Size();
		power_count = (power_count < 340) ? (power_count) : (340);
		printf("power_count : %d\r\n", power_count);
		for (int j = 0; j < power_count; j++)
		{
			ADIP[i].optical_power_row[j] = doc[i]["optical_power_row"][j].GetFloat();
		}
	}

	printf("====================================== adip file adip count end ====================================================\n");

	return 0;
}

int ArrayCount(const char *address)
{
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(address, ifstream::in);
	if (!in.is_open())
	{
		cout << "Can't find data!" << endl;
		return -1;
	}
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError()) {
		cout << "Illegal json data!" << endl;
		return -1;
	}
	return doc.Size();
	
}

int AnalyseArray(const char* address, float a[])
{
	if (ArrayCount(address) == -1)
	{
		return -1;
	}
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(address, ifstream::in);
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();
	// parse json from string.
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	for (int i = 0; i < doc.Size(); i++)
	{
		if (!doc[i].IsFloat() )
		{
			cout << "Error: Illegal data." << endl;
			return -1;
		}
		a[i] = doc[i].GetFloat();
	}
}


void req_json_to_mtcp(const char *data,char *type,char *ID)
{
	string readdate;
	readdate = data;
	Document document;
	document.Parse<0>(readdate.c_str());
	if (!document.HasParseError())
	{
		//ȡ���Լ���Ҫ��ֵ 
		if (document.HasMember("function"))
		{
			Value &node = document["function"];
			if (node.IsString())
			{
				int type_cn = 0, id_cn = 0;
				int checksum = 0;//��־�ڼ����������� 
				char *str = new char[1024];
				memcpy(str, node.GetString(), 1024);
				for (int i = 0; i < strlen(node.GetString()); i++){
					if (checksum == 0 && str[i] != '#'){
						type[type_cn] = str[i];
						type_cn++;
					}
					else if (checksum == 0 && str[i] == '#'){
						checksum++;
					}
					else if (checksum == 1 && str[i] != '#'){
						ID[id_cn] = str[i];
						id_cn++;
					}
				}
				delete[] str;
				str = nullptr;
			}
		}
	}
}
 
int mtcp_to_json(char *data,char *result)
{
	Document doc;
	doc.SetObject();    //key-value �൱��map
	//doc.Setvalue();        //������ �൱��vector
	Document::AllocatorType &allocator = doc.GetAllocator(); //��ȡ������
	//��doc����ֵ
	//result.SetString(data);
	doc.AddMember("result",StringRef(data), allocator);
	//��doc�����ֵд���ַ���
	StringBuffer buffer;
	//PrettyWriter<StringBuffer> writer(buffer);  //PrettyWriter�Ǹ�ʽ����json�������Writer���ǻ��пո�ѹ�����json  
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	memcpy(result, buffer.GetString(), strlen(buffer.GetString())+1);
	return 0;
}

int mtcp_to_json(int &errc, char *errs, char *result)
{
	Document document;
	document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();
	Value array(rapidjson::kArrayType);
	Value object(rapidjson::kObjectType);
	object.AddMember("errc", errc, allocator);
	object.AddMember("errs", StringRef(errs), allocator);
	array.PushBack(object, allocator);

	document.AddMember("result", array, allocator);
	StringBuffer  buffer;
	Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	memcpy(result, buffer.GetString(), strlen(buffer.GetString()) + 1);
	return 0;
}

void Getaddress(char *type, char *_ip, int &_port)
{
	
		file<> fdoc("load.xml");//��ȡxml�ļ�
		
		xml_document<>   doc;
		doc.parse<0>(fdoc.data());
		xml_node<>* root = doc.first_node();//xml�ļ����ڵ�
		xml_node<>* plugin = root->first_node();//ָ���һ����ַ��Ϣ���
		while (plugin)
		{
			if (strcmp(plugin->first_attribute()->value(), type) == 0){
				strcpy(_ip, plugin->first_node("ip")->value());
				_port = atoi(plugin->first_node("port")->value());
			}
			plugin = plugin->next_sibling();
		}
		
}



int ALPHCount(const char *data, char **address){
	//count pictures' address
	char*  p = new char[strlen(data) + 1];
	strcpy(p, data);

	//char*  dd;
	char*  k = strtok(p, ",");
	int index = 0;
	while (k)
	{
		strcpy(address[index++],k);
		k = strtok(NULL, ",");
	}
	return index;
	delete[] p;
	p = nullptr;
}


char* PortAdapter(char* type, char* ctrl){
	char* CtrlList = new char[100];
	char* CtrlNote = new char[100];
	char* port = new char[100];
	file<> fdoc("load.xml");//��ȡxml�ļ�
	xml_document<>   doc;
	doc.parse<0>(fdoc.data());
	//doc.parse<0>(XmlData);
	xml_node<>* root = doc.first_node();
	xml_node<>* plugin = root->first_node();
	while (plugin)
	{
		if (strcmp(plugin->first_attribute()->value(), type) == 0){
			strcpy(CtrlList, plugin->first_node("port2")->value());
			char *TokenCtrl = strtok(CtrlList, ",");
			strcpy(CtrlNote, TokenCtrl);
			while (TokenCtrl != NULL && strcmp(ctrl, TokenCtrl) != 0) {
				TokenCtrl = strtok(NULL, ",");
				if (TokenCtrl){
					strcpy(CtrlNote, TokenCtrl);
				}
			}
		}
		plugin = plugin->next_sibling();
	}
	if (0 == strcmp(ctrl, CtrlNote)){
		strcpy(port, "req_napa_port2");
	}
	else
	{
		strcpy(port, "req_napa_port1");
	}
	doc.clear();
	delete[] CtrlList;
	delete[] CtrlNote;
	//delete[] XmlData;

	return port;
}

//int main(){
//	float aa[4096];
//	float bb[4096];
//	string f = "G:\\vcal_file\\tMTCP_data_ADIP.json";
//	int a = 0;
//	//tMTCP_data_ADIP *adip = (tMTCP_data_ADIP *)malloc(sizeof(tMTCP_data_ADIP)* 2);
//	tMTCP_data_ADIP adip[2];
//	a = analyseADIP((char *)f.c_str(), adip);
//
//	//a = AnalyseValue((char *)f.c_str(), aa, bb);
//	for (int i = 0; i < 5; i++){
//		cout << "-------------adip[i].I_pulse:" << adip[i].I_pulse << endl;
//	}
//	//cout << a << endl;
//	system("pause");
//	return 0;
//}