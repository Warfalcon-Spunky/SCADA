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
 m_CurveRect(0, 0, iMaxWidth, iMaxHeight),	// ��������
 m_CXInternal(CXInternal),
 m_CYInternal(CYInternal),
 m_crBkColor(RGB(255, 255, 255)),			// Ĭ�ϱ�����ɫΪ��ɫ		
 m_crFtColor(RGB(0, 0, 0)),					// ����ǰ����ɫΪ��ɫ
 m_crCpColor(RGB(0, 0, 255)),				// ���ñ���������ɫΪ��ɫ
 m_crTipBkColor(RGB(127, 127, 127)),		// �����ʾ������ɫ
 m_crTipFtColor(RGB(200, 200, 200)),		// �����ʾǰ����ɫ
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
	bmpinfo.bmiHeader.biWidth = m_iMaxWidth;				// ��������ͼƬ, �ߴ���Ϊ��Ļ�ߴ���
	bmpinfo.bmiHeader.biHeight = m_iMaxHeight;				// ��������ͼƬ, �ߴ�߶�Ϊ��Ļ�ߴ���
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;
	
	m_MemDcDraw.CreateCompatibleDC(m_pDC);												// �������ݻ���, ����������
	m_HbtMap = CreateDIBSection(m_MemDcDraw, &bmpinfo, DIB_RGB_COLORS, NULL, NULL, 0);	// �������豸�޹ص��ڴ�
	m_MemDcDraw.SelectObject(m_HbtMap);													// ѡ��ָ���ڴ�
	m_MemDcDraw.SetBkMode(TRANSPARENT);													// ��������Ϊ͸��

	// ����������߶���
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

// ���¸����ڿͻ����ĳߴ�, �ڸ����ڵ�WM_SIZE��Ϣ�����е���
void CCurve::SetClientRect(CRect cRect)
{
	m_ParentFrameRect = cRect;

	m_CurveFrameRect.left = m_MaxKeepSpaceLeftValue;
	m_CurveFrameRect.right = cRect.Width() - m_MaxKeepSpaceRightValue;

	m_CurveFrameRect.top = m_MaxKeepSpaceTopValue;
	m_CurveFrameRect.bottom = cRect.Height() - m_MaxKeepSpaceBottomValue;
}

// ���ñ���ɫ
void CCurve::SetBackGround(COLORREF crBkColor)
{
	m_crBkColor = crBkColor;
}

// ����ǰ��ɫ
void CCurve::SetFrontGround(COLORREF crFtColor)
{
	m_crFtColor = crFtColor;
}

// �������߱������ɫ
void CCurve::SetCaptionColor(COLORREF crCpColor)
{
	m_crCpColor = crCpColor;
}

// ���������ʾ��ı���ɫ
void CCurve::SetTipBkColor(COLORREF crTipBkColor)
{
	m_crTipBkColor = crTipBkColor;
}

// ���������ʾ���ǰ��ɫ
void CCurve::SetTipFtColor(COLORREF crTipFtColor)
{
	m_crTipFtColor = crTipFtColor;
}

// ����������ɫ
void CCurve::SetCurveColor(int PointMapNum, COLORREF crCurveColor)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_crCurveColor[PointMapNum] = crCurveColor;
}

// �������߱�������
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

// �����������, �ú����ڸ�����WM_MOUSEMOVE��Ϣ�����е���
void CCurve::UpdateMousePoint(POINT MousePoint)
{
	m_MousePoint = MousePoint;
}

// �ػ汳����ɫ
void CCurve::DrawBackGround(void)
{
	CBrush brBack, *pOldcrBack;

	// ����ˢ�Ӳ�����������, �ߴ�Ϊ�����ڵĿͻ�����С
	brBack.CreateSolidBrush(m_crBkColor);
	m_MemDcDraw.FillRect(&m_ParentFrameRect, &brBack);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	

	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();	
}

// �ػ��������꺯��
void CCurve::DrawCoordinateSystem(void)
{
	CPen GrayPen, *pOldGrayPen;
	CPen DashPen, *pOldDashPen;

	int Index;
	int InternalWidth, InternalHeight;

	// ����X��Y����������ߵĸ���
	InternalWidth = m_CurveFrameRect.Width() / m_CXInternal;
	InternalHeight = m_CurveFrameRect.Height() / m_CYInternal;

	// ����������������, ��ֹ�����ڳߴ�ı�
	m_CurveRect.left = m_CurveFrameRect.left;
	m_CurveRect.top = m_CurveFrameRect.top;
	m_CurveRect.right = InternalWidth * m_CXInternal + m_CurveFrameRect.left;
	m_CurveRect.bottom = InternalHeight * m_CYInternal + m_CurveFrameRect.top;

	// ��������߿�
	GrayPen.CreatePen(PS_SOLID, 3, m_crFtColor);
	pOldGrayPen = m_MemDcDraw.SelectObject(&GrayPen);
//	m_MemDcDraw.Rectangle(&m_CurveRect);					// ���ο�����ɫ�뱳����һ��

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

	// �����ߵ�������
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

	// ��ʾ����
	CFont Font;
	CFont *pOldFont;
	Font.CreateFont(36, 0, 0, 0, FW_HEAVY, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "��������");
	pOldFont = m_MemDcDraw.SelectObject(&Font);

	m_MemDcDraw.SetTextColor(m_crCpColor);
	m_MemDcDraw.DrawText(m_strCaption, CRect(m_ParentFrameRect.left, m_ParentFrameRect.top, m_ParentFrameRect.right, m_ParentFrameRect.top + 50), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_MemDcDraw.SelectObject(pOldFont);
	Font.DeleteObject();

	// ͼ������
	m_MemDcDraw.SetTextColor(m_crFtColor);
	m_MemDcDraw.TextOut(m_CurveRect.left, m_CurveRect.top - 20, "Temperature���棩");

	if (m_iKilnType == 3)
	{
		m_MemDcDraw.TextOut(m_CurveRect.right + 15, m_CurveRect.bottom + 5, "metre");
	}
	else
	{
		m_MemDcDraw.TextOut(m_CurveRect.right + 15, m_CurveRect.bottom + 5, "positon");
	}

	CString str;
	// ����X���α��ַ�
	for (Index = 2; Index <= (m_CXInternal + 2); Index++)
	{
		if (m_iKilnType == 3)						// �ƶ�Ҥ
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

	// ����Y���α��ַ�
	for (Index = 2; Index <= (m_CYInternal + 2); Index++)
	{
		str.Format("%d", (Index - 2) * 50);
		m_MemDcDraw.TextOut(m_CurveRect.left - 40, m_CurveRect.bottom - InternalHeight * (Index - 2) - 10, str);
		str.Empty();
	}
}

// �ػ������ʾ����
// �����ָ������ָ�����������, ����긽����ʾ���ݵ������ֵ
void CCurve::DrawScale(void)
{
	POINTMAP PointMap, NextPointMap, NextNextPointMap;
	CRect rect;
	int i, j, len;
	int x, y;
	double TempVal;
	CString strScale;

	// ��������
	CFont Font, *pOldFont;
	Font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����_GB2312");
	pOldFont = m_MemDcDraw.SelectObject(&Font);

	// ��������, ����ʾ�����
	CPen GrayPen, *pOldGrayPen;
	GrayPen.CreatePen(PS_SOLID, 1, m_crTipBkColor);
	pOldGrayPen = m_MemDcDraw.SelectObject(&GrayPen);

	// ����ˢ��, �����ʾ��
	CBrush brBack, *pOldcrBack;
	brBack.CreateSolidBrush(m_crTipBkColor);	
	pOldcrBack = m_MemDcDraw.SelectObject(&brBack);	

	// ������е�����
	for (j = 0; j < MAX_POINT_MAP_CNT; j++)
	{
		// ��ָ�����ݶ��е�����ȡ�������������, �Ƚ���������Ƿ���֮��
		for (i = 0, len = PointLength(j); i < len; i++)
		{
			// ��ȡָ���������ݲ��������������
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

			// �������ݵ�ķ�Χ����
			rect.left = x - 2;
			rect.right = x + 2;
			rect.top = y - 2;
			rect.bottom = y + 2;

			// �ж���������Ƿ���ָ���ķ�Χ��
			if (rect.PtInRect(m_MousePoint))
			{
				// �����ַ���
				strScale.Empty();

				if (m_iKilnType == 3)						// �ƶ�Ҥ
				{
					strScale.Format("����: %d��  �¶�: %d��", (int)(PointMap.CarNumber), (int)(TempVal));
				}
				else
				{
					strScale.Format("��λ: %d��  �¶�: %d��", (int)(PointMap.CarNumber), (int)(TempVal));
				}
				
				// ���Ҫ����ʾ������������, ������ʾ�����ʾλ��
				if ((m_MousePoint.x + 300) > m_CurveFrameRect.right)	// ��ʾ�������������·�
				{
					rect.left = m_MousePoint.x - 300;
					rect.right = m_MousePoint.x + 5;
					rect.top = m_MousePoint.y - 5;
					rect.bottom = m_MousePoint.y - 64;
				}
				else													// ��ʾ�������������·�
				{
					rect.left = m_MousePoint.x + 5;
					rect.right = m_MousePoint.x + 300;
					rect.top = m_MousePoint.y - 5;
					rect.bottom = m_MousePoint.y - 64;
				}

				// ����ʾ������
				m_MemDcDraw.RoundRect(&rect, CPoint(24, 24));
				
				// ��ʾ��ʾ����������������ɫ
				m_MemDcDraw.SetTextColor(m_crTipFtColor);
				m_MemDcDraw.DrawText(strScale, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
		}
	}

	// �ͷ�ˢ���ڴ����
	m_MemDcDraw.SelectObject(pOldcrBack);
	brBack.DeleteObject();

	// �ͷŻ����ڴ����
	m_MemDcDraw.SelectObject(pOldGrayPen);
	GrayPen.DeleteObject();

	// �ͷ������ڴ����
	m_MemDcDraw.SelectObject(pOldFont);
	Font.DeleteObject();
}

// �ػ����߲���
// ����: ָ���洢���к�
void CCurve::DrawCurve(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	int   i, len;
	POINT StartPoint, EndPoint;
	POINTMAP PointMap, NextPointMap, NextNextPointMap;

	// ��ȡָ�����д洢��Ԫ����, ����2���޷���ֱ��, ֱ���˳�
	len = PointLength(PointMapNum);
	if (len < 2)
	{
		return;
	}

	// �������ʻ�����, ָ��������ɫ
	CPen CurvePen, *pOldCurvePen;
	CurvePen.CreatePen(PS_SOLID, 4, m_crCurveColor[PointMapNum]);
	pOldCurvePen = m_MemDcDraw.SelectObject(&CurvePen);

	// ����ˢ�ӻ�Բ��, ָ��Բ����ɫ
	CBrush CurveBrush;  
	CurveBrush.CreateSolidBrush(m_crCurveColor[PointMapNum]);
	CBrush *pOldCurveBrush = m_MemDcDraw.SelectObject(&CurveBrush);

	// α��1�ų�λ�¶�
	StartPoint.x = TranslateXValue(0.0);
	StartPoint.y = TranslateYValue(0.0);

// 	// �����ߺ�Բ��
// 	for (i = 0; i < len; i++)
// 	{
// 		m_MemDcDraw.MoveTo(StartPoint);						// �趨������ʼ����
// 
// 		PointMap     = GetPoint(PointMapNum, i);			// ��ָ�������л�ȡ����
// 		if (i < (len - 1)) 
// 		{
// 			NextPointMap = GetPoint(PointMapNum, i + 1);		// ��ָ�������л�ȡ����(��һ�����)
// 		}
// 		else
// 		{
// 			NextPointMap.CarNumber = -1;
// 			NextPointMap.TempValue = -1;
// 		}
// 		
// 		if (PointMap.CarNumber == NextPointMap.CarNumber)
// 		{
// 			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X�ᵥλ�߶�ת��
// 			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue) / 2);	// Y�ᵥλ�߶�ת��
// 			i++;
// 		}
// 		else
// 		{
// 			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X�ᵥλ�߶�ת��
// 			EndPoint.y = TranslateYValue(PointMap.TempValue);	// Y�ᵥλ�߶�ת��
// 		}
// 		
// 		m_MemDcDraw.LineTo(EndPoint);						// ���߲���
// 
// 		// �����ݵ��ϻ�Բ��
// 		m_MemDcDraw.Ellipse(EndPoint.x - 4, EndPoint.y - 4, EndPoint.x + 4, EndPoint.y + 4);
// 
// 		StartPoint = EndPoint;								// �����յ��������¶�ֱ�ߵ���ʼ����
// 	}

	// �����ߺ�Բ��
	for (i = 0; i < len; i++)
	{
		m_MemDcDraw.MoveTo(StartPoint);						// �趨������ʼ����
		
		PointMap = GetPoint(PointMapNum, i);				// ��ָ�������л�ȡ����
		if (i < (len - 1)) 
		{
			NextPointMap = GetPoint(PointMapNum, i + 1);	// ��ָ�������л�ȡ����(��һ�����)
		}

		if (i < (len - 2))
		{
			NextNextPointMap = GetPoint(PointMapNum, i + 2);	// ��ָ�������л�ȡ����(����һ�����)
		}
		
		if (i >= len)
		{
			NextPointMap.CarNumber = -1;
			NextPointMap.TempValue = -1;
		}
		
		if ((PointMap.CarNumber == NextPointMap.CarNumber) && (PointMap.CarNumber == NextNextPointMap.CarNumber))
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X�ᵥλ�߶�ת��
			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue + NextNextPointMap.TempValue) / 3.0);	// Y�ᵥλ�߶�ת��
			i = i + 2;
		}
		else if (PointMap.CarNumber == NextPointMap.CarNumber)
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X�ᵥλ�߶�ת��
			EndPoint.y = TranslateYValue((PointMap.TempValue + NextPointMap.TempValue) / 2);	// Y�ᵥλ�߶�ת��
			i++;
		}
		else
		{
			EndPoint.x = TranslateXValue(PointMap.CarNumber);	// X�ᵥλ�߶�ת��
			EndPoint.y = TranslateYValue(PointMap.TempValue);	// Y�ᵥλ�߶�ת��
		}
		
		m_MemDcDraw.LineTo(EndPoint);						// ���߲���
		
		// �����ݵ��ϻ�Բ��
		m_MemDcDraw.Ellipse(EndPoint.x - 4, EndPoint.y - 4, EndPoint.x + 4, EndPoint.y + 4);
		
		StartPoint = EndPoint;								// �����յ��������¶�ֱ�ߵ���ʼ����
	}

	// �ж����һ�����µ��Ƿ������1����λ, ������Ǿ�α��
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


	// �ͷŻ����ڴ����
	m_MemDcDraw.SelectObject(pOldCurvePen);
	CurvePen.DeleteObject();

	// �ͷ�ˢ���ڴ����
	m_MemDcDraw.SelectObject(pOldCurveBrush);
	CurveBrush.DeleteObject();
}

// ��ʾ���º���
// ����˳��: ����->��������->����->�����ʾ->д����ʾ�豸
void CCurve::PaintUpdate(CDC *pDC)
{
	DrawBackGround();										// ������
	DrawCoordinateSystem();									// ������

	for (int i = 0; i < MAX_POINT_MAP_CNT; i++)
	{
		DrawCurve(i);										// ������	
	}
		
	DrawScale();											// �������ʾ����
	
	// ����ʾ�ڴ�д���豸�ڴ�
	pDC->StretchBlt(0, 0, m_ParentFrameRect.Width(), m_ParentFrameRect.Height(), &m_MemDcDraw, 0, 0, m_ParentFrameRect.Width(), m_ParentFrameRect.Height(), SRCCOPY);
}

// ������ת��Ϊͼ�ε�X����
int CCurve::TranslateXValue(double CarNumber)
{
	if (m_iKilnType == 3)						// �ƶ�Ҥ
	{
		return ((double)(CarNumber) * (double)(m_CurveRect.Width()) / ((double)(m_CXInternal) * 10.0)) + (double)(m_CurveRect.left);
	}
	else
	{
		return ((double)(CarNumber) * (double)(m_CurveRect.Width()) / ((double)(m_CXInternal))) + (double)(m_CurveRect.left);
	}
}

// ���¶�ת��Ϊͼ�ε�Y����
int CCurve::TranslateYValue(double TempValue)
{
	return (double)(m_CurveRect.Height()) - ((double)(TempValue) / (double)(m_CYInternal * 50) * (double)(m_CurveRect.Height())) + (double)(m_CurveRect.top);
}

// ��ͼ�ε�X����ת��Ϊ����
double CCurve::TranslateXCoordinate(double CoordinateXValue)
{
	return 1 + (CoordinateXValue - (double)(m_CurveRect.left)) * (double)(m_CXInternal) / (double)(m_CurveRect.Width());
}

// ��ͼ�ε�Y����ת��Ϊ�¶�
double CCurve::TranslateYCoordinate(double CoordinateYValue)
{
	return ((double)(m_CurveRect.Height()) - CoordinateYValue + (double)(m_CurveRect.top)) / (double)(m_CurveRect.Height()) * (double)(m_CYInternal * 50);
}

// ��ָ�����в���洢��Ԫ
void CCurve::AddPoint(int PointMapNum, double CarNumber, double TempValue)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	POINTMAP PointMap;
	PointMap.CarNumber = CarNumber;
	PointMap.TempValue = TempValue;
	m_PointMap[PointMapNum].push_back(PointMap);
}

// ��ָ�����в���洢��Ԫ
void CCurve::AddPoint(int PointMapNum, POINTMAP PointMap)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_PointMap[PointMapNum].push_back(PointMap);
}

// ��ȡָ�����е�ָ���洢��Ԫ
POINTMAP CCurve::GetPoint(int PointMapNum, int iIndex)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	return m_PointMap[PointMapNum][iIndex];
}

// ���ָ������
void CCurve::ClearPoint(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	m_PointMap[PointMapNum].clear();
}

// ��ȡָ�����еĴ洢��Ԫ����
int CCurve::PointLength(int PointMapNum)
{
	assert(PointMapNum < MAX_POINT_MAP_CNT);

	return m_PointMap[PointMapNum].size();
}
