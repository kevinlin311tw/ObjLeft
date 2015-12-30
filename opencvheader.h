#ifndef _OPENCV_HEADER_H_
#define _OPENCV_HEADER_H_

#include <opencv2/opencv.hpp> // new stlye start from opencv2.4.2 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
//#include <opencv/cvaux.hpp>
#include <opencv2/ml/ml.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"


// #include "cv.h" // include standard OpenCV headers, same as before
// #include "cxcore.h"
// #include "highgui.h"
// #include "cvaux.h"
// #include "ml.h"


#if CV_MAJOR_VERSION == 1
	#if CV_MINOR_VERSION == 2
		#pragma comment(lib,"cv120.lib")
		#pragma comment(lib,"ml120.lib")
		#pragma comment(lib,"highgui120.lib")
		#pragma comment(lib,"cxcore120.lib")
		#pragma comment(lib,"cvaux120.lib")
	#else
		#pragma comment(lib,"cv.lib")
		#pragma comment(lib,"ml.lib")
		#pragma comment(lib,"highgui.lib")
		#pragma comment(lib,"cxcore.lib")
		#pragma comment(lib,"cvaux.lib")
		#pragma comment(lib,"ffopencv.lib")
	#endif
#elif CV_MAJOR_VERSION == 2
	#if CV_MINOR_VERSION == 0
			#if _DEBUG
				#pragma comment(lib,"cv200d.lib")
				#pragma comment(lib,"ml200d.lib")
				#pragma comment(lib,"highgui200d.lib")
				#pragma comment(lib,"cxcore200d.lib")
				#pragma comment(lib,"cvaux200d.lib")
			#else
				#pragma comment(lib,"cv200.lib")
				#pragma comment(lib,"ml200.lib")
				#pragma comment(lib,"highgui200.lib")
				#pragma comment(lib,"cxcore200.lib")
				#pragma comment(lib,"cvaux200.lib")
			#endif
	#elif CV_MINOR_VERSION == 1
		#if CV_SUBMINOR_VERSION == 0
			#if _DEBUG
				#pragma comment(lib,"cv210d.lib")
				#pragma comment(lib,"ml210d.lib")
				#pragma comment(lib,"highgui210d.lib")
				#pragma comment(lib,"cxcore210d.lib")
				#pragma comment(lib,"cvaux210d.lib")
			#else
				#pragma comment(lib,"cv210.lib")
				#pragma comment(lib,"ml210.lib")
				#pragma comment(lib,"highgui210.lib")
				#pragma comment(lib,"cxcore210.lib")
				#pragma comment(lib,"cvaux210.lib")
			#endif
		#elif CV_SUBMINOR_VERSION == 1
			#pragma comment(lib,"opencv_calib3d211.lib")
			#pragma comment(lib,"opencv_contrib211.lib")
			#pragma comment(lib,"opencv_core211.lib")
			#pragma comment(lib,"opencv_features2d211.lib")
			#pragma comment(lib,"opencv_ffmpeg211.lib")

			//#pragma comment(lib,"opencv_haartraining_engine.lib")
			#pragma comment(lib,"opencv_highgui211.lib")
			#pragma comment(lib,"opencv_imgproc211.lib")
			#pragma comment(lib,"opencv_legacy211.lib")
			#pragma comment(lib,"opencv_ml211.lib")

			#pragma comment(lib,"opencv_objdetect211.lib")
			#pragma comment(lib,"opencv_ts211.lib")
			#pragma comment(lib,"opencv_video211.lib")
		#endif
	#elif CV_MINOR_VERSION == 2
		#if CV_SUBMINOR_VERSION == 0
			#if _DEBUG
				#pragma comment(lib,"opencv_calib3d220d.lib")
				#pragma comment(lib,"opencv_contrib220d.lib")
				#pragma comment(lib,"opencv_core220d.lib")
				#pragma comment(lib,"opencv_features2d220d.lib")
				#pragma comment(lib,"opencv_ffmpeg220d.lib")
				#pragma comment(lib,"opencv_flann220d.lib")
				#pragma comment(lib,"opencv_gpu220d.lib")
				#pragma comment(lib,"opencv_highgui220d.lib")
				#pragma comment(lib,"opencv_imgproc220d.lib")
				#pragma comment(lib,"opencv_legacy220d.lib")
				#pragma comment(lib,"opencv_ml220d.lib")
				#pragma comment(lib,"opencv_objdetect220d.lib")
				#pragma comment(lib,"opencv_video220d.lib")
				#pragma comment(lib,"opencv_ts220.lib")
			#else
				#pragma comment(lib,"opencv_calib3d220.lib")
				#pragma comment(lib,"opencv_contrib220.lib")
				#pragma comment(lib,"opencv_core220.lib")
				#pragma comment(lib,"opencv_features2d220.lib")
				#pragma comment(lib,"opencv_ffmpeg220.lib")
				#pragma comment(lib,"opencv_flann220.lib")
				#pragma comment(lib,"opencv_gpu220.lib")
				#pragma comment(lib,"opencv_highgui220.lib")
				#pragma comment(lib,"opencv_imgproc220.lib")
				#pragma comment(lib,"opencv_legacy220.lib")
				#pragma comment(lib,"opencv_ml220.lib")
				#pragma comment(lib,"opencv_objdetect220.lib")
				#pragma comment(lib,"opencv_video220.lib")
				#pragma comment(lib,"opencv_ts220.lib")
			#endif
		#endif
	#elif CV_MINOR_VERSION == 4
		#if CV_SUBMINOR_VERSION == 2
			#if _DEBUG
				#pragma comment(lib,"opencv_calib3d242d.lib")
				#pragma comment(lib,"opencv_contrib242d.lib")
				#pragma comment(lib,"opencv_core242d.lib")
				#pragma comment(lib,"opencv_features2d242d.lib")
				#pragma comment(lib,"opencv_flann242d.lib")
				#pragma comment(lib,"opencv_gpu242d.lib")
				#pragma comment(lib,"opencv_haartraining_engined.lib")
				#pragma comment(lib,"opencv_highgui242d.lib")
				#pragma comment(lib,"opencv_imgproc242d.lib")
				#pragma comment(lib,"opencv_legacy242d.lib")
				#pragma comment(lib,"opencv_ml242d.lib")
				#pragma comment(lib,"opencv_nonfree242d.lib")
				#pragma comment(lib,"opencv_objdetect242d.lib")
				#pragma comment(lib,"opencv_photo242d.lib")
				#pragma comment(lib,"opencv_stitching242d.lib")
				#pragma comment(lib,"opencv_ts242d.lib")
				#pragma comment(lib,"opencv_video242d.lib")
				#pragma comment(lib,"opencv_videostab242d.lib")
			#else
				#pragma comment(lib,"opencv_calib3d242.lib")
				#pragma comment(lib,"opencv_contrib242.lib")
				#pragma comment(lib,"opencv_core242.lib")
				#pragma comment(lib,"opencv_features2d242.lib")
				#pragma comment(lib,"opencv_flann242.lib")
				#pragma comment(lib,"opencv_gpu242.lib")
				#pragma comment(lib,"opencv_haartraining_engine.lib")
				#pragma comment(lib,"opencv_highgui242.lib")
				#pragma comment(lib,"opencv_imgproc242.lib")
				#pragma comment(lib,"opencv_legacy242.lib")
				#pragma comment(lib,"opencv_ml242.lib")
				#pragma comment(lib,"opencv_nonfree242.lib")
				#pragma comment(lib,"opencv_objdetect242.lib")
				#pragma comment(lib,"opencv_photo242.lib")
				#pragma comment(lib,"opencv_stitching242.lib")
				#pragma comment(lib,"opencv_ts242.lib")
				#pragma comment(lib,"opencv_video242.lib")
				#pragma comment(lib,"opencv_videostab242.lib")
			#endif
		#endif
	#elif CV_MINOR_VERSION == 4
		#if CV_SUBMINOR_VERSION == 5
			#if _DEBUG
				#pragma comment(lib,"opencv_calib3d245d.lib")
				#pragma comment(lib,"opencv_contrib245d.lib")
				#pragma comment(lib,"opencv_core245d.lib")
				#pragma comment(lib,"opencv_features2d245d.lib")
				#pragma comment(lib,"opencv_flann245d.lib")
				#pragma comment(lib,"opencv_gpu245d.lib")
				#pragma comment(lib,"opencv_haartraining_engined.lib")
				#pragma comment(lib,"opencv_highgui245d.lib")
				#pragma comment(lib,"opencv_imgproc245d.lib")
				#pragma comment(lib,"opencv_legacy245d.lib")
				#pragma comment(lib,"opencv_ml245d.lib")
				#pragma comment(lib,"opencv_nonfree245d.lib")
				#pragma comment(lib,"opencv_objdetect245d.lib")
				#pragma comment(lib,"opencv_photo245d.lib")
				#pragma comment(lib,"opencv_stitching245d.lib")
				#pragma comment(lib,"opencv_ts245d.lib")
				#pragma comment(lib,"opencv_video245d.lib")
				#pragma comment(lib,"opencv_videostab245d.lib")
			#else
				#pragma comment(lib,"opencv_calib3d245.lib")
				#pragma comment(lib,"opencv_contrib245.lib")
				#pragma comment(lib,"opencv_core245.lib")
				#pragma comment(lib,"opencv_features2d245.lib")
				#pragma comment(lib,"opencv_flann245.lib")
				#pragma comment(lib,"opencv_gpu245.lib")
				#pragma comment(lib,"opencv_haartraining_engine.lib")
				#pragma comment(lib,"opencv_highgui245.lib")
				#pragma comment(lib,"opencv_imgproc245.lib")
				#pragma comment(lib,"opencv_legacy245.lib")
				#pragma comment(lib,"opencv_ml245.lib")
				#pragma comment(lib,"opencv_nonfree245.lib")
				#pragma comment(lib,"opencv_objdetect245.lib")
				#pragma comment(lib,"opencv_photo245.lib")
				#pragma comment(lib,"opencv_stitching245.lib")
				#pragma comment(lib,"opencv_ts245.lib")
				#pragma comment(lib,"opencv_video245.lib")
				#pragma comment(lib,"opencv_videostab245.lib")
			#endif
		#endif
	#endif
#endif


#endif /* _OPENCV_HEADER_H_ */
