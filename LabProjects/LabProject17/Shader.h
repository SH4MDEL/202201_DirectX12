#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Camera.h"

//��ü�� �������� �� �����ϴ� ��� ���� ������
struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4 m_xmf4x4World;
	//��ü�� ����� ���� ��ȣ
	UINT m_nMaterial;
};

//�÷��̾� ��ü�� �������� �� �����ϴ� ��� ���� ������
struct CB_PLAYER_INFO
{
	XMFLOAT4X4 m_xmf4x4World;
};

//���̴� �ҽ� �ڵ带 �������ϰ� �׷��Ƚ� ���� ��ü�� �����Ѵ�. 
class CShader
{
public:
	CShader();
	virtual ~CShader();
private:
	int m_nReferences = 0;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(const WCHAR* pszFileName, LPCSTR pszShaderName,
		LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dRootSignature);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	//virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList,
	//	XMFLOAT4X4* pxmf4x4World);

	virtual void ReleaseUploadBuffers();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, void* pContext = nullptr);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4
		* pxmf4x4World) { }
	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, MATERIAL
		* pMaterial) { }

protected:
	//���̴��� �����ϴ� ���� ��ü���� ����Ʈ(�迭)�̴�. 
	CGameObject **m_ppObjects = nullptr;
	int m_nObjects = 0;

	//���������� ���� ��ü���� ����Ʈ(�迭)�̴�. 
	ID3D12PipelineState **m_ppd3dPipelineStates = nullptr;
	int m_nPipelineStates = 0;
};


class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList,
		XMFLOAT4X4* pxmf4x4World);

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

protected:
	//�÷��̾� ��ü�� ���� ���ҽ��� ���ҽ� ������
	ID3D12Resource* m_pd3dcbPlayer = NULL;
	CB_PLAYER_INFO* m_pcbMappedPlayer = NULL;
};

//��CObjectsShader�� Ŭ������ ���� ��ü���� �����ϴ� ���̴� ��ü�̴�. 
class CObjectsShader : public CShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();

	virtual void BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void ReleaseObjects();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob * *ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob * *ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device * pd3dDevice, ID3D12RootSignature
		* pd3dGraphicsRootSignature);

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

protected:
	CGameObject** m_ppObjects = NULL;
	int m_nObjects = 0;

	//���̴� ��ü�� ���ԵǾ� �ִ� ��� ���� ��ü�鿡 ���� ���ҽ��� ���ҽ� ������
	ID3D12Resource* m_pd3dcbGameObjects = NULL;
	UINT8* m_pcbMappedGameObjects = NULL;
};
