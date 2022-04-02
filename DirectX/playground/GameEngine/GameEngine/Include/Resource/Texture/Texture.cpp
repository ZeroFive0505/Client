#include "Texture.h"
#include "../../PathManager.h"
#include "../../Device.h"

CTexture::CTexture() :
    m_Scene(nullptr),
    m_ImageType(Image_Type::Atlas)
{
}

CTexture::~CTexture()
{
    size_t size = m_vecTextureInfo.size();

    for (size_t i = 0; i < size; i++)
    {
        SAFE_DELETE(m_vecTextureInfo[i]);
    }
}

bool CTexture::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    sTextureResourceInfo* info = new sTextureResourceInfo;

    SetName(name);

    const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

    TCHAR* fullPath = new TCHAR[MAX_PATH];
    memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);

    if (path)
        lstrcpy(fullPath, path->path);

    lstrcat(fullPath, fileName);

    info->fullPath = fullPath;

    info->fileName = new TCHAR[MAX_PATH];
    memset(info->fileName, 0, sizeof(TCHAR) * MAX_PATH);

    lstrcpy(info->fileName, fileName);

    info->pathName = new char[MAX_PATH];
    memset(info->pathName, 0, sizeof(char) * MAX_PATH);

    strcpy_s(info->pathName, pathName.length() + 1, pathName.c_str());

    char ext[_MAX_EXT] = {};
    char fullPathMultiByte[MAX_PATH] = {};

#ifdef UNICODE

    int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultiByte, convertLength, nullptr, nullptr);

#else

    strcpy_s(fullPathMultiByte, fullPath);

#endif // UNICODE

    _splitpath_s(fullPathMultiByte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

    _strupr_s(ext);

    ScratchImage* Image = new ScratchImage;

    if (strcmp(ext, ".DDS") == 0)
    {
        if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, *Image)))
        {
            SAFE_DELETE(Image);
            SAFE_DELETE(info);
            return false;
        }
    }
    else if (strcmp(ext, ".TGA") == 0)
    {
        if (FAILED(LoadFromTGAFile(fullPath, nullptr, *Image)))
        {
            SAFE_DELETE(Image);
            SAFE_DELETE(info);
            return false;
        }
    }
    else
    {
        if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, *Image)))
        {
            SAFE_DELETE(Image);
            SAFE_DELETE(info);
            return false;
        }
    }

    info->Image = Image;

    m_vecTextureInfo.push_back(info);

    return CreateResource(0);
}

bool CTexture::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    sTextureResourceInfo* info = new sTextureResourceInfo;

    SetName(name);

    TCHAR* temp = new TCHAR[MAX_PATH];
    memset(temp, 0, sizeof(TCHAR) * MAX_PATH);

    lstrcpy(temp, fullPath);

    info->fullPath = temp;

    info->fileName = new TCHAR[MAX_PATH];
    memset(info->fileName, 0, sizeof(TCHAR) * MAX_PATH);

    int pathLength = lstrlen(info->fullPath);

    for (int i = pathLength - 1; i > 0; i--)
    {
        // 역슬래쉬이면서 최소 4문자 이상있으면 확인한다.
        if (info->fullPath[i] == '\\' && i >= 4)
        {
            // /Bin를 찾는다.
            if ((info->fullPath[i - 1] == 'n' || info->fullPath[i - 1] == 'N') &&
                (info->fullPath[i - 2] == 'i' || info->fullPath[i - 2] == 'I') &&
                (info->fullPath[i - 3] == 'b' || info->fullPath[i - 3] == 'B') &&
                info->fullPath[i - 4] == '\\')
            {
                lstrcpy(info->fileName, &info->fullPath[i + 1]);
                break;
            }
        }
    }

    TCHAR _fileExt[_MAX_EXT] = {};

    _wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, _fileExt, _MAX_EXT);

    info->pathName = new char[MAX_PATH];
    memset(info->pathName, 0, sizeof(char) * MAX_PATH);

    strcpy_s(info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);

    char ext[_MAX_EXT] = {};

#ifdef UNICODE

    int convertLength = WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, ext, convertLength, nullptr, nullptr);

#else

    strcpy_s(ext, _fileExt);

#endif // UNICODE

    _strupr_s(ext);

    ScratchImage* Image = new ScratchImage;

    if (strcmp(ext, ".DDS") == 0)
    {
        if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, *Image)))
        {
            SAFE_DELETE(info);
            SAFE_DELETE(Image);
            return false;
        }
    }
    else if (strcmp(ext, ".TGA") == 0)
    {
        if (FAILED(LoadFromTGAFile(fullPath, nullptr, *Image)))
        {
            SAFE_DELETE(info);
            SAFE_DELETE(Image);
            return false;
        }
    }
    else
    {
        if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, *Image)))
        {
            SAFE_DELETE(info);
            SAFE_DELETE(Image);
            return false;
        }
    }

    info->Image = Image;

    m_vecTextureInfo.push_back(info);

    return CreateResource(0);

}

bool CTexture::LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName,
    const std::string& pathName)
{
    m_ImageType = Image_Type::Frame;

    SetName(name);

    const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

    size_t size = vecFileName.size();

    for (size_t i = 0; i < size; i++)
    {
        sTextureResourceInfo* info = new sTextureResourceInfo;

        TCHAR* fullPath = new TCHAR[MAX_PATH];
        memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);

        if (path)
            lstrcpy(fullPath, path->path);

        lstrcat(fullPath, vecFileName[i]);

        info->fullPath = fullPath;

        info->fileName = new TCHAR[MAX_PATH];
        memset(info->fileName, 0, sizeof(TCHAR) * MAX_PATH);

        lstrcpy(info->fileName, vecFileName[i]);

        info->pathName = new char[MAX_PATH];
        memset(info->pathName, 0, sizeof(char) * MAX_PATH);

        strcpy_s(info->pathName, pathName.length() + 1, pathName.c_str());

        char ext[_MAX_EXT] = {};
        char fullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

        int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
        WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, convertLength, nullptr, nullptr);

#else
        strcpy_s(fullPathMultibyte, fullPath);

#endif // UNICODE

        _splitpath_s(fullPathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

        _strupr_s(ext);

        ScratchImage* image = new ScratchImage;

        if (strcmp(ext, ".DDS") == 0)
        {
            if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }
        else if (strcmp(ext, ".TGA") == 0)
        {
            if (FAILED(LoadFromTGAFile(fullPath, nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }
        else
        {
            if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }

        info->Image = image;

        m_vecTextureInfo.push_back(info);

        if (!CreateResource((int)i))
            return false;
    }

    return true;
}

bool CTexture::LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
    m_ImageType = Image_Type::Frame;

    SetName(name);

    size_t size = vecFullPath.size();

    for (size_t i = 0; i < size; i++)
    {
        sTextureResourceInfo* info = new sTextureResourceInfo;

        TCHAR* fullPath = new TCHAR[MAX_PATH];
        memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);

        lstrcpy(fullPath, vecFullPath[i]);

        info->fullPath = fullPath;

        info->fileName = new TCHAR[MAX_PATH];
        memset(info->fileName, 0, sizeof(TCHAR) * MAX_PATH);

        int pathLength = lstrlen(info->fullPath);

        for (int i = pathLength - 1; i > 0; i--)
        {
            if (info->fullPath[i] == '\\' && i >= 4)
            {
                if ((info->fullPath[i - 1] == 'n' || info->fullPath[i - 1] == 'N') &&
                    (info->fullPath[i - 2] == 'i' || info->fullPath[i - 2] == 'I') &&
                    (info->fullPath[i - 3] == 'b' || info->fullPath[i - 3] == 'B') &&
                    info->fullPath[i - 4] == '\\')
                {
                    lstrcpy(info->fileName, &info->fullPath[i + 1]);
                    break;
                }
            }
        }

        TCHAR _fileExt[_MAX_EXT] = {};

        _wsplitpath_s(vecFullPath[i], nullptr, 0, nullptr, 0, nullptr, 0, _fileExt, _MAX_EXT);

        info->pathName = new char[MAX_PATH];
        memset(info->pathName, 0, sizeof(char) * MAX_PATH);

        strcpy_s(info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);

        char ext[_MAX_EXT] = {};

#ifdef UNICODE

        int convertLength = WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, nullptr, 0, nullptr, nullptr);
        WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, ext, convertLength, nullptr, nullptr);

#else

        strcpy_s(ext, _fileExt);

#endif // UNICODE

        _strupr_s(ext);

        ScratchImage* image = new ScratchImage;

        if (strcmp(ext, ".DDS") == 0)
        {
            if (FAILED(LoadFromDDSFile(vecFullPath[i], DDS_FLAGS_NONE, nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }
        else if (strcmp(ext, ".TGA") == 0)
        {
            if (FAILED(LoadFromTGAFile(vecFullPath[i], nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }
        else
        {
            if (FAILED(LoadFromWICFile(vecFullPath[i], WIC_FLAGS_NONE, nullptr, *image)))
            {
                SAFE_DELETE(info);
                SAFE_DELETE(image);
                return false;
            }
        }

        info->Image = image;

        m_vecTextureInfo.push_back(info);

        if (!CreateResource((int)i))
            return false;
    }

    return true;
}

bool CTexture::CreateResource(int index)
{
    sTextureResourceInfo* info = m_vecTextureInfo[index];

    if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), info->Image->GetImages(),
        info->Image->GetImageCount(), info->Image->GetMetadata(), &info->SRV)))
        return false;

    info->width = (unsigned int)info->Image->GetImages()[0].width;
    info->height = (unsigned int)info->Image->GetImages()[0].height;

    return true;
}

void CTexture::SetShader(int registerNum, int shaderType, int index)
{
    if (m_ImageType != Image_Type::Array)
    {
        if (shaderType & (int)Buffer_Shader_Type::Vertex)
            CDevice::GetInst()->GetContext()->VSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);


        if (shaderType & (int)Buffer_Shader_Type::Pixel)
            CDevice::GetInst()->GetContext()->PSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);


        if (shaderType & (int)Buffer_Shader_Type::Domain)
            CDevice::GetInst()->GetContext()->DSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);


        if (shaderType & (int)Buffer_Shader_Type::Hull)
            CDevice::GetInst()->GetContext()->HSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);


        if (shaderType & (int)Buffer_Shader_Type::Geometry)
            CDevice::GetInst()->GetContext()->GSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);


        if (shaderType & (int)Buffer_Shader_Type::Compute)
            CDevice::GetInst()->GetContext()->CSSetShaderResources(registerNum, 1, &m_vecTextureInfo[index]->SRV);
    }
    else
    {

    }
}

void CTexture::ResetShader(int registerNum, int shaderType, int index)
{
    ID3D11ShaderResourceView* SRV = nullptr;

    if (m_ImageType != Image_Type::Array)
    {
        if (shaderType & (int)Buffer_Shader_Type::Vertex)
            CDevice::GetInst()->GetContext()->VSSetShaderResources(registerNum, 1, &SRV);


        if (shaderType & (int)Buffer_Shader_Type::Pixel)
            CDevice::GetInst()->GetContext()->PSSetShaderResources(registerNum, 1, &SRV);


        if (shaderType & (int)Buffer_Shader_Type::Domain)
            CDevice::GetInst()->GetContext()->DSSetShaderResources(registerNum, 1, &SRV);


        if (shaderType & (int)Buffer_Shader_Type::Hull)
            CDevice::GetInst()->GetContext()->HSSetShaderResources(registerNum, 1, &SRV);


        if (shaderType & (int)Buffer_Shader_Type::Geometry)
            CDevice::GetInst()->GetContext()->GSSetShaderResources(registerNum, 1, &SRV);


        if (shaderType & (int)Buffer_Shader_Type::Compute)
            CDevice::GetInst()->GetContext()->CSSetShaderResources(registerNum, 1, &SRV);
    }
    else
    {

    }
}

void CTexture::Save(FILE* pFile)
{
    // 텍스처의 이름의 길이와 이름을 저장한다.
    int length = (int)m_Name.length();
    fwrite(&length, sizeof(int), 1, pFile);
    fwrite(m_Name.c_str(), sizeof(char), length, pFile);

    // 이미지의 타입을 저장한다.
    fwrite(&m_ImageType, sizeof(Image_Type), 1, pFile);

    // 현재 텍스처 클래스가 여러가지 텍스쳐를 들고있을 수도 있으므로 모든 텍스처 정보를 저장.
    int infoCount = (int)m_vecTextureInfo.size();

    // 먼저 정보의 수를 저장한다.
    fwrite(&infoCount, sizeof(int), 1, pFile);

    // 수만큼 반복한다.
    for (int i = 0; i < infoCount; i++)
    {
        // 텍스처의 전체경로의 길이를 구해온다.
        int pathSize = (int)lstrlen(m_vecTextureInfo[i]->fullPath);

        // 전체경로의 길이와 전체경로를 저장한다.
        fwrite(&pathSize, sizeof(int), 1, pFile);
        fwrite(m_vecTextureInfo[i]->fullPath, sizeof(TCHAR), pathSize, pFile);

        // 파일이름의 길이를 구해온다.
        pathSize = (int)lstrlen(m_vecTextureInfo[i]->fileName);

        // 파일이름의 길이와 전체 경로 저장
        fwrite(&pathSize, sizeof(int), 1, pFile);
        fwrite(m_vecTextureInfo[i]->fileName, sizeof(TCHAR), pathSize, pFile);

        // 경로의 이름을 저장한다(ROOT_PATH, ANIMATION_PATH)같은 상수들
        pathSize = (int)strlen(m_vecTextureInfo[i]->pathName);

        fwrite(&pathSize, sizeof(int), 1, pFile);
        fwrite(m_vecTextureInfo[i]->pathName, sizeof(char), pathSize, pFile);
    }
}

void CTexture::Load(FILE* pFile)
{
}
