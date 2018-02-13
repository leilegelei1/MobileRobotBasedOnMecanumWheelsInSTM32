#include "kinematics.h"
#include "usart.h"

#define pi 3.1415926

float velocity[3];

float matrix[4][3];

float Matrix[4][3]={{-190.9859,190.9859,103.1324},{190.9859,190.9859,-103.1324},{-190.9859,190.9859,-103.1324},
						{190.9859,190.9859,103.1324}};

float Speed[4];

u8 cflag;

void MatrixInit()
{
	matrix[0][0]=-1/4;
	matrix[0][1]=1/4;
	matrix[0][2]=LengthA/4 + LengthB/4;
	
	matrix[1][0]=1/4;
	matrix[1][1]=1/4;
	matrix[1][2]=-1*LengthA/4 - LengthB/4;
	
	matrix[2][0]=-1/4;
	matrix[2][1]=1/4;
	matrix[2][2]=-1*LengthA/4 - LengthB/4;
	
	matrix[3][0]=1/4;
	matrix[3][1]=1/4;
	matrix[3][2]=LengthA/4 + LengthB/4;
	
}

int pwmold[3]={1500,1500,1500};
int pwmnew[3]={1500,1500,1500};
u8 cflag;

void GetVelocity()
{
	//printf("%d %d %d\r\n",pwmout1,pwmout2,pwmout3);
	pwmnew[0]=pwmout1;
	pwmnew[1]=pwmout2;
	pwmnew[2]=pwmout3;
	for(cflag=0;cflag<3;cflag++)
	{
		if(pwmnew[cflag]<0||pwmnew[cflag]>4000)
			pwmnew[cflag]=pwmold[cflag];
	}
	for(cflag=0;cflag<3;cflag++)
		pwmold[cflag]=pwmnew[cflag];
	
	//printf("%d %d %d\r\n",pwmnew[0],pwmnew[1],pwmnew[2]);
	
//	velocity[0] = (pwmnew[0]-1500)*0.00112199;
//	velocity[1] = (pwmnew[1]-1500)*0.00112199;
//  velocity[2] = (pwmnew[2]-1500)*0.00207777;
//	
//	if(pwmnew[0]>1420&&pwmnew[0]<1580)
//		velocity[0]=0;
//	if(pwmnew[1]>1420&&pwmnew[1]<1580)
//		velocity[1]=0;
//	if(pwmnew[2]>1420&&pwmnew[2]<1580)
//		velocity[2]=0;
	
		velocity[0]=(pwmnew[0]-2000)*0.00025;
		velocity[1]=(pwmnew[1]-2000)*0.00025;
				//justtest=USART_RX_BUF[0]*256+USART_RX_BUF[1];
		velocity[2]=(pwmnew[2]-2000)*0.00025;
	
		if(pwmnew[0]>1900&&pwmnew[0]<2100)
		velocity[0]=0;
	if(pwmnew[1]>1900&&pwmnew[1]<2100)
		velocity[1]=0;
	if(pwmnew[2]>1900&&pwmnew[2]<2100)
		velocity[2]=0;
//	velocity[0] = ((pwmnew[0]-1500)/420)*0.47123889;
//	velocity[1] = ((pwmnew[1]-1500)/420)*0.47123889;
//  velocity[2] = ((pwmnew[2]-1500)/420)*0.87266461;
	//printf("%d %f \r\n",pwmnew[0],velocity[0]);
	//printf("%d %d %d\r\n",pwmnew[0],pwmnew[1],pwmnew[2]);
//	if(pwmnew[0]>900&&pwmnew[0]<1100)
//		velocity[0]=0;
//	if(pwmnew[1]>900&&pwmnew[1]<1100)
//		velocity[1]=0;
//	if(pwmnew[2]>900&&pwmnew[2]<1100)
//		velocity[2]=0;
}

void GetDisire()
{	
	MyDisire.RightForward = 0;
	MyDisire.LeftForward = 0;
	MyDisire.LeftBehind = 0;
	MyDisire.RightBehind = 0;
	
	Speed[0] = Matrix[0][0]*velocity[1] + Matrix[0][1]*velocity[0] + Matrix[0][2]*velocity[2];
	Speed[1] = Matrix[1][0]*velocity[1] + Matrix[1][1]*velocity[0] + Matrix[1][2]*velocity[2];
	Speed[2] = Matrix[2][0]*velocity[1] + Matrix[2][1]*velocity[0] + Matrix[2][2]*velocity[2];
	Speed[3] = Matrix[3][0]*velocity[1] + Matrix[3][1]*velocity[0] + Matrix[3][2]*velocity[2];
	
	MyDisire.RightForward = Speed[0];
	MyDisire.LeftForward =  Speed[1];
	MyDisire.LeftBehind =  Speed[2];
	MyDisire.RightBehind = Speed[3];
	
//	MyDisire.RightForward =  matrix[0][0]*velocity[0] + matrix[0][1]*velocity[1] + matrix[0][2]*velocity[2];
//	MyDisire.LeftForward =  matrix[1][0]*velocity[0] + matrix[1][1]*velocity[1] + matrix[1][2]*velocity[2];
//	MyDisire.LeftBehind =  matrix[2][0]*velocity[0] + matrix[2][1]*velocity[1] + matrix[2][2]*velocity[2];
//	MyDisire.RightBehind =  matrix[3][0]*velocity[0] + matrix[3][1]*velocity[1] + matrix[3][2]*velocity[2];
//	
//	MyDisire.RightForward =  MyDisire.RightForward/2/pi*60;
//	MyDisire.LeftForward = MyDisire.LeftForward/2/pi*60 ;
//	MyDisire.LeftBehind = MyDisire.LeftBehind/2/pi*60 ;
//	MyDisire.RightBehind = MyDisire.RightBehind/2/pi*60 ;
	//printf("%d %d\r\n",MyDisire.RightForward,MyDisire.LeftForward);
	//printf("%d %d %d %d \r\n",MyDisire.RightForward,MyDisire.LeftForward,MyDisire.LeftBehind,MyDisire.RightBehind);
}

