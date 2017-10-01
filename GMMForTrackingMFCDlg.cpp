
// GMMForTrackingMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GMMForTrackingMFC.h"
#include "GMMForTrackingMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGMMForTrackingMFCDlg 对话框



CGMMForTrackingMFCDlg::CGMMForTrackingMFCDlg(CWnd* pParent /*=NULL*/)//构造函数
	: CDialogEx(CGMMForTrackingMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//参数变量初始化
	gmm_max_compont = 5;//GMM混合模型的最大分量默认为5
	gmm_learn_alpha = 0.005;//学习率默认为0.005
	gmm_threshold_sumw = 0.7;//背景率默认为0.7
	gmm_test_T = 2.5;//前景检测阈值默认为2.5
	gmm_end_frame = 200;//GMM模型初始训练帧数默认为200

	strRunState = _T("");
}

void CGMMForTrackingMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RunState, m_editRunState_View);
}

BEGIN_MESSAGE_MAP(CGMMForTrackingMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Frame, &CGMMForTrackingMFCDlg::OnBnClickedButtonFrame)
	ON_BN_CLICKED(IDC_BUTTON_Run, &CGMMForTrackingMFCDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_AirPlane, &CGMMForTrackingMFCDlg::OnBnClickedButtonAirplane)
	ON_BN_CLICKED(IDC_BUTTON_Video, &CGMMForTrackingMFCDlg::OnBnClickedButtonVideo)
END_MESSAGE_MAP()


// CGMMForTrackingMFCDlg 消息处理程序

BOOL CGMMForTrackingMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	//初始化参数文本框
	SetDlgItemInt(IDC_EDIT_MaxCompont, gmm_max_compont); 

	char s[128];
	sprintf(s,"%f",gmm_learn_alpha); 
	CString str = CString(s); 
	SetDlgItemText(IDC_EDIT_LearnAlpha, str);

	char s1[128];
	sprintf(s1,"%f",gmm_threshold_sumw); 
	CString str1 = CString(s1); 
	SetDlgItemText(IDC_EDIT_T, str1);

	char s2[128];
	sprintf(s2,"%f",gmm_test_T); 
	CString str2 = CString(s2); 
	SetDlgItemText(IDC_EDIT_TestT, str2);

	SetDlgItemInt(IDC_EDIT_TrainFrameNumber, gmm_end_frame); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGMMForTrackingMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGMMForTrackingMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGMMForTrackingMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGMMForTrackingMFCDlg::OnBnClickedButtonFrame()//读取视频帧序列
{
	// TODO: 在此添加控件通知处理程序代码
	//选择文件夹
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	/*CString         */strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("请选择一个文件夹：");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  
	strFolderPath += "\\*.*"; 

	strRunState += _T("/*******读入数据*****/\r\n") + strFolderPath;
	m_editRunState_View.SetWindowText(strRunState); 
}


void CGMMForTrackingMFCDlg::OnBnClickedButtonRun()//运行事件--测试算法性能
{
	// TODO: 在此添加控件通知处理程序代码
	//获取并保存文本框中的内容
	CString strMaxCompont;
	CString strLearnAlpha;
	CString strT;
	CString strTestT;
	CString strEndFrame;
	GetDlgItem(IDC_EDIT_MaxCompont)->GetWindowText(strMaxCompont);
	GetDlgItem(IDC_EDIT_LearnAlpha)->GetWindowText(strLearnAlpha);
	GetDlgItem(IDC_EDIT_T)->GetWindowText(strT);
	GetDlgItem(IDC_EDIT_TestT)->GetWindowText(strTestT);
	GetDlgItem(IDC_EDIT_TrainFrameNumber)->GetWindowText(strEndFrame);

	if(strFolderPath == "")
	{
		// 显示消息对话框   
		MessageBox(_T("必须先读取数据！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}

	if(strMaxCompont == "")
	{
		// 显示消息对话框   
		MessageBox(_T("GMM最大分量数必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strLearnAlpha == "")
	{
		// 显示消息对话框   
		MessageBox(_T("学习率必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strT == "")
	{
		// 显示消息对话框   
		MessageBox(_T("背景率必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strTestT == "")
	{
		// 显示消息对话框   
		MessageBox(_T("前景检测阈值必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strEndFrame == "")
	{
		// 显示消息对话框   
		MessageBox(_T("训练帧数必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	gmm_max_compont = _tstoi(strMaxCompont);
	gmm_learn_alpha = _tstof(strLearnAlpha);
	gmm_threshold_sumw = _tstof(strT);
	gmm_test_T = _tstof(strTestT);
	gmm_end_frame = _tstoi(strEndFrame);

	GMM gmm(gmm_max_compont,gmm_learn_alpha,gmm_threshold_sumw,gmm_test_T,gmm_end_frame);

	int num = -3;

	CFileFind finder;
	CString strPath;
	CString strDir = strFolderPath;
	BOOL bWorking = finder.FindFile(strDir);

	while (bWorking)
	{
		num++;
		bWorking = finder.FindNextFile();
		strPath=finder.GetFilePath();
		if (num>=0)
		{
			//strPath就是所要获取Test目录下的文件夹和文件（包括路径）
			if (num <= gmm.end_frame)
			{
				strRunState = _T("/*******训练*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}
			else
			{
				strRunState = _T("/*******测试*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}


			//CString -> string
			CStringA stra(strPath.GetBuffer(0));
			strPath.ReleaseBuffer();
			std::string imgpath=stra.GetBuffer(0);
			stra.ReleaseBuffer();

			IplImage* image = NULL;
			IplImage* image_gray = NULL;
			IplImage* foreground = NULL;

			image = cvLoadImage(imgpath.c_str());
			if(!image)
			{
				cout<<"something is wrong!!"<<endl;
				cvWaitKey(0);
				//release images
				cvDestroyWindow("src");
				cvDestroyWindow("foreground");
				return ;
			}
			cvShowImage("src",image);
			image_gray = cvCreateImage(cvGetSize(image),8,1);
			foreground = cvCreateImage(cvGetSize(image),8,1);
			cvCvtColor(image,image_gray,CV_BGR2GRAY);
			/****when it is the first frame,set the default parameter****/
			if (num == 0)
			{
				gmm.gmm_init(image_gray);
				gmm.gmm_first_frame(image_gray);
			}
			//the train of gmm phase
			if (num<gmm.end_frame)
			{
				gmm.gmm_update(image_gray);
			}
			/****chose the fitting number of component in gmm****/
			if (num == gmm.end_frame)
			{
				gmm.gmm_fit_num(image_gray);
			}
			/****start the test phase****/
			if (num>gmm.end_frame)
			{
				gmm.gmm_test(image_gray);
				gmm.gmm_update(image_gray);
				gmm.gmm_fit_num(image_gray);
				foreground = cvCloneImage(gmm.FGMask);
				cvShowImage("foreground",foreground);
			}

			//release images
			cvReleaseImage(&image);
			cvReleaseImage(&image_gray);
			cvReleaseImage(&foreground);

			char c=(char)waitKey(1);
			if(c==27)//if press the ESC key,the exit the proggram
				break;
		}
		

	}
	finder.Close();


	cvDestroyWindow("src");
	cvDestroyWindow("foreground");
}


void CGMMForTrackingMFCDlg::OnBnClickedButtonAirplane()//应用--飞机跟踪
{
	// TODO: 在此添加控件通知处理程序代码
	//获取并保存文本框中的内容
	CString strMaxCompont;
	CString strLearnAlpha;
	CString strT;
	CString strTestT;
	CString strEndFrame;
	GetDlgItem(IDC_EDIT_MaxCompont)->GetWindowText(strMaxCompont);
	GetDlgItem(IDC_EDIT_LearnAlpha)->GetWindowText(strLearnAlpha);
	GetDlgItem(IDC_EDIT_T)->GetWindowText(strT);
	GetDlgItem(IDC_EDIT_TestT)->GetWindowText(strTestT);
	GetDlgItem(IDC_EDIT_TrainFrameNumber)->GetWindowText(strEndFrame);

	if(strFolderPath == "")
	{
		// 显示消息对话框   
		MessageBox(_T("必须先读取数据！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}

	if(strMaxCompont == "")
	{
		// 显示消息对话框   
		MessageBox(_T("GMM最大分量数必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strLearnAlpha == "")
	{
		// 显示消息对话框   
		MessageBox(_T("学习率必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strT == "")
	{
		// 显示消息对话框   
		MessageBox(_T("背景率必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strTestT == "")
	{
		// 显示消息对话框   
		MessageBox(_T("前景检测阈值必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	else if (strEndFrame == "")
	{
		// 显示消息对话框   
		MessageBox(_T("训练帧数必须指定！"), _T("提示"), MB_OK | MB_ICONWARNING);   
		return;
	}
	gmm_max_compont = _tstoi(strMaxCompont);
	gmm_learn_alpha = _tstof(strLearnAlpha);
	gmm_threshold_sumw = _tstof(strT);
	gmm_test_T = _tstof(strTestT);
	gmm_end_frame = _tstoi(strEndFrame);

	GMM gmm(gmm_max_compont,gmm_learn_alpha,gmm_threshold_sumw,gmm_test_T,gmm_end_frame);

	int num = -3;

	CFileFind finder;
	CString strPath;
	CString strDir = strFolderPath;
	BOOL bWorking = finder.FindFile(strDir);

	while (bWorking)
	{
		num++;
		bWorking = finder.FindNextFile();
		strPath=finder.GetFilePath();
		if (num>=0)
		{
			//strPath就是所要获取Test目录下的文件夹和文件（包括路径）
			if (num <= gmm.end_frame)
			{
				strRunState = _T("/*******训练*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}
			else
			{
				strRunState = _T("/*******测试*****/\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
				char s[128];
				sprintf(s,"%d",num); 
				CString str = CString(s); 
				strRunState += str + _T(" frame processing!\r\n");
				m_editRunState_View.SetWindowText(strRunState); 
			}


			//CString -> string
			CStringA stra(strPath.GetBuffer(0));
			strPath.ReleaseBuffer();
			std::string imgpath=stra.GetBuffer(0);
			stra.ReleaseBuffer();

			IplImage* image = NULL;
			IplImage* image_gray = NULL;
			IplImage* foreground = NULL;
			IplImage* foregroundRefine = NULL;

			image = cvLoadImage(imgpath.c_str());
			if(!image)
			{
				cout<<"something is wrong!!"<<endl;
				cvWaitKey(0);
				//release images
				cvDestroyWindow("src");
				cvDestroyWindow("foreground");
				cvDestroyWindow("refine");
				return ;
			}

			IplImage* imageScale1 = NULL;
			IplImage* imageScale2 = NULL;

			imageScale1 = cvCreateImage(cvSize(image->width/2,image->height/2),8,3);
			cvPyrDown(image,imageScale1);//缩小四倍

			imageScale2 = cvCreateImage(cvSize(imageScale1->width/2,imageScale1->height/2),8,3);
			cvPyrDown(imageScale1,imageScale2);//缩小四倍

			cvShowImage("src",imageScale2);
			image_gray = cvCreateImage(cvGetSize(imageScale2),8,1);
			foreground = cvCreateImage(cvGetSize(imageScale2),8,1);
			cvZero(foreground);
			cvCvtColor(imageScale2,image_gray,CV_BGR2GRAY);

			//提取跑道ROI
			cvSetImageROI(image_gray,cvRect(0,cvRound(image_gray->height*0.44),image_gray->width,cvRound(image_gray->height*0.2)));
			IplImage* image_grayROI = cvCreateImage(cvGetSize(image_gray),8,1);
			cvCopy(image_gray,image_grayROI);

			/****when it is the first frame,set the default parameter****/
			if (num == 0)
			{
				gmm.gmm_init(image_grayROI);
				gmm.gmm_first_frame(image_grayROI);
			}
			//the train of gmm phase
			if (num<gmm.end_frame)
			{
				gmm.gmm_update(image_grayROI);
			}
			/****chose the fitting number of component in gmm****/
			if (num == gmm.end_frame)
			{
				gmm.gmm_fit_num(image_grayROI);
			}
			/****start the test phase****/
			if (num>gmm.end_frame)
			{
				gmm.gmm_test(image_grayROI);
				gmm.gmm_update(image_grayROI);
				gmm.gmm_fit_num(image_grayROI);
				/*foreground = cvCloneImage(gmm.FGMask);*/
				for (int y=0;y<gmm.FGMask->height;y++)
				{
					for (int x=0;x<gmm.FGMask->width;x++)
					{
						int pixel = cvRound(cvGetReal2D(gmm.FGMask,y,x));
						cvSetReal2D(foreground,y+cvRound(foreground->height*0.44),x,pixel);
					}
				}
				foregroundRefine = cvCloneImage(foreground);
				cvMorphologyEx(foreground,foregroundRefine,NULL,NULL,CV_MOP_CLOSE,1);
				cvShowImage("foreground",foreground);
				cvShowImage("refine",foregroundRefine);

				//视频中画出飞机区域
				CvRect airPlaneRect = AirPlaneDetection(foregroundRefine);
				cvRectangle(imageScale2,cvPoint(airPlaneRect.x,airPlaneRect.y),cvPoint(airPlaneRect.x+airPlaneRect.width,airPlaneRect.y+airPlaneRect.height),CV_RGB(255,0,0),2);
				cvShowImage("src",imageScale2);
			}
			//release images
			cvReleaseImage(&image);
			cvReleaseImage(&image_gray);
			cvReleaseImage(&foreground);
			cvReleaseImage(&foregroundRefine);
			cvReleaseImage(&imageScale1);
			cvReleaseImage(&imageScale2);
			cvReleaseImage(&image_grayROI);
			char c=(char)waitKey(1);
			if(c==27)//if press the ESC key,the exit the proggram
				break;
		}


	}
	finder.Close();

	cvDestroyWindow("src");
	cvDestroyWindow("foreground");
	cvDestroyWindow("refine");
}


CvRect CGMMForTrackingMFCDlg::AirPlaneDetection(IplImage *imageBi)//检测飞机
{
	CvSeq *pContour = NULL; 
	CvMemStorage *pStorage = cvCreateMemStorage(0); 
	int n=cvFindContours(imageBi,pStorage,&pContour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	CvRect planeRect = cvRect(0,0,0,0);
	int maxarea = 0;
	for(;pContour!=NULL;pContour=pContour->h_next)
	{
		int area=(int)cvContourArea(pContour);
		CvRect rect=cvBoundingRect(pContour);//计算点集的最外面（up-right）矩形边界 
		if (area>maxarea)
		{
			maxarea = area;
			planeRect = rect;
		}
	}
	//释放内存
	cvReleaseMemStorage(&pStorage);
	return planeRect;
}

void CGMMForTrackingMFCDlg::OnBnClickedButtonVideo()//读入视频数据
{
	// TODO: 在此添加控件通知处理程序代码
	//待完善
}
