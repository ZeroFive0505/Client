
#include <iostream>
#include "HashTable.h"

/*
HashTable : 실제 저장되는 데이터보다 훨씬 큰 메모리 공간을 만들어놓고 데이터를 Hash 함수에 의해서
인덱스를 구한다음 저장하는 방식이다. 실제 데이터가 저장될때 연속으로 저장되는 것이 아니라
배열에 빈공간들을 두고 저장이 되는 방식이다. 즉, 메모리가 낭비되는 것은 막을 수 없지만 메모리를
희생해서 탐색속도를 그대화 시키는 알고리즘이다.
*/

int main()
{
	CHashTable<const char*, const char*>	Table;

	Table.insert("야스오", "과학");
	Table.insert("요네", "수학");
	Table.insert("티모", "나쁜놈");
	Table.insert("트린다미어", "이기주의자");
	Table.insert("베인", "지만아는놈");
	Table.insert("ABC", "ABC");
	Table.insert("ACB", "ACB");

	std::cout << Table["야스오"] << std::endl;
	std::cout << Table["트린다미어"] << std::endl;

	Table["Test"] = "Test다.";
	Table["트린다미어"] = "빽도어 전문가";
	std::cout << Table["Test"] << std::endl;
	std::cout << Table["트린다미어"] << std::endl;
	std::cout << Table["티모"] << std::endl;


	CHashTable<const char*, const char*>::iterator	iter = Table.Find("티모");

	if (Table.IsValid(iter))
		std::cout << "티모는 정상적인 키값입니다." << std::endl;

	iter = Table.Find("아크샨");

	if (Table.IsValid(iter))
		std::cout << "아크샨은 정상적인 키값입니다." << std::endl;

	else
		std::cout << "아크샨은 잘못된 키값입니다." << std::endl;

	return 0;
}
