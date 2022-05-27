//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	//CTreeObject* pTreeObject = new CTreeObject();
	//CGameObject *pGameObject = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"Models/Tree.txt");
	//pTreeObject->SetChild(pGameObject);
	//pTreeObject->OnInitialize();
	//pTreeObject->SetScale(1.5f, 1.5f, 1.5f);
	//pTreeObject->Rotate(0.0f, 90.0f, 0.0f);
	//pTreeObject->SetPosition(0.0f, 0.0f, 0.0f);
	//m_vGameObjects.push_back(pTreeObject);

	CGameObject* pGameObject1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"Models/Cactus.txt");
	m_vGameObjects.push_back(new CCactusObject());
	m_vGameObjects.back()->SetChild(pGameObject1);
	m_vGameObjects.back()->OnInitialize();
	m_vGameObjects.back()->Rotate(0.0f, -90.0f, 0.0f);
	m_vGameObjects.back()->SetScale(10.0f, 10.0f, 10.0f);
	m_vGameObjects.back()->SetPosition(10.0f, 0.0f, 10.0f);

	CGameObject* pGameObject2 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"Models/Cactus.txt");
	m_vGameObjects.push_back(new CCactusObject());
	m_vGameObjects.back()->SetChild(pGameObject2);
	m_vGameObjects.back()->OnInitialize();
	m_vGameObjects.back()->Rotate(0.0f, -90.0f, 0.0f);
	m_vGameObjects.back()->SetScale(10.0f, 10.0f, 10.0f);
	m_vGameObjects.back()->SetPosition(10.0f, 10.0f, 10.0f);

	for (int i = 0; i < 300; ++i) {
		m_vGameObjects.push_back(new CCactusObject());
		m_vGameObjects.back()->SetChild(CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, L"Models/Cactus.txt"));
		m_vGameObjects.back()->OnInitialize();
		m_vGameObjects.back()->Rotate(0.0f, -90.0f, 0.0f);
		m_vGameObjects.back()->SetScale(10.0f, 10.0f, 10.0f);
		m_vGameObjects.back()->SetPosition(10.0f, i * 10.0f, 10.0f);
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	while (m_vGameObjects.size())
	{
		m_vGameObjects.pop_back();
	}

	ReleaseShaderVariables();
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)& pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CScene::ReleaseShaderVariables()
{
}

void CScene::ReleaseUploadBuffers()
{
	for (const auto& elem : m_vGameObjects) elem->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': m_vGameObjects[0]->MoveForward(+1.0f); break;
		case 'S': m_vGameObjects[0]->MoveForward(-1.0f); break;
		case 'A': m_vGameObjects[0]->MoveStrafe(-1.0f); break;
		case 'D': m_vGameObjects[0]->MoveStrafe(+1.0f); break;
		case 'Q': m_vGameObjects[0]->MoveUp(+1.0f); break;
		case 'R': m_vGameObjects[0]->MoveUp(-1.0f); break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (const auto& elem : m_vGameObjects) elem->Animate(fTimeElapsed, NULL);

	for (const auto& elem : m_vGameObjects) elem->UpdateTransform(NULL);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	for (const auto& elem : m_vGameObjects) elem->Render(pd3dCommandList, pCamera);
}