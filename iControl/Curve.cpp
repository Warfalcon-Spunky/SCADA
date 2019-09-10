#include "stdafx.h"
#include "Curve.h"
#include "assert.h"

CCurve::CCurve(HWND hParent, CDC *pDC, int iMaxWidth, int iMaxHeight, int CXInternal, int CYInternal, 
			   int MaxKeepSpaceLeftValue, int MaxKeepSpaceRightValue, int MaxKeepSpaceTopValue, 
			   int MaxKeepSpaceBottomValue, int iKilnType)
:m_hParent(hParent),
 m_pDC(pDC),
 m_iMaxWidth(iMaxWidth),
 m_iMaxHeight(iMaxHeight),
 m_ParentFrameRect(0, 0, iMaxWidth, iMaxHeight),
 m_CurveRect(0, 0, iMaxWidth, iMaxHeight),	// 曲线区域
 m_CXInternal(CXInternal),
 m_CYInternal(CYInternal),
 m_crBkColor(RGB(255, 255, 255)),			// 默认背景颜色为白色		
 m_crFtColor(RGB(0, 0, 0)),					// 设置前景颜色为黑色
 m_crCpColor(RGB(0, 0, 255)),				// 设置标题字体颜色为蓝色
 m_crTipBkColor(RGB(127, 127, 127)),		// 鼠标提示背景颜色
 m_crTipFtColor(RGB(200, 200, 200)),		// 鼠标提示前景颜色
 m_strCaption(""),
 m_MaxKeepSpaceLeftValue(MaxKeepSpaceLeftValue),
 m_MaxKeepSpaceRightValue(MaxKeepSpaceRightValue),
 m_MaxKeepSpaceTopValue(MaxKeepSpaceTopValue),
 m_MaxKeepSpaceBottomValue(MaxKeepSpaceBottomValue),
 m_iKilnType(iKilnType),
 m_CurveFrameRect(MaxKeepSpaceLeftValue, MaxKeepSpaceTopValue, iMaxWidth - MaxKeepSpaceRightValue, iMaxHeight - MaxKeepSpaceBottomValue)
{
	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = m_iMaxWidth;				// 创建最大的图片, 尺寸宽度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biHeight = m_iMaxHeight;				// 创建最大的图片, 尺寸高度为屏幕尺寸宽度
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	
	m_MemDcDraw.CreateCompatibleDC(m_pDC);												// 创建兼容画布, 关联父窗口
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);	// 创建与设备无关的内存
	m_MemDcDraw.SelectObject(m_HbtMap);													// 选中指定内存
	m_MemDcDraw.SetBkMode(TRANSPARENT);													// 背景设置为透明

	// 清除所有曲线队列
	for (int i = 0; i < MAX_POINT_MAP_CNT; i++)
	{
		ClearPoint(i);
	}
}

CCurve::~CCurve(void)
{
	DeleteObject(m_HbtMap);
	DeleteDC(m_MemDcDraw);

//	m_MemDcDraw.DeleteDC();
//	m_pDC->DeleteDC();
}

// 更新父窗口客户区的尺寸, 在父窗口的WM_SIZE消息函数中调用
void CCurve::SetClientRect(CRect cRect)
{
	m_ParentFrameRect = cRect;

	m_CurveFrameRect.left = m_MaxKeepSpaceLeftValue;
	m_CurveFrameRect.right = cRect.Width() - m_MaxKeepSpaceRightValue;

	m_CurveFrameRect.top = m_MaxKeepSpaceTopValue;
	m_CurveFrameRect.bottom = cRect.Height() - m_MaxKeepSpaceBottomValue;
}

// 设置背景色
void CCurve::SetBackGround(COLORREF crBkColor)
{
	m_crBkColor = crBkColor;
}

// 设置前景色
void CCurve::SetFrontGround(COLORREF crFtColor)
{
	m_crFtColor = crFtColor;
}

// 设置曲线标题的颜色
void CCurve::SetCaptionColor(COLORREF crCpColor)
{
	m_crCpColor = crCpColor;
}

// 设置鼠标提示框的背景色
void CCurve::SetTipBkColor(COLORREF crTipBkColor)
{
	m_crTipBkColor = crTipBkColor;
}

// 设置鼠标提示框的前景色
void CCurve::SetTipFtColor(COLORREF crTipFtColor)
{
	m_crTipFtColor = crTipFtColor;
}

// 设置曲线颜色
void CCurve::SetCurveColor(int PointMapNum, COLORREF crCurveColor)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_crCurveColor[PointMapNum] = crCurveColor;
}

// 设置曲线标题文字
void CCurve::SetCaptionStr(const char *pCaptionStr)
{
	m_strCaption.Empty();
	m_strCaption.Format("%s", pCaptionStr);
}

void CCurve::SetTotalCars(int iTotalCars)
{
	assert(iTotalCars > 0);

	m_CXInternal = iTotalCars;
}

// 更新鼠标坐标, 该函数在父窗口WM_MOUSEMOVE消息函数中调用
void CCurve::UpdateMousePoint(POINT MousePoint)
{
	m_MousePoint = MousePoint;
}

// 重绘背景颜色
void CCurve::DrawBackGround(void)
{
	CBrush brBack, *pOldcrBack;

	// 创建刷子并绘制填充矩形, 尺寸为父窗口的客户区大小
	brBack.CreateSolidBrush(m_crBkColor);
	m_MemDcDraw.FillRect(&m_ParentFrameRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	

	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	
}

// 重绘曲线坐标函数
void CCurve::DrawCoordinateSystem(void)
{
	CPen GrayPen, *pOldGrayPen;
	CPen DashPen, *pOldDashPen;

	int Index;
	int InternalWidth, InternalHeight;

	// 计算X和Y坐标的网格线的个数
	InternalWidth = m_CurveFrameRect.Width() / m_CXInternal;
	InternalHeight = m_CurveFrameRect.Height() / m_CYInternal;

	// 设置曲线外框的区域, 防止父窗口尺寸改变
	m_CurveRect.left = m_CurveFrameRect.left;
	m_CurveRect.top = m_CurveFrameRect.top;
	m_CurveRect.right = InternalWidth * m_CXInternal + m_CurveFrameRect.left;
	m_CurveRect.bottom = InternalHeight * m_CYInternal + m_CurveFrameRect.top;

	// 画曲线外边框
	GrayPen.CreatePen(PS_SOLID, 3, m_crFtColor);
	pOldGrayPen = m_MemDcDraw.SelectObject(&GrayPen);
//	m_MemDcDraw.Rectangle(&m_CurveRect);					// 矩形框内颜色与背景不一样

	m_MemDcDraw.MoveTo(m_CurveRect.left, m_CurveRect.top);
	m_MemDcDraw.LineTo(m_CurveRect.right, m_CurveRect.top);

	m_MemDcDraw.MoveTo(m_CurveRect.right, m_CurveRect.top);
	m_MemDcDraw.LineTo(m_CurveRect.right, m_CurveRect.bottom);
	
	m_MemDcDraw.MoveTo(m_CurveRect.right, m_CurveRect.bottom);
	m_MemDcDraw.LineTo(m_CurveRect.left, m_CurveRect.bottom);

	m_MemDcDraw.MoveTo(m_CurveRect.left, m_CurveRect.bottom);
	m_MemDcDraw.LineTo(m_CurveRect.left, m_CurveRect.top);


	m_MemDcDraw.SelectObject(pOldGrayPen);
	GrayPen.DeleteObject();

	// 画曲线的网格线
	DashPen.CreatePen(PS_DOT, 1, m_crFtColor);
	pOldDashPen = m_MemDcDraw.SelectObject(&DashPen);

	for (Index = 1; Index < m_CXInternal; Index++)
	{
		m_MemDcDraw.MoveTo(m_CurveRect.left + Index * InternalWidth, m_CurveRect.top);
		m_MemDcDraw.LineTo(m_CurveRect.left + Index * InternalWidth, m_CurveRect.bottom);
	}

	for (Index = 1; Index < m_CYInternal; Index++)
	{
		m_MemDcDraw.MoveTo(m_CurveRect.left, m_CurveRect.top + Index * InternalHeight);
		m_MemDcDraw.LineTo(m_CurveRect.right, m_CurveRect.top + Index * InternalHeight);
	}

	m_MemDcDraw.SelectObject(pOldDashPen);
	DashPen.DeleteObject();

	// 显示标题
	CFont Font;
	CFont *pOldFont;
	Font.CreateFont(36, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "华文隶书");
	pOldFont = m_MemDcDraw.SelectObject(&Font);

	m_MemDcDraw.SetTextColor(m_crCpColor);
	m_MemDcDraw.DrawText(m_strCaption, CRect(m_ParentFrameRect.left, m_ParentFrameRect.top, m_ParentFrameRect.right, m_ParentFrameRect.top + 50), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_MemDcDraw.SelectObject(pOldFont);
	Font.DeleteObject();

	// 图例文字
	m_MemDcDraw.SetTextColor(m_crFtColor);
	m_MemDcDraw.TextOut(m_CurveRect.left, m_CurveRect.top - 20, "Temperature（℃）");

	if (m_iKilnType == 3)
	{
		m_MemDcDraw.TextOut(m_CurveRect.right + 15, m_CurveRect.bottom + 5, "metre");
	}
	else
	{
		m_MemDcDraw.TextOut(m_CurveRect.right + 15, m_CurveRect.bottom + 5, "positon");
	}

	CString str;
	// 绘制X轴游标字符
	for (Index = 2; Index <= (m_CXInternal + 2); Index++)
	{
		if (m_iKilnType == 3)						// 移动窑
		{
			str.Format("%02d", (Index - 2) * 10);
		}
		else
		{
			str.Format("%02d", Index - 2);
		}		

		m_MemDcDraw.TextOut(m_CurveRect.left + InternalWidth * (Index - 2) - 10, m_CurveRect.bottom + 5, str);
		str.Empty();
	}

	// 绘制Y轴游标字符
	for (Index = 2; Index <= (m_CYInternal + 2); Index++)
	{
		str.Format("%d", (Index - 2) * 50);
		m_MemDcDraw.TextOut(m_CurveRect.left - 40, m_CurveRect.bottom - InternalHeight * (Index - 2) - 10, str);
		str.Empty();
	}
}

// 重绘鼠标提示函数
// 但鼠标指向数据指定的坐标点上, 在鼠标附近显示数据点的数据值
void CCurve::DrawScale(void)
{
	POINTMAP PointMap, NextPointMap, NextNextPointMap;
	CRect rect;
	int i, j, len;
	int x, y;
	double TempVal;
	CString strScale;

	// 创建字体
	CFont Font, *pOldFont;
	Font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "楷体_GB2312");
	pOldFont = m_MemDcDraw.SelectObject(&Font);

	// 创建画笔, 画提示框外边
	CPen GrayPen, *pOldGrayPen;
	GrayPen.CreatePen(PS_SOLID, 1, m_crTipBkColor);
	pOldGrayPen = m_MemDcDraw.SelectObject(&GrayPen);

	// 创建刷子, 填充提示框
	CBrush brBack, *pOldcrBack;
	brBack.CreateSolidBrush(m_crTipBkColor);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	

	// 检查所有的曲线
	for (j = 0; j < MAX_POINT_MAP_CNT; j++)
	{
		// 将指定数据队列的数据取出并换算成坐标, 比较鼠标坐标是否在之内
		for (i = 0, len = PointLength(j); i < len; i++)
		{
			// 获取指定队列数据并换算成坐标数据
			PointMap = GetPoint(j, i);
			if (i < (len - 1))
			{
				NextPointMap = GetPoint(j, i + 1);
			}

			if (i < (len - 2))
			{
				NextNextPointMap = GetPoint(j, i + 2);
			}
			
			if (i >= len)
			{
				NextPointMap.CarNumber = -1;
				NextPointMap.TempValue = -1;
			}

			if ((NextPointMap.CarNumber == PointMap.CarNumber) && (NextNextPointMap.CarNumber == PointMap.CarNumber))
			{
				x = TranslateXValue(PointMap.CarNumber);
				
				TempVal = (PointMap.TempValue + NextPointMap.TempValue + NextNextPointMap.TempValue) / 3.0;
				y = TranslateYValue(TempVal);
				
				i = i + 2;
			}
			else if (NextPointMap.CarNumber == PointMap.CarNumber)
			{
				x = TranslateXValue(PointMap.CarNumber);
				
				TempVal = (PointMap.TempValue + NextPointMap.TempValue) / 2;
				y = TranslateYValue(TempVal);

				i++;
			}
			else
			{
				x = TranslateXValue(PointMap.CarNumber);

				TempVal = PointMap.TempValue;
				y = TranslateYValue(TempVal);
			}

			// 设置数据点的范围坐标
			rect.left = x - 2;
			rect.right = x + 2;
			rect.top = y - 2;
			rect.bottom = y + 2;

			// 判断鼠标坐标是否在指定的范围内
			if (rect.PtInRect(m_MousePoint))
			{
				// 设置字符串
				strScale.Empty();

				if (m_iKilnType == 3)						// 移动窑
				{
					strScale.Format("距离: %d米  温度: %d℃", (int)(PointMap.CarNumber), (int)(TempVal));
				}
				else
				{
					strScale.Format("车位: %d号  温度: %d℃", (int)(PointMap.CarNumber), (int)(TempVal));
				}
				
				// 如果要画提示框操作曲线外框, 调整显示框的显示位置
				if ((m_MousePoint.x + 300) > m_CurveFrameRect.right)	// 提示框出现在鼠标左下方
				{
					rect.left = m_MousePoint.x - 300;
					rect.right = m_MousePoint.x + 5;
					rect.top = m_MousePoint.y - 5;
					rect.bottom = m_MousePoint.y - 64;
				}
				else													// 提示框出现在鼠标右下方
				{
					rect.left = m_MousePoint.x + 5;
					rect.right = m_MousePoint.x + 300;
					rect.top = m_MousePoint.y - 5;
					rect.bottom = m_MousePoint.y - 64;
				}

				// 画提示框的外框
				m_MemDcDraw.RoundRect(&rect, CPoint(24, 24));
				
				// 显示提示文字且设置文字颜色
				m_MemDcDraw.SetTextColor(m_crTipFtColor);
				m_MemDcDraw.DrawText(strScale, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
		}
	}

	// 释放刷子内存操作
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();

	// 释放画笔内存操作
	m_MemDcDraw.SelectObject(pOldGrayPen);
	GrayPen.DeleteObject();

	// 释放字体内存操作
	m_MemDcDraw.SelectObject(pOldFont);
	Font.DeleteObject();
}

// 重绘曲线操作
// 参数: 指定存储队列号
void CCurve::DrawCurve(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	int   i, len;
	POINT StartPoint, EndPoint;
	POINTMAP PointMap, NextPointMap, NextNextPointMap;

	// 获取指定队列存储单元个数, 低于2个无法画直线, 直接退出
	len = PointLength(PointMapNum);
	if (len < 2)
	{
		return;
	}

	// 创建画笔画曲线, 指定曲线颜色
	CPen CurvePen, *pOldCurvePen;
	CurvePen.CreatePen(PS_SOLID, 4, m_crCurveColor[PointMapNum]);
	pOldCurvePen = m_MemDcDraw.SelectObject(&CurvePen);

	// 创建刷子画圆点, 指定圆点颜色
	CBrush CurveBrush;  
	CurveBrush.CreateSolidBrush(m_crCurveColor[PointMapNum]);
	CBrush *pOldCurveBrush = m_MemDcDraw.SelectObject(&CurveBrush);

	// 伪造1号车位温度
	StartPoint.x = TranslateXValue(0.0);
	StartPoint.y = TranslateYValue(0.0);

// 	// 画曲线和圆点
// 	for (i = 0; i < len; i++)
// 	{
// 		m_MemDcDraw.MoveTo(StartPoint);						// 设定曲线起始坐标
// 
// 		PointMap     = GetPoint(PointMapNum, i);			// 从指定队列中获取数据
// 		if (i < (len - 1)) 
// 		{
// 			NextPointMap = GetPoint(PointMapNum, i + 1);		// 从指定队列中获取数据(下一个点的)
// 		}
// 		else
// 		{
// 			NextPointMap.CarNumber = -1;
// 			NextPointMap.TempValue = -1;
// 		}
// 		
// 		if (PointMap.CarNumber == NextPointMap.CarNumber)
// 		{
// 			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X轴单位尺度转换
// 			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue) / 2);	// Y轴单位尺度转换
// 			i++;
// 		}
// 		else
// 		{
// 			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X轴单位尺度转换
// 			EndPoint.y = TranslateYValue(PointMap.TempValue);	// Y轴单位尺度转换
// 		}
// 		
// 		m_MemDcDraw.LineTo(EndPoint);						// 画线操作
// 
// 		// 在数据点上画圆点
// 		m_MemDcDraw.Ellipse(EndPoint.x - 4, EndPoint.y - 4, EndPoint.x + 4, EndPoint.y + 4);
// 
// 		StartPoint = EndPoint;								// 本次终点坐标做下端直线的起始坐标
// 	}

	// 画曲线和圆点
	for (i = 0; i < len; i++)
	{
		m_MemDcDraw.MoveTo(StartPoint);						// 设定曲线起始坐标
		
		PointMap = GetPoint(PointMapNum, i);				// 从指定队列中获取数据
		if (i < (len - 1)) 
		{
			NextPointMap = GetPoint(PointMapNum, i + 1);	// 从指定队列中获取数据(下一个点的)
		}

		if (i < (len - 2))
		{
			NextNextPointMap = GetPoint(PointMapNum, i + 2);	// 从指定队列中获取数据(下下一个点的)
		}
		
		if (i >= len)
		{
			NextPointMap.CarNumber = -1;
			NextPointMap.TempValue = -1;
		}
		
		if ((PointMap.CarNumber == NextPointMap.CarNumber) && (PointMap.CarNumber == NextNextPointMap.CarNumber))
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X轴单位尺度转换
			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue + NextNextPointMap.TempValue) / 3.0);	// Y轴单位尺度转换
			i = i + 2;
		}
		else if (PointMap.CarNumber == NextPointMap.CarNumber)
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X轴单位尺度转换
			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue) / 2);	// Y轴单位尺度转换
			i++;
		}
		else
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X轴单位尺度转换
			EndPoint.y = TranslateYValue(PointMap.TempValue);	// Y轴单位尺度转换
		}
		
		m_MemDcDraw.LineTo(EndPoint);						// 画线操作
		
		// 在数据点上画圆点
		m_MemDcDraw.Ellipse(EndPoint.x - 4, EndPoint.y - 4, EndPoint.x + 4, EndPoint.y + 4);
		
		StartPoint = EndPoint;								// 本次终点坐标做下端直线的起始坐标
	}

	// 判断最后一个测温点是否是最后1个车位, 如果不是就伪造
	PointMap = GetPoint(PointMapNum, len - 1);

	if (m_iKilnType == 3)
	{
		if (PointMap.CarNumber < (m_CXInternal * 10))
		{
			m_MemDcDraw.MoveTo(StartPoint);
			EndPoint.x = TranslateXValue((double)(m_CXInternal * 10));
			EndPoint.y = TranslateYValue(0.0);
			m_MemDcDraw.LineTo(EndPoint);
		}
	}
	else
	{
		if (PointMap.CarNumber < m_CXInternal)
		{
			m_MemDcDraw.MoveTo(StartPoint);
			EndPoint.x = TranslateXValue((double)(m_CXInternal));
			EndPoint.y = TranslateYValue(0.0);
			m_MemDcDraw.LineTo(EndPoint);
		}
	}


	// 释放画笔内存操作
	m_MemDcDraw.SelectObject(pOldCurvePen);
	CurvePen.DeleteObject();

	// 释放刷子内存操作
	m_MemDcDraw.SelectObject(pOldCurveBrush);
	CurveBrush.DeleteObject();
}

// 显示更新函数
// 操作顺序: 背景->曲线坐标->曲线->鼠标提示->写入显示设备
void CCurve::PaintUpdate(CDC *pDC)
{
	DrawBackGround();										// 画背景
	DrawCoordinateSystem();									// 画坐标

	for (int i = 0; i < MAX_POINT_MAP_CNT; i++)
	{
		DrawCurve(i);										// 画曲线	
	}
		
	DrawScale();											// 画鼠标提示窗口
	
	// 将显示内存写入设备内存
	pDC->StretchBlt(0, 0, m_ParentFrameRect.Width(), m_ParentFrameRect.Height(), &m_MemDcDraw, 0, 0, m_ParentFrameRect.Width(), m_ParentFrameRect.Height(), SRCCOPY);
}

// 将车号转换为图形的X坐标
int CCurve::TranslateXValue(double CarNumber)
{
	if (m_iKilnType == 3)						// 移动窑
	{
		return ((double)(CarNumber) * (double)(m_CurveRect.Width()) / ((double)(m_CXInternal) * 10.0)) + (double)(m_CurveRect.left);
	}
	else
	{
		return ((double)(CarNumber) * (double)(m_CurveRect.Width()) / ((double)(m_CXInternal))) + (double)(m_CurveRect.left);
	}
}

// 将温度转换为图形的Y坐标
int CCurve::TranslateYValue(double TempValue)
{
	return (double)(m_CurveRect.Height()) - ((double)(TempValue) / (double)(m_CYInternal * 50) * (double)(m_CurveRect.Height())) + (double)(m_CurveRect.top);
}

// 将图形的X坐标转换为车号
double CCurve::TranslateXCoordinate(double CoordinateXValue)
{
	return 1 + (CoordinateXValue - (double)(m_CurveRect.left)) * (double)(m_CXInternal) / (double)(m_CurveRect.Width());
}

// 将图形的Y坐标转换为温度
double CCurve::TranslateYCoordinate(double CoordinateYValue)
{
	return ((double)(m_CurveRect.Height()) - CoordinateYValue + (double)(m_CurveRect.top)) / (double)(m_CurveRect.Height()) * (double)(m_CYInternal * 50);
}

// 向指定队列插入存储单元
void CCurve::AddPoint(int PointMapNum, double CarNumber, double TempValue)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	POINTMAP PointMap;
	PointMap.CarNumber = CarNumber;
	PointMap.TempValue = TempValue;
	m_PointMap[PointMapNum].push_back(PointMap);
}

// 向指定队列插入存储单元
void CCurve::AddPoint(int PointMapNum, POINTMAP PointMap)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_PointMap[PointMapNum].push_back(PointMap);
}

// 获取指定队列的指定存储单元
POINTMAP CCurve::GetPoint(int PointMapNum, int iIndex)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	return m_PointMap[PointMapNum][iIndex];
}

// 清除指定队列
void CCurve::ClearPoint(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_PointMap[PointMapNum].clear();
}

// 获取指定队列的存储单元个数
int CCurve::PointLength(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	return m_PointMap[PointMapNum].size();
}
