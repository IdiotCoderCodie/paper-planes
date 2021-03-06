#include "D3D.h"
#include "d3d_safe_release.h"

#include <AntTweakBar.h>

D3D::D3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen) :
    m_initialized(false),
    m_vsyncEnabled(vsync),
    m_videoCardMemory(),
    m_videoCardDesc(),
    m_swapChain(0),
    m_device(0),
    m_deviceContext(0),
    m_renderTargetView(0),
    m_depthStencilBuffer(0),
    m_depthStencilState(0),
    m_depthDisabledStencilState(0),
    m_depthStencilView(0),
    m_rasterState(0),
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight),
    m_alphaEnableBlendingState(0),
    m_alphaDisableBlendingState(0)
{
    m_initialized = InitializeD3D(screenWidth, screenHeight, vsync, hwnd, fullscreen);
    TwInit(TW_DIRECT3D11, m_device);
    TwWindowSize(screenWidth, screenHeight);
}


D3D::~D3D(void)
{
    try
    {
        Shutdown();
    }
    catch (int& e)
    {
        // EXCEPTION THROWN TRYING TO SHUTDOWN
    }
}


bool D3D::InitializeD3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
	HRESULT result; //used to test things are created ok.
    vsync;
    //----------------------------------------------------------------------------------------------
    // Fetch the numerator and denominator for refresh rate and video card description.
    // Create DirectX Graphics Interface factory.
	IDXGIFactory* factory;
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if(FAILED(result))
        return false;

    // Create adapter.
	IDXGIAdapter* adapter;
    result = factory->EnumAdapters(0, &adapter);
    if(FAILED(result))
        return false;

    // Enumerate primary adapter output (monitor).
	IDXGIOutput* adapterOutput;
    result = adapter->EnumOutputs(0, &adapterOutput);
    if(FAILED(result))
        return false;

    // Get the number of modes that fit the DXGI_R8G8B8A8_UNORM display format for adpater output.
    unsigned int numModes;
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
                                DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if(FAILED(result))
        return false;

    // Create a list to hold all possible display modes.
    DXGI_MODE_DESC* displayModeList;
    displayModeList = new DXGI_MODE_DESC[numModes];
    if(!displayModeList)
        return false;

    // Fill list.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
                                DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if(FAILED(result))
        return false;

    // Loop through modes and find one that matches screen width and height, store numerator and
    // denominator for corresponding refresh rate.
    unsigned int numerator, denominator;
    for(unsigned int i = 0; i < numModes; ++i)
    {
        if(displayModeList[i].Width  == (unsigned int)screenWidth &&
           displayModeList[i].Height == (unsigned int)screenHeight)
        {
            numerator = displayModeList[i].RefreshRate.Numerator;
            denominator = displayModeList[i].RefreshRate.Denominator;
        }
    }

    // Get adapter description.
    DXGI_ADAPTER_DESC adapterDesc;
    result = adapter->GetDesc(&adapterDesc);
    if(FAILED(result))
        return false;

    // Store dedicated video card memory (in mb).
    m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to char array and store.
    unsigned int stringLength;
    int error = wcstombs_s(&stringLength, m_videoCardDesc, 128, adapterDesc.Description, 128);
    if(error != 0)
        return false;

    // Release unneeded memory.
    delete[] displayModeList;
    displayModeList = 0;

    adapterOutput->Release();
    adapterOutput = 0;

    adapter->Release();
    adapter = 0;

    factory->Release();
    factory = 0;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Set up SwapChain description and create swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    
    // Setup back buffer.
    swapChainDesc.BufferCount       = 1;
    swapChainDesc.BufferDesc.Width  = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // Refresh Rate.
    if(m_vsyncEnabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator      = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator    = denominator;  
    }
    else
    { // Draw as soon as possible.
        swapChainDesc.BufferDesc.RefreshRate.Numerator      = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator    = 1;
    }

    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow          = hwnd;
    // Turn off multisampling.
    swapChainDesc.SampleDesc.Count      = 1;
    swapChainDesc.SampleDesc.Quality    = 0;

    swapChainDesc.Windowed = !fullscreen;

    swapChainDesc.BufferDesc.ScanlineOrdering   = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling            = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect                    = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags                         = 0;

    // Create the swap chain.
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 
                1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, 
                &m_deviceContext);
    if(FAILED(result))
        return false;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Set up render target view.
    ID3D11Texture2D* backBufferPtr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
		return false;

    // Create render target view with back buffer ptr.
    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if(FAILED(result))
        return false;

    backBufferPtr->Release();
    backBufferPtr = 0;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Set up depthStencilBuffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    
    // Set up description.
    depthBufferDesc.Width               = screenWidth;
	depthBufferDesc.Height              = screenHeight;
	depthBufferDesc.MipLevels           = 1;
	depthBufferDesc.ArraySize           = 1;
	depthBufferDesc.Format              = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count    = 1;
	depthBufferDesc.SampleDesc.Quality  = 0;
	depthBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags      = 0;
	depthBufferDesc.MiscFlags           = 0;

    // Create depth/stencil buffer.
    result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
    if(FAILED(result))
        return false;

    // Setup depth stencil description.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    
    depthStencilDesc.DepthEnable                    = true;
	depthStencilDesc.DepthWriteMask                 = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc                      = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable                  = true;
	depthStencilDesc.StencilReadMask                = 0xFF;
	depthStencilDesc.StencilWriteMask               = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc          = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state.
    result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if(FAILED(result))
        return false;

    // Set depth stencil state.
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
    //----------------------------------------------------------------------------------------------



    //----------------------------------------------------------------------------------------------
    // Create second depth stencil state with depth disabled. (for 2D rendering)
    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
    ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

    depthDisabledStencilDesc.DepthEnable                    = false;
	depthDisabledStencilDesc.DepthWriteMask                 = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc                      = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable                  = true;
	depthDisabledStencilDesc.StencilReadMask                = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask               = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc          = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;

    result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, 
                            &m_depthDisabledStencilState);
    if(FAILED(result))
        return false;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create description of the view of the depth stencil buffer.
    // Do this so D3D knows to use the depth buffer as a depth stencil texture.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

    result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc,
                            &m_depthStencilView);
    if(FAILED(result))
        return false;

    // Bind render target view and depth stencil buffer to output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create rasterizer state and viewport.
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable    = false;
	rasterDesc.CullMode                 = D3D11_CULL_BACK;
	rasterDesc.DepthBias                = 0;
	rasterDesc.DepthBiasClamp           = 0.0f;
	rasterDesc.DepthClipEnable          = true;
	rasterDesc.FillMode                 = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise    = false;
	rasterDesc.MultisampleEnable        = false;
	rasterDesc.ScissorEnable            = false;
	rasterDesc.SlopeScaledDepthBias     = 0.0;

    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if(FAILED(result))
        return false;

    m_deviceContext->RSSetState(m_rasterState);

    // Set up viewport.
    D3D11_VIEWPORT viewport;
    viewport.Width     = (float)screenWidth;
    viewport.Height    = (float)screenHeight;
    viewport.MinDepth  = 0.0f;
    viewport.MaxDepth  = 1.0f;
    viewport.TopLeftX  = 0.0f;
    viewport.TopLeftY  = 0.0f;

    m_deviceContext->RSSetViewports(1, &viewport);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create blend states.
    D3D11_BLEND_DESC blendStateDesc;
    ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
    blendStateDesc.RenderTarget[0].BlendEnable           = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    result = m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState);
    if(FAILED(result))
    {
        return false;
    }

    blendStateDesc.RenderTarget[0].BlendEnable = FALSE;
    result = m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState);
    if(FAILED(result))
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------

    return true;
}


void D3D::Shutdown()
{
    if(m_swapChain)
	{
		m_swapChain = 0;
	}

	if(m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if(m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView =0;
	}

	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if(m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if(m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

    d3d_safe_release(m_alphaEnableBlendingState);
    d3d_safe_release(m_alphaDisableBlendingState);
}


void D3D::BeginScene(float r, float g, float b, float a)
{
    float color[4] = { r, g, b, a };

    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void D3D::EndScene()
{
    if(m_vsyncEnabled)
    {
        m_swapChain->Present(1, 0);
    }
    else
    {
        m_swapChain->Present(0, 0);
    }
}


ID3D11Device& D3D::GetDevice()
{
    return *m_device;
}


ID3D11DeviceContext& D3D::GetDeviceContext()
{
    return *m_deviceContext;
}


void D3D::GetVideoCardInfo(char* cardName, int& memory) const
{
    strcpy_s(cardName, 128, m_videoCardDesc);
    memory = m_videoCardMemory;
}


void D3D::TurnZBufferOn()
{
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3D::TurnZBufferOff()
{
    m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void D3D::ClearDepthStencilView()
{
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D::SetBackBufferRenderTarget()
{
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}


void D3D::EnableAlphaBlending()
{
    float blendFactor[4];
    blendFactor[0] = 0.0f;
    blendFactor[1] = 0.0f;
    blendFactor[2] = 0.0f;
    blendFactor[3] = 0.0f;

    m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3D::DisableAlphaBlending()
{
     float blendFactor[4];
    blendFactor[0] = 0.0f;
    blendFactor[1] = 0.0f;
    blendFactor[2] = 0.0f;
    blendFactor[3] = 0.0f;

    m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}