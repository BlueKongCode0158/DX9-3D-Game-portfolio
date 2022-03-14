
// MFCToolView.h : CMFCToolView Ŭ������ �������̽�
//
#include "MFCToolDoc.h"
#include "Engine_Defines.h"
#pragma once

BEGIN(Engine)
class CRenderer;
END

class CMFCToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)

// Ư���Դϴ�.
public:
	CMFCToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
private:
	CRenderer*	m_pRendererCom = nullptr;
// �����Դϴ�.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
private:
	void	Update_Tick();
	HRESULT	Add_Prototype_Object();
	HRESULT	Add_Prototype_Component();
	HRESULT Add_GameObject();
	HRESULT	Add_LightDest();
private:
	CPoint	m_pCursorPoint;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCToolView.cpp�� ����� ����
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif

