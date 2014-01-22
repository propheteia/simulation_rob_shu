#ifndef DRAWING_H
#define DRAWING_H
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <math.h>
#include "rvdraw.h"
#include <iostream>
#include "vector.h"
#include <deque>


#define ROBOVIS_HOST "localhost"
#define ROBOVIS_PORT "32769"
#define TEST_DURATION 10000

using namespace std;
class Drawing
{
    public:
	Drawing();
	~Drawing();
	void swapBuffers(const string* setName);
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float thickness, float r, float g, float b,
		      const string* setName);
	void drawCircle(float x, float y, float radius, float thickness, float r, float g, float b, const string* setName);
	void drawSphere(float x, float y, float z, float radius, float r, float g, float b, const string* setName);
	void drawPoint(float x, float y, float z, float size, float r, float g, float b, const string* setName);
	void drawPolygon(const float* v, int numVerts, float r, float g, float b,
	    float a, const string* setName);
	void renderAnimatedShapes(salt::Vector3f pos);
	float maxf(float a, float b);
	void renderStaticShapes();
	void runTest();
	int connect();
	void draw_trajectory(salt::Vector3f pos);
    private:
        struct addrinfo hints, *servinfo;
	int sockfd;
	struct addrinfo* p;
	double angle;
	Rvdraw* rd;
	std::deque<salt::Vector3f> traj;
};
#endif