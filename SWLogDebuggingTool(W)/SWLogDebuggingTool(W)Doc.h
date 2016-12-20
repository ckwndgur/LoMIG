
// SWLogDebuggingTool(W)Doc.h : CSWLogDebuggingToolWDoc 클래스의 인터페이스
//


#pragma once


class CSWLogDebuggingToolWDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSWLogDebuggingToolWDoc();
	DECLARE_DYNCREATE(CSWLogDebuggingToolWDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
// 구현입니다.
public:
	virtual ~CSWLogDebuggingToolWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


