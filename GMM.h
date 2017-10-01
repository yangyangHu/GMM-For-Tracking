/******************************************************************************************
*
*	The class implement the paper"Adaptive background mixture models for real-time tracking"
*		by Yangyang Hu at 2014/11/19
*
*******************************************************************************************/
#include "Global.h"

class GMM
{
public:
	//定义gmm模型用到的参数变量
	int gmm_max_compont;  //GMM混合模型的最大分量数5
	float gmm_learn_alpha; //学习率0.005
	float gmm_threshold_sumw;  //背景率0.7
	int end_frame;  //训练帧数
	float default_sigma; //默认的初始方差为15
	float default_weight;//默认的权值为0.005
	float default_var_threshold;//默认的方差阈值为2.5*2.5

	float gmm_testT;//前景检测阈值

	//gmm的权值w，均值u，方差矩阵sigma
	Mat w[GMM_MAX_NUM];
	Mat u[GMM_MAX_NUM];
	Mat sigma[GMM_MAX_NUM];

	//输入图像每个像素gmm合适的个数
	Mat fit_num;

	//定义前景掩膜
	IplImage* FGMask;

public:
	//成员函数
	GMM(int gmm_max_compont1,float gmm_learn_alpha1,float gmm_threshold_sumw1,float gmm_testT1,int end_frame1);
	~GMM();
	void gmm_init(IplImage *img);//gmm整体初始化函数

	void gmm_first_frame(IplImage *img);  //gmm第一帧初始化函数

	void gmm_update(IplImage* img);//gmm更新过程

	void gmm_fit_num(IplImage* img);//对输入图像每个像素gmm选择合适的个数

	void gmm_test(IplImage* img);//gmm的测试函数

};