#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_POINT_MAP_CNT		2		// 同时显示的曲线个数

#include <vector>
#include <map>
using namespace std;

// 曲线队列结构体
typedef struct tagPOINTMAP
{
	double CarNumber;
	double TempValue;
} POINTMAP, FAR *LPPOINTMAP, PPOINTMAP;

class CCurve
{
public:
	CCurve(HWND hParent, CDC *pDC, int iMaxWidth, int iMaxHeight, int CXInternal, int CYInternal, 
		   int MaxKeepSpaceLeftValue, int MaxKeepSpaceRightValue, int MaxKeepSpaceTopValue, 
		   int MaxKeepSpaceBottomValue, int iKilnType);
	~CCurve(void);
	
public:
	void SetBackGround(COLORREF crBkColor);						// 设置曲线背景色
	void SetFrontGround(COLORREF crFtColor);					// 设置曲线前景色
	void SetCaptionColor(COLORREF crCpColor);					// 设置曲线标题颜色
	void SetTipBkColor(COLORREF crTipBkColor);					// 设置鼠标提示背景颜色
	void SetTipFtColor(COLORREF crTipFtColor);					// 设置鼠标提示字体颜色
	void SetTotalCars(int iTotalCars);
	void SetCurveColor(int PointMapNum, COLORREF crCurveColor);	// 设置曲线颜色

	void SetCaptionStr(const char *pCaptionStr);			// 设置曲线标题字符

	void SetClientRect(CRect cRect);				// 设置客户端的尺寸, 在父窗口的WM_SIZE消息函数中调用
	void UpdateMousePoint(POINT MousePoint);		// 更新鼠标的坐标, 在父窗口的WM_MOUSEMOVE消息函数中调用

	// 队列操作函数
	void AddPoint(int PointMapNum, double CarNumber, double TempValue);	// 添加一个队列单元值
	void AddPoint(int PointMapNum, POINTMAP PointMap);					// 添加一个队列单元值
	POINTMAP GetPoint(int PointMapNum, int iIndex);						// 获取一个队列单元值
	void ClearPoint(int PointMapNum);									// 清除队列
	int PointLength(int PointMapNum);									// 获取队列单元个数

	void PaintUpdate(CDC *pDC);							// 曲线更新, 在WM_PAINT消息函数中调用

private:
	void DrawBackGround(void);						// 重绘背景函数
	void DrawCoordinateSystem(void);				// 重绘坐标线及刻度
	void DrawCurve(int PointMapNum);				// 重绘曲线
	void DrawScale(void);							// 鼠标提示显示操作

	int TranslateXValue(double CarNumber);					// 根据实际值转换为X坐标值
	int TranslateYValue(double TempValue);					// 根据实际值转换为Y坐标值
	double TranslateXCoordinate(double CoordinateXValue);	// 根据X坐标值转换为实际值
	double TranslateYCoordinate(double CoordinateYValue);	// 根据Y坐标值转换为实际值

private:
	HWND    m_hParent;		// 父窗口指针
	CDC    *m_pDC;			// 父窗口显示设备指针
	CDC     m_MemDcDraw;	// 兼容设备内存指针, 作为显示缓存
	HBITMAP m_HbtMap;		// 兼容画布

	int     m_iKilnType;	// 
		
	int		m_iMaxWidth;	// 最大的屏幕尺寸, 根据此值创建位图大小. 只在初始化中使用
	int		m_iMaxHeight;	// 最大的屏幕尺寸, 根据此值创建位图大小

	int     m_MaxKeepSpaceLeftValue;	// 曲线框架左边需要空出来的空间大小, 在初始化中赋值
	int     m_MaxKeepSpaceRightValue;	// 曲线框架右边需要空出来的空间大小, 在初始化中赋值
	int     m_MaxKeepSpaceTopValue;		// 曲线框架顶部需要空出来的空间大小, 在初始化中赋值
	int     m_MaxKeepSpaceBottomValue;	// 曲线框架底部需要空出来的空间大小, 在初始化中赋值

	CRect   m_ParentFrameRect;			// 父窗口尺寸大小, 但父窗口的尺寸改变时, 调用SetClientRect更新此变量
	CRect   m_CurveFrameRect;			// 曲线框架的尺寸大小, 但父窗口的尺寸改变时, 调用SetClientRect更新此变量
	CRect   m_CurveRect;

	int     m_CXInternal;				// 需要的X轴刻度个数, 总车位个数, 每隔1个
	int     m_CYInternal;				// 需要的Y轴刻度个数, 总温度间隔个数, 每隔50℃

	COLORREF m_crBkColor;						// 背景颜色
	COLORREF m_crFtColor;						// 前景颜色: 网线 坐标系 刻度值 单位文字
	COLORREF m_crCpColor;						// 标题文字颜色
	COLORREF m_crTipBkColor;					// 鼠标提示背景颜色
	COLORREF m_crTipFtColor;					// 鼠标提示前景颜色
	COLORREF m_crCurveColor[MAX_POINT_MAP_CNT];	// 曲线颜色

	CString  m_strCaption;						// 曲线标题变量					

	POINT    m_MousePoint;						// 鼠标坐标, 由父窗口WM_MOUSEMOVE消息函数中更新

	vector<POINTMAP> m_PointMap[MAX_POINT_MAP_CNT];	// 曲线队列定义
};
