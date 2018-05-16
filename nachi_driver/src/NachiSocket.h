#if !defined(AFX_NACHISOCKET_H__445ABEDC_DF94_43DD_BAB8_F80B9668A225__INCLUDED_)
#define AFX_NACHISOCKET_H__445ABEDC_DF94_43DD_BAB8_F80B9668A225__INCLUDED_
#define MAX_PATH 1024
//#include <winsock2.h>   // Requires wsock32.lib
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CNachiSocket view
// global variables
const int MAXMODULES1 = 1;   // maximum number of supported modules
const int NMAXCHANNELS1 = 1024;  // maximum channels per module
const int socketBufferSize1 = MAXMODULES1*NMAXCHANNELS1*sizeof(int);

class CNachiSocket
{
public:
	CNachiSocket();           // protected constructor used by dynamic creation
	~CNachiSocket(); 
	// Attributes
public:
	char socketBuffer[socketBufferSize1];
	//int sockfd, connection, len, nmodules, nChannels[MAXMODULES1];
	int sock_cli;
        struct sockaddr_in servaddr;
	char pFileName[MAX_PATH];
	
	// Operations
	public:
                bool nachiClose();
		bool nachiStressBasicMotionDirectAdjusted(float x, float y, float z, int stationNumber, float startAng, float endAng, int divide, int method, int direction);
		bool nachiStressBasicMotionDirect(float x, float y, float z, float startAng, float endAng, int divide, int method, int direction);
		void saveStopNumber(int method, int direction, double stop1, double stop2, double stop3, double stop4, double stop5, double stop6, double stop7);
		bool nachiIsChangedPos(float curX, float curY, float curZ);
		bool m_ifCanTestAdjusted;
		void calculatePosAllRequest(float* stopNumber, int stationNum, float x, float y, float z, float ang, int height=999);
		void calculatePositionRequst(float* stopNumber, int stationNum, float x, float y, float z, float ang, int method, int direction, int height=999);
		bool nachiStressBasicMotionAdjusted(float x, float y, float z, int stationNumber, float startAng, float endAng, int divide, int method, int direction, int height=999);
		void calculatePosAll(int stationNum, float x, float y, float z, float ang, int height=999);
		int m_isCalculated;
		float m_height;
		bool nachiIsRightPos();
		int m_direction;
		int m_method;
		bool isConnected;
		void nachiSendRealTimeStop();
                void nachiSendRealTimeSingleDirStep2(float Xdistance, float Ydistance, float Zdistance);
                void nachiSendRealTimeSingleDirStep1(int time1, int time2, float speed);
		bool nachiStressBasicMotion(float x, float y, float z, float startAng, float endAng, int divide, int method, int direction, int height=999);
		void calculatePosition(int stationNum, float x, float y, float z, float ang, int method, int direction, int height=999);
                float findValue(string begin_string, string end_string);
		bool nachiPosState();
		bool nachiGetPosStateXYZ(float& curX, float& curY, float& curZ);
		bool nachiBeginMove(float speed = 3.0);
		bool nachiSendMoveReady(int steps);
		bool nachiSendSinglePoint(float input[6], int index);
		bool init();
		bool send_arg(const char *message);
		bool receive(int length=socketBufferSize1);
		double m_curX, m_curY, m_curZ, m_curA, m_curB, m_curC;
		float m_stopNumber[4][8][6];
                bool nachiHandHandle(int Open);

		
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NACHISOCKET_H__445ABEDC_DF94_43DD_BAB8_F80B9668A225__INCLUDED_)
