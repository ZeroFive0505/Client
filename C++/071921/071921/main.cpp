
#include <iostream>

struct Player
{
    char    Name[32];
    int     Attack;
    int     Armor;
    int     HP;
    int     HPMax;
};

int main()
{
    /*
    �����͵� �ᱹ �޸� �ּҸ� �����ϴ� �����̱� ������ const�� �̿��ؼ� �����
    ������� �� �ִ�.
    ������ ������ const�� �ٿ��ִ� ����� ũ�� 3������ ����� �ִ�.
    const int* ó�� �տ� ���̴� ��� : �����ϰ� �ִ� ������ ���� ������ �� ����.
    int* const ó�� �ڿ� ���̴� ��� : �����ϰ� �ִ� �޸� �ּҸ� ������ �� ����.
    const int* const �Ѵ� ���̴� ��� : �Ѵ� �ȵȴ�.
    */
    int Number = 100;
    int Number1 = 200;
    int* pNumber = &Number;
    pNumber = &Number1;

    const int* cpNumber = &Number;
    cpNumber = &Number1;

    //*cpNumber = 300;
    std::cout << *cpNumber << std::endl;

    int* const pcNumber = &Number;
    //pcNumber = &Number1;
    *pcNumber = 3030;

    std::cout << Number << std::endl;

    const int* const cpcNumber = &Number;
    //cpcNumber = &Number1;
    //*cpcNumber = 999;

    /*
    ���������� : �����͵� �����̱� ������ �޸𸮿� ������ �Ҵ�ȴ�.
    32bit�϶��� 4����Ʈ 64bit�϶��� 8����Ʈ�� �Ҵ�ȴ�.
    ������ ������ �ּҸ� �����ϴ� �����͵� ���� �� �ִ�.
    �Ϲ� ������ �ּҴ� ������ ������ �����ϰ� ������ ������ �ּҴ� �������� ������ ������
    �����Ѵ�.
    int Number = 100;
    int* pNumber = &Number;
    int** ppNumber = &pNumber;
    int*** pppNumber = &ppNumber;
    */
    pNumber = &Number;

    int** ppNumber = &pNumber;
    int*** pppNumber = &ppNumber;

    // ������ ������ �ٸ� ������ �ּҸ� ����� ���ÿ� �������ٰ��� �ƴ϶��
    // �ݵ�� nullptr�� �ʱ�ȭ�� ���ִ°��� ����.
    int* pNumber1 = nullptr;
    int* pNumberArray[2] = {};
    pNumberArray[0] = &Number;
    pNumberArray[1] = &Number1;

    std::cout << "Number Address : " << &Number << std::endl;
    std::cout << "pNumber Value : " << pNumber << std::endl;
    std::cout << "pNumber Address : " << &pNumber << std::endl;
    std::cout << "ppNumber Value : " << ppNumber << std::endl;
    std::cout << "ppNumber Address : " << &ppNumber << std::endl;
    std::cout << "pppNumber Value : " << pppNumber << std::endl;
    std::cout << "pppNumber Address : " << &pppNumber << std::endl;

    std::cout << "*pNumber : " << *pNumber << std::endl;
    std::cout << "*ppNumber : " << *ppNumber << std::endl;
    std::cout << "**ppNumber : " << **ppNumber << std::endl;
    std::cout << "*pppNumber : " << *pppNumber << std::endl;
    std::cout << "**pppNumber : " << **pppNumber << std::endl;
    std::cout << "***pppNumber : " << ***pppNumber << std::endl;

    Player  player;

    Player* pPlayer = &player;

    // ���������� . �� ������ �켱������ ����.
    // �׷��Ƿ� �׳� *pPlayer.Attack �̷��� �ع��� ��� ������ �ȴ�.
    // Attack������ ���� �������� �ǹ����� ������ ������ ���� ����ü ũ�⸸ŭ �޸𸮰�
    // ��������� ���Ǵ� ������ �ƴϱ� ������ �����ϴ� player �� Attack ������ �����ϴ�
    // ���� �ƴϰ� �ȴ�.
    // ������ �켱������ ���� (*pPlayer) �� �ؼ� .�� �̿��� �����ؾ� �Ѵ�.
    (*pPlayer).Attack = 100;

    // (*pPlayer). �� pPlayer-> �� ����Ҽ��� �ִ�.
    // -> �� �ϸ� pPlayer �� ������ �ִ� �޸� �ּ��� Attack�� �ǹ��Ѵ�.
    // player.Attack �� �ǹ��Ѵ�.
    pPlayer->Attack = 300;

    std::cout << player.Attack << std::endl;

    Player  playerArray[10] = {};
    Player* pPlayerArray = playerArray;

    // �Ʒ� ������ ����.
    pPlayerArray[0].Armor = 3939;
    (*(pPlayerArray + 0)).Armor = 49049;

    /*
    void : Ÿ���� ���°��� �ǹ��Ѵ�.
    void* Ÿ���� � Ÿ���� �޸� �ּҵ� ��� ������ �� �ִ�.
    void*�� �������� �Ұ����ϴ�.
    void*�� �ܼ��ϰ� �޸� �ּҸ� �����صδ� �뵵�θ� ����� �ȴ�.
    ���� �������� �ϱ� ���ؼ��� ��Ȯ�ϰ� �־��� Ÿ������ ����ȯ�� ���ؼ�
    �������� �ؾ� �Ѵ�.
    */
    float  NumberF = 3.14f;
    float* pNumberF = &NumberF;
    pNumber = (int*)pNumberF;
    float* pNumberF1 = (float*)pNumber;
    //pNumberF1 = pNumber;
    void* pTest = &Number;
    pTest = &NumberF;
    pTest = &player;

    ((Player*)pTest)->Attack = 300;

    std::cout << *pNumber << std::endl;
    std::cout << *pNumberF1 << std::endl;

    const char* pText = "Test String";

    std::cout << pText << std::endl;
    std::cout << (int*)pText << std::endl;

    return 0;
}
