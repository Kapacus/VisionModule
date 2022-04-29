#ifndef LIBRARY_H
#define LIBRARY_H
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>
#include <XnFPSCalculator.h>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

#define SAMPLE_XML_PATH "../Config/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"
#define MAX_NUM_USERS 3//Changed from 15 //Skeleton
//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------
using namespace xn;



class libr{
//private:
	
public:
	//Common	
	XnStatus nRetVal;	
	EnumerationErrors errors;

	//Depth	
	Context context;
	ScriptNode scriptNode;
	DepthGenerator depth;
	DepthMetaData depthMD;

	//Skeleton
	Context g_Context;
	ScriptNode g_scriptNode;
	XnUserID aUsers[MAX_NUM_USERS];
    	XnUInt16 nUsers;
	XnSkeletonJointTransformation torsoJoint;

	//void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);	
	//void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);
	//void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/);
	//void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/);
	//void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/);	
		
	int initDepth();
	int initSkeleton();
	int SkeletonHeadposition();
	int DepthMiddle();
	int DepthResolution();
	int DepthPoint(int x , int y);
	int DepthClose();
	int SkeletonClose();
	unsigned int output_int[6];
	float output_float[6][3];
	int FrameID; //Used in middle();
 	float FPS;  //Used in middle(); 	
		
};


#endif
