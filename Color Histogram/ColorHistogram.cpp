#include "ColorHistogram.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define cvQueryHistValue_2D( hist, idx0, idx1 ) cvGetReal2D( (hist)->bins, (idx0), (idx1) ) 

ColorHistogram::~ColorHistogram(void)
{
}

bool ColorHistogram::Show()
{
	/** H ��������Ϊ16���ȼ���S��������Ϊ8���ȼ� */
	int h_bins = 16, s_bins = 8;
	int hist_size[] = {h_bins, s_bins};

	/** H �� S �����ı仯��Χ */
	float h_ranges[] = { 0, 180 }; 
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };

	/** ����ֱ��ͼ����ά, ÿ��ά���Ͼ��� */
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	/** ����H,S����ƽ������ͳ��ֱ��ͼ */
	cvCalcHist( planes, hist, 0, 0 );

	/** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ */
	float max_value;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );


	/** ����ֱ��ͼ��ʾͼ�� */
	int height = 240;
	int width = (h_bins*s_bins*6);
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );

	/** ��������HSV��RGB��ɫת������ʱ��λͼ�� */
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound(bin_val*height/max_value);

			/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */
			cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));
			cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color,0,0);

			cvRectangle( hist_img, cvPoint(i*bin_w,height),
				cvPoint((i+1)*bin_w,height - intensity),
				color, -1, 8, 0 );
		}
	}

	//cvNamedWindow( "Source", 1 );
	//cvShowImage( "Source", image );

	cvNamedWindow( "Color Histogram", 1 );
	cvShowImage( "Color Histogram", hist_img );

	return true;
}