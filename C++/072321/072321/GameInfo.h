// #pragma once는 이 헤더파일을 1번만 포함시키도록 해주는 기능이다.
#pragma once

#include <iostream>
#include <time.h>

// #define은 앞에 #이 있기때문에 전처리기로 처리된다.
// 전처리기란 컴파일 이전에 먼저 처리하는 것을 말한다.
// 그래서 전처리기가 모두 처리되고 그 후에 완성된 코드로 컴파일이 일어나게 되는 것이다.
// 예를 들어 코드상에 COUNT를 하면 컴파일 전에 COUNT 뒤에 있는 코드를 그 위치에 대체한다.
#define COUNT 10


// #define은 한줄로 작성되어야 한다.
#define PRINTDEFINE std::cout <<"PrintDefine\n"