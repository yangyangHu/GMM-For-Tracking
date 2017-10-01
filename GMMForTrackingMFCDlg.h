
// GMMForTrackingMFCDlg.h : 头文件
//

#pragma once
#include "GMM.h"
#include "afxwin.h"

// CGMMForTrackingMFCDlg 对话框
class CGMMForTrackingMFCDlg : public CDialogEx
{
// 构造
public:
	CGMMForTrackingMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GMMFORTRACKINGMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString         strFolderPath; //存储文件夹的路径
	CString			strRunState;//存储程序的运行状态
	int gmm_max_compont;//GMM混合模型的最大分量
	float gmm_learn_alpha;//学习率
	float gmm_threshold_sumw;//背景率
	float gmm_test_T;//前景检测阈值
	int gmm_end_frame;//GMM模型初始训练帧数

public:
	CvRect AirPlaneDetection(IplImage *imageBi);//检测飞机

public:
	afx_msg void OnBnClickedButtonFrame();//读取视频帧序列
	afx_msg void OnBnClickedButtonRun();//运行事件
	CEdit m_editRunState_View;//运行状态框变量
	afx_msg void OnBnClickedButtonAirplane();//应用--飞机跟踪
	afx_msg void OnBnClickedButtonVideo();//读入视频数据
};
