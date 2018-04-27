#include <cstdio>
#include <Tchar.h>
#include <windows.h>

#include "public.h"
#include "vjoyinterface.h"

int main(){
	using namespace std;
	
	char name[20];
	
	printf("Enter Communications port name (eg. COM7): ");
	scanf("%s", name);
	
	printf("Attempting to open a serial connection on port name: %s\n", name);
	
	HANDLE comm = CreateFile(name, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	DCB opt;
	
	//Get the current settings for the serial connection
	if(!GetCommState(comm, &opt)){
		fprintf(stderr, "Failed to retrieve settings from Serial Connection\n");
		CloseHandle(comm);
		return 1;
	}
	
	//Set options for port communication
	opt.BaudRate = CBR_9600;
	opt.ByteSize = 8;
	opt.Parity = NOPARITY;
	opt.StopBits = ONESTOPBIT;
	
	//Attempt to apply the options
	if(!SetCommState(comm, &opt)){
		fprintf(stderr, "Failed to apply settings from Serial Connection\n");
		CloseHandle(comm);
		return 1;
	}
	
	//Prepare Serial event handling
	DWORD commStat;
	SetCommMask(comm, EV_RXCHAR | EV_ERR);
	
	
	//Ensure that Vjoy is installed and enabled
	if (!vJoyEnabled()){
		_tprintf("Failed Getting vJoy attributes.\n");
		return -2;
	}else{
		_tprintf("Vendor: %S\nProduct :%S\nVersion Number:%S\n",
			TEXT(GetvJoyManufacturerString()),
			TEXT(GetvJoyProductString()),
			TEXT(GetvJoySerialNumberString()));
	};
	
	//Ensure that the installed version of vjoy is compatible with the available vjoy dll
	WORD VerDll, VerDrv;
	if (!DriverMatch(&VerDll, &VerDrv)){
		_tprintf("Failed\r\nvJoy Driver (version %04x) does not match vJoyInterface DLL(version %04x)\n", VerDrv ,VerDll);
	}
	else{
		_tprintf( "OK - vJoy Driver and vJoyInterface DLL match vJoyInterface DLL (version%04x)\n", VerDrv);
	}
	
	UINT iInterface = 1; //ID of the vJoy device to connect to
	_JOYSTICK_POSITION iReport;
	iReport.bDevice = (BYTE)iInterface;
	
	BYTE in[3];
	DWORD bytesRead = 0;
	
	AcquireVJD(iInterface);
	
	WriteFile(comm, "x", 1, &bytesRead, NULL);
	
	PurgeComm(comm, PURGE_RXCLEAR);
	
	do{
		WaitCommEvent(comm, &commStat, 0);
		ReadFile(comm, &in, 8, &bytesRead, 0);
		if(bytesRead != 8){
			CloseHandle(comm);
			return 1;
		}
		iReport.wAxisX = (long)in[0];
		iReport.wAxisY = (long)in[1];
		iReport.wAxisZ = (long)in[2];
		iReport.wAxisXRot = (long)in[3];
		iReport.wAxisYRot = (long)in[4];
		iReport.wAxisZRot = (long)in[5];
		iReport.wSlider = (long)in[6];
		iReport.lButtons = (long)in[7];
		//printf("(%d, %d)  -  %d\n", in[0], in[1], in[2]);
		//printf("%ld\n", (LONG)in);
		UpdateVJD(iInterface, &iReport);
	}while(!(commStat & EV_ERR));
	
	CloseHandle(comm);
	return 0;
}
