#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include <unordered_map>
#include <string>
#include "ConstantBuffer.h"
#include "GraphicsResourceHandles.h"


class ConstantBufferPair;

class ShaderResourcer 
{
public:
    int GenerateVertexShaderFromFile(
        ID3D11Device* device, ID3D11DeviceContext* deviceContext,  
        const char* FileName, const char *EntryPoint, const char *ShaderModel);

    int GeneratePixelShaderFromFile(
        ID3D11Device* device, ID3D11DeviceContext* deviceContext,  
        const char* FileName, const char *EntryPoint, const char *ShaderModel);

    VertexShaderHandle getVertexShaderHandle(std::string shaderFileName);
    PixelShaderHandle getPixelShaderHandle(std::string shaderFileName);

    ID3D11VertexShader* getVertexShaderByFile(std::string shaderFileName);
    ID3D11PixelShader* getPixelShaderByFile(std::string shaderFileName);
    ID3D11InputLayout* getInputLayoutByFile(std::string shaderFileName);
    ConstantBufferPair getConstantBufferByFile(std::string shaderFileName);

    ID3D11VertexShader* getVertexShader(VertexShaderHandle handle);
    ID3D11InputLayout* getInputLayout(VertexShaderHandle handle);
    ID3D11PixelShader* getPixelShader(PixelShaderHandle handle);
    ConstantBufferPair getConstantBuffer(VertexShaderHandle handle);

    ConstantBufferPair* getAllConstantBuffers(int& size);
    int debug_GetInputLayoutByteLength(VertexShaderHandle vHandle);

    void Dispose();

    static ShaderResourcer& Instance()
    {
        static ShaderResourcer  instance; 
        return instance;
    }

private:
    std::unordered_map<std::string, int> nameToVertexShaderID;
    std::unordered_map<std::string, int> nameToPixelShaderID;

    std::unordered_map<int, ID3D11VertexShader*> vertexShaderMap;
    std::unordered_map<int, ID3D11PixelShader*> pixelShaderMap;
    std::unordered_map<int, ID3D11InputLayout*> inputLayouts;
    std::unordered_map<int, int> inputLayoutByteLength;
    std::unordered_map<int, ConstantBufferPair> constantBuffers;

    static int IDGen_vs;
    static int IDGen_ps;
    int generateVertexShaderID();
    int generatePixelShaderID();

    int GenerateConstantBuffers(
        ID3D11Device* device, 
        ID3D11Buffer** ppInfrequentBuffer, ID3D11Buffer** ppPerFrameBuffer);

    //Singleton
    ShaderResourcer() {};
    ShaderResourcer(ShaderResourcer const&);              
    void operator=(ShaderResourcer const&); 

};

class ConstantBufferPair
{
public: 
    ConstantBufferPair() :
        infrequentBuffer(nullptr), frameBuffer(nullptr)
    {}

    ConstantBufferPair(ID3D11Buffer* infrequentBuffer_In, ID3D11Buffer* frameBuffer_In) : 
        infrequentBuffer(infrequentBuffer_In), frameBuffer(frameBuffer_In) 
    {}

    ~ConstantBufferPair()
    {}

    void Dispose()
    {
        infrequentBuffer->Release();
        frameBuffer->Release();
    }

    ID3D11Buffer* infrequentBuffer;
    ID3D11Buffer* frameBuffer;
};