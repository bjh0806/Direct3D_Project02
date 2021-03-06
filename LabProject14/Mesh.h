#pragma once

class CVertex
{
protected:    
	XMFLOAT3 m_xmf3Position;

public: 
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); } 
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; } 
	~CVertex() { }
};

class CDiffusedVertex : public CVertex 
{

protected:
	XMFLOAT4 m_xmf4Diffuse;

public: 
	CDiffusedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); } 
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; } 
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; } 
	~CDiffusedVertex() { }
};

class CMesh
{

public:    
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {};
	virtual ~CMesh();

private: 
	int m_nReferences = 0;

public: 
	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

protected: 
	ID3D12Resource *m_pd3dVertexBuffer = NULL; 
	ID3D12Resource *m_pd3dVertexUploadBuffer = NULL;	   
	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; 
	
	UINT m_nSlot = 0; 
	UINT m_nVertices = 0; 
	UINT m_nStride = 0; 
	UINT m_nOffset = 0;

protected: 
	ID3D12Resource *m_pd3dIndexBuffer = NULL; 
	ID3D12Resource *m_pd3dIndexUploadBuffer = NULL; 

	D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView;
		   
	UINT m_nIndices = 0;
	UINT m_nStartIndex = 0;
	int m_nBaseVertex = 0;

public: 
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};

class CTriangleMesh : public CMesh { 

public:    
	CTriangleMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);    

	virtual ~CTriangleMesh() { }
};

class CCubeMeshDiffused : public CMesh {

public: 
	CCubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f); 
	virtual ~CCubeMeshDiffused(); 
};

class CAirplaneMeshDiffused : public CMesh { 

public:
	CAirplaneMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f)); 
	virtual ~CAirplaneMeshDiffused();
};

class CHeightMapImage {
private: 
	BYTE *m_pHeightMapPixels;
	int m_nWidth; int m_nLength;
	XMFLOAT3 m_xmf3Scale;

public: 
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale); 
	~CHeightMapImage(void);
		
	float GetHeight(float x, float z);
	XMFLOAT3 GetHeightMapNormal(int x, int z);
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
		
	BYTE *GetHeightMapPixels() { return(m_pHeightMapPixels); } 
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapGridMesh : public CMesh {
protected: 
	int m_nWidth; 
	int m_nLength; 
	XMFLOAT3 m_xmf3Scale;

public: 
	CHeightMapGridMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL); virtual ~CHeightMapGridMesh();
	XMFLOAT3 GetScale() { return(m_xmf3Scale); } int GetWidth() { return(m_nWidth); } int GetLength() { return(m_nLength); }
	
	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext); 
};