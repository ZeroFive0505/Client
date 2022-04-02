#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer() :
    m_Buffer(nullptr),
    m_Size(0),
    m_RegisterNum(0),
    m_ConstantBufferShaderType(0)
{
}

CConstantBuffer::~CConstantBuffer()
{
    SAFE_RELEASE(m_Buffer);
}

bool CConstantBuffer::Init(int size, int registerNum, int constantBufferShaderType)
{
    m_Size = size;
    m_RegisterNum = registerNum;
    m_ConstantBufferShaderType = constantBufferShaderType;

    D3D11_BUFFER_DESC desc = {};

    // 사용용도는 다이나믹으로 한다.
    // 왜냐하면 월드 매트릭스는 게임안에서 빈번하게 바뀌기때문
    // 상수버퍼는 월드 뷰 프로젝션 매트릭스 이외에도 외부에서 색깔을 지정하거나 애니메이션의 온 오프 여부등 다양하게 이용될 수 있다.
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    // 사용용도는 상수
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    // CPU에서 쓸 수 있다.
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &m_Buffer)))
        return false;

    return true;
}

void CConstantBuffer::UpdateBuffer(void* data)
{
    // 들어온 data로 새롭게 월드 행렬을 만들어낸다.

    D3D11_MAPPED_SUBRESOURCE Map = {};

    // Map함수를 이용해서 메모리 위치를 가져온다.
    CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

    // data를 가져온 위치에 복사한다.
    memcpy(Map.pData, data, m_Size);

    // Map해제
    CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);

    /*
    * 버텍스 쉐이더, 픽셀 쉐이더 등등 다른 쉐이더들도 상수버퍼를 이용 할 수 있다.
    */

    if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Vertex)
        CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);

    if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Pixel)
        CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);

    if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Domain)
        CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);

    if(m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Hull)
        CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);

    if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Geometry)
        CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);

    if (m_ConstantBufferShaderType & (int)Buffer_Shader_Type::Compute)
        CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_RegisterNum, 1, &m_Buffer);
}
