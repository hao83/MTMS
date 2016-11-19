#include "../mtcp_module.h"
#include "../MTCPConnection.h"

FLDI::FLDI()
{
	memset(&_req, 0, sizeof(_req));
	_req.POS_X = 125;

	// Anjue add
	flat_rawdata = nullptr;
	image_rawdata = nullptr;
	rawdata = nullptr;
	flat_len = 0;
	image_len = 0;
	rawdata_len = 0;

	m_data = new char[BUFFER_SIZE+1];
	m_size = BUFFER_SIZE;
	m_length = 0;
}


FLDI::~FLDI()
{
	delete[] m_data;
}

int FLDI::setImageRawdata(char* filename) //return -1 read fail,return 0 read success
{
	if (NULL == filename)
	{
		return -1;
	}

	m_img_filepath = std::string(filename);

	return 0;

	//FILE * f = fopen(filename, "rb");
	//if (f == NULL)
	//{
	//	printf("##read file fail....\n");
	//	return -1;
	//}
	//fseek(f, 0, SEEK_END);
	//long size = ftell(f);
	//if (image_rawdata){
	//	delete[] image_rawdata;
	//	image_rawdata = nullptr;
	//	rawdata_len = 0;
	//}
	//image_rawdata = new char[size];
	//memset(image_rawdata, 0, size);
	//fseek(f, 0, SEEK_SET);
	//size_t iRead = fread(image_rawdata, size, 1, f);
	//fclose(f);

	///*FILE * f1 = fopen("G:\\Image\\wirtepic.tif", "wb");
	//fwrite(image_rawdata, size, 1, f1);
	//fclose(f1);*/

	//_req.IMG_WIDTH = 3296;//3296;//700;
	//_req.IMG_HEIGHT = 2472;//2472;//700;
	//_req.IMG_SIZE = size;
	//image_len = size;
	//cout << "image_len:" << image_len << endl;
	//return 0;
}

int FLDI::setFloatRawdata(char* filename )
{
	if (NULL == filename)
	{
		return -1;
	}

	m_ffd_filepath = std::string(filename);

	return 0;

	//FILE * f = fopen(filename, "r");
	//if (f == NULL)
	//{
	//	printf("##read file fail....\n");
	//	return -1;
	//}
	//fseek(f, 0, SEEK_END);
	//long size = ftell(f);
	//if (flat_rawdata){
	//	delete[] flat_rawdata;
	//	flat_rawdata = nullptr;
	//	rawdata_len = 0;
	//}
	///*_req.IMG_WIDTH = 1024;
	//_req.IMG_HEIGHT = 768;*/
	//flat_len = _req.IMG_WIDTH * _req.IMG_HEIGHT * sizeof(float);
	////flat_len = size;
	//flat_rawdata = new char[flat_len];
	//memset(flat_rawdata, 0, flat_len);
	//int ture_size = size - flat_len;
	//fseek(f,ture_size, SEEK_SET);
	//size_t iRead = fread(flat_rawdata, size, 1, f);
	//fclose(f);
	////GetIMAGE("FLDI", (int*)(&_req.IMG_WIDTH), (int*)(&_req.IMG_HEIGHT));
	//cout << "_req.IMG_WIDTH:..." << _req.IMG_WIDTH << "flat_len:" << flat_len << "float:..."<<sizeof(float) << endl;
	//return 0;
}

int FLDI::setAllRawdata()// 
{
	return 0;

		if (!image_rawdata){
			if (!flat_rawdata){
				//  no image,no float array
				rawdata = nullptr;
				rawdata_len;
				return 0;
			}
			//only float array
			rawdata_len = flat_len;
			rawdata = new char[rawdata_len];
			memset(rawdata, 0, rawdata_len);
			memcpy(rawdata, flat_rawdata, flat_len);
		}
		else{
			if (!flat_rawdata){
				//only image
				rawdata_len = image_len;
				rawdata = new char[rawdata_len];
				memset(rawdata, 0, rawdata_len);
				memcpy(rawdata, image_rawdata, image_len);
			}
			// both image and float array,
			rawdata_len = image_len + flat_len;
			rawdata = new char[rawdata_len];
			memset(rawdata, 0, rawdata_len);
			memcpy(rawdata, flat_rawdata, flat_len);
			memcpy(rawdata + flat_len, image_rawdata, image_len);
		}

	cout << "image_len:" << image_len << "flat_len:" << flat_len << "rawdata_len" << rawdata_len << endl;
	
	return 0;
}

int FLDI::setvalue(char *what, char* value)
{
	if (0 == strcmp(what, "IMG_INDEX")){
		IMG_INDEX(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "IMG_WIDTH")){
		IMG_WIDTH(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "IMG_HEIGHT")){
		IMG_HEIGHT(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "IMG_SIZE")){
		IMG_SIZE(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "POS_X")){
		POS_X(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "POS_Y")){
		POS_Y(atoi(value));
		return 0;
	}
	if (0 == strcmp(what, "POWER_TO_PIXEL_RATIO")){
		POWER_TO_PIXEL_RATIO(atof(value));
		return 0;
	}

	return -1;
}


tMTCP_payload_TEST_RSP FLDI::SendFLDI()
{
	m_length = 0;

	if (fill_FLoat_Raw_Data() < 0)
	{
		return tMTCP_payload_TEST_RSP();
	}

	//int ret = SendFrame(kMTCP_CTRL_FLDI, (void*)&_req, sizeof(_req), rawdata, rawdata_len);

	int ret = SendFrame(kMTCP_CTRL_FLDI, (void*)&_req, sizeof(_req), m_data, m_length);

	tMTCP_payload_TEST_RSP rep;

	memset(&rep, 0, sizeof(rep));
	RecvResponse(&rep, sizeof(rep));

	//if (rawdata){
	//	delete[] rawdata;
	//	rawdata = nullptr;
	//	rawdata_len = 0;
	//}
	//if (image_rawdata){
	//	delete[] image_rawdata;
	//	image_rawdata = nullptr;
	//	image_len = 0;
	//}
	//if (flat_rawdata){
	//	delete[] flat_rawdata;
	//	flat_rawdata = nullptr;
	//	flat_len = 0;
	//}

	return rep;
}

int FLDI::real_setImageRawdata(char* filename) //return -1 read fail,return 0 read success
{
	FILE * f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("##read file fail....\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	if (image_rawdata){
		delete[] image_rawdata;
		image_rawdata = nullptr;
		rawdata_len = 0;
	}
	image_rawdata = new char[size];
	memset(image_rawdata, 0, size);
	fseek(f, 0, SEEK_SET);
	size_t iRead = fread(image_rawdata, size, 1, f);
	fclose(f);

	/*FILE * f1 = fopen("G:\\Image\\wirtepic.tif", "wb");
	fwrite(image_rawdata, size, 1, f1);
	fclose(f1);*/

	_req.IMG_WIDTH = 3296;//3296;//700;
	_req.IMG_HEIGHT = 2472;//2472;//700;
	_req.IMG_SIZE = size;
	image_len = size;
	cout << "image_len:" << image_len << endl;

	return 0;
}

int FLDI::real_setFloatRawdata(char* filename)
{
	FILE * f = fopen(filename, "r");
	if (f == NULL)
	{
		printf("##read file fail....\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	if (flat_rawdata){
		delete[] flat_rawdata;
		flat_rawdata = nullptr;
		rawdata_len = 0;
	}
	/*_req.IMG_WIDTH = 1024;
	_req.IMG_HEIGHT = 768;*/
	flat_len = _req.IMG_WIDTH * _req.IMG_HEIGHT * sizeof(float);
	//flat_len = size;
	flat_rawdata = new char[flat_len];
	memset(flat_rawdata, 0, flat_len);
	int ture_size = size - flat_len;
	fseek(f, ture_size, SEEK_SET);
	size_t iRead = fread(flat_rawdata, size, 1, f);
	fclose(f);
	//GetIMAGE("FLDI", (int*)(&_req.IMG_WIDTH), (int*)(&_req.IMG_HEIGHT));
	cout << "_req.IMG_WIDTH:..." << _req.IMG_WIDTH << "flat_len:" << flat_len << "float:..." << sizeof(float) << endl;
	return 0;
}

// fill 
int FLDI::fill_FLoat_Raw_Data()
{
	if (m_ffd_filepath.length() <= 0 || m_img_filepath.length() <= 0)
	{
		return -1;
	}

	FILE * imgfp = fopen(m_img_filepath.c_str(), "rb");
	FILE * ffifp = fopen(m_ffd_filepath.c_str(), "rb");
	if (NULL == imgfp || NULL == ffifp)
	{
		if (imgfp) fclose(imgfp);
		if (ffifp) fclose(ffifp);
		return -1;
	}

	fseek(imgfp, 0, SEEK_END);
	fseek(ffifp, 0, SEEK_END);

	int img_len = ftell(imgfp);
	int ffi_len = ftell(ffifp);
	int flat_len = _req.IMG_WIDTH * _req.IMG_HEIGHT * sizeof(float);
	int head_len = ffi_len - flat_len;

	if (m_size < (img_len + flat_len))
	{
		delete[] m_data;
		m_size = img_len + flat_len;
		m_data = new char[m_size + 1];
	}

	_req.IMG_SIZE = img_len;
	m_length = img_len + flat_len;

	fseek(imgfp, 0, SEEK_SET);
	fseek(ffifp, head_len, SEEK_SET);

	fread(m_data, flat_len, 1, ffifp);
	fread((m_data + flat_len), img_len, 1, imgfp);

	fclose(ffifp);
	fclose(imgfp);

	m_data[m_length] = 0;

	return m_length;
}

