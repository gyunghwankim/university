






void main(void)
{



}









/*
	1. malloc <stdlib.h> <malloc.h>

		void *malloc(size_t size);

			size  :  할당할 바이트 수.

		size 만큼 할당하고, offset 0의 주소값을 반환한다.

		malloc은 할당된 공간에 대한 void 포인터를 반환하거나,
		malloc, calloc, realloc 모두 사용 가능한 메모리가 부족한 경우 NULL을 반환한다.



	2. calloc <stdlib.h> <malloc.h>

		void *calloc(size_t count, size_t size);

			count  :  원소의 개수
			size   :  각 원소의 크기

		size 크기를 count 수만큼 할당하고, 0으로 초기화한다. (size*count/bytes allocated)
		offset 0의 주소값을 반환한다.


	3. realloc <stdlib.h> <malloc.h>

		void *realloc(void *block, size_t size);

			block  :  이전에 할당된 메모리 블록
			size : 다시 할당할 바이트 수

		이전 block의 초기화된 데이터를 복사하고 size만큼 재할당한다. 이외의 데이터는 0으로 초기화되지 않는다.
		이전 block은 해제되며, 데이터는 유지된다.
		재할당된 block에 대한 void 포인터를 반환한다.



	3. free <stdlib.h> <malloc.h>

		void free(void *block);

			block  :  이전에 할당된 메모리 블록.

		block을 해제한다.

		free를 통한 메모리 해제 시, 사용하지 않을 포인터 변수들은 NULL을 가리켜주면 좋다. (프로그램 종료 시, 에러 발생 가능성)
*/














/*
	질문은 24시간 받는다. 왜냐하면 잘 땐 폰이 무음이기 때문.



	1.
	< Spatial Filtering > (공간 필터링) 부분을 읽는다.
	->
	// < Histogram Modification > (히스토그램 변형) 이전까지.
	// 전혀 모르겠는 건 무시한다. 예를 들어 -> ""spatial filtering"은, "Local Histogram Processing"과 내부 연산이 다른 것 빼고 유사하다."
	// convolution, correlation 설명도 제외한다.
	!! 가중치의 합이 0이면, 원본값이 남지 않고 변화값만 남는다. 가중치의 합이 1이면, 원본이 남는다. 가중치의 합이 2라는 것은, 예를 들어 가중치 합 1일 때 150의 값과 변화값이 출력되어 대충 130 ~ 170이 나온다고 하면, 가중치의 합을 2라고 하는 것은 원본 150값 * 2 -> 300값 언저리를 출력하겠다는 말이기에 말이 안 된다. 밝기 두 배와 비슷하다.
	// 선형 필터는 (원본 크기, 마스크 크기, 패딩 방식 등은 필수 인자이니 제외하고) 커널만 다르므로, 모든 선형 필터를 하나의 함수로 구현할 수 있다.
	// normalizing(정규화) : 처리 이후 값의 범위가 예를 들어 [5, 1234]라고 한다면, 이를 [0, 255]로 mapping하는 것.
	// embossing을 이해했다면, 연산 과정은 각자 원하는 방식으로 구현해도 좋다.



	2.
	선형 공간 필터링을 구현한 코드를 첨부하니, 코드를 확실하게 이해한다.



	3.
	과제 1:
	grascale 헤더에 설명되어 있는 선형 필터링을 전부 실습하여 확인한다.

	과제 2:
	첨부 코드를 다음과 같이 변경한다.
	->
	"마스크가 int 타입인데, 가우시안 쓰려면 실수로 줘야 한단 말야?
	아니면 어차피 가중치의 합으로 나누니까, 최소공배수?를 곱해서 정수로 넘겨줄 수도 있겠네.
	여튼! 모든 데이터 타입의 마스크를 받을 수 있는 선형 필터링 함수를 만들고 싶어."
	->
	핵심 기술
	1.
	마스크 타입은 enum으로.
	1.
	타입캐스팅 명시 습관.
	1.
	포인터 타입캐스팅.
*/














/*
교수님 뵈러 오신 선배 현업자분들에게 질문
->
	1.
	최적화 작업을 할 때, 어셈블리까지 거치는지??
	관련 직종 사람들 대부분이 어셈블리어까지 알아야 하는지??
	::	몰라도 된다.
		차피 입사하면 다시 배운다.


	2.
	요새 go, rust, 그리고 파이썬의 스피드 패치까지. 임베디드는 로우레벨언어고, 새로운 좋은 언어들이 탄생하고 있다. C만으로 괜찮은지?
	::  죽을 때 까지는 걱정하지 말자. 그렇다고 라이브러리,프레임워크까지 제외하지는 말고.
        메이플, 카트, 크아가 아직까지 살아있는 이유라고 생각하면 된다.
		세계에선 이미 C언어로 구현해왔고, 잘 최적화된 라이브러리 또한 아주 많이 있으니, 관습인 셈이다.
*/
















/*
전처리기	  :	main()함수 이전에 전역에, 컴파일하기에 앞서서 소스 파일을 처리하는 컴파일러의 한 부분이다.

#define		매크로 정의
#include	파일 포함
#undef		매크로 정의 해제
#if . #else . #endif	조건 매크로.
등등


<stdio.h>의 읽기 . 쓰기 함수들에서도
매크로로 이루어진 멤버도 있고, 함수로 이루어진 멤버도 있다.
매크로는 함수보다 더 빠르지만,
매크로로 정의된 멤버함수의 주의점으로, 수식을 사용할 경우 문제가 발생할 수 있다.
만약 매크로 정의 함수에서 수식을 사용할 경우, 반드시 괄호로 구분을 해주어야 한다.

매크로는 파일에서만 적용된다.
link-time에서 매크로는 linked되지 않는다.

또한, [C]에서 매크로와 함수가 같은 이름의 같은 형태일 경우,
함수가 무시되고, 매크로가 실행된다.

매크로는, 컴파일이 안된다. == 기계어로 번역되지 않는다.
매크로를 사용할 때마다 매번 해당 매크로를 번역해야 한다.
?? 이거 obj랑, linked안되는거랑 관련지어서 확인해보자.
*/









/* 정리되지 않은 부분은 @@@@@@@@@@@ 로 표시.



탭은 4칸이고, 탭의 오른쪽에 글이 있으면, 모든 탭이 띄어쓰기로 바뀐다.
글이 있어도, 글을 써도, 탭은 탭으로 남도록 바꾸자.@@@@@@@@@@@@@@@@@@@@@@@@@



*  1. 목적에 맞게 최대한 세부적으로 알고리즘을 만들고, 코드를 작성한다.
*  2. 시뮬레이션 또는 디버깅을 통해 코드의 전체 또는 부분을 수정한다.
*  3. 전체적으로 코드를 수정한다면, 알고리즘 또한 수정해야 하며, 1번으로 돌아가서 반복한다.
*  4. 세부적으로 코드를 수정한다면, 알고리즘과 코드를 검토하며, 문제 발생 시, 1번으로 돌아가서 반복한다.
*  5. 검토가 완료되었다면, 최적화와 가독성에 중점을 두어 코드를 수정하고, 주석 또한 정리하여 마무리한다.

* 메모리에 저장된 동일한 데이터를 읽는다 해도, 관점에 따라 메모리를 읽는 방법이 다르다. 원리를 이해하는 것에 중점을 둘 것.



<주석>

	컴파일러가 코드로 인식하지 않는다.



* 모듈화(mudularity), 모듈(module)

	ㅡ 모듈
	- 프로그램을 구성하는 구성 요소의 일부
	- 관련된 데이터와 함수들이 묶여서 모듈을 형성
	- 파일 단위로 나뉘는 것이 일반적이다.

	ㅡ 모듈화
	각각의 기능들을 파일별로 나누어 놓는 것.




<자료형> (dataTypes)

	자료형(資料型) 또는 데이터 타입(data type)은
	컴퓨터 과학과 프로그래밍 언어에서 실수, 정수, 불린 자료형 따위의 여러 종류의 데이터를 식별하는 분류로서,
	더 나아가 해당 자료형에 대한 가능한 값, 해당 자료형에서 수행을 마칠 수 있는 명령들,
	데이터의 의미, 해당 자료형의 값을 저장하는 방식을 결정한다.

	[C]에서 dataType을 정의하지 않을 시, int로 정의된다. // 이젠 안 그럼.



<정수 자료형> (integer types)

	dataType(bytes)
	signed   or   unsigned		+		bool(1) char(1) short(2) int(4) long(4) long long(8)
	부호 O		   부호 X

	// 초창기에 4byte는 long이고, int는 가변적으로 설계된 dataType인데,
	// 오랜 시간 사용되다가 4byte로 굳어진 케이스이다.


	ㅡ 메모리 활용

	MSB(Most Significant Bit) : 가장 오른쪽에 있는 비트. signed dataType에서 부호비트이다.
	LSB(Least Significant Bit) : 가장 왼쪽에 있는 비트.
	비트 번호는 LSB가 0번으로 시작한다. e.g. int의 MSB 비트번호는 31번.

	MSB						ㅡ 부호		0 : 양수		1 : 음수
	LSB를 포함한 나머지 비트	ㅡ 크기



	ㅡ 양음수 표현 방식

	1의 보수 : 모든 0과 1을 서로 바꾼 값.
	2의 보수 : 1의 보수에 1을 더한 값.
	어떠한 양의 정수 +N의 2의 보수가, -N이다.

	subtraction(뺄셈) 원리
	a - b == a + (-b)

	부호 없는 unsigned dataType은, MSB가 부호가 아닌 크기 표현에 관여한다.



<실수 자료형> (floating-point types, real number)

	dataType(bytes)
	float(4) double(8) long double(8)

	실수의 경우 기본적으로 dataType이 double이다.		e.g. 10.3 == (double)10.3		10.3f == (float)10.3

	MSB가 부호비트로 사용되며, 모두 unsigned 불가하고 생략된 signed이다.

	실수 표현방식은 정밀도에 의존한다.

	ㅡ 부동소수점 표현 방식을 따른다. 검색해볼 것.

	정수표현 방식과 실수표현 방식은 다르다.

***	두 표현방식의 피연산자가 연산될 경우 명시적으로 변환하자.

***	부동소수 표현은 같음/다름을 테스트해서는 안된다. (비교연산자 == 와 != 를 사용해서는 안된다.) ( 순환소수 저장 문제가 있기 때문.)
	// Compiler에 따라 가능할 수 있다.



<연산자> (operators) ㅡ 아주아주 다양하다.

* 피연산자가 무엇이냐에 따라 연산의 개념이 다르다.

	연산자 우선순위가 있다.
	C:\Users\bibib\OneDrive\바탕 화면\C언어\사진, 캡처\연산자 우선순위1.png

*** [C++]
		우선 순위나 결합 순서도 변경할 수 없다.
		+ 연산자의 기능을 바꾸어 곱셈이나 누승을 하도록 오버로딩했다 하더라도,
		이 연산자의 우선 순위는 원래의 + 연산자의 것과 동일하다.
		컴파일러는 연산자가 논리적으로 어떤 연산을 하는지 다른 연산과의 관계가 어떠한지까지는 판단할 수 없다.
		참고 : http://soen.kr/lecture/ccpp/cpp3/28-2-3.htm

		전위 . 후위 연산자 오버로딩의 문법만 알면 되는 듯하다.



<비교 연산자> (comparison operators, 관계 연산자 - relational operators)

***	return true/false

	A relational operator checks the relationship between two operands.
	If the relation is true, then it will return value 1. Otherwise, it returns value 0.

	== (equal to)
	!= (not equal to)
	< (less than)
	<= (less than or equal to)
	> (greater than)
	>= (greater than equal to)



<대입 연산자> (assignment operators)

***	return Lvalue

	= (assignment operator)
	오른쪽 피연산자의 값을 왼쪽 피연산자의 값에 저장한다.

	ㅡ 복합대입 연산자 (compound assignment perators)
	+=  -=  *=  /=  %=  <=  >=  &=  |=  ^=  etc.		e.g.  a = a + b		==		a += b



<산술 연산자> (arithmetic operators)

	+ (addition)
	- (subtraction)
	* (multiplication)
	/ (division)
	% (Modulus operator to get remainder in integer division)
	++ (increment)
	-- (decrement)

	나머지(modulus)는 있으나, 몫과 거듭제곱은 없다. (**  //)

	ㅡ 모듈러스의 좌항은 음수일 수 있으며, 우항은 입력의 절댓값을 받는다.
	

	증감 연산자의 전위연산과 후위연산 ( == 전치, 후치)

	++a : 전위연산 (pre-operation) : 하나의 statement에서 연산자 우선순위가 처음이다.
	a++ : 후위연산 (post-operation) : 하나의 statement에서 연산자 우선순위가 마지막이다.


* 후위증감연산할 경우가 드물며, 의도치 않는 한 전위로 쓸 것.

		[C++]	전위 . 후위 증감연산자의 오버로딩 과정에서
				내부에서 객체생성에 대한 복사비용과, 이를 반환할 때의 복사비용이 든다는 이유도 있다.



<논리 연산자> (logical operators)

	&& (logical AND operator) : 모두 true일 때만 true
	|| (logical OR)			  : 하나라도 true일 때 true
	!  (logical NOT)		  : true와 false를 뒤바꾼다.

	true : 0이 아닌 값. C언어에서 true의 출력값은 1이며, 그렇기에 우리도 1을 주로 쓴다.
	false : 0

	dataType ㅡ bool : true와 false만을 값으로 가지는 자료형이다. 출력 시 0과 1만 출력된다.
	C++에는 있으며, C에는 없다.
	타입 재정의 하거나, stdbool.h에 정의되어 있으므로, 가져다 쓸 것.



<비트 연산자> (bitwise operators)

	Bitwise operators perform operations on integer data at the individual bit-level.
	These are only available in integer. not in real number.
	같은 비트번호끼리 연산한다.

	& (bitwise AND operator)				ㅡ 모두 1이면 1
	| (bitwise OR operator)					ㅡ 하나라도 1이면 1
	^ (bitwise XOR operator)				ㅡ 같으면 0, 다르면 1
	~ (bitwise complement operator)			ㅡ 0과 1을 뒤바꾼다. 1의 보수와 같다.3
	<< (bitwise left shift operator)		ㅡ 비트를 왼쪽으로 옮긴다.
	>> (bitwise right shift operator)		ㅡ 비트를 오른쪽으로 옮긴다.

	<< ㅡ 값의 크기에 따라 표기가 되며, 왼쪽으로 64칸 옮기면 되돌아온다. ( Warning C26452 산술 오버플로 )			// 20231125: compiler에 따라 다르다. 사용하지 말자.
	>> ㅡ 2^-1부터 비트정보가 소실된다.


<비트 연산 활용>

	ㅡ 플래그 (flags)
	깃발에서 유래한 용어로, 올리면 on, 내리면 off를 말함으로써
	정수의 비트에 활용한다.
	적은 공간에 정보를 저장해야 하고, 빠른 속도가 필요할 때 사용한다.
	대표적으로 내부 저장 공간이 매우 작은 CPU에서 각종 상태를 비트로 저장한다.
	예시코드를 검색해볼 것.



	ㅡ 상태 추가
	iStatus |= status1;

	ㅡ 상태 확인
	if (iStatus & status1) {}

*	ㅡ 특정 자리 비트 제거
	iStatus &= ~status1;


<조건 연산자(삼항 연산자)> (conditional operator)

	?:
*  조건 연산자는, 조건에 따라 실행문(혹은 값)을 반환한다는 것에 중점을 둘 것.

	int v1;
	if (condition)
	{														Syntax
		v1 = value1;
	}							==			v1 = (condition) ? (value1) : (value2)
	else
	{
		v2 = value2;
	}


	(condition) ? (statement1), (statement2), ... : (statement11), (statement12), ...;


	// int a = 10;			void TEST1() {}          int TEST2() { return 1; }
	a = (1 < 2) ? TEST2(), a = 20, TEST2() : TEST2(), TEST2(), TEST1();				// compile ERROR: 아래에서 설명.

	참일 경우, 실행 순서는 다음과 같다.			// 대입 연산은, Lvalue를 반환한다.
	a = TEST2(),
		a = a = 20,
		a = TEST2();

	위의 실행 순서에 따라 조건 연산자가 다른 연산자와 함께 할 때, 참/거짓 모든 실행문에 반환이 있어야 한다.

	위의 식에서 조건 연산자를 다른 연산자인 대입 연산자와 함께 쓰는데,
	거짓 영역 실행문 중 하나인 TEST1() 함수가 반환하지 않으므로 컴파일 오류이다. (a = TEST1();  -> ERROR)



<조건문> (conditional statements)

	conditional operator  >  if-else statement  >  switch-case statement
	가독성의 차이가 있으므로, 참조하여 쓸 것.


	ㅡ if-else statement

	if (condition1)
	{
		statements;
	}
	else if (condition2)
	{
		statements;
	}
	else if (condition3)
	{
		statements;
	}
	...
	else
	{
		statements;
	}



	ㅡ switch-case statement

	switch (expression)
	{
	case int-constant1:
		statements;
		break;
	case int-constant2:
		statements;
		break;
	...
	default:
		statements;
		break;
	}

	break; : switch문을 중단한다. 쓰지 않는 것이 문법 오류가 아니므로, 의도적인 경우도 있을 것이다.



	ㅡ break;  :  Jumps out of the loop or switch : 루프/스위치에서만 사용할 수 있으며, 해당 scope를 벗어난다.

	ㅡ continue;  :  Jumps to the next cycle of the loop directly : 루프에서만 사용할 수 있으며, 해당 회차를 종료한다.

	ㅡ goto : Jumps from the 'goto' to a labeled statement in the same function. : 실행라인을 다음 label로 이동한다. 절차를 무시하는 치트키이며, 절대로 쓰면 안 된다.

	func1()
	{
 //	labelName:;					// use either cases.
		...

		goto labelName;

		...
 //	labelName:;
	}

	*** [C++]-class-friend 키워드가 객체/캡슐 파괴의 주범이듯이, goto도 [C]의 절차 파괴의 주범이다.



<반복문> (looping statements)

	ㅡ for loop  :  정해진 횟수만큼 반복한다.
	ㅡ while loop  :  조건식이 true일 경우에 계속해서 반복한다.

	for (initialization; condition; increment/decrement)		e.g.  for (int i = 1; i<=10; ++i){}
	{
		statements;
	}

* for문의 인자를 직접 수정하는 것은 위험한 코딩이라고 한다.



	while (condition)
	{
		statements;
	 //	variable increment or decrement;
	}



	ㅡ do-while loop

	do
	{
		statements;
	}
	while (condition);



	ㅡ nested for loop

	for (initialization; condition; increment/decrement)
	{
	~~
		for (initialization; condition; increment/decrement)
		{
		~~
		}
	~~
	}



<재귀함수> (recursive, reentrant function) (functionName_Re)@@@@@@@@@@@@@@@@@@@@@@@

	장점 : 가독성, 구현의 용이, 계층 구조 표현에 효율적이다.(tree)

	단점 : 함수 호출비용, 해제비용이 들어가며, 성능이 떨어진다.
		이중재귀, 다중재귀는 연산에 엄청난 시간이 들어간다. 꼬리재귀 등으로 해결 가능하다.



<배열> (arrays)

***	for . while 반복문을 통한 배열 초기화 시, buffer overflow warning이 뜨지 않을 수 있다.

***	몇 차원 배열이든 범위를 벗어난 인덱스를 사용하면, buffer overflow이다.
	다만 2차원 배열을 1차원 배열처럼 다루듯이
	배열 자체의 할당범위 내에 있다면
	buffer overflow 경고는 뜨되, 의도한 대로 정상 동작한다.



	ㅡ 행렬을 표현할 수 있다.

	ㅡ 1차원 배열
	dataType arrayName[count] = { } ;

	[count] = [0], [1], [2], ..., [count - 1]


	ㅡ 2차원 배열
	dataType arrayName[count1][count2] = { {...}, {...}, ... };

	[count1][count2] = [0][0], [0][1], ..., [0][count2 - 1], [1][0], [1][1], ..., [count1 - 1][count2 - 1]


	ㅡ 다차원 역시 동일한 형태이다.


	ㅡ 2차원 배열의 예시
		휴대폰 카메라의 HDR : High Dynamic Range
		여러장의 사진을 동시에 촬영한 뒤 합성해서 명암비가 좋은 결과물을 만드는 방법이다.
		흑백이라면, 제일 밝은게 255, 제일 어두운게 0으로, 픽셀 당 8bits가 필요하다.
		이외의 모든 영상처리.



*  배열원소들의 주소는 연속적이다.
*  주소값 : arrayName = &arrayName = &arrayName[0]
*  main이 아닌 함수에 인자로 *arrayName 또는 arrayName[0]을 받아올 경우,
*  배열원소의 개수를 sizeof(arrayName)으로는 할 수 없으므로, 주의할 것.



<구조체> (structures)

	사용자 정의 자료형.(user-defined dataType)

	1. Declare new structure on global.
	2. Declare struct newName on function.
	3. Initialize and use.

	1.	struct _tagName						2. struct _tagName tMy;
		{
			type member1;					3. tMy.member1 = ~~;
			type member2;					   tMy.member2 = ~~;
			...								   ...
		};


	(use typedef for nickname)
	1.	typedef struct _tagName				2. Name tMy;		or		 2. struct _tagName tMy;
		{
			type member1;					3. tMy.member1 = ~~;
			type member2;					   tMy.member2 = ~~;
			...								   ...
		}Name;



	ㅡ typedef를 사용하여 새 구조체타입 선언 시의 장점.

	기존의 자료형들과 같은 방법으로 해당 구조체형의 객체를 생성할 때 매번 struct를 붙여야 하므로, (dataType*) == (struct ST1*)
	typedef를 사용하여, 해당 구조체형을 별칭으로 대신한다.
	구조체형의 쓰임은 기존의 자료형들과 같은 방식이며, 아래의 경우가 있다.

	1. 해당 구조체형의 구조체변수 선언
	2. 해당 구조체형의 포인터변수 선언
	...

	구조체임을 표시하는 목적의 가독성을 고려하여 사용할 것.



<분할 구현>

	함수 원형 (function prototype)  :  컴파일러에게 미리 함수에 대한 정보를 알리는 것.
										선언의 개념이다.

	함수 사용 시점 위에 선언이 완료 되어야만 사용할 수 있다.
	함수 원형 선언만 하고 정의를 하지 않는다면, 링크단계에서 에러가 발생한다.

	no prototype

	void Test1()					함수 정의가 함수 호출보다 앞서면,
	{								함수 원형을 정의하지 않아도 된다.
		statements;
	}
	main()
	{
		Test1()
	}



	use function prototype			그러나, 일반적인 방법은 아니다.

	void Test1();					일반적으로 헤더파일에 함수 원형이 선언되어 있고,
	main()									  소스파일에 함수가 정의되어 있다.
	{
		Test1()
	}
	void Test1()
	{
		statements;
	}



	분할 구현을
		1. main 함수가 포함된 코드실행파일.
		2. 함수 원형을 적은 헤더파일.
		3. 함수 내용을 적은 소스파일.

	이렇게 코드를 세 개로 분할한다.
	코드를 분할해 더욱 효과적으로 분류하여 편하게 관리하고자 함에서
	분할 구현을 한다.



<분할 구현의 문제점>

	ㅡ 분할 컴파일은 코드관리 부분에 대해서 어마어마한 장점이 있지만, 전역변수를 활용하기 힘들어진다.

	main.c				#include "func.h"
						main(){}

	func.h				int Add(int, int);

	func.c				#include "func.h"
						int Add(int a, int b) { return a + b; }

	이렇게 main.c에서 func.h를 #include하여 사용한다.

	#include의 역할은 그저 복사 붙여넣기다.
	헤더파일의 코드들을 그대로 가져오는 것 이외의 역할은 하지 않는다.



	ㅡ 위의 상황에서 헤더파일에 선언 뿐만 아니라 정의까지 한 경우.

		각 소스파일을 컴파일하는 과정에서 .obj파일은 정상적으로 생성된다.
		그러나, 링크 과정에서 소스파일들이 합쳐지는데,
		같은 헤더파일을 가져온 소스파일들이 링크가 된다. == 중복정의 error

		전역변수 또한 마찬가지다.



<변수의 종류>

	모든 함수와 변수는 선언된 지역 내에서만 사용할 수 있다.                         (외부변수 제외)
	모든 변수의 scope는 정의된 지역이다.                                        (외부변수 제외)
	정의할 때 dataType에 따른 메모리를 할당하며, 바꾸지 못한다.
	그렇기에 같은 local 내에서 여러번 정의할 수 없다.  :  중복정의, 재정의 오류

	1. 지역변수 (local variable, internal variable)
	2. 전역변수 (global variable)
	3. 정적변수 (static variable)
	4. 외부변수 (extern variable, external variable)



	ㅡ 지역 (local)
		지역변수 : 해당 함수
		전역변수 : 해당 소스파일
		정적변수 : 정의된 local
		외부변수 : 프로젝트(모든 소스파일)



	ㅡ 전역 (global) : 코드를 실행하는 부분.



	ㅡ 정적 변수 (static variable)

	static  :  데이터 메모리에 할당되며, 주소가 고정되어 사라지지 않는다.
	함수 또는 변수로 사용 가능하다.
	매개변수로는 사용할 수 없다.

	1.  주소가 고정되어 있으며,
		선언된 소스파일 내에서만 정의하고 사용 가능하다.

		분할구현에서 정적변수를 헤더파일에 선언하여 각 소스파일에서 가져와 사용할 경우,
		각 소스파일에 동일한 이름의 정적변수가 정의되어 있지만,
		주소가 달라서 각기 다른 소스파일전용 변수로 구분된다.
		모든 static이 마찬가지다.

	2.  함수 내에 선언하여 사용할 경우,
		스택 메모리에 할당된 함수가 종료되어도,
		static 대상자의 주소와 값이 유지된다.



	ㅡ 외부변수 (extern)

	파일, 함수, 어디에 선언하든
	외부변수의 local은 프로젝트이다.
	(거의 모든 것들의 최대 local은 전역이며, extern만을 예외로 알고 있자.)

	일반적으로 각 소스파일의 전역에 선언한다.
	외부변수의 특성상 지역에 선언할 이유가 있을지 모르겠다.
	이미 extern 선언된 변수를 다른 소스파일의 전역이 아닌 지역에 처음으로 선언할 경우, extern 태그명으로 선언 후 사용 가능하다.



		분할구현에서 정적변수와 외부변수

			정적변수 : 파일 또는 local별로 같은 이름이지만, 각기 다른 주소를 가지므로, 각기 다른 값을 가진다.
			외부변수 : 프로젝트 내에서 같은 이름의 같은 주소를 가진다.



<포인터> (pointers)

*	구조체, 문자열과 같은 거대한 타입 또는 자주 사용하는 함수일 경우,
	주소참조만 하는 것이 빠르다. (주소 == 8bytes)

	ㅡ 포인터 변수

	8bytes의 주소값을 저장하며, 크기가 8bytes인 포인터 변수. (x86 == 32bit 운영체제는 주소값의 크기가 4bytes)
	포인터 변수 그들도 변수이므로, 주소를 가지고 있다.

	NULL pointer는 메모리 주소 0을 가리키는 pointer라는 뜻이다. 0번지는 일반적으로 접근할 수 없는 메모리 영역이다.
	따라서 pointer가 아무것도 가리키지 않을 때 NULL pointer로 초기화 한다.

	dataType of NULL  :  pointer (void *)0

	모든 포인터 변수는, 일반변수의 dataType과 관계없이 8 Bytes를 할당하여 주소값을 저장한다. == 주소의 값은 8bytes로 표현 가능하다.
	포인터 변수의 (dataType*)은, 역참조 시에 데이터를 읽는 방식이다.

	주소값의 단위는 1Byte.
	e.g.  0x0000004b48f7f404  <32bytes>  0x0000004b48f7f424



	ㅡ 주소값 연산

	8Bytes(0x0000,0000,0000,0000)로 표현 가능한 주소값 1이 저장하는 메모리는 1Byte.

	1. 대입 연산자를 제외한 표현식에서의 피연산자 중 포인터변수 또는 &var은 하나 이하여야 한다.

	2. 표현식에서의 연산 규칙.

	   a - (void*)형 포인터변수만 있을 경우  :  (ERROR) E0852 + C2036  :  (포인터)타입캐스팅할 것.

	   아래 b.c.d의 포인터변수는 (dataType*)이 정의되었다.

	   b - &일반변수와 포인터변수가 모두 없을 경우  :  (+ 1) == + 1Byte.

	   c - &일반변수만 있을 경우  :  (+ 1) == + 주소참조되는 일반변수의 dataType의 크기.

	   d - 포인터변수가 있을 경우  :  (+ 1) == + 포인터변수의 선언.정의에서의 (dataType*)의 크기.
		   이는 처음부터 정해지며, &로 참조되는 변수의 dataType과 뒤이은 역참조에서의 타입캐스팅보다 우선시된다.


		e.g.  (dataType*) p = &a;				p + 1 ==  char, bool - 1Byte  ==  0x000000e3fc6ff7d8 + 1
			  p = 0x000000e3fc6ff7d8					  short - 2Bytes  ==  0x000000e3fc6ff7d8 + 2
														  int, long, float - 4Bytes  ==  0x000000e3fc6ff7d8 + 4
														  long long, double - 8Bytes  ==  0x000000e3fc6ff7d8 + 8



* e.g.
	일반변수, 포인터변수 선언.정의
		int a;
		char* p;


	참조 : 주소를 읽는다.

			p = &a				ㅡ (포인터변수 p)		ㅡ p에 a의 주소를 저장한다.
			&a					ㅡ (앰퍼샌드 a)		ㅡ a는 자기 주소에 살으니,


	역참조 : 해당 주소에 접근하고, 값을 읽는다.

			*p				ㅡ 포인터변수 p의 dataTYpe - char로 읽는다.		ㅡ -1(char)
			*&a (== a)		ㅡ 일반변수 a의 dataType - int로 읽는다.			ㅡ 255(int)


	값 대입 : 대입되는 데이터는, 참조자의 dataType에 따라 다르다.
			
			// p = &a;
			a = 256;						256
			*p = 1;							257
			*p = 0;				a의 값   :	256
			*p = -1;						511
			*p = -128;						384


		&a에 들어가 있는 값을 읽거나 대입할 때,
		p는 char로 읽고, char로 대입하며,
		a는 int로 읽고, int로 대입한다.

		a의 값은, 참조자의 dataType에 따라 다르다.



*  (dataType*)address  :  주소 역참조 시의 dataType이다.

	주소 역참조 시의 (dataType*)은,
	2진법의 데이터를 읽는 방법 그 자체이기 때문에,
	대입 연산자 또는 (dataType)value를 통한 형변환과는 다른 의미이다.

	이미 다른 dataType으로 역참조를 했다면,
	의도치 않은 이상 값은 이미 변질되었다.

	우선, 같은 dataType으로 읽은 뒤,
	값을 저장해주는 또 다른 일반변수를 통하여 형변환을 할 것.



<포인터의 동작 방식>

	ㅡ 포인터와 배열

	offset과 index는, 시작점에서의 상대적인 위치를 말한다.

	기준  -	 offset  :  주소값 연산에서 (+ 1)의 Byte단위
			 index   :  배열의 dataType

		e.g
			int iArr[3] = { 0 };

			*iArr = 10;				==		iArr[0] = 10;
			*(iArr + 1) = 20;		==		iArr[1] = 20;
			*(iArr + 2) = 30;		==		iArr[2] = 30;



*  arrayName[N]  ==  *(arrayName + N)								// 대괄호의 정의, 문법이다.

*  (dataType)arrayName[N]  ==  *((dataType*)(arrayName + N))

*  p가 포인터변수일 때,
*  *(p + N)  ==  p[N]				*((dataType*)(p + N))  ==  (dataType)p[N]
*  p[N]  ==  *(p + N)				(dataType)p[N]  ==  *((dataType*)(p + N))



	ㅡ 포인터와 구조체

	- 구조체의 주소 할당. (C++의 클래스도 동일하다.)
		1. 구조체 멤버들은 선언 순서대로 메모리가 할당된다.
		2. 각각의 구조체 멤버가 위치한 offset은, 자신의 자료형 크기의 배수이다.
		3. 구조체의 크기는, 멤버로 사용된 가장 큰 자료형 크기의 최소 배수이다.
		4. (1, 2, 3)을 통하여 구조체의 크기가 결정된다.
		5. 배열과는 다르게 구조체형 변수이름이 주소값은 아니다. 배열과 같이 &(구조체형 변수이름)이 그의 offset 0의 주소값이기는 하다.



	- 구조체형 포인터

		구조체 또한 자료형이므로, 포인터 또한 해당 구조체타입으로 선언.정의할 수 있다.
		주소값 연산 또한 해당 구조체형의 크기를 따른다.

		구조체형으로 역참조 시, 멤버로 나뉜다.
		e.g.

		typedef struct ST1
		{
			int a;
			float b;
		}ST1;

		void main(void)
		{
			struct ST1 ST_Test1 = { 0 };

			ST1* pST = &ST_Test1;

			(*pST).a = 100;		//  ==  pST->a = 100;			//  (*structureType).member  ==  structureType->member
			(*pST).b = 10.3f;	//  ==  pST->b = 10.3f;
		}

		구조체형 타입캐스팅은 불가능하다. (ERROR)



<const 한정자, Lvalue, Rvalue> (상수)

	const 한정자의 역할.
		1. 변수의 값이 변할 수 없다는 것을 명시적으로 지정함으로써, 코드 분석을 쉽게 할 수 있다.
		2. 프로그램 오류 시 디버깅으로 데이터의 변화를 추적한다.
			이 때 const를 제외시킴으로써 파악해야 하는 변수가 줄어들고, 디버깅이 수월해진다.
		3. const 한정자에 대한 문법은,
			C, C++, 등 언어마다 다르고, 컴파일러마다 다르다.
			이에 대한 하나의 예시는 main.cpp - <강제 타입캐스팅과 const 한정자>에 있다.


	C언어에는 left_value, right_value 두 가지 값이 존재한다.
	역사적으로, C언어의 대입연산자를 사용한 식에서 Lvalue와 Rvalue가 나왔다.
	모든 존재하는 값은 Lvalue와 Rvalue로써 존재한다.

	C 표준에서는 대입 연산자(=)를 기준으로,
	왼쪽과 오른쪽에 모두 사용될 수 있는 값은 Lvalue이고
	오른쪽에만 사용될 수 있는 값이 Rvalue라고 정의한다.

	주소값을 가지고 있다 == 문법적으로 object의 주소를 알아낼 수 있는 장치가 존재한다.
	Lvalue는 다른 말로 locator value 이며, object의 주소값이라는 의미이다.					// &(Lvalue)   (O)
	Rvalue는 다른 말로 value of an expression 이며, 식을 계산한 후의 값이라는 의미이다.		// &(Rvalue)   (X)

	C언어에서 const 로 선언 하는 부분에 있어서는 특별하다.
	어떤 특정 변수를 바뀌지 않는 "상수"로 선언하는 부분에 있어 보통 사용한다.
	보통은 해당 코드에 대한 "안전성"을 위해 선언한다.

	const 변수는 선언 후 Lvalue가 아니게 되어 값의 초기화.대입이 불가능하므로,
	반드시 선언 시 초기화를 해야 한다.

	e.g.
	const int num = 10;    ==    int const num = 10;

	const int* ptr = &num;    ==    int const* ptr = &num;

	int* const ptr = &num;

	const int* const ptr = &num;    ==    int const* const ptr == &num;


	1. int num = 10;					// num, ptr, *ptr 모두 바꿀 수 있다.
	   int* ptr = &num;

	2. const int num = 10;			// num, ptr, *ptr 모두 바꿀 수 없다.
	   const int* const ptr = &num;



<volatile>

	컴파일러가 해당 변수를 최적화에서 제외하여 항상 메모리에 접근하도록 한다.
	즉, volatile 변수를 참조할 경우 레지스터에 로드된 값을 사용하지 않고 매번 메모리를 참조한다.

	메모리의 특정 주소값이 외부 센서로 인해 변경될 때 이는 컴파일러가 알지 못하므로,
	임베디드 프로그래밍에서는 절대적으로 필요하다.

	register와 대비되는 키워드이다.



	컴파일러는 코드 최적화를 수행할 때, 변수가 코드 내에서 변경되지 않는다고 판단되면 해당 변수를 메모리에서 읽지 않고, 레지스터에 저장된 값을 반복적으로 사용할 수 있다.
	그런데 mainLoop와 ISR의 두 코드 블록은 비동기적으로 실행되기 때문에, ISR에서 변경되는 변수는 컴파일러가 예상하지 못하는 시점에 변경될 수 있습니다.
	따라서 mainLoop에서 사용되며 ISR에 의해 변경되는 변수는 volatile로 선언해야 한다.
	->
	volatile 키워드는, mainLoop에서 사용하고자 하는 변수가 mainLoop와 비동기적으로 변경될 수 있을 때 사용한다.

	// volatile 변수를 가리키는 포인터/레퍼런스 변수도 반드시 volatile로 선언해야 한다.

	register <-> volatile



<void>

	return하지 않는다.



	ㅡ void*

	선언.정의에서 (dataType*)을 정해두지 않는다.
	뒤이은 주소값 연산 또는 역참조에서 타입캐스팅이 필요하다.



<문자>

	ㅡ 문자전용자료형

	char    :  1Byte 문자				//  ASCII
	wchar_t :  2Bytes문자				//  ASCII에 이어서 한글, 한자 등 다양한 기호가 포함되어 있다.  unsigned short형이다.

	wchar_t wc = L'한';
	wchar_t wcArr[5] = L"ab한글";		//  == { L"ad한글" }  == { L'a', L'b', L'한', L'글' };

	L  :  문자를 2Bytes로 인식한다.

	배열을 큰따옴표를 이용한 문자열로 초기화 시,
	(WARNING) C6001 초기화되지 않은 메모리 'wcArr'을(를) 사용하고 있습니다.
	우선은 중괄호를 사용하여 초기화할 것.



<문자열>

	C언어는 문자열을 표현할 수 있는 자료형을 제공하지 않는다.
	그러므로, 배열을 사용하여 문자열을 표현한다.

	문자들이 배열을 이룬 것이다.

*  문자열의 끝을 NULL로 인식하며, 배열의 크기에 포함된다. 꼭 한 칸을 비울 것.



	wchar_t arrayName[5] = L"ab한글";			//  == { L'a', L'b', L'한', L'글' };

					//  큰 따옴표를 통한 문자열(배열) 초기화는, 문자전용자료형에서만 사용할 수 있다.

	1.  wchar_t* p = arrayName;						//  == &arrayName[0];

					//  디버깅에서 주소값	 arrayName == 0x0000004d264ff8f8 L"ab한글"
					//  						 p == 0x0000004d264ff8f8 L"ab한글"

					//  스택메모리에 할당된 arrayName의 주소값과 같다.

	2.  wchar_t* p = L"ab한글";

					//  디버깅에서 주소값	 arrayName == 0x000000203918fb28 L"ab한글"
					//  						 p == 0x00007ff7cedb9d80 L"ab한글"

		*p = L'뷁';
					//  예외가 throw됨: 쓰기 액세스 위반입니다.
					//  p이(가) 0x7ff7cedb9d80였습니다.



*  포인터변수를 큰따옴표를 이용한 문자열로 초기화 시, 포인터변수를 위한 문자열의 값을 가진 읽기전용 메모리가 할당된다.
*  즉, arrayName이라는 배열과 무관하며,
*  arrayName과 똑같은 문자열을 포인터변수를 위해, 높은 위치의 읽기전용 메모리를 할당해준다.



<동적할당> (dynamic (memory) allocation)

***	동적 할당에서 배열의 길이를 해당 지역의 외부에서 받아오는 경우, buffer overflow 경고가 뜬다.
	안그래도 동적할당이어서 compile-time에 할당되지 않는데, 배열의 길이까지 정해지지 않아서 compiler가 알지 못한다.
	compiler가 알지 못하므로, Visual Studio에서 이는 잠재적 경고이며, "인덱스 연산만 확실하게" 하면 된다.



	정적 할당은 compile-time에 할당하며
	동적 할당은 run-time에 할당한다.

	아래의 함수들은,
	힙 메모리 영역에 런타임 도중 할당 . 해제시킨다.

	1. malloc
	2. calloc
	3. realloc
	4. free



	1. malloc <stdlib.h> <malloc.h>

		void *malloc(size_t size);

			size  :  할당할 바이트 수.

		size 만큼 할당하고, offset 0의 주소값을 반환한다.

		malloc은 할당된 공간에 대한 void 포인터를 반환하거나,
		malloc, calloc, realloc 모두 사용 가능한 메모리가 부족한 경우 NULL을 반환한다.



	2. calloc <stdlib.h> <malloc.h>

		void *calloc(size_t count, size_t size);

			count  :  원소의 개수
			size   :  각 원소의 크기

		size 크기를 count 수만큼 할당하고, 0으로 초기화한다. (size*count/bytes allocated)
		offset 0의 주소값을 반환한다.


	3. realloc <stdlib.h> <malloc.h>

		void *realloc(void *block, size_t size);

			block  :  이전에 할당된 메모리 블록
			size : 다시 할당할 바이트 수

		이전 block의 초기화된 데이터를 복사하고 size만큼 재할당한다. 이외의 데이터는 0으로 초기화되지 않는다.
		이전 block은 해제되며, 데이터는 유지된다.
		재할당된 block에 대한 void 포인터를 반환한다.



	3. free <stdlib.h> <malloc.h>

		void free(void *block);

			block  :  이전에 할당된 메모리 블록.

		block을 해제한다.

		free를 통한 메모리 해제 시, 사용하지 않을 포인터 변수들은 NULL을 가리켜주면 좋다. (프로그램 종료 시, 에러 발생 가능성)



<가변 배열> (dynamic array) (가변 길이 배열) (variable-length array)

	malloc calloc realloc free 동적할당.해제 함수들과, *(Arr + N) == Arr[N] 이것을 사용하면,
	이것만으로 가변배열이라고 할 수 있다.
	프로젝트의 목적, 코드의 가독성, 최적화를 위해
	또는 확실하게 assert()함수를 사용한 함수들로 메모리 관리를 위해서
	함수로도 만들 수 있다.

	2차원 가변 배열의 경우, 더블포인터를 사용한다.



<연결 리스트> (linked list)

	ㅡ dynamic array  VS  linked list

	dynamic array (only malloc())
	주소값이 연속적이며, 주소값 연산이 가능하다.
		- 정해진 대상으로의 접근에 대한 시간복잡도 : O(1) - 해당 원소의 정확한 주소값 또는 인덱스를 알아야 한다.
		- 접근, 탐색 속도는 조금 더 빠르다.
	PushBack 작업을 제외하고, 대부분이 비효율적이다.
	구조체  ::  _tagArr
	_tagArr - pointer, iCount, iMaxCount


	linked list
	주소값이 연속적이지 않으며, 주소값 연산이 불가능하다.
		- 정해진 대상으로의 접근에 대한 시간복잡도 : O(1) - 해당 원소의 정확한 주소값을 알아야 한다.
		- 접근, 탐색 속도는 더 느리다.
	가변배열보다 확장, 축소, 해제가 위치에 상관없이 자유로우며, 빠르다.
		- PushBack  PushFront  PushMiddle  Release(any Node)
	구조체  ::  _tagList  _tagNode
	_tagList.member - pHeadNode(pointer for first Node), iCount(the quantity of Nodes)
	_tagNode.member - pNextNode(pointer for next Node), iData



	ㅡ linked list

	각각의 공간을 노드(node)라고 한다.
	하나의 노드에는 데이터와, 다음 노드를 가리키는 주소값이 있다.
	헤드를 포함한 마지막 노드의 경우, NULL을 가리켜준다.



<정렬> (sort)

	버블 정렬(bubble sort)
	삽입 정렬(insertion sort)
	선택 정렬(selection sort)
	합병 정렬 or 병합 정렬(merge sort)
	쾌속 정렬(quick sort)
	기수 정렬(Radix sort)
	힙 정렬(heap sort) - heap 알고리즘이 필요하다. 힙 정렬은 힙 트리를 이용하는 알고리즘이다.
	힙이란, 자료구조 중 하나이다. 최소, 최대를 빠르게 찾는다. 배열로 구현이 가능한 완전 이진트리를 구현한 다음 힙 정렬을 구현하자.
	...

	함수 구현하고 파일 분할할 것.
	다 하면, <정렬> 지울 것.



<난수>

	구현할 것.
	rand()
	srand()
	srand(time(NULL))
	time() < time.h
	헤더파일



<함수 포인터> (function pointer)

	매개변수가 void or parameters이고, 반환형이 returnType인 Test1 함수를 참조하기 위한 포인터변수 pFunc의 선언.정의.

	returnType(*pFunc)(void or parameters);
	pFunc = Test1;

	e.g.
	int Add(int a, int b)
	{
		return a + b;
	}
	void main(void)
	{
		int(*fp)(int, int);    or    int(*fp)();		// 함수포인터 정의

		fp = Add;										// fp == &Add

		int iValue = fp(10, 20);						// iValue == 30
	}

*  returnType은 꼭 맞춰줍시다~

	다른 사람의 코드와 같이, 참조만 가능한 함수들을 참조하여 코드를 작성할 때,
	함수포인터가 쓰일 수 있다.





	2024.07.28
	// 일반적으로 하나의 포인터는 하나의 entity만 가리키도록 설계하는 것이 좋다.
    
	데이터 변수와 포인터 변수로의 구분이 있고, 데이터 포인터와 함수 포인터로의 구분은 없으며 타입캐스팅이 가능하다.
    포인터는 데이터를 가리킬 수 있고, 함수를 가리킬 수 있다.


    데이터 포인터의 선언과 타입캐스팅
    ->
    decl  :  dataType* ptrName;       // 또는 dataType(*ptrName);
    typeCasting  :  (dataType*)ptrName


    함수 포인터의 선언과 타입캐스팅
    ->
    decl  :  returnType (*ptrName)(para1Type, para2Type, ...);
    typeCasting  :  (returnType (*)(para1Type, para2Type, ...) ptrName


			e.g.

			typedef int* ptr_t;
			typedef int (*funcPtr_t)(int, int);

			int add(int, int);
			void main(void)
			{
				int i1 = 10;

				// typedef로 새로 생성한 newType으로 선언.
				ptr_t ptr1 = &i1;
				funcPtr_t funcPtr1 = add;


				// 타입캐스팅 및 역참조(데이터 사용)
				int val = *(int*)ptr1;
				val = ((int (*)(int, int))funcPtr1)(10, 20);
			}
			int add(int a, int b)
			{
				return a + b;
			}



    !!! 모든 변수는 배열로 선언할 수 있다. 데이터 변수, 데이터 포인터, 함수 포인터 등
    예시
    ->
    데이터 변수의 배열  :  가변 배열(dynamic array) 등

    데이터 포인터의 배열  :  allocTable, garbage collection, ...

    함수 포인터의 배열  :  인터럽트 번호 별로 해당되는 ISR 함수의 주소를 저장하는 interrupt vector table (AVR의 경우 vectorTable은 0번지.)





<계산 복잡도 이론 - 컴퓨터>

	runtime의 시간복잡도,
	메모리 할당량의 공간복잡도로 나뉜다.

	특정 연산의 효율성을 따질 때 사용될 수 있다.



	e.g. push_front()

		1. Dynamic Array

			어느 가변배열의 원소가 N개이다.

			메모리 하나를 다음 칸으로 옮기는 시간비용을 a라고 하면,
			총 시간비용은 (a * N)이다.

			여기서 계수를 무시하고 N에 관한 1차식으로 표현한다.

			Best  : O(1)
			AVG   : O(N/2)
			Worst : O(N)

			가변배열에 push_front 작업을 할 때의 시간복잡도 : O(N)



		2. Linked List

			연결리스트의 노드가 N개이다.

			PushFront 작업에 걸리는 시간은, N과 관계없다.

			연결리스트에 push_front 작업을 할 때의 시간복잡도 : O(1)  :  고정시간이며, 보통 없는취급한다.



<표준 입출력 스트림> (standard input/output stream) (standard i/o stream)

								[C]에서는, 스트림(stream)이라는 개념을 사용하여 일관된 방식으로 입출력을 수행한다.
								스트림은, 모든 입력과 출력을 BYTE들의 흐름으로 생각하는 것을 말하며, 데이터가 한 방향으로만 흐른다.
								즉, 어떤 입출력 장치든 바이트 단위로 입출력이 이루어진다.
								입력 . 출력 . 오류 스트림이 따로 존재한다.

								이러한 방식의 스트림의 최대 장점은, 장치 독립성이다.
								입출력 장치가 무엇이든 상관없이 프로그램을 작성할 수 있다.
								e.g. 네트워크 소켓통신 etc...

								입력 스트림 . 출력 스트림은 줄 단위의 버퍼링을 하며, 한 줄을 완성해야 출력된다. ( "\n" )
								그렇기에, 프로그램은 쓸 데이터가 얼마나 남았는지, 어떤 식으로 묶여있는지에 대해 알 필요가 없다.

								오류 스트림은 버퍼링 없이 바로바로 출력된다.
								e.g.
								fprintf(stdout, "%s", message);
								fprintf(stderr, "%s", message);		- 표준 오류가 먼저 콘솔에 찍힌다. 이는 표준 오류에 "\n"이 없어서 생기는 현상이다.







	프로그램 동작에 필수적이고 기본적인 스트림은 프로그램 실행시에 자동으로 생성된다.
	이를 표준 스트림이라고 한다.
	스트림 참고	: https://dogrushdev.tistory.com/35?category=857735
				: https://araikuma.tistory.com/593
				: https://sjh836.tistory.com/41



	ㅡ 버퍼(buffer)	  :	컴퓨터의 주 기억장치와 주변장치 사이에서 데이터를 주고받을 때,
						둘 사이의 전송속도 차이를 해결하기 위해, 전송할 정보를 임시로 저장하는 고속 기억장치이다.
						용량이 한정되어 있으며, 오버플로우가 발생할 수 있다.
						입출력 속도는 생각보다 느리기 때문에, 버퍼를 사용한다.			// ++i 와 printf("%d\n", ++i) 의 반복 속도 차이만 봐도 알 수 있다.
						표준 라이브러리의 입출력 함수들은 모두 버퍼를 사용한다.			// 표준 라이브러리 함수가 아닌 함수는 확실치 않다. e.g. putch() . getch()
	*** 버퍼 참고	  :	https://dogrushdev.tistory.com/39?category=857735
						// 각 스트림에 속한 버퍼가 있으므로, 스트림과 버퍼를 연관지어 이해하면 편하다.

	ㅡ 버퍼링  :	정보의 송수신을 원활하게 할 수 있도록 수신되는 정보를 버퍼라는 공간에 일시적으로 저장해서
				처리 속도의 차이를 흡수하는 방법을 말한다.
				// 두 매체 사이에서 정보가 송수신될 때 발생할 수 있는 충돌 현상을 완화하는 동작이라고 할 수 있다.



		<stdio.h>의 멤버변수.
		||
		stdin		표준입력 스트림		키보드			scanf etc...
		stdout		표준출력 스트림		모니터			printf etc...
		stderr		표준오류 스트림		모니터			perror etc...

		stdin     :	표준 입력 스트림(일반적으로 키보드)을 참조하는 FILE에 대한 포인터.
		stdout	  :	표준 출력 스트림(일반적으로 디스플레이 터미널)을 참조하는 FILE에 대한 포인터.
		stderr	  :	표준 오류 스트림(항상 디스플레이 터미널)을 참조하는 FILE에 대한 포인터.

				// stderr는 에러를 출력하기 위한 스트림이다.
				// V.S.에서는 stdout과 같은 디바이스(콘솔)로 출력되며, 당연히 스트림과 버퍼는 다르다.
				// redirect함으로써 디바이스를 변경할 수 있다.
				// 이는 GUI에서는 무의미하다. 참고 : https://namu.wiki/w/GUI

				// stdout은 printf, stderr는 에러를 출력하기 위한 스트림으로 프로그래머에 의해 다르게 정의될 수 있으나, 보통 stdout과 같이 사용된다.



		ㅡ <stdio.h>

			<stdio.h>에 선언되어 있는 함수는 일반적으로 파일 조작 함수 . 콘솔 입출력 함수로 구분된다.

			참고 : https://ko.wikipedia.org/wiki/Stdio.h
					<stdio.h>의 멤버 함수 . 상수 . 변수 . 형식에 대한 정보가 있다.



		ㅡ 표준 입출력 스트림에 대해서 주로 사용되는 입출력 함수들.

			[분류표]
											표준 스트림			일반 스트림					설명
			ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
											getchar()			fgetc(FILE *f, ...)			문자 입력

			타입이 존재하지 않음.				putchar()			fputc(FILE *f, ...)			문자 출력
			문자열 형태.
											gets()				fgets(FILE *f, ...)			문자열 입력

											puts()				fputs(FILE *f, ...)			문자열 출력

			ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

											scanf()				fscanf(FILE *f, ...)		타입 지정 입력
			타입이 존재함.
											printf()			fprintf(FILE *f, ...)		타입 지정 출력
			ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	ㅡ scanf(), printf() 함수의 구조
		참고 : https://m.blog.naver.com/vjhh0712v/221495007317



<파일 입출력> (file input/output) (file i/o)

													EOF : End Of File
													#define EOF    (-1)
													#define WEOF ((wint_t)(0xFFFF))		// 65535

	<stdio.h>에 정의되어 있다.

	실제로 우리가 흔히 아는 그 파일을 말한다.

	객체는 프로그램 종료 시에 소멸되므로(메모리), 영구적으로 기억할 수 없다.
	파일 입출력은 이것을 가능하도록 한다.(디스크)

	[C]에서 파일을 사용하기 위해서,
	파일 스트림을 생성하고 이를 파일 구조체형 포인터변수에 대입하여 사용한다.
	이 파일포인터는, 파일의 위치(커서)를 제어하는 역할을 한다.
	즉, 파일 읽기 . 쓰기 실행 시 파일포인터값이 변경된다.

	[C]에서 지원하는 파일의 유형은 두 가지.
	텍스트 파일과 이진 파일이다.

	ㅡ file

		ㅡ text file					: 문자들로 이루어져 있고, 연속적인 라인으로 구성된 형태이다.
										e.g. 메모장, 프로그램의 소스파일, etc.

		ㅡ binary file(이진 파일)	: '0'과 '1'로만 이루어져 있고, 라인으로 구성되지 않은 형태이다.
										e.g. 프로그램 실행파일, 사운드 파일, 이미지 파일, etc.



	프로그램에서 파일 처리를 할 때는 반드시 아래의 루틴을 따라야 한다.

					open (파일 열기)			->			파일 읽기/쓰기			->			close (파일 닫기)
		public   :	fopen()								etc...								fclose()



	structType FILE : 파일을 가리키는 구조체 자료형이며, 'FILE*'와 같이 포인터로 사용된다.

***	FILE 구조체 포인터 객체(fp)가 사용되는 모든 read/write 함수는 file pointer를 사용한다.
***	consol을 포함하여 모든 입출력이 file pointer를 사용한다.
***	file pointer의 위치를 변경하는 fseek() . rewind() 함수를 중시할 것.

*** 파일을 열면, 모드에 따라 파일을 초기화하고, 파일을 닫아야 작성한 글이 저장된다.

	파일 읽기/쓰기 함수가 인자로 받은 파일의 주소값이 NULL이라면, ERROR를 띄우고 프로그램은 종료된다.
	그러므로, 유효성 검사는 파일 오픈 시에 하는 것이 좋다.



	<file open function>  :  filename에 대한 파일 스트림을 생성하고, 해당 파일을 개방한다.
								각 함수의 return 값을 받아 유효성 검사를 할 수 있다.
								( 아래의 함수 모두 FILE* pFile 만으로 유효성 검사가 가능하다. )



		  FILE* pFile =	FILE* fopen							  FILE* pFile =	FILE* wfopen
						(													(
							const char* filename,								const wchar_t* filename
							const char* mode									const wchar_t* mode
						);													);

						return : pass(&filename)  or  fail(NULL)			return : pass(&filename)  or  fail(NULL)
						pFile  : pass(&filename)  or  fail(NULL)			pFile  : pass(&filename)  or  fail(NULL)



 errno_t err_fileopen =	errno_t fopen_s				 errno_t err_fileopen =	errno_t _wfopen_s
						(													(
							FILE** pFile,										FILE** pFile,
							const char *filename,								const wchar_t *filename,
							const char *mode									const wchar_t *mode
						);													);

						return : pass(0)		  or  fail(!0)				return : pass(0)		  or  fail(!0)
						pFile  : pass(&filename)  or  fail(NULL)			pFile  : pass(&filename)  or  fail(NULL)


						pFile		: 열린 파일에 대한 포인터를 수신할 파일 포인터에 대한 포인터이다.
									  성공 (파일 주소값)  or  실패 (NULL)

						filename	: 경로를 포함한 파일 이름이다.
									  // 절대 경로와 파일 이름을 모두 작성하거나, 현재 프로젝트 내인 상대 경로로 파일 이름만 작성해도 된다.
									  // V.S. 경로 표시 기호는 슬래시이다. (횟수는 한 번, 두 번, 그 이상 가능하다.)



						mode		: 허용되는 액세스 형식이다.
									  // 대괄호는 선택사항이다.

									  mode = "파일 오픈 모드"
									  // 파일 오픈 모드 == 파일 접근 모드 [파일 입출력 모드] [파일 추가 모드] [ccs=FLAG]

									  ccs  :  default ANSI  or  [ FLAG  =  UNICODE  or  UTF-8  or  UTF-16LE ]
									  // encoding . decoding 방식을 지정한다. 기본값은 ANSI이다.



									  ㅡ 파일 오픈 모드

											접근모드 . 추가모드 . 입출력모드를 조합하여 파일오픈모드를 결정한다.
											입출력모드는 선택사항이다.
											e.g. r  rt  r+  r+t ( == rt+ )


											ㅡ 접근 모드		( r  w  a )

												r  :  읽기(read) 전용.
													  no file : FAIL

												r+ :  읽기, 쓰기 전용.				overwrite
													  no file : FAIL

												w  :  쓰기(write) 전용.				open (reset to new file)
													  no file : create new file

												w+ :  읽기, 쓰기 전용.				open (reset to new file)
													  no file : create new file

												a  :  추가(append) 쓰기 전용.			subsequent write
													  no file : create new file

												a+ :  읽기, 추가쓰기 전용.			subsequent write
													  no file : create new file


											ㅡ 추가 모드		( x  + )

												x  :  (exclusive mode)	열고자 하는 파일이 이미 존재하면, 파일 개방에 실패한다.

												+  :  (update mode)		읽기 . 쓰기 모두 지원한다.


											ㅡ 입출력 모드	( t  b )

												t  :  text file mode

												b  :  binary file mode



	<file close function>  :  fclose		: 해당 파일 스트림을 소멸하고, 해당 파일을 닫는다.

							  _fcloseall	: 모든 열린 파일 스트림을 소멸하고, 모든 파일을 닫는다.
											  // stdin, stdout, stderr(MS-DOS의 경우 _stdaux 및 _stdprn)을 제외하고 모든 열린 스트림을 닫는다.
											  // 사용에 주의하자. 참고 : https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170

								스트림버퍼에 남아있는 데이터를 파일로 완전히 내보내고, 파일 입출력을 위해 내부적으로 생성했던 FILE 구조체를 해제한다.
								// overwrite와 subsequent write로 파일을 변경하면, 해당 파일에 대한 스트림 버퍼를 수정하는 것이며,
								// 파일을 닫아야 저장된다고 생각하면 된다.
								// 프로그램이 비정상적으로 종료될 때 모든 스트림버퍼는 소멸된다.




						int fclose											int _fcloseall( void );
						(
							FILE* stream
						);

						return : pass(0)    or    fail(EOF)					return : pass(닫힌 총 스트림 수)    or  fail(EOF)

						stream : FILE 구조체에 대한 포인터이다.



	<other member of <stdio.h>>	  :	이 공간에 <stdio.h>에 속한 멤버들을 모두 정리해놓았다.
									// 조건에 따라 또는 그 자체로 동일한 기능을 하는 함수들이 있다.
									// Microsoft manual에서 차이점을 확인할 수 있다.

								   get은 입력 . 읽기,
								   put은 출력 . 쓰기이다.



***************	ㅡ 파일에서 한글 또는 다른 언어를 읽고 쓰기 위한 방법이다.

								아래의 1번 ~ 4번을 모두 동일하게 설정한다.

								1.	compiler의 character sets(문자 집합)

								2.	해당 파일의 encoding 방식 . fopen 함수에서의 encoding 방식

								3.	<locale.h> -> setlocale(LC_ALL, NULL or "KOREAN" or "KOREA")
									// 일단 이렇게만 쓰자.

								4.	입출력 함수의 형식 선택
									// e.g.
										[ㄱ]	  :	"~~" 1(Byte)  or  L"~~" 2(Bytes)
										[ㄴ]	  :	fprintf  or  fwprintf
										[ㄱ]과 [ㄴ] 중에서 하나만 적용되어야 한다.
										모두 적용함은 모두 적용하지 않음과 같다.



***************	ㅡ 파일의 경로 . 이름은, 이를 저장하는 버퍼(char* . wchar_t*)를 사용한다.




				ㅡ freopen() . wfreopen() . freopen_s() . wfreopen_s()
								oldStream을 닫고, newStream을 연다.
								// oldStream이 닫혀있다면, oldStream포인터가 가리켰던 주소에 새로 할당된 무언가의 데이터를 덮어쓸 수 있다.
								// newStream포인터가 가리키던 Stream이 있다면, 메모리 누수가 일어난다.
								// newStream포인터가 가리키던 Stream을 다시 여는 것은 두 번 여는 것으로 ERROR이다.
								// 정상동작 시, newStream == oldStream		( oldStream의 주소에 newStream을 덮어쓴다.)
								하나의 파일포인터로 기존의 파일을 닫고 새로운 파일을 여는 것이 가장 바람직하다.

						FILE* freopen										FILE* wfreopen
						(													(
							const char* filename,								const wchar_t* filename,
							const char* mode,									const wchar_t* mode,
							FILE* oldStream										FILE* oldStream
						);													);
						return : pass(FILE* newStream)    or    fail(NULL)


						errno_t freopen_s									errno_t wfreopen_s
						(													(
							FILE** newStream,									FILE** newStream,
							const char* filename,								const wchar_t* filename,
							const char* mode,									const wchar_t* mode,
							FILE* oldStream										FLIE* oldStream
						);													);
						return	  : pass(0)					 or    fail(error code)
						newStream : pass(FILE* newStream)    or    fail(NULL)




				ㅡ remove() . _wremove()
								FileName으로 지정된 파일을 삭제한다.
								// 파일 이름이 멤버 이름을 포함하면, 멤버가 제거되거나 파일이 삭제된다.@@@@@@@@@@@@@@@@@@@??
								// 존재하지 않는 파일을 삭제할 수 없다.
								// 열린 파일을 삭제할 수 없다.

						int remove											int _wremove
						(													(
							const char* FileName								const wchar_t* FileName
						);													);
						return : pass(0)    or    fail(-1)					return : pass(0)    or    fail(-1)




				ㅡ rename() . _wrename()
								파일 또는 디렉토리의 이름을 바꾼다.								// DOS명령어 ren . unix/linux명령어 mv
								oldname에서 지정한 파일 이름을 newname으로 지정한 이름으로 바꾼다.
								// 두 파일이 속한 디스크의 볼륨 레이블이 같아야 한다.
								// 열린 파일의 이름을 변경할 수 없다.




						int rename											int _wrename
						(													(
							const char* oldname,								const wchar_t* oldname,
							const char* newname									const wchar_t* newname
						);													);
						return : pass(0)    or    fail(!0)					return : pass(0)    or    fail(!0)




				ㅡ rewind()
								해당 스트림과 연관된 파일 포인터의 위치를 파일의 시작 부분으로 변경한다.
								// 스트림에 대한 오류 인디케이터(표시기) . 파일 끝 인디케이터(표시기)를 지운다.
								// type == record 로 열린 파일에 대해 지원하지 않는다.

						void rewind
						(
							FILE* stream
						);




				ㅡ fseek() . _fseeki64()
								해당 스트림에 연관된 파일 포인터의 위치를 새로운 위치로 변경한다.
								파일 끝 인디케이터를 지운다.
								( fseek(fp, 0, SEEK_SET) ~= rewind(fp) )
								// C++의 파일포인터가 32비트라서 2GB 이상의 파일을 액세스하지 못하는 문제가 있다.
								// 알아보니 C언어의 <stdio.h>에서 _fseeki64()를 지원하는 것을 확인하였다.
								// fseek를 꼭 사용해야 하는 경우 유용할 것 같다.
								// http://inadrawer.blogspot.com/2017/08/c-fseeki64-2gb.html

						int fseek											int _fseeki64
						(													(
							FILE* stream,										FILE* stream,
							long offset,										__int64 offset,
							int origin											int origin
						)													)
						return : pass(0)    or    fail(!0)					return : pass(0)    or    fail(!0)

						'offset'  : 파일 포인터를 ('origin') +- ('offset' * BYTE) 위치로 이동시킨다.
									파일의 char . wchar_t 문자열과 상관없이, 무조건 BYTE 단위로 이동한다.
									파일 끝 너머로 이동할 수 있다.
									양음수 모두 가능하다.

						'origin'  :	SEEK_SET(처음) . SEEK_END(끝) . SEEK_CUR(현재)




				ㅡ ftell() . _ftelli64()
								파일포인터의 offset(0)에 대한 현재 위치값을 반환한다.
								// 실패 시 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
								// 두 함수의 차이점은, 위의 fseek() . _fseeki64() 함수와 동일하다.

						long ftell											__int64 _ftelli64
						(													(
							FILE* stream										FILE* stream
						);													);




				ㅡ tmpfile() . tmpfile_s()
								임시 바이너리 파일을 "w+b" 모드로 생성한다.
								// 파일을 닫거나 프로그램이 종료되면, 파일이 자동으로 제거된다.
								// 임시 파일은, 루트 디렉토리에 생성되며, 루트가 아닌 디렉토리에 생성하려면,   fopen   +   (tmpnam_s   or   tempnam)

						FILE* tmpfile( void );								errno_t tmpfile_s
																			(
																				FILE** pFilePtr
																			);
						return : pass(stream*)    or    fail(NULL)			return : pass(0)    or    fail(ERROR code)




				ㅡ perror() . _wperror()
								오류 메세지(message)를 stderr에 출력한다.
								메시지가 먼저 인쇄되고 콜론이 그 뒤를 잇고 오류를 생성한 마지막 라이브러리 호출에 대한 시스템 오류 메시지와 마지막으로 줄 바꿈 문자가 표시된다.
								// 출력 순서 : message  >  :(콜론)  >  오류를 생성한 마지막 라이브러리 호출에 대한 system error message  >  \n(줄 바꿈 문자)
								메시지가 null 포인터이거나 null 문자열에 대한 포인터인 경우 perror는 시스템 오류 메시지만 출력한다.
								// 정확한 결과를 얻으려면, 라이브러리 함수가 오류를 리턴한 다음 바로 perror() 함수가 호출되어야 한다.
								// 그렇지 않으면, 후속 호출은 errno 값을 덮어쓰게 될 수 있다.

						void perror											void _wperror
						(													(
							const char* message									const wchar_t* message
						);													);




				ㅡ feof()
								해당 스트림의 파일포인터에 대한 파일 끝을 확인한다.
								파일 끝이 아니면 0을 반환하고, 파일 끝이면 !0을 반환한다.
								// 파일 끝 플래그는 rewind() . fsetpos() . fseek() . clearerr() 함수를 호출하여 지울 수 있다.

						int feof
						(
							FILE *stream
						);
						return : no_finish(0)    or    finish(!0)




				ㅡ ferror()
								해당 스트림의 오류 표시자를 확인한다.
								오류 표시자가 설정되어 있지 않으면 0을 반환한다. 그렇지 않으면, 0이 아닌 값을 반환한다.
								// 스트림과 연결된 파일에서 읽기 . 쓰기 오류를 테스트한다.
								// 오류가 발생한 경우,
								// 스트림이 닫히거나 다시 풀릴 때까지 또는 스트림에 대해 더 명확하게 호출될 때까지
								// 스트림에 대한 오류 표시기가 설정된 상태로 유지된다.
								// // 아래는 같은 말이다.
								// 오류가 발생한 경우 스트림을 닫고,
								// rewind() 함수를 호출하거나 clearerr() 함수를 호출할 때까지, stream에 대한 오류 인디케이터가 설정된 채로 유지된다.

						int ferror
						(
							FILE* stream
						);
						return : pass(0)    or    fail(!0)



				ㅡ clearerr() . clearerr_s()
								스트림에 대한 오류 표시기를 재설정한다.
								// 스트림에 대한 오류 표시기와 파일 끝 표시기를 재설정한다.
								// 오류 표시기는 자동으로 지워지지 않는다.
								// 지정된 스트림에 대한 오류 표시기가 설정되면, 해당 스트림에 대한 작업은
								// clearerr() . fseek() . fsetpos() . rewind() 이 함수들이 호출되기 전까지 설정된 채로 남아있는다.

						void clearerr										errno_t clearerr_s
						(													(
							FILE* stream										FILE* stream
						);													);
																			return : pass(0)    or    스트림이 NULL인 경우, EINVAL이다.



				ㅡ getc() . getwc()
								스트림에서 문자를 읽는다.
								읽은 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 오류 또는 파일의 끝을 나타낸다.
								// 매크로로 구현되어 있으므로, 부작용이 발생할 수 있고 인자가 수식으로 오면 안된다.		e.g. char c = getc(*p++); XXXXX
								// fgetc() 함수보다 속도가 빠르다.
								// 'put' 함수들도 이와 같다.

						int getc											wint_t getwc
						(													(
							FILE* stream										FILE* stream
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ putc() . putwc()
								스트림에 문자를 쓴다.
								쓴 문자를 반환하거나, EOF 또는 WEOF를 반환하여 오류 또는 파일의 끝을 나타낸다.
								// putc 루틴은 현재 위치의 출력 스트림에 단일 문자 c를 씁니다. 모든 정수는 putc에 전달할 수 있지만 하위 8비트만 기록된다.
								// https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/putc-putwc?view=msvc-170
								// type == record 로 열린 파일에 대해 지원하지 않는다.

						int putc											wint_t putwc			// @@@@@@@@@@ 한글 표기 안되는 이유가??
						(													(
							int c,												wchar_t wc,
							FILE* stream										FILE* stream
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(c)    or    fail(WEOF)




				ㅡ gets_s() . _getws_s()																// gets() . getws() : 더 이상 사용되지 않는다.
								stdin에서 문자열을 읽는다.
								buffer를 반환하거나, NULL 포인터를 반환하여 오류 또는 파일의 끝을 나타낸다.

						char* gets_s										wchar_t _getws_s
						(													(
							char* buffer,										wchar_t* wbuffer,
							size_t sizeInCharacters								size_t sizeInCharacters
						);													);
						return : pass(buffer)    or    fail(NULL)			return : pass(wbuffer)    or    fail(NULL)




				ㅡ puts() . _putws()
								stdout에 문자열을 쓴다.
								음수가 아닌 값을 반환하거나(unint), EOF 또는 WEOF를 반환하여 오류를 나타낸다.

						int puts											int _putws
						(													(
							const char* str										const char* str
						);													);
						return : pass(unint)    or    fail(EOF)				return : pass(unint)    or    fail(WEOF)




				ㅡ _getch() . _getwch()																// getch() : 더 이상 사용되지 않는다.
								에코를 사용하지 않고, 콘솔에서 문자를 입력받는다.
								읽은 문자를 반환하며, 오류 반환이 없다.
								// 확장키를 입력하기 위해서는 함수를 두 번 호출해야 한다.
								// 키 코드 = 첫 반환값(0xE0) + 다음 반환값
								// 버퍼 사용 : X
								// 화면 표시 : X
								// 종료 인식 : \r

						int _getch( void );									wint_t _getwch( void );

						return : pass(c)									return : pass(c)

						<conio.h>											<conio.h>    or    <wchar.h>




				ㅡ _getche() . _getwche()															// getche() : 더 이상 사용되지 않는다.
								에코를 사용하여 콘솔에서 문자를 입력받는다.
								읽은 문자를 반환하며, 오류 반환이 없다.
								// 버퍼 사용 : X
								// 화면 표시 : O
								// 종료 인식 : \r

						int _getche( void );								wint_t _getwche( void );

						return : pass(c)									return : pass(c)

						<conio.h>											<conio.h>    or    <wchar.h>




				ㅡ getchar() . getwchar()
								stdin에서 문자를 읽는다.			// getchar() == getc(stdin)
								읽은 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 파일의 오류 또는 끝을 나타낸다.
								// 버퍼 사용 : O
								// 화면 표시 : O
								// 종료 인식 : \n

						int getchar( void );								wint_t getwchar( void );

						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ _putch() . _putwch()																// putch() : 더 이상 사용되지 않는다.
								콘솔에 문자를 쓴다.
								쓴 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 오류를 나타낸다.
								// 버퍼 사용 : X

						int _putch											wint_t _putwch
						(													(
							int c												wchar_t wc
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ putchar() . putwchar()
								stdout에 문자를 쓴다.				// putchar(c) == putc(c, stdout)
								쓴 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 파일의 오류 또는 파일의 종료 조건을 나타낸다.
								// type == record 로 열린 파일에 대해 지원하지 않는다.

						int putchar											wint_t putwchar
						(													(
							char c												wchar_t wc
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ fgetc() . fgetwc()
								스트림에서 문자를 읽는다.
								읽은 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 오류 또는 파일의 끝을 나타낸다.

						int fgetc											wint_t fgetwc
						(													(
							FILE *stream										FILE *stream
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(EOF)




				ㅡ fputc() . fputwc()
								스트림에 문자를 쓴다.
								쓴 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 오류를 나타낸다.

						int fputc											wint_t fputwc
						(													(
							int c,												wchar_t wc,
							FILE* stream										FILE* stream
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ fgets() . fgetws()
								스트림에서 문자열을 읽는다.
								읽은 문자열을 반환하거나(str . wstr), NULL을 반환하여 오류 또는 파일의 끝을 나타낸다.
								// '\n'까지 읽는다.

						char* fgets											wchar_t* fgetws
						(													(
							char* str,											wchar_t* wstr,
							int numChars,										int numChars,
							FILE* stream;										FILE* stream;
						);													);
						return : pass(str)    or    fail(NULL)				return : pass(wstr)    or    fail(NULL)




				ㅡ fputs() . fputws()
								스트림에 문자열을 쓴다.
								음수가 아닌 값을 반환하거나(unint), EOF를 반환하여 오류를 나타낸다.

						int fputs											int fputws
						(													(
							const char* str,									const wchar_t wstr,
							FILE* stream										FILE* stream
						);													);
						return : pass(unint)    or    fail(EOF)				return : pass(unint)    or    fail(EOF)




				ㅡ _fgetchar() . _fgetwchar()
								stdin에서 문자를 읽는다.
								읽은 문자를 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 파일의 오류 또는 끝을 나타낸다.

						int _fgetchar( void );								wint_t _fgetwchar( void );

						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ _fputchar() . _fputwchar()
								stdout에 문자를 쓴다.
								쓴 문자열을 반환하거나(c . wc), EOF 또는 WEOF를 반환하여 오류를 나타낸다.

						int _fputchar										wint_t _fputwchar
						(													(
							char c												wchar_t wc
						);													);
						return : pass(c)    or    fail(EOF)					return : pass(wc)    or    fail(WEOF)




				ㅡ fscanf() . fscanf_s() . fwscanf() . fwscanf_s()
								스트림에서 형식이 지정된 데이터를 읽는다.
								// 각 함수에 대한 security 함수는, buffer가 필요한 인자의 다음으로 buffer를 적어줘야 한다. (문자, 문자열, 배열 등)
								// 첫 인자로 스트림 포인터를 받는 것을 제외하고, scanf() . scanf_s() . wscanf() . wscanf_s() 함수와 형식이 같다.

						int fscanf()    or    int fscanf_s()				int fwscanf()    or    int fwscanf_s()
						(													(
							FILE* stream,										FILE* stream,
							const char* format [,								const char* format [,
							argument ]...										argument ]...
						);													);
						return : pass(성공한 필드 수)    or    fail(EOF)		return : pass(성공한 필드 수)    or    fail(EOF)




				ㅡ fprintf() . fprintf_s() . fwprintf() . fwprintf_s()
								형식이 지정된 데이터를 해당 스트림에 출력한다.
								// 각 함수에 대한 security 함수는, buffer가 필요한 인자의 다음으로 buffer를 적어줘야 한다. (문자, 문자열, 배열 등)
								// 첫 인자로 스트림 포인터를 받는 것을 제외하고, printf() . printf_s() . wprintf() . wprintf_s() 함수와 형식이 같다.

						int fprintf()    or    int fprintf_s()				int fwprintf()    or    int fwprintf_s()
						(													(
							FILE* stream,										FILE* stream,
							const char* format [,								const wchar_t* format [,
							argument_list ]...									argument_list ]...
						);													);
						return : pass(성공한 필드 수)    or    fail(-N)		return : pass(성공한 필드 수)    or    fail(-N)




				ㅡ fread() . fread_s()
								바이너리파일을 읽는다.
								// fread_s() : 일반적으로 bufferSize == elementSize * count.

						size_t fread										size_t fread_s
						(													(
							void* buffer,										void* buffer,
																				size_t bufferSize,
							size_t elementSize,									size_t elementSize,
							size_t count,										size_t count,
							FILE* stream										FILE* stream
						);													);
						return : pass(성공한 필드 수)    or    fail(0)		return(성공한 필드 수	)    or    fail(0)




				ㅡ fwrite()
								바이너리파일에 쓴다.
								// 오류 발생 시, 파일 위치 표시기를 확인할 수 없다.

						size_t fwrite
						(
							const voud* buffer,
							size_t elementSize,
							size_t count,
							FILE* stream
						);
						return : pass(성공한 필드 수)    or    fail(0)





	ㅡ put . printf 함수들은, 정해진 버퍼 또는 정해진 형식으로 해당 스트림에 쓰기 때문에 제외한다.
	   또한 바이너리 파일에서 주로 쓰이는 fread . fwrite 함수도 제외한다.

		스트림에서 문자열을 읽을 때, 끝을 인식하는 방법은 두 가지이다.

			End of String	 ==	'\0'	  :	ASCII(0)	==    NUL(NULL)

					scanf_s("%s", Arr, Count);
					fscanf_s(stdin, "%s", Arr, Count);


			End of String	 ==	'\n'	  :	ASCII(10)	==    LF(Line Feed)

					gets_s(buffer, Count);
					fgets(buffer, Count, Stream);












*/