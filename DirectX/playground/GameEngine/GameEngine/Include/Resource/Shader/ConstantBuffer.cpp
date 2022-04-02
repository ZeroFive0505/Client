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

    // ���뵵�� ���̳������� �Ѵ�.
    // �ֳ��ϸ� ���� ��Ʈ������ ���Ӿȿ��� ����ϰ� �ٲ�⶧��
    // ������۴� ���� �� �������� ��Ʈ���� �̿ܿ��� �ܺο��� ������ �����ϰų� �ִϸ��̼��� �� ���� ���ε� �پ��ϰ� �̿�� �� �ִ�.
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    // ���뵵�� ���
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    // CPU���� �� �� �ִ�.
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &m_Buffer)))
        return false;

    return true;
}

void CConstantBuffer::UpdateBuffer(void* data)
{
    // ���� data�� ���Ӱ� ���� ����� ������.

    D3D11_MAPPED_SUBRESOURCE Map = {};

    // Map�Լ��� �̿��ؼ� �޸� ��ġ�� �����´�.
    CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

    // data�� ������ ��ġ�� �����Ѵ�.
    memcpy(Map.pData, data, m_Size);

    // Map����
    CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);

    /*
    * ���ؽ� ���̴�, �ȼ� ���̴� ��� �ٸ� ���̴��鵵 ������۸� �̿� �� �� �ִ�.
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
