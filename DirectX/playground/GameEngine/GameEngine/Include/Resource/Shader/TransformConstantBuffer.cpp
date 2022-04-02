#include "TransformConstantBuffer.h"
#include "ConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer() :
    m_BufferData{}
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& buffer) :
    CConstantBufferBase(buffer)
{
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::Init()
{
    // 리소스매니저에서 쉐이더매니저를 초기화할때 만든 상수버퍼를 이용한다.
    SetConstantBuffer("TransformCBuffer");

    return true;
}

void CTransformConstantBuffer::UpdateCBuffer()
{
    m_BufferData.matWV = m_BufferData.matWorld * m_BufferData.matView;
    m_BufferData.matWVP = m_BufferData.matWV * m_BufferData.matProj;
    m_BufferData.matVP = m_BufferData.matView * m_BufferData.matProj;

    // Shader에서 사용할 수 있게 하기 위해서 행렬을 전치해서 넘겨준다.
    m_BufferData.matWorld.Transpose();
    m_BufferData.matView.Transpose();
    m_BufferData.matProj.Transpose();
    m_BufferData.matWV.Transpose();
    m_BufferData.matWVP.Transpose();
    m_BufferData.matVP.Transpose();

    m_Buffer->UpdateBuffer(&m_BufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
    return new CTransformConstantBuffer(*this);
}
