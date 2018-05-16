// NachiSocket.cpp : implementation file
//

//#include "stdafx.h"
#include "NachiSocket.h"

#define HDEG(x) (x/180.0*3.14159265)

/////////////////////////////////////////////////////////////////////////////
// CNachiSocket
// default program options
//bool debug = false;
bool tcp1 = true;
char host1[100] = "192.168.0.80";
int port1 = 48952;
char extraS[] = "ASJHDGASDGJAHSGJDASKDHGAKSHDGKJSHVDJHASDJKHVASJDHVASHVDAH\
SVDJKHAVSDJHAVSJDHVASJKHDVAJHKSVDJAHVSDJHVASDHJVASJHDVAKJSHVD\
JKASVDJKHAVSDJHAVSJHDVAJSHDVJAHSVDJHAVSDJHVchASJHDVAJSHVDJAHSVBD\
JHASJDHVAJSHVDJAHVSDJHVASJHDVAHJSVDJHAVSDJVASJKHDVAJKSHDVAJKHSVDJA\
KSVDJHAVSDJHVASJDHVAJSHDVAJKSHVDJAHSVDJHASVDJKHVSADJKHsdasdasdasdasda\
sdasdasdasdasdasdasdaASJHDGASDGJAHSGJDASKDHGAKSHDGKJSHVDJHASDJKHVASJDHVA\
SHVDAHSVDJKHAVSDJHAVSJDHVASJKHDVAJHKSVDJAHVSDJHVASDHJVASJHDVAKJSHVDJKASVDJK\
HAVSDJHAVSJHDVAJSHDVJAHSVDJHAVSDJHVchASJHDVAJSHVDJAHSVBDJHASJDHVAJSHVDJAHVSDJH\
VASJHDVAHJSVDJHAVSDJVASJKHDVAJKSHDVAJKHSVDJAKSVDJHAVSDJHVASJDHVAJSHDVAJKSHVDJAHS\
VDJHASVDJKHVSADJKHsdasdasdasdasdasdasdasdasdasdasdasdaASJHDGASDGJAHSGJDASKDHGAKSHDGK\
JSHVDJHASDJKHVASJDHVASHVDAHSVDJKHAVSDJHAVSJDHVASJKHDVAJHKSVDJAHVSDJHVASDHJVASJHDVAKJSHVDJKASVDJKHAVSDJH\
AVSJHDVAJSHDVJAHSVDJHAVSDJHVchASJHDVAJSHVDJAHSVBDJHASJDHVAJSHVDJAHVS\
DJHVASJHDVAHJSVDJHAVSDJVASJKHDVAJKSHDVAJKHSVDJAKSVDJHAVSDJHVASJDHVAJasdasdasdasdasda\
shdgajsgdkasdhkjashdkasdasdasdasd";	




CNachiSocket::CNachiSocket()
{
	isConnected = false;
	m_method = 1;
	m_direction = 1;
	m_height = 180;
	m_isCalculated = 0;
	m_ifCanTestAdjusted = false;
// 	m_curX = 350;
//	m_curY = 350;
// 	m_curZ = 350;
}

CNachiSocket::~CNachiSocket()
{
}


bool CNachiSocket::nachiHandHandle(int Open)
{
        //printf("sending!");
        char command[400*sizeof(char)];
        memset(command, 0, 400*sizeof(char)); // clear buffer
        sprintf(command, "TaskName=ENDEFFECTER TaskNameEnd BUTTON=%d BUTTONEnd", Open);
        try
        {

                if(send_arg(command)){
                        //cout<<"sended!"<<endl;
                        if (receive()){
                                return true;
                        }
                        else{
                                return false;
                        }
                }

        }
        catch(...)
        {
                printf("connection wrong!");
                return false;
        }
}

// Socket Initialization
bool CNachiSocket::init()
{
    sock_cli = socket(PF_INET,SOCK_STREAM, 0);
	
    //struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port1);
    servaddr.sin_addr.s_addr = inet_addr(host1);
	
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
	
	//CString csFullPath(pFileName);
        //printf("\n%s\n", csFullPath);
        isConnected = true;
        return true;
}

bool CNachiSocket::nachiClose()
{
    //shutdown(sock_cli, SHUT_WR);
    //send_arg("NULL");
    close(sock_cli);
    return true;
}

// Send a command to the DCS
bool CNachiSocket::send_arg(const char *message)
{
	if(isConnected == false){
                printf("Not connect!");
		return true;
	}
	memset(socketBuffer, 0, socketBufferSize1); // clear buffer
	sprintf(socketBuffer, "%s%s", message, extraS); // print command to buffer
	//printf("%s\n",message);
	int n;
	if (!tcp1) 
		//n = sendto(sockfd, socketBuffer, strlen(socketBuffer), 0, (struct sockaddr*) &serverName, sizeof (serverName));
                //send(sock_cli, socketBuffer, strlen(socketBuffer),0); ///发送
                n = write(sock_cli, socketBuffer, 1024);
	else 
		//n = send(sockfd, socketBuffer, 1024, 0);
                n = write(sock_cli, socketBuffer, 1024); ///发送
	
	//if (debug) printf("%i bytes sent: %s\n", n, socketBuffer);
	if (n < 0)
	{
		printf("Error: Could not send command\n");
		return false;
	}
	return true;
}

// Receive the response from the DCS and save it in the buffer
bool CNachiSocket::receive(int length)
{
	if (length > socketBufferSize1)
	{
		printf("Error: Socket buffer too small\n");
		return false;
	}
        //cout << "received!"<<endl;
	
	struct sockaddr_in addrRemote;
	int cbAddr = sizeof(addrRemote);
	
	memset(socketBuffer, 0, socketBufferSize1); // clear buffer
	int responseLength = 0, n, readMax;

//	int nNetTimeout = 1000*30;//ms;
// 	if (SOCKET_ERROR ==  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int))) 
// 	{ 
// 		printf("Set Ser_RecTIMEO error !\r\n"); 
// 	} 

	//while (true) // read, until the expected response length could be read (the response might be fragmented into several packets)
	//{
		readMax = length - responseLength; // remaining size to read
                //cout<<"begin rev"<<endl;
		if (!tcp1) 
			//n = recvfrom(sockfd, socketBuffer + responseLength, readMax, 0, (struct sockaddr *) &addrRemote, &cbAddr);
                        n = read(sock_cli, socketBuffer + responseLength, readMax); ///接收
		else 
                        //n = recv(sockfd, socketBuffer + responseLength, readMax, 0);
                        n = read(sock_cli, socketBuffer + responseLength, readMax); ///接收
		if (n <= 0)
		{
			printf("Error: Could not receive response\n");
			return false;
		}
		
		responseLength += n;
		//if (responseLength == 4 /*Server returned error code*/ || responseLength == length) 
			//break;
	//}
        //printf("receive33333!");
        //printf("%i bytes received: %s \n", responseLength, socketBuffer);
        //cout << "received!";
	return true;
}




/////////////////////////////////////////////////////////////////////////////
// CNachiSocket message handlers

bool CNachiSocket::nachiSendSinglePoint(float input[6], int index)
{
        //printf("sending!");
	char command[400*sizeof(char)];
	memset(command, 0, 400*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=RECEICEMOVEPOINT TaskNameEnd SNS=%d SNEND X=%f Y=%f Z=%f A=%f B=%f C=%f c=END ", index, input[0], input[1], input[2], input[3], input[4], input[5]);
	try
	{

		if(send_arg(command)){
                        //cout<<"sended!"<<endl;
// 			if (receive()){
// 				return true;
// 			}
// 			else{
// 				return false;
// 			}
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
		return false;
        }
}

bool CNachiSocket::nachiSendMoveReady(int steps)
{
        //cout<<"success"<<endl;
	char command[200*sizeof(char)];
	memset(command, 0, 200*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=INITRECEICEMOVEPOINT TaskNameEnd STATE=READY STATEEnd SNS=%d SNEND X=-80.98 Y=52.17 Z=-11.45 A=-0.41 B=42.04 C=-56.02 END BUTTON=0 BUTTONEnd ", steps);
	try
	{
		if(send_arg(command)){
                        //printf("sended!");
			if (receive()){
                                //printf("receive!");
				return true;
			}
			else{
                                //printf("receive11111!");
				return false;
			}
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
		return false;
    }
}

bool CNachiSocket::nachiBeginMove(float speed)
{
	if(isConnected == false){
                printf("Not connect!");
		return true;
	}
        //cout<<"success"<<endl;
	char command[200*sizeof(char)];
	memset(command, 0, 200*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=BEGINMOVE TaskNameEnd SPEED=%f SPEEDEnd", speed);
        //cout<<"success"<<endl;
	try
	{
		if(send_arg(command)){

			//printf("sended!");
			if (receive()){
                            //cout<<"success"<<endl;
				return true;
			}
			else{
				return false;
			}
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
		return false;
    }
}



bool CNachiSocket::nachiPosState()
{
	if(isConnected == false){
                printf("Not connect!");
		return true;
	}
	char command[200*sizeof(char)];
	memset(command, 0, 200*sizeof(char)); // clear buffer
        sprintf(command, "TaskName=READAXISSTATE TaskNameEnd");
	try
	{
		if(send_arg(command)){
			//printf("sended!");
                        if (receive())
                        {
				m_curX = findValue("X=", "Y=");
				m_curY = findValue("Y=", "Z=");
				m_curZ = findValue("Z=", "A=");
				m_curA = findValue("A=", "B=");
				m_curB = findValue("B=", "C=");
				m_curC = findValue("C=", "NUMBER");
				return true;
			}
			else{
				return false;
			}
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
		return false;
    }
}

bool CNachiSocket::nachiGetPosStateXYZ(float& curX, float& curY, float&	curZ)
{
	if(isConnected == false){
		printf("没有连接机器人！");
		return true;
	}
	char command[200*sizeof(char)];
	memset(command, 0, 200*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=READPOSSTATE TaskNameEnd");
	try
	{
		if(send_arg(command)){
			//printf("sended!");
			if (receive()){
				curX = findValue("X=", "Y=");
				curY = findValue("Y=", "Z=");
				curZ = findValue("Z=", "A=");
				return true;
			}
			else{
				return false;
			}
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
		return false;
    }
}


float CNachiSocket::findValue(string begin_string, string end_string)
{
	char *ptr;
        char searStr[1000];
        const char* begin = begin_string.c_str();
        const char* end = end_string.c_str();
        ptr = strstr((char *)socketBuffer, begin);
	int beginindex=ptr-(char *)socketBuffer;
        ptr = strstr((char *)socketBuffer, end);
	int endindex=ptr-(char *)socketBuffer;
	memset(searStr, 0, sizeof(searStr));
        strncpy((char *)searStr, (char *)socketBuffer+beginindex+sizeof(begin)/sizeof(int), endindex-beginindex-sizeof(begin)/sizeof(int)-1);
	//printf("%s \n", searStr);
	return atof((char *)searStr);
}

bool CNachiSocket::nachiStressBasicMotion(float x, float y, float z, float startAng, float endAng, int divide, int method, int direction, int height)
{
	printf("dilapidated!");
	return true;
	if(isConnected == false){
		printf("没有连接机器人！");
		return true;
	}
	if (height>400)
	{
		height = m_height;
	}
	float next_angle;
	nachiSendMoveReady(divide);
	for(int i=0; i<divide; i++){
		next_angle = (endAng - startAng)/((float)divide)*(i+1.0)+startAng;
		if(method == 0/*同倾法*/){
			float degree = 0.00;
			if(direction == 0/*X direction*/){
				float input[] = {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0+next_angle, -180};
				nachiSendSinglePoint(input, i+1);
			}
			else if(direction == 1)/*Y direction*/{
				next_angle = -next_angle;
				if(next_angle >=0){
					float input[] = {x, y-height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, 0, -(180-next_angle)};
					nachiSendSinglePoint(input, i+1);
				}
				else{ 
					float input[] = {x, y-height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, 0, 180+next_angle};
					nachiSendSinglePoint(input, i+1);
				}
			}

		}else/*侧倾法*/{
			float degree = -90.0;
			if(direction == 1/*Y direction*/){ 
				float input[] = {x, y+height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, -next_angle, -180};
				nachiSendSinglePoint(input, i+1);
			}
			else if (direction == 0)/*X direction*/{
				if(next_angle >= 0){ 
					float input[] = {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0, (180-next_angle)};
					nachiSendSinglePoint(input, i+1);
				}else{
					float input[] = {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0, -180-next_angle};
				nachiSendSinglePoint(input, i+1);
				}
			}

		}
	}
	return true;
}

void CNachiSocket::nachiSendRealTimeSingleDirStep1(int time1, int time2, float speed)
{
	char command[400*sizeof(char)];
	memset(command, 0, 400*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=REALTIMEMOVECONTI TaskNameEnd TIME1=%d TIME1END TIME2=%d TIME2END SPEED=%f SPEEDEND ",time1, time2, speed);
	try
	{
		if(send_arg(command)){
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
    }
}

void CNachiSocket::nachiSendRealTimeSingleDirStep2(float Xdistance, float Ydistance, float Zdistance)
{
	char command[400*sizeof(char)];
	memset(command, 0, 400*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=USERCHANGEDIR TaskNameEnd X=%f XEND Y=%f YEND Z=%f ZEND ", Xdistance, Ydistance, Zdistance);
	try
	{
		if(send_arg(command)){
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
    }
}

void CNachiSocket::nachiSendRealTimeStop()
{
	char command[400*sizeof(char)];
	memset(command, 0, 400*sizeof(char)); // clear buffer
	sprintf(command, "TaskName=USERDIRECTIONSTOP TaskNameEnd  X= XEND  Y= YEND  Z= ZEND ");
	try
	{
		if(send_arg(command)){
		}
		
	}  
	catch(...)
	{
		printf("connection wrong!");
    }
}

bool CNachiSocket::nachiIsRightPos()
{
		//nachiPosState();
		//float t_x = m_curX;
		//float t_y = m_curY;
		//float t_z = m_curZ;
		float t_x, t_y, t_z;
		nachiGetPosStateXYZ(t_x, t_y, t_z);
		if (t_x < (310-0.05) || t_x > 350.05)
		{
			return false;
		}
		if (t_y < (-120-0.05) || t_y > (120+0.05))
		{
			return false;
		}
		if (t_z < (300-0.05-182.0f) || t_z > (380+0.05-182.0f))
		{
			return false;
		}	
		return true;
}

void CNachiSocket::calculatePosition(int stationNum, float x, float y, float z, float ang, int method, int direction, int height)
{
	if (height > 400)
	{
		height = m_height;
	}
	if(method == 0/*同倾法*/){
		float degree = 0.00;
		if(direction == 0/*X direction*/){
			float input[] = {x, y, z, degree, 0+ang, -180};
			memcpy(m_stopNumber[0][stationNum], input, 6*sizeof(float));
			//m_stopNumber[0][] = {x+height*sin(HDEG(ang)), y, z-height*(1-cos(HDEG(ang))), degree, 0+ang, -180};
		}
		else if(direction == 1)/*Y direction*/{
			ang = -ang;
			if(ang >=0){
				float input[] = {x, y, z, degree, 0, -(180-ang)};
				memcpy(m_stopNumber[1][stationNum], input, 6*sizeof(float));
			}
			else{ 
				float input[] = {x, y, z, degree, 0, 180+ang};
				memcpy(m_stopNumber[1][stationNum], input, 6*sizeof(float));
			}
		}
		
	}else/*侧倾法*/{
		float degree = -90.0;
		if(direction == 1/*Y direction*/){ 
			float input[] = {x, y, z, degree, -ang, -180};
			memcpy(m_stopNumber[3][stationNum], input, 6*sizeof(float));
		}
		else if (direction == 0)/*X direction*/{
			if(ang >= 0){ 
				float input[] = {x, y, z, degree, 0, (180-ang)};
				memcpy(m_stopNumber[2][stationNum], input, 6*sizeof(float));
			}else{
				float input[] = {x, y, z, degree, 0, -180-ang};
				memcpy(m_stopNumber[2][stationNum], input, 6*sizeof(float));
			}
		}
		
	}
}

void CNachiSocket::calculatePositionRequst(float* stopNumber, int stationNum, float x, float y, float z, float ang, int method, int direction, int height)
{
	if (height > 400)
	{
		height = m_height;
	}
	if(method == 0/*同倾法*/){
		float degree = 0.00;
		if(direction == 0/*X direction*/){
			float input[] = {x, y, z, degree, 0+ang, -180};
			memcpy(stopNumber+0*6*8+stationNum*6, input, 6*sizeof(float));
		}
		else if(direction == 1)/*Y direction*/{
			ang = -ang;
			if(ang >=0){
				float input[] = {x, y, z, degree, 0, -(180-ang)};
				memcpy(stopNumber+1*6*8+stationNum*6, input, 6*sizeof(float));
			}
			else{ 
				float input[] = {x, y, z, degree, 0, 180+ang};
				memcpy(stopNumber+1*6*8+stationNum*6, input, 6*sizeof(float));
			}
		}
		
	}else/*侧倾法*/{
		float degree = -90.0;
		if(direction == 1/*Y direction*/){ 
			float input[] = {x, y, z, degree, -ang, -180};
			memcpy(stopNumber+3*6*8+stationNum*6, input, 6*sizeof(float));
		}
		else if (direction == 0)/*X direction*/{
			if(ang >= 0){ 
				float input[] = {x, y, z, degree, 0, (180-ang)};
				memcpy(stopNumber+2*6*8+stationNum*6, input, 6*sizeof(float));
			}else{
				float input[] = {x, y, z, degree, 0, -180-ang};
				memcpy(stopNumber+2*6*8+stationNum*6, input, 6*sizeof(float));
			}
		}	
	}
}

void CNachiSocket::calculatePosAll(int stationNum, float x, float y, float z, float ang, int height)
{
	//first test for existing
	
	//no existing then calculate all the position
	calculatePosition(stationNum, x, y, z, ang, 0, 0, height);
	calculatePosition(stationNum, x, y, z, ang, 0, 1, height);
	calculatePosition(stationNum, x, y, z, ang, 1, 0, height);
	calculatePosition(stationNum, x, y, z, ang, 1, 1, height);
	m_isCalculated = 1;
}


void CNachiSocket::calculatePosAllRequest(float* stopNumber, int stationNum, float x, float y, float z, float ang, int height)
{
	calculatePositionRequst(stopNumber, stationNum, x, y, z, ang, 0, 0, height);
	calculatePositionRequst(stopNumber, stationNum, x, y, z, ang, 0, 1, height);
	calculatePositionRequst(stopNumber, stationNum, x, y, z, ang, 1, 0, height);
	calculatePositionRequst(stopNumber, stationNum, x, y, z, ang, 1, 1, height);
}


bool CNachiSocket::nachiStressBasicMotionAdjusted(float x, float y, float z, int stationNumber, float startAng, float endAng, int divide, int method, int direction, int height)
{
	printf("dilapidated!");
	return true;
	if(isConnected == false){
		printf("没有连接机器人！");
		return true;
	}
	if (height>400)
	{
		height = m_height;
	}
	float next_angle;
	float input[6];
	int directionMode;
	if(m_method == 0/*同倾法*/){
		if (m_direction == 0)
		{
			directionMode = 0;
		}
		else if(m_direction == 1)/*Y direction*/{
			directionMode = 1;
		}
	}
	else/*侧倾法*/{
			if (m_direction == 1)
			{
				directionMode = 3;
			}
			else if (m_direction == 0)/*X direction*/{
				directionMode = 2;
			}
	}

	int flag = 0;
	if(method == 0/*同倾法*/){
		float degree = 0.00;
		if(direction == 0/*X direction*/){
			float input_tmp[] = {x+height*sin(HDEG(endAng)), y, z-height*(1-cos(HDEG(endAng))), degree, 0+endAng, -180};
			memcpy(input, input_tmp, 6*sizeof(float));
		}
		else if(direction == 1)/*Y direction*/{
			float endAng1 = -endAng;
			if(endAng1 >=0){
				float input_tmp[] = {x, y-height*sin(HDEG(endAng1)), z-height*(1-cos(HDEG(endAng1))), degree, 0, -(180-endAng1)};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
			else{ 
				float input_tmp[] = {x, y-height*sin(HDEG(endAng1)), z-height*(1-cos(HDEG(endAng1))), degree, 0, 180+endAng1};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
		}
		
	}else/*侧倾法*/{
		float degree = -90.0;
		if(direction == 1/*Y direction*/){ 
			float input_tmp[] = {x, y+height*sin(HDEG(endAng)), z-height*(1-cos(HDEG(endAng))), degree, -endAng, -180};
			memcpy(input, input_tmp, 6*sizeof(float));
			//input = ;
		}
		else if (direction == 0)/*X direction*/{
			if(endAng >= 0){ 
				float input_tmp[] = {x+height*sin(HDEG(endAng)), y, z-height*(1-cos(HDEG(endAng))), degree, 0, (180-endAng)};
				memcpy(input, input_tmp, 6*sizeof(float));
			}else{
				float input_tmp[] =  {x+height*sin(HDEG(endAng)), y, z-height*(1-cos(HDEG(endAng))), degree, 0, -180-endAng};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
		}
	}	
	for(int ii=0; ii<3; ii++){
		if(abs(m_stopNumber[directionMode][stationNumber][ii]-input[ii]) > 10){
			flag = 1;
		}
	}
	for(int j=0; j<3; j++){
		if(abs(m_stopNumber[directionMode][stationNumber][j+3]-input[j+3]) > 5){
			flag = 1;
		}
	}
	
	if (flag == 1)
	{
		//printf("final_step cannot added!");
		nachiSendMoveReady(divide);
	} 
	else
	{
		nachiSendMoveReady(divide+1);
	}
	

	for(int i=0; i<divide; i++){
		next_angle = (endAng - startAng)/((float)divide)*(i+1.0)+startAng;
		if(method == 0/*同倾法*/){
			float degree = 0.00;
			if(direction == 0/*X direction*/){
				float input_tmp[] = {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0+next_angle, -180};
				memcpy(input, input_tmp, 6*sizeof(float));
				nachiSendSinglePoint(input, i+1);
			}
			else if(direction == 1)/*Y direction*/{
				next_angle = -next_angle;
				if(next_angle >=0){
					float input_tmp[] = {x, y-height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, 0, -(180-next_angle)};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input = ;
					nachiSendSinglePoint(input, i+1);
				}
				else{ 
					float input_tmp[] = {x, y-height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, 0, 180+next_angle};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input = ;
					nachiSendSinglePoint(input, i+1);
				}
			}
			
		}else/*侧倾法*/{
			float degree = -90.0;
			if(direction == 1/*Y direction*/){ 
				float input_tmp[] = {x, y+height*sin(HDEG(next_angle)), z-height*(1-cos(HDEG(next_angle))), degree, -next_angle, -180};
				memcpy(input, input_tmp, 6*sizeof(float));
				//input = ;
				nachiSendSinglePoint(input, i+1);
			}
			else if (direction == 0)/*X direction*/{
				if(next_angle >= 0){ 
					float input_tmp[] = {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0, (180-next_angle)};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input = ;
					nachiSendSinglePoint(input, i+1);
				}else{
					float input_tmp[] =  {x+height*sin(HDEG(next_angle)), y, z-height*(1-cos(HDEG(next_angle))), degree, 0, -180-next_angle};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input =;
					nachiSendSinglePoint(input, i+1);
				}
			}	
		}
	}

	
	if (flag == 1)
	{
		printf("final_step cannot added!");
	} 
	else
	{
		nachiSendSinglePoint(m_stopNumber[directionMode][stationNumber], divide+1);
	}
	return true;
}


bool CNachiSocket::nachiIsChangedPos(float curX, float curY, float curZ)
{
	float r_curX, r_curY, r_curZ;
	nachiGetPosStateXYZ(r_curX, r_curY, r_curZ);
	//nachiPosState();
	if (curX < (r_curX-0.05) || curX > (r_curX+0.05))
		{
		return true;
	}
	if (curY < (r_curY-0.05) || curY > (r_curY+0.05))
	{
		return true;
	}
	if (curZ < (r_curZ-0.05) || curZ > (r_curZ+0.05))
	{
		return true;
	}
	return false;
}

void CNachiSocket::saveStopNumber(int method, int direction, double stop1, double stop2, double stop3, double stop4, double stop5, double stop6, double stop7)
{
}

bool CNachiSocket::nachiStressBasicMotionDirect(float x, float y, float z, float startAng, float endAng, int divide, int method, int direction)
{
	if(isConnected == false){
		printf("没有连接机器人！");
		return true;
	}
	float next_angle;
	nachiSendMoveReady(divide);
	for(int i=0; i<divide; i++){
		next_angle = (endAng - startAng)/((float)divide)*(i+1.0)+startAng;
		if(method == 0/*同倾法*/){
			float degree = 0.00;
			if(direction == 0/*X direction*/){
				float input[] = {x, y, z, degree, 0+next_angle, -180};
				nachiSendSinglePoint(input, i+1);
			}
			else if(direction == 1)/*Y direction*/{
				next_angle = -next_angle;
				if(next_angle >=0){
					float input[] = {x, y, z, degree, 0, -(180-next_angle)};
					nachiSendSinglePoint(input, i+1);
				}
				else{ 
					float input[] = {x, y, z, degree, 0, 180+next_angle};
					nachiSendSinglePoint(input, i+1);
				}
			}

		}else/*侧倾法*/{
			float degree = -90.0;
			if(direction == 1/*Y direction*/){ 
				float input[] = {x, y, z, degree, -next_angle, -180};
				nachiSendSinglePoint(input, i+1);
			}
			else if (direction == 0)/*X direction*/{
				if(next_angle >= 0){ 
					float input[] = {x, y, z, degree, 0, (180-next_angle)};
					nachiSendSinglePoint(input, i+1);
				}else{
					float input[] = {x, y, z, degree, 0, -180-next_angle};
				nachiSendSinglePoint(input, i+1);
				}
			}

		}
	}
	return true;
}

bool CNachiSocket::nachiStressBasicMotionDirectAdjusted(float x, float y, float z, int stationNumber, float startAng, float endAng, int divide, int method, int direction)
{
	if(isConnected == false){
		printf("没有连接机器人！");
		return true;
	}
	float next_angle;
	float input[6];
	int directionMode;
	if(m_method == 0/*同倾法*/){
		if (m_direction == 0)
		{
			directionMode = 0;
		}
		else if(m_direction == 1)/*Y direction*/{
			directionMode = 1;
		}
	}
	else/*侧倾法*/{
			if (m_direction == 1)
			{
				directionMode = 3;
			}
			else if (m_direction == 0)/*X direction*/{
				directionMode = 2;
			}
	}

	int flag = 0;
	if(method == 0/*同倾法*/){
		float degree = 0.00;
		if(direction == 0/*X direction*/){
			float input_tmp[] = {x, y, z, degree, 0+endAng, -180};
			memcpy(input, input_tmp, 6*sizeof(float));
		}
		else if(direction == 1)/*Y direction*/{
			float endAng1 = -endAng;
			if(endAng1 >=0){
				float input_tmp[] = {x, y, z, degree, 0, -(180-endAng1)};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
			else{ 
				float input_tmp[] = {x, y, z, degree, 0, 180+endAng1};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
		}
		
	}else/*侧倾法*/{
		float degree = -90.0;
		if(direction == 1/*Y direction*/){ 
			float input_tmp[] = {x, y, z, degree, -endAng, -180};
			memcpy(input, input_tmp, 6*sizeof(float));
			//input = ;
		}
		else if (direction == 0)/*X direction*/{
			if(endAng >= 0){ 
				float input_tmp[] = {x, y, z, degree, 0, (180-endAng)};
				memcpy(input, input_tmp, 6*sizeof(float));
			}else{
				float input_tmp[] =  {x, y, z, degree, 0, -180-endAng};
				memcpy(input, input_tmp, 6*sizeof(float));
			}
		}
	}	
	for(int ii=0; ii<3; ii++){
		if(abs(m_stopNumber[directionMode][stationNumber][ii]-input[ii]) > 10){
			flag = 1;
		}
	}
	for(int j=0; j<3; j++){
		if(abs(m_stopNumber[directionMode][stationNumber][j+3]-input[j+3]) > 5){
			flag = 1;
		}
	}
	
	if (flag == 1)
	{
		//printf("final_step cannot added!");
		nachiSendMoveReady(divide);
	} 
	else
	{
		nachiSendMoveReady(divide+1);
	}
	
	
	if (flag == 1)
	{
		for(int i=0; i<divide; i++){
			next_angle = (endAng - startAng)/((float)divide)*(i+1.0)+startAng;
			if(method == 0/*同倾法*/){
				float degree = 0.00;
				if(direction == 0/*X direction*/){
					float input_tmp[] = {x, y, z, degree, 0+next_angle, -180};
					memcpy(input, input_tmp, 6*sizeof(float));
					nachiSendSinglePoint(input, i+1);
				}
				else if(direction == 1)/*Y direction*/{
					next_angle = -next_angle;
					if(next_angle >=0){
						float input_tmp[] = {x, y, z, degree, 0, -(180-next_angle)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}
					else{ 
						float input_tmp[] = {x, y, z, degree, 0, 180+next_angle};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}
				}
				
			}else/*侧倾法*/{
				float degree = -90.0;
				if(direction == 1/*Y direction*/){ 
					float input_tmp[] = {x, y, z, degree, -next_angle, -180};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input = ;
					nachiSendSinglePoint(input, i+1);
				}
				else if (direction == 0)/*X direction*/{
					if(next_angle >= 0){ 
						float input_tmp[] = {x, y, z, degree, 0, (180-next_angle)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}else{
						float input_tmp[] =  {x, y, z, degree, 0, -180-next_angle};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input =;
						nachiSendSinglePoint(input, i+1);
					}
				}	
			}
		}
		printf("final_step cannot added!");
	} 
	else
	{
		float dis_x = m_stopNumber[directionMode][stationNumber][0] - x;
		float dis_y = m_stopNumber[directionMode][stationNumber][1] - y;
		float dis_z = m_stopNumber[directionMode][stationNumber][2] - z;
		float dis_ang1 = 0.0;
		float dis_ang2 = 0.0;
		float dis_ang3 = 0.0;
		for(int i=0; i<divide; i++){
			next_angle = (endAng - startAng)/((float)divide)*(i+1.0)+startAng;
			if(method == 0/*同倾法*/){
				float degree = 0.00;
				if(direction == 0/*X direction*/){
					dis_ang1 = m_stopNumber[directionMode][stationNumber][4]-(endAng+0);
					dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
					dis_ang3 = m_stopNumber[directionMode][stationNumber][5]-(-180);
					float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+next_angle+dis_ang1*((float)(i+1))/((float)divide), -180+dis_ang3*((float)(i+1))/((float)divide)};
					memcpy(input, input_tmp, 6*sizeof(float));
					nachiSendSinglePoint(input, i+1);
					/*if(stationNumber == 0){
						dis_ang1 = m_stopNumber[directionMode][stationNumber][4]-(endAng+0);
						dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
						dis_ang3 = m_stopNumber[directionMode][stationNumber][5]-(-180);
						float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+next_angle+dis_ang1*((float)(i+1))/((float)divide), -180+dis_ang3*((float)(i+1))/((float)divide)};
						memcpy(input, input_tmp, 6*sizeof(float));
						nachiSendSinglePoint(input, i+1);
					}
					else{
						float update1 = (m_stopNumber[directionMode][stationNumber][0] - m_stopNumber[directionMode][stationNumber-1][0])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][0];
						float update2 = (m_stopNumber[directionMode][stationNumber][1] - m_stopNumber[directionMode][stationNumber-1][1])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][1];
						float update3 = (m_stopNumber[directionMode][stationNumber][2] - m_stopNumber[directionMode][stationNumber-1][2])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][2];
						float update4 = (m_stopNumber[directionMode][stationNumber][3] - m_stopNumber[directionMode][stationNumber-1][3])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][3];
						float update5 = (m_stopNumber[directionMode][stationNumber][4] - m_stopNumber[directionMode][stationNumber-1][4])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][4];
						float update6 = (m_stopNumber[directionMode][stationNumber][5] - m_stopNumber[directionMode][stationNumber-1][5])*((float)(i+1))/((float)divide)+m_stopNumber[directionMode][stationNumber-1][5];
						//printf("\n %f-%f-%f-%f-%f-%f", update1, update2, update3, update4, update5, update6);
						float input_tmp[] = {update1, update2, update3, update4, update5, update6};
						memcpy(input, input_tmp, 6*sizeof(float));
						nachiSendSinglePoint(input, i+1);
					}*/
					
				}
				else if(direction == 1)/*Y direction*/{
					next_angle = -next_angle;
					if(next_angle >=0){
						dis_ang1 = (m_stopNumber[directionMode][stationNumber][5]-(-(180+endAng)));
						if (dis_ang1 > 180.0){
							dis_ang1 = 360.0 - dis_ang1;
						}
						else if(dis_ang1 < -180.0){
							dis_ang1 = 360.0 + dis_ang1;
						}
						dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
						dis_ang3 = m_stopNumber[directionMode][stationNumber][4]-(0);
						float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+dis_ang3*((float)(i+1))/((float)divide), -(180-next_angle)+dis_ang3*((float)(i+1))/((float)divide)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//printf("\n %f-%f", 180+next_angle, dis_ang1);
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}
					else{ 
						dis_ang1 = (m_stopNumber[directionMode][stationNumber][5]-(180-endAng));
						if (dis_ang1 > 180.0){
							dis_ang1 = 360.0 - dis_ang1;
						}
						else if(dis_ang1 < -180.0){
							dis_ang1 = 360.0 + dis_ang1;
						}
						dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
						dis_ang3 = m_stopNumber[directionMode][stationNumber][4]-(0);
						float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+dis_ang3*((float)(i+1))/((float)divide), 180+next_angle+dis_ang3*((float)(i+1))/((float)divide)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//printf("\n %f_%f_%f_%f", 180+next_angle, dis_ang1, 180-endAng, m_stopNumber[directionMode][stationNumber][5]);
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}
				}
				
			}else/*侧倾法*/{
				float degree = -90.0;
				if(direction == 1/*Y direction*/){ 
					dis_ang1 = m_stopNumber[directionMode][stationNumber][4]-(-endAng);
					dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
					dis_ang3 = m_stopNumber[directionMode][stationNumber][5]-(-180);
					float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), -next_angle+dis_ang1*((float)(i+1))/((float)divide), -180+dis_ang3*((float)(i+1))/((float)divide)};
					memcpy(input, input_tmp, 6*sizeof(float));
					//input = ;
					nachiSendSinglePoint(input, i+1);
				}
				else if (direction == 0)/*X direction*/{
					if(next_angle >= 0){ 
						dis_ang1 = m_stopNumber[directionMode][stationNumber][5]-((180-endAng));
						if (dis_ang1 > 180.0){
							dis_ang1 = 360.0 - dis_ang1;
						}
						else if(dis_ang1 < -180.0){
							dis_ang1 = 360.0 + dis_ang1;
						}
						dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
						dis_ang3 = m_stopNumber[directionMode][stationNumber][4]-(0);
						float input_tmp[] = {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+dis_ang3*((float)(i+1))/((float)divide), (180-next_angle)+dis_ang1*((float)(i+1))/((float)divide)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input = ;
						nachiSendSinglePoint(input, i+1);
					}else{
						dis_ang1 = m_stopNumber[directionMode][stationNumber][5]-(-180-endAng);
						if (dis_ang1 > 180.0){
							dis_ang1 = 360.0 - dis_ang1;
						}
						else if(dis_ang1 < -180.0){
							dis_ang1 = 360.0 + dis_ang1;
						}
						dis_ang2 = m_stopNumber[directionMode][stationNumber][3]-degree;
						dis_ang3 = m_stopNumber[directionMode][stationNumber][4]-(0);
						float input_tmp[] =  {x+dis_x*((float)(i+1))/((float)divide), y+dis_y*((float)(i+1))/((float)divide), z+dis_z*((float)(i+1))/((float)divide), degree+dis_ang2*((float)(i+1))/((float)divide), 0+dis_ang3*((float)(i+1))/((float)divide), -180-next_angle+dis_ang1*((float)(i+1))/((float)divide)};
						memcpy(input, input_tmp, 6*sizeof(float));
						//input =;
						nachiSendSinglePoint(input, i+1);
					}
				}	
			}
		}
		nachiSendSinglePoint(m_stopNumber[directionMode][stationNumber], divide+1);
	}
	return true;
}
