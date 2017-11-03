//--------------------------------------------------------------------------------------
// Copyright (c) Stefan Petersson 2012. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>
#include <tchar.h>

#include "WICTextureLoader.h"



//#ifndef SAFE_RELEASE
//#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
//#endif
//
//#ifndef SAFE_DELETE
//#define SAFE_DELETE(p)      { if (p) { delete (p); (p)=NULL; } }
//#endif

extern ID3D11Device* g_Device;
extern ID3D11DeviceContext* g_DeviceContext;

enum COMPUTE_BUFFER_TYPE
{
	STRUCTURED_BUFFER,
	RAW_BUFFER,
	APPEND_BUFFER
};

class ComputeBuffer
{
public:
	ID3D11Buffer*				GetResource()
	{ return _Resource; }
	ID3D11ShaderResourceView*	GetResourceView()
	{ return _ResourceView; }
	ID3D11UnorderedAccessView*	GetUnorderedAccessView()
	{ return _UnorderedAccessView; }
	ID3D11Buffer*				GetStaging()
	{ return _Staging; }
	void CopyToStaging()
	{ g_DeviceContext->CopyResource(_Staging, _Resource); }

	template<class T>
	T* Map()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource; 
		T* p = NULL;
		if(SUCCEEDED(g_DeviceContext->Map( _Staging, 0, D3D11_MAP_READ, 0, &MappedResource )))
			p = (T*)MappedResource.pData;

		return p;
	}

	void Unmap()
	{ 	g_DeviceContext->Unmap( _Staging, 0 ); }

	explicit ComputeBuffer()
	{
		_Resource = NULL;
		_ResourceView = NULL;
		_UnorderedAccessView = NULL;
		_Staging = NULL;	
	}

	~ComputeBuffer()
	{ Release(); }
	void Release()
	{

		SAFE_RELEASE(_Resource);
		SAFE_RELEASE(_ResourceView);
		SAFE_RELEASE(_UnorderedAccessView);
		SAFE_RELEASE(_Staging);

	}
private:
	ComputeBuffer(const ComputeBuffer& cb) {}

	ID3D11Buffer*				_Resource;
	ID3D11ShaderResourceView*	_ResourceView;
	ID3D11UnorderedAccessView*	_UnorderedAccessView;
	ID3D11Buffer*				_Staging;

	friend class ComputeWrap;
};

class ComputeTexture
{
public:
	ID3D11Texture2D*			GetResource()
	{ return _Resource; }
	ID3D11ShaderResourceView*	GetResourceView()
	{ return _ResourceView; }
	ID3D11UnorderedAccessView*	GetUnorderedAccessView()
	{ return _UnorderedAccessView; }
	ID3D11Texture2D*			GetStaging()
	{ return _Staging; }
	void CopyToStaging()
	{ g_DeviceContext->CopyResource(_Staging, _Resource); }

	template<class T>
	T* Map()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource; 
		T* p = NULL;
		if(SUCCEEDED(g_DeviceContext->Map( _Staging, 0, D3D11_MAP_READ, 0, &MappedResource )))
			p = (T*)MappedResource.pData;

		return p;
	}

	void Unmap()
	{ 	g_DeviceContext->Unmap( _Staging, 0 ); }

	explicit ComputeTexture()
	{
		_Resource = NULL;
		_ResourceView = NULL;
		_UnorderedAccessView = NULL;
		_Staging = NULL;	
	}

	~ComputeTexture()
	{ Release(); }
	void Release()
	{
		SAFE_RELEASE(_Resource);
		SAFE_RELEASE(_ResourceView);
		SAFE_RELEASE(_UnorderedAccessView);
		SAFE_RELEASE(_Staging);
	}
private:
	ComputeTexture(const ComputeBuffer& cb) {}

	ID3D11Texture2D*			_Resource = NULL;
	ID3D11ShaderResourceView*	_ResourceView = NULL;
	ID3D11UnorderedAccessView*	_UnorderedAccessView = NULL;
	ID3D11Texture2D*			_Staging = NULL;

	friend class ComputeWrap;
};

class ComputeShader
{
	friend class ComputeWrap;

	ID3D11Device*               mD3DDevice = NULL;
	ID3D11DeviceContext*        mD3DDeviceContext = NULL;

	ID3D11ComputeShader*		mShader = NULL;
private:
	explicit ComputeShader();

	bool Init(std::string shaderFile, ID3D11Device* d3dDevice, ID3D11DeviceContext*d3dContext);

public:
	~ComputeShader();

	void Set();
	void Unset();
};

class ComputeWrap
{
	ID3D11Device*               mD3DDevice = NULL;
	ID3D11DeviceContext*        mD3DDeviceContext = NULL;

public:
	ComputeWrap(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
	{
		mD3DDevice = d3dDevice;
		mD3DDeviceContext = d3dContext;
	}

	ComputeShader* CreateComputeShader(TCHAR* shaderFile);

	ID3D11Buffer* CreateConstantBuffer(UINT uSize, VOID* pInitData, char* debugName = NULL);

	ComputeBuffer* CreateBuffer(COMPUTE_BUFFER_TYPE uType, UINT uElementSize,
		UINT uCount, bool bSRV, bool bUAV,bool bAppend, VOID* pInitData, bool bCreateStaging = false, char* debugName = NULL);

	ComputeTexture* CreateTexture(DXGI_FORMAT dxFormat,	UINT uWidth,
		UINT uHeight, UINT uRowPitch, VOID* pInitData, bool bCreateStaging = false, char* debugName = NULL);

	ComputeTexture* CreateTexture(TCHAR* textureFilename, char* debugName = NULL);

private:
	ID3D11Buffer* CreateStructuredBuffer(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData);
	ID3D11Buffer* CreateAppendBuffer(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData);
	ID3D11Buffer* CreateRawBuffer(UINT uSize, VOID* pInitData);
	ID3D11ShaderResourceView* CreateBufferSRV(ID3D11Buffer* pBuffer);
	ID3D11UnorderedAccessView* CreateBufferUAV(ID3D11Buffer* pBuffer);
	ID3D11UnorderedAccessView* CreateBufferUAVAppend(ID3D11Buffer* pBuffer);
	ID3D11Buffer* CreateStagingBuffer(UINT uSize);

	//texture functions
	ID3D11Texture2D* CreateTextureResource(DXGI_FORMAT dxFormat,
		UINT uWidth, UINT uHeight, UINT uRowPitch, VOID* pInitData);
	//ID3D11Buffer* CreateRawBuffer(UINT uSize, VOID* pInitData);
	ID3D11ShaderResourceView* CreateTextureSRV(ID3D11Texture2D* pTexture);
	ID3D11UnorderedAccessView* CreateTextureUAV(ID3D11Texture2D* pTexture);
	ID3D11Texture2D* CreateStagingTexture(ID3D11Texture2D* pTexture);
	
	void SetDebugName(ID3D11DeviceChild* object, char* debugName);
};