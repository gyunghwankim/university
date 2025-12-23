





int main(void)
{

}




/*
네이버 지식인에 올린 질문 제목 질문
C++ 함수 (function)
정리하자.
*/














/*
#include <stdint.h>
#include <stdbool.h>


int main(void)
{
    //어떤 연산자를 쓰든 두 개 타입이 반드시 일치해야 한다.
    //이를 위해 (개발자가) 명시적으로 혹은 (컴파일러가) 암시적으로 형변환을 한다.

    //암시적 형변환(implicit type conversion, type-coercion)에는, 우리가 이미 알고 있는 두 가지
    //-> 1. 대입받는 쪽으로의 형변환,    2. integer promotions(정수 승격)
    //이들 외에 여러 연산자들로 이루어진 표현식 단위에 사용되는 usual arithmetic conversions(일반적 산술 변환 (규칙))가 있다.

    //C89, C99 등 언어의 버전, C, C++ 등 언어의 종류, 그리고 칩마다 전부 규칙이 다르다고 한다. // 큰 틀이 바뀌지는 않지만, 예상치 못한 결과를 도출할 수 있다는 점이 중요하다.

    //그나마 쉽게 설명한 글: https://blog.naver.com/suresofttech/220783389880

    //생각보다 우리의 상식 선에서 type-coercion이 일어나지 않는다.

    //아래의 bool 결과와 좌우항 형변환 과정은 오른쪽 주석에 적었다.

    uint32_t comparison = 10;

    int8_t int8 = -20;
    int16_t int16 = -20;
    int32_t int32 = -20;
    int64_t int64 = -20;

    uint8_t uint8 = -20;
    uint16_t uint16 = -20;
    uint32_t uint32 = -20;
    uint64_t uint64 = -20;


    // >, +, 어떤 연산자를 쓰든 산술 변환 규칙은 동일하게 적용된다. 변환 과정에 집중한다.

    bool _int8_compa = comparison > int8;           // false            (uint32_t)10 > (int8_t)-20    ->    (uint32_t)10 > (uint32_t)4294967276
    bool _int16_compa = comparison > int16;         // false            (uint32_t)10 > (int16_t)-20    ->    (uint32_t)10 > (uint32_t)4294967276
    bool _int32_compa = comparison > int32;         // false            (uint32_t)10 > (int32_t)-20    ->    (uint32_t)10 > (uint32_t)4294967276
    bool _int64_compa = comparison > int64;         // true             (uint32_t)10 > (int64_t)-20    ->    (int64_t)10 > (int64_t)-20
    // 얘네는 크기 늘리고 부호 없애는 듯.

    bool _uint8_compa = comparison + uint8;         // true             (uint32_t)10 + (uint8_t)-20    ->    (uint32_t)10 + (uint32_t)236
    bool _uint16_compa = comparison + uint16;       // true             (uint32_t)10 + (uint16_t)-20    ->    (uint32_t)10 + (uint32_t)65516
    bool _uint32_compa = comparison + uint32;       // true             (uint32_t)10 + (uint32_t)-20    ->    (uint32_t)10 + (uint32_t)4294967276
    bool _uint64_compa = comparison + uint64;       // true             (uint32_t)10 + (uint64_t)-20    ->    (uint64_t)10 + (uint64_t)18446744073709551596
    // 얘네는 부호 없애고 크기를 늘림.



    // 이해가 되었다면, 자주 쓰는 아래의 식은 어떤 과정으로 처리가 되는지 다시 봐야 한다.

    unsigned int uint1 = 10;
    unsigned int uint2 = 20;

    int32_t val1 = uint1 - uint2;       // val1 == -10

        //연산 과정은 다음과 같다.
        //->
        //(uint32_t)10 - (uint32_t)20
        //-> 빼기
        //(uint32_t)4294967286
        //-> 이걸 int 타입으로 바라보기
        //(int32_t)-10

        //!!! 둘이 빼서 그냥 -10이 되는 게 아니다.
        //->
        //V.S. 환경에서 연산 기본 타입이 int여서 4294967286라는 수가 나왔기에 val1 값이 -10으로 나왔으나,
        //int가 int16_t(2 bytes)인 것과 같은 다른 환경에선 val1 값이 65526이 나올 수도 있다.


    // 전부 타입 명시한다면, 이것들 전부 몰라도 된다. 연산할 때 괄호 명시해주면 연산자 우선순위 몰라도 되는 것과 마찬가지다.
}
*/












/*
    1.
    C, C++에서 함수는 배열의 크기를 알 수 없다. 함수의 매개변수로 배열 크기를 명시하는 것은 인터페이스 차이만 있다.
    일반적으로 sizeof(arrayName) 연산자로 전체 바이트 수를 전달한다. (sizeof 연산자는 compile-time에 평가된다.)
    고정 크기 배열인 std::array를 대신 사용할 수 있다.

    1.
    생성자의 초기화 리스트에서 멤버는 초기화를 위한 대입 또는 복사가 가능하거나 생성자가 있어야 한다.
    기본 타입 배열은 이러한 면에서도 문법적으로도 불가능하기에 초기화 리스트에서 초기화가 불가능하다. (생성자가 있는 STL의 std::array 등은 가능하다.)
    const 기본 타입의 고정 크기 배열 멤버는 멤버 선언에서만 초기화할 수 있다. 따라서 모든 인스턴스에 공통적으로 적용된다.

    애초에 배열은 대입, 복사가 안 되어 for loop를 돌리거나 memcpy를 쓴다는 것을 기억하라.

    1.
    https://cutemoomin.tistory.com/entry/%EA%B0%99%EC%9D%80-%EC%9E%90%EB%A3%8C%ED%98%95%EC%9D%B8-%EB%91%90-%EA%B0%92%EC%9D%84-%EA%B5%90%ED%99%98%ED%95%98%EB%8A%94-%ED%95%A8%EC%88%98-%ED%98%95%EC%8B%9D-%EB%A7%A4%ED%81%AC%EB%A1%9C-swap-%EB%A7%A4%ED%81%AC%EB%A1%9C
    매크로에 대해서 다룬다. 확인하여 정리하자.
*/


























/*
    int arr[z][y][x] = { 0, };

    expression - 포인터 타입 표현, 배열 타입 표현(일반적으로 함수에서 다차원 배열을 받을 때 매개변수 타입 표현방식), 굳이 포인터로 따졌을 때
    ->
    arr[0][0][0]    - int
    &arr[0][0][0]   - int*, int [], int*
    arr[0][0]       - int*, int [], int*
    arr[0]          - int (*)[x], int [][x], int**
    &arr[0]         - int (*)[y][x], int [][y][x], int***
    arr             - int (*)[y][x], int [][y][x], int***
    &arr            - int (*)[z][y][x], ??, ??          // 배열 전체를 가리킨다.

    !!! 몇 차원이든 arr와 &arr는 다르다.
    ->
        "배열 인덱싱은 내부적으로 포인터 연산으로 변환된다.", 그리고 주소값 연산에 따라서
        int arr[10] = { 0, };
        arr + 1         // arr[1]의 주소인 &arr[1]
        &arr + 1        // +1 단위가 arr 전체 크기이므로, 주소값만 보면 &arr[10] 또는 &arr[1][0], 즉 범위를 벗어난 주소이다.


    error, warning 리딩하거나 디버깅하다 보면 별의 별 타입을 다 겪는다.
    데이터 라인과 주소 라인, 함수 포인터의 타입, 그리고 위의 다차원 배열에 대한 타입을 눈에 익힌다면, 더블 포인터는 너무 쉽다.

    더해서 일반적으로 함수가 배열을 받는 형식이 단일 포인터이다. // 난 단일 포인터로 받았으니, 이게 몇 차원인지 모르는 게 함수의 억울한 입장이다.
    함수가 배열을 어떻게 받을 것인지를 결정하는 것이, 함수에서 다차원 배열 사용의 핵심이다.
    // 함수에서 1차원 배열을 받을 때 arr와 &arr가 같다고 본 부분은, 주소값 자체는 같지만 함수 지가 int*로 받아 쓰겠다고 해서, 뭔 타입으로 보내주든 배열의 처음 주소값만 건네주면 되기 때문이었다.
    // 타입을 확실히 이해한다면, 어떤 타입으로 받아서 어떤 문법으로 쓸 것인지 내 편한대로 정할 수 있다.
    // "C언어에서 함수가 2차원, 3차원 배열을 어떻게 받아서 사용하는지, 간단하게 예시 코드좀" 이라고 GPT한테 물어보면 됨.
*/







/* 정리되지 않은 부분은 @@@@@@@@@@@ 로 표시.



*  1. 목적에 맞게 최대한 세부적으로 알고리즘을 만들고, 코드를 작성한다.
*  2. 시뮬레이션 또는 디버깅을 통해 코드의 전체 또는 부분을 수정한다.
*  3. 전체적으로 코드를 수정한다면, 알고리즘 또한 수정해야 하며, 1번으로 돌아가서 반복한다.
*  4. 세부적으로 코드를 수정한다면, 알고리즘과 코드를 검토하며, 문제 발생 시, 1번으로 돌아가서 반복한다.
*  5. 검토가 완료되었다면, 최적화와 가독성에 중점을 두어 코드를 수정하고, 주석 또한 정리하여 마무리한다.



<C++ Class>

    기존 C에서의 구조체에 없던 많은 객체지향적인 특성들을 C++의 클래스가 해준다.

    Class : 사용자 정의 자료형(user-defined dataType)

    struct tMy
    {
        mem
    };

    class CMy
    {
        mem
    };

    ㅡ instance (인스턴스)
        클래스형의 변수를 인스턴스라 한다.
        클래스가 메모리에 구현된 실체이고 변수의 개념과 동일하다.
        오버로드된 생성자, 연산자 또한 인스턴스라고 부르기도 한다.

    ㅡ 멤버 함수
        해당 클래스의 어느 객체가 사용하는 전용 함수이다.

    ㅡ 구조체와 클래스의 차이점
        C언어에서 가변배열의 동작을 지원하는 전역함수들이 있는 것처럼,
        C++에서는 클래스 외부가 아닌, 클래스 내부에 해당 클래스 전용의 멤버함수로 정의가 가능하며, 객체지향적인 다양한 기능을 가진다.

        C++에서 구조체와 클래스는 한 가지를 제외하고 모두 같다.
        C++로 넘어오면서 새롭게 클래스라는 기능이 나왔다고 해서, 기존의 구조체가 굳이 C 스타일로 머무를 이유가 없기 때문이다.

            1.  디폴트 필드 : struct(public:) . class(private:)



    ㅡ 클래스에는 이미 정의되어있는 것들이 있다.

            1. 객체에 대한 디폴드 생성자 . 디폴트 소멸자.
            2. 객체에 대한 디폴트 대입연산자.
            3. 객체에 대한 디폴트 복사생성자. - 참고 : C:\Users\bibib\OneDrive\바탕 화면\C언어\[C++] 지식 - 복사 생성자
            4. 멤버함수 내에 this포인터.

        4번은 <'this' pointer>에서 설명한다.
        아래의 소괄호는 <이니셜라이저>에서 추가로 설명한다.
        (객체의 데이터가 변경되려면, 이니셜라이저 또는 연산자를 거쳐야 한다.)



<class : static 멤버>

    C++에서 static 멤버는 객체 기반이 아닌 클래스 기반으로, 전역에 생성되므로, 멤버는 아래의 특성을 가진다.
    당연히 필드의 영향을 받는다.
    (일반적인 멤버는, 클래스 기반이 아닌 객체 기반이며 객체를 필요로 한다.  ( (ERROR) E0245 : 비정적 멤버 참조는 특정 개체에 상대적이어야 합니다. )


    ㅡ static 멤버변수

        객체와 상관없이 클래스를 통해 호출할 수 있다. (클래스 변수라고도 한다.)

        클래스 내에 static 멤버를 선언하는 것만으로는 메모리가 할당되지 않는다.
        객체를 생성했다 하더라도, static 멤버는 메모리 할당을 위해 클래스 외부에서 정의를 해야 한다.


    ㅡ static 멤버함수

        객체와 상관없이 클래스를 통해 호출할 수 있다. (클래스 함수라고도 한다.)

        메모리 할당을 위한 별도의 선언이 불필요하다.



*************** static member는 또한 static의 성질을 온전히 따른다.

                        static member의 scope가, 클래스가 아닌 전역이다.

                        쉽게 말해서,

                        scope      : global
                        namespace  : class

*********************** 이로 인해 scope가 class인 일반적인 멤버함수들과 다르게, this포인터를 갖지 않는다.

                        객체와 상관없이 클래스를 통해 호출할 수 있지만,
                        this포인터가 없기 때문에, 멤버변수를 사용하기 위한 멤버함수로서는 되려 반드시 해당 클래스 객체의 주소값을 인자로 받아야 한다.

                        // 해당 클래스 객체를 인자로 받지 않는다면, 일반적인 static 전역함수와 다른 점이 없으며, 오히려 namespace만 생기면서 불편해진다.
                        // static 전역함수가 해당 클래스의 멤버를 사용하기 위해 class 내부에 정의함으로써, static 멤버함수가 되는 것으로 이해해도 좋다.



<class : friend 키워드>

    friend로 선언된 대상에게, 필드를 무시하고 자신의 모든 멤버를 공개한다.

    ㅡ 장점 : 코드작성이 수월해진다.
    ㅡ 단점 : 캡슐 파괴의 주범이다.

*** 상속의 경우, friend 키워드가 무시된다.
*** 선언자와 대상과의 관계이며, 복수의 friend는 서로 호환되지 않고 개별적으로 적용된다.


    friend의 대상은 클래스, 멤버함수, 전역함수가 있다.
    접근 권한만 생기며, 주소값은 모른다. (현관 비밀번호만 알고, 집주소는 모른다.)
    이너클래스는 외부클래스에 속해있기 때문에 주소값 또한 알고 있으므로, 예외이다.

    ㅡ 해결 방법
        1. 인자로 객체의 주소값을 넘겨준다.
        2. 선언클래스의 멤버를 static 처리하여, 객체없이 클래스만으로 사용할 수 있게 한다.



                            아래는 클래스와 멤버함수를 대상으로 한 friend 선언의 예시이며,
                            왼쪽은 해결 방법 1번이고 오른쪽은 해결 방법 2번이다.
                            ( 대상:클래스  ==  모든 멤버함수,        대상:멤버함수  ==  단 하나의 멤버함수 )



                            class A;                                                            class A;


                            class C                                                             class C
                            {                                                                   {
                            public:                                                             public:
                                void TestC_1(A& CLASS);                                             void TestC_1();
                                void TestC_2(A& CLASS);                                             void TestC_2();
                            };                                                                  };


                            class B                                                             class B
                            {                                                                   {
                            public:                                                             public:
                                void TestB(A& CLASS);                                               void TestB();
                            };                                                                  };


                            class A                                                             class A
                            {                                                                   {
                                friend class B;                                                     friend class B;
                                friend void C::TestC_1(A& CLASS);                                   friend void C::TestC_1();

                            private:                                                            private:
                                int m_i;                                                            static int m_i;
                                void TestA() {}                                                     static void TestA() {}
                            };                                                                  };
                                                                                                int A::m_i;




                            inline void C::TestC_1(A& CLASS)                                    inline void C::TestC_1()
                            {                                                                   {
                                CLASS.m_i = 30;                                                     A::m_i = 30;
                                CLASS.TestA();                                                      A::TestA();
                            }                                                                   }



                            inline void C::TestC_2(A& CLASS)                                    inline void C::TestC_2()
                            {                                                                   {
                                CLASS.m_i = 30;              // 접근 불가                           A::m_i = 30;               // 접근 불가
                                CLASS.TestA();               // 접근 불가                           A::TestA();                // 접근 불가
                            }                                                                   }


                            inline void B::TestB(A& CLASS)                                      inline void B::TestB()
                            {                                                                   {
                                CLASS.m_i = 30;                                                     A::m_i = 30;
                                CLASS.TestA();                                                      A::TestA();
                            }                                                                   }






                            아래는 전역함수를 대상으로 한 friend 선언의 예시이다.
                            왼쪽은 해결 방법 1번이고 오른쪽은 해결 방법 2번이다.



                            class A                                                             class A
                            {                                                                   {
                            private:                                                            private:
                                int m_i;                                                            static int m_i;

                            public:                                                             public:
                                friend void Test_GlobalFunction(A& CLASS);                          friend void Test_GlobalFunction();
                            };                                                                  };
                                                                                                int A::m_i = 0;


                            void Test_GlobalFunction(A& CLASS)                                  void Test_GlobalFunction()
                            {                                                                   {
                                CLASS.m_i = 30;                                                     A::m_i = 30;
                            }                                                                   }



                            int main()                                                          int main()
                            {                                                                   {
                                A TestA;

                                Test_GlobalFunction(TestA);                                         Test_GlobalFunction();

                                return 0;                                                           return 0;
                            }                                                                   }





    ㅡ 아래의 글은 friend 키워드에 대한 적절한 설명 예시이다.

                            friend는 접근을 허용하기 위해 쓰는 것이 아니라 접근을 제한하기 위해 쓰면 의미가 명확해집니다
                            그게 그건데 싶지만 약간 다릅니다
                            예를들어 ui라이브러리를 만든다치고
                            대부분의 컴포넌트는 widget을 상속 받을 겁니다
                            어떤 자식class가 widget의 private를 억세스하기 위해 friend를 써야한다면 또는 쓸 수 있다면 잘못 설계된겁니다
                            맥락을 달리해서 uiconfig라는 모델/스토어성 class가 있다고 치죠.
                            라이브러리가 제공하는 모든 widget의 private코드에서 해당 오브젝트의 변수를 억세스해서 씁니다
                            하지만 widget을 상속해서 개발하는 개발자는 해당 private에 억세스 못하므로 있는 걸 알아도 쓰지를 못합니다
                            그렇다고 protected로 열어주면 변수가 컨텍스트에 관계없이 공개될 수 있어서 잘못사용하면 디버깅에 심각한 문제가 생기죠
                            그래서 굳이 uiconfig에 억세스하고 싶은 사람을 위해 uiconfigadapter를 제공하면 됩니다.
                            하지만 uiconfig가 public class라면 어떤 개발자는 adapter를 쓰지않고 uiconfig를 바로 상속 받아서 사용하려고 할 수 있습니다.
                            uiconfig를 private class로 만들고 adapter에 friend를 주면 uiconfig는 adapter에서만 접근 가능합니다.
                            개발자가 접근하는 창구를 단일화해서 라이브러리가 의도치 않은 방식으로 사용하는 걸 방지할 수 있고,
                            디버깅 시에도 검색해야할 컨텍스트를 매우 줄일 수 있습니다.
                            출처: https://kldp.org/node/162797



< :: 범위 확인 연산자 > (scope overator)

    정보 : https://docs.microsoft.com/ko-kr/cpp/cpp/scope-resolution-operator?view=msvc-170

    현재 C++ 표준에서 namespace의 이름은, 이미 선언된 이름과 겹칠 수 없다.

    using (namespace) 사용으로 인하여 객체의 이름이 겹칠 경우,
    global symbol은 global scope operator로 구분할 수 있다.

                        e.g.

                        int i;                              - [가]

                        namespace A
                        {
                            int i;                          - [나]
                        }

                        namespace NamespaceE {
                            class ClassE {
                            public:
                                class ClassE1 {
                                public:
                                    int x;                  - [다]
                                };
                            };
                        }

                        int main()
                        {
                            NamespaceE::ClassE::ClassE1 e1;
                            e1.x = 7;                       - [다]

                            {
                                using namespace A;

                                i = 10;                     ERROR : 객체가 모호합니다. : [가] or [나]

                                A::i = 50;                  - [나]

                                ::i = 30;                   - [가] - global scope operator
                            }
                        }



<접근 제어자> (access modifiers)         ==        <접근 지정자> (access specifiers)          ==      접근 제한자, 접근 제한 지정자 등.

    객체지향의 중요한 특징인 정보 은닉과 캡슐화를 지원한다.
    구조체 또는 클래스의 멤버에 대한 접근 레벨을 결정한다.
    // (구조체와 클래스)의 (멤버변수나 멤버함수)에 대한 내외부에서의 사용 가능 범위를, 접근 지정자에 따라서 제한할 수 있다.
    // 프라이뱃 필드, 퍼블릭 필드, 프로텍티드 필드라고 말하기도 한다.

    1. private     :  동일한 클래스 멤버 함수만 접근 가능. (멤버변수가 private이더라도, 멤버함수를 통한 접근과 변경이 가능하다.)

    2. protected   :  자신과, 자신을 상속받은 클래스만 복사 가능. (이후의 <상속> 에서 다룬다.)

    3. public      :  외부 함수도 접근 가능.


    // C와 C++ 모두 main파일에서 함수를 가릴 때는, 포함하는 헤더에서 정의부분을 주석처리하면 되었지만,
    // C언어의 구조체 멤버는 모두 공개되어 있다.

    // C++에서 접근 지정자가 생겼으며,
    // 기본적으로 구조체의 멤버변수는 public 이고, 클래스의 멤버변수와 멤버함수는 private 이다.



<'this' pointer> ('this'포인터)

******************************* ERROR (E0258)  :  this포인터는, !비정적! 멤버 함수 내에서만 사용할 수 있습니다.
******************************* ERROR (E0245)  :  비정적 멤버 참조는, 특정 개체에 상대적이어야 합니다.
                                                  (this포인터가 가리킬 객체가 있어야 하기 때문이다.)

    'this' pointer  :  해당 클래스의 멤버함수를 호출한 객체의 주소이다.
                       생성자의 경우, this가 임시객체로 생성되고, 이니셜라이징을 거쳐 반환된다.

                       컴파일러에 의해 멤버함수 내에서 매개변수와 함께 생략되어 있다.
                       모든 멤버함수에 정의되어 있으며, 디버깅에서 this 변수를 확인할 수 있다.



            ㅡ 아래의 코드들은, 멤버함수와 this포인터가 적용되는 과정이다. (접근 제어자는 별개로 두었다.)
               또한, C++ 문법에 모두 어긋나지 않는다.

                1. 멤버함수와 this포인터가 없는 경우.
                    구조체와 동일한 형태이다.
                                class CMy
                                {
                                public:
                                    int m_i;
                                };

                                void SetInt(CMy* _this, int i)
                                {
                                    _this->m_i = i;
                                }

                                int main()
                                {
                                    CMy Test1;
                                    SetInt(&Test1, 100);

                                    return 0;
                                }

                2. 1.의 코드에 멤버함수를 적용시킨다.
                    private 멤버변수에 접근할 수 있다.
                                class CMy
                                {
                                private:
                                    int m_i;

                                public:
                                    void SetInt(CMy* _this, int i)
                                    {
                                        _this->m_i = i;
                                    }
                                };

                                int main()
                                {
                                    CMy Test1;
                                    Test1.SetInt(&Test1, 100);

                                    return 0;
                                }

                3. 2.의 코드에 'this'포인터 문법을 적용시킨다.
                                class CMy
                                {
                                private:
                                    int m_i;
                                    
                                public:
                                    void SetInt(int i)
                                    {
                                        m_i = i;
                                    }
                                };

                                int main()
                                {
                                    CMy Test1;
                                    Test1.SetInt(100);

                                    return 0;
                                }

                **  :  C++에서 클래스의 멤버는, 해당 클래스의 객체를 통해서만 호출이 가능할 뿐이다.
                ??  :  "클래스의 멤버 함수를 호출할 때 [C++]는 어떻게 호출할 객체(인스턴스)를 찾는가?"
                !!  :  'this'라는 숨겨진 포인터를 사용한다.



<생성자> (constructor)

*** 생성자는, 임시객체를 생성하고, 이니셜라이징을 거친 후, 임시객체를 반환한다.

    개체가 생성될 때, 컴파일러에 의해 자동으로 실행된다.
    접근 레벨에 관계없이 멤버변수의 이니셜라이저 역할을 한다.

    Class의 멤버함수이며, 함수 이름은 클래스의 Name과 같다.
    매개변수가 없다.
    반환하지 않는다.
    이 역시 함수이므로, 오버로딩이 가능하다.

                            e.g.
                            Class CMy
                            {
                                int m_i1;
                                int m_i2;
                                float m_f1;
                                float m_f2;
                                float m_f3;

                            public:
                                CMy()                   //           constructor
                                    : m_i1(100)         //  1.
                                    , m_i2(200)
                                    , m_f1(10.3f)
                                {
                                    m_f2 = 10.4f;       //  2.
                                    m_f3 = 10.5f;
                                }
                            }

    클래스 생성자의 문법이다.
    1. 2. 두 가지 방법이 있으며, 중괄호는 필수이다.

    1.    : 이니셜라이저 리스트.
                클래스의 멤버변수 이니셜라이징만 가능하다.
                아니라면 중괄호에 적으면 된다.



<소멸자> (destructor)

    개체가 범위를 벗어나거나 호출에 의해 명시적으로 제거될 때, 자동으로 호출되며, 메모리를 정리(청소)한다.
    // 생성자에서 할당한 리소스의 소멸에 사용된다.

    호출 조건, 소멸 순서 등 소멸자에 관한 정보 :
    https://docs.microsoft.com/ko-kr/cpp/cpp/destructors-cpp?view=msvc-170

    Class의 멤버함수이며, 함수 이름은 클래스의 ~Name이다.
    매개변수가 없다.
    반환하지 않는다.
    이 역시 함수이므로, 오버로딩이 가능하다.             // 소멸자는 오버로딩이 불가능하다.

                            e.g.
                            Class CMy
                            {
                                int m_i1;
                                int m_i2;
                                float m_f1;
                                float m_f2;
                                float m_f3;

                            public:
                                CMy()                   //            constructor
                                    : m_i1(100)         //  1.
                                    , m_i2(200)
                                    , m_f1(10.3f)
                                {
                                    m_f2 = 10.4f;       //  2.
                                    m_f3 = 10.5f;
                                }

                                ~CMy()                  //            destructor
                                {

                                }
                            }



    ㅡ 생성자와 소멸자의 기능

            C++에서 반드시 실행되는 함수들이므로, 이를 이용해서
            멤버 변수의 이니셜라이징, heap 메모리 할당, 해제 등의 다양한 명령어들을 추가할 수 있다.



    ㅡ 직접 호출

        기본은, 객체를 통해서 호출하는 것이다.

            e.g.
            CMy Test1;

            Test1.CMy::CMy();           // constructor

            Test1.CMy::~CMy();          // destructor
         == Test1.~CMy();


            1. 생성자 : 객체를 통해 호출한 멤버함수더라도, 멤버함수 내외부 어디서 쓰이든 간에 객체를 필요로 한다.

                                1. className::className();           :  이니셜라이징 된 멤버를 받는 객체가 없으므로, 의미가 없다.

                                2. Object.className::className();    :  객체를 대상으로 생성자가 호출된다.
                               ( == this->className::className(); )


            2. 소멸자 : 객체를 통해 호출한 멤버함수 내에서는, 객체를 필요로 하지 않는다.
                        외부에서는 당연히 필요로 한다.



    ㅡ 생성자 활용

        1.  생성자를 반환할 경우, 해당 생성자 인스턴스를 거친 임시객체가 생성되어 반환된다.
            (생성자가 아닌 멤버함수를 반환하면 안되며, 생성자를 포함한 멤버함수도 안된다.)

                            class A
                            {
                            private:
                                int i;

                            public:
                                A returnA()
                                {
                                    return A(10);
                                }

                                A(int _i)
                                    : i(_i)
                                {
                                }
                            };

                            int main()
                            {
                                A TestA = TestA.returnA();      // TestA.i = 10

                                return 0;
                            }


        2.  이니셜라이징은, 해당 생성자 인스턴스만을 사용한다.
            대입연산은, 해당 대입연산자 인스턴스만을 사용한다.
            그런데 아래의 코드는, 객체의 대입연산이 아닌 이니셜라이저가 실행되었다. ( <이니셜라이저>에서 자세히 설명하겠다. )

            우선, 생성자를 거쳐도 초기화가 되지 않은 멤버변수는 쓰레기 값을 가진다.

                            class A
                            {
                            private:
                                int i;
                                int j;

                            public:
                                A()
                                    : i(0)
                                    , j(0)
                                {
                                }

                                A(int _i)
                                    : i(_i)
                                {
                                }

                                A(int _i, int _j)
                                    : i(_i)
                                    , j(_j)
                                {
                                }


                            };

                            int main()
                            {
                                A TestA1(10, 20);
                                TestA1 = { 10, 20 };        // {i=10 j=20 }

                                TestA1 = 10;                // {i=100 j=-422174759 }

                                return 0;
                            }



<기본 생성자, 기본 소멸자> (default constructor, default destructor)
    생성자와 소멸자는 C++ 문법, 규칙상 반드시 존재해야 하므로, 작성하지 않아도 컴파일러가 자동으로 생성한다.
    이렇게 생성된 생성자와 소멸자를 default constructor(기본 생성자), default destructor(기본 소멸자)라고 한다.
    다만, 이렇게 만들어진 둘은 구조상의 필요로 생성될 뿐이므로,
    정말로 아무 기능이 없다.

*************** 명시적으로, 복사 생성자도 포함한 생성자 인스턴스가 하나라도 존재하면,
                컴파일러는 디폴트 생성자를 생성하지 않는다.
                소멸자 또한 마찬가지다.

                    사용자의 의도에 따라 디폴트 복사 생성자가 사용되선 안되는 상황이 있기 때문이다.
                    (디폴트 복사 생성자가 존재하면 충돌이 일어나는 상황을 말한다.)



<이니셜라이저> (initializer)

    초기값을 넣어주는 초기화와 다르며, 문법적인 초기화를 말한다.

    초기값 대입   :  객체 생성 후 초기값을 대입하는 두 단계로 실행된다.
                    여기서의 초기값 대입은, 첫 대입일 뿐이다.

    이니셜라이저  :  객체 생성과 동시에 초기값 대입이 실행된다.
                    const  private  reference 등의 객체는 이니셜라이저가 필요하다.

    e.g.
    [C]
    int i = 10;
    int iArr = { 1, 2, };

    [C++]
    int i(10);        ==        int i{10};
    int iArr{ 1, 2, };



    C언어에서는 대입 연산자(=)가 이니셜라이저의 역할을 했지만,
    C++에서는 객체에 대한 이니셜라이저 문법이 추가로 생겼다.

        변수 . 단일멤버변수     : 소괄호 ()
        배열 . 복수멤버변수     : 중괄호 {}



    ㅡ 동적 할당과 관련해서.
        calloc 함수가 배열을 0으로 초기화해주는 반면,
        new에서는 위의 이니셜라이저 문법에 따라 원하는 값으로 초기화가 가능하다.



******* 객체의 데이터가 변경되려면, 이니셜라이저 또는 대입연산자를 거쳐야 한다.

        ㅡ 이니셜라이징     : 해당 생성자 인스턴스를 사용한다.
        ㅡ 대입연산         : 해당 대입연산자 인스턴스를 사용한다.



        클래스 객체의 대입연산  ->  해당 대입연산자 인스턴스가 존재하니??  ->  해당 대입연산자 인스턴스만 실행된다. (end)
        CTest TEST;
        TEST = 10;                              ||

                                               false                   ->  대입연산이 아닌 이니셜라이징으로 인식된다.  ->  해당 생성자 인스턴스가 존재하니??  ->      [ㄱ] (end)

                                                                                                                                   ||

                                                                                                                                  false                 ->     ERROR (end)

                [ㄱ]   : 해당 생성자 인스턴스를 실행한다. 그런데,
                         포인터가 아닌 레퍼런스 또는 일반으로 해당 클래스형의 객체를 인자로 받는 대입연산자 인스턴스가 존재한다면,
                         해당 생성자 인스턴스를 실행한 후, 대입연산자 인스턴스가 실행된다.
                         (문제가 생긴다. : 둘 다 실행되는 것도 모자라, 대입연산자 오버로드의 아예 다른 인스턴스가 실행되었다는 것이다.)

                         따라서, 인스턴스를 명시적으로 정의해주자.



<구조체> (structures)

    사용자 정의 자료형.(user-defined dataType)

    1. C언어의 문법을 따르며, C언어와 동일하게 사용할 수 있다.

    2. C++에서는 _tagName 자체가 하나의 dataType으로 인정된다.
       따라서 struct _tagName 의 struct 키워드를,
       typedef 를 사용하여 생략하고 별칭을 대신 쓰는 것은 하지 않아도 된다.



<레퍼런스 변수 ( & ), 참조자, 참조형 변수> (reference variable)

    C++에 주소 참조 기능의 레퍼런스가 추가되었다.

    참조되는 일반 변수와 레퍼런스 변수는 동일시된다.
    Rvalue에 대한 참조는 당연히 포인터, 레퍼런스 모두 불가능하다.

*** 레퍼런스의 목적
        1. 주소 참조
        2. 복사 비용 감소

            1번과 관계없을 때, 2번을 생각하자.
            주소 참조로 return하면 안되는 지역변수의 경우, 레퍼런스를 사용하면 안된다.
            스택 메모리에 저장되는 지역변수는, 함수가 종료되어 해제되기 때문이다.


    또한 레퍼런스 변수는, 오버로딩을 통한 참조 대상의 별칭 . 별명이기도 하다.





    ㅡ 포인터 변수와 레퍼런스 변수의 문법과 기능에 따른
                                                    공통점 - 참조 기능. 복사 비용에 대한 부담이 적다.

                                                    차이점 - 1. 레퍼런스 변수는 참조 대상 변경 불가.
                                                                    e.g.
                                                                    int i = 100;
                                                                    int* const p_i = &a;    ==    int& Ref_i = a;

                                                            2. 레퍼런스 변수는 주소값 연산 불가.

                                                            3. 포인터 변수는 역참조 연산자를 매번 사용해야 하지만,
                                                               레퍼런스는 이에 비해 아주 편하다.

    포인터 변수는 주소를 가져온다고 말하고,
    레퍼런스 변수는 참조한다고 말한다.
    실제로 두 변수를 컴파일하고 디스어셈블리에서 확인하면, 똑같다.



<const 한정자>

    정확한 문법은 언어와 컴파일러마다 다르다.

    공통적으로,
    const 객체는 Rvalue가 된다.
    객체를 직접 변경하는 것이 막힌다.



<강제 타입캐스팅과 const 한정자>

    e.g.
    const int i = 100;              // C++에서 포인터 대신 레퍼런스를 사용해도 결과는 같다.
    const int* p = &i;
    *(int*)p = 200;   ----[ㄱ]
    int b = i;

    (debugging)
        C       :       b = 200,   i = 200
        C++     :       b = 100,   i = 200

    우선 V.S.의 C++컴파일러는 피연산자로 i를 쓰면,
    const의 의미를 따라 초기값 그대로 나온다.



    ㅡ 타입캐스팅 또는 우회를 통한 const 무시에 대해서.

        1.  [C]에서는 마음만 먹으면 아주 쉽게 변경 가능하다.

        2.  const 동작 원리에 대하여 문장 [ㄱ]의 의미가 무엇인지, 알려하지 말고
            const 단어의 개념과 역할까지 선을 그을 것.



<멤버함수와 const 한정자>

    멤버함수에서 const의 위치에 따라 대상이 다르다.

        e.g.
        const CMy& operator = (const CMy& _other) const
        {
            m_i = _other.m_i;           // 1번에 의해 ERROR
            m_f = _other.m_f;           // 1번에 의해 ERROR
            return *this;
        }

        1. 함수명 뒤의 const         - 멤버 변수(this) - 객체의 상태, 멤버변수를 수정하지 않는다.

        2. 매개변수의 const          - 매개 변수 (일반변수의 const와 동일한 개념이다.)

        3. 반환타입 앞의 const       - 반환값@@@@@@@@@@@@@ (반환값은 어차피 Rvalue인데, const가 된다는 의미가 무엇인지??)



<연산자 오버로딩> (operator overload, operator overloading, overloading operator)

    클래스의 멤버함수를 통해 구현할 수 있다.

    C/C++에서 기본 자료형이 아닌 사용자 정의 자료형인 구조체형과 클래스형에 대해서는
    디폴트 복사 생성자와 디폴트 대입 연산자 뿐이며,
    C++에서 구조체형과 클래스형에 대한 나머지는 직접 구현해야 한다.

    함수의 형식과 비슷하다.

    참고 : [C++] 지식 - 연산자 오버로딩에 관하여 필요한 지식.

*** 함수의 return 반환값은 레지스터에 저장된 후, 스택 메모리의 함수가 해제된다.
    모든 함수의 반환값은 Rvalue이고, 값을 변경할 수 있다 한들 의미가 없다.
    반환타입을 volatile로 쓴다고 Lvalue가 되는 것도 아니다.
    애초에 함수의 반환 과정에 의해 반환값이 레지스터에 저장되는 것이어서,
    함수 형식(함수 반환타입)에 volatile은 더욱 의미가 없다.

    C++ 오버로딩이 가능한 연산자(다시 정의할 수 있는 연산자) + 연산자 오버로딩에 관한 정보.
    https://docs.microsoft.com/ko-kr/cpp/cpp/operator-overloading?view=msvc-170

    각각의 연산자 오버로드 문법이 있으나, 대체로 비슷하다.



    ㅡ 전위 . 후위 연산자 오버로드

                    Type& operator ++ or -- ()
                    {
                        statements;
                        return *this;
                    }
                    Type operator ++ or -- (int)
                    {
                        statements;
                        return prev;
                    }

                    호출 시점은 변경할 수 없다.
                    이처럼 후위연산자처럼 사용되도록 구현해야 하며, 멤버함수 내에서 어떻게든 해결해야 한다.



<new, delete> (dynamic memory allocator in C++, new/delete operator in C++)

    참고 : [C++] 지식 - new/delete VS malloc/free

    new와 delete 연산자는, C++의 기본 연산자이다.
*** 오버로딩이 가능하다.
    참고 : https://nomad-programmer.tistory.com/362

    현재 C++에서는 메모리가 부족한 경우 예외를 return nullptr 대신 std::bad_alloc throw 한다. (throw_bad_alloc.cpp)
    참고 : https://docs.microsoft.com/ko-kr/cpp/cpp/new-and-delete-operators?view=msvc-170#allocation-failure-behavior



    Syntax.                                                     변수의 괄호, 배열의 중괄호는 이니셜라이저이며,
        1. allocation                                           대입 연산자가 아닌 객체 생성 시의 이니셜라이저로만 사용할 수 있다.

            MYTYPE* ptr = new MYTYPE;                               ==  MYTYPE* ptr{ new MYTYPE };

            delete ptr;      ==      delete(ptr);      ==      delete    (ptr);



        2. allocation + initialization

            MYTYPE* ptr = new MYTYPE(InitData);                     ==  MYTYPE* ptr{ new MYTYPE(InitData) };

            delete ptr;



        3. allocation(Array)

            MYTYPE* ptr = new MYTYPE[Count];                        ==  MYTYPE* ptr{ new MYTYPE[Count] };

            delete[] ptr;



        4. allocation(Array) + initialization

            MYTYPE* ptr = new MYTYPE[Count]  {InitArray};           == MYTYPE* ptr{ new MYTYPE[Count]  {InitArray} };

            delete[] ptr;



        이니셜라이저가 가능하려면, 해당 타입에 맞는 단일, 배열 생성자가 있어야 한다.
        built-in type의 경우, 함께 내장되어있는 디폴트 생성자가 있지만,
        user-defined type의 경우 이니셜라이저 문법은,
        인수 목록이 일치하는 해당 타입 생성자의 인스턴스가 있어야 한다.




<상속> (inheritance) - 1/2

    ㅡ 상속의 목적

        기존의 클래스를 재활용하는 것이 기본이지만, 상속에 의해 부차적으로 발생하는 효과도 있다.

        1.  기존의 클래스를 재활용한다.
        2.  공통되는 부분을 상위 클래스에 통합하여 반복을 제거하고 유지, 보수를 편리하게 한다.
        3.  공동의 조상을 가지는 계층을 만듬으로써, 객체의 집합에 다형성을 부여한다.



    ㅡ 상속 : 이미 정의되어 있는 클래스의 모든 특성을 물려받아 새로운 클래스를 정의하는 기법. (friend 키워드를 무시한다.)
                (모든 특성에는 생성자와 소멸자를 포함하며, 이를 실행 시에 기반클래스부터 실행된다.)
                (기반 클래스와 파생 클래스의 멤버는 주소값이 다른 별개의 멤버이다.)

        (상속 해주는 클래스는 상위 기초(base), 슈퍼(super), 부모(parent), 기반 클래스라고 부른다.)
        (상속 받는 클래스는   하위 유도(derived), 서브(sub), 자식(child), 파생 클래스라고 부른다.)


        1.  일반적으로 private 필드의 멤버변수에 대해 public 필드의 멤버함수로 접근하지만, 어쨋든 멤버변수와 멤버함수와의 관계가 있다.

        2.  모든 멤버에 대한 3가지 필드, 파생클래스로의 상속에 대한 3가지 필드가 있다.

        3.  상속에 대한 개념들은 모든 멤버에게 적용될 수 있다.

        1, 2, 3이 모두 엮이므로, 캡슐의 구조가 배로 복잡해질 수 있다.
        설계하고 클래스를 작성하되, 이후에 문제가 있을 경우 구조적으로 접근하자.






    ㅡ 상속 접근 지정자 (다양한 정의가 있으나, 가장 이해하기 좋았던 것으로 썼다.)

        기반클래스의 멤버 필드 3가지와 파생클래스로의 상속형태(상속필드) 3가지에 따라서
        기반클래스의 멤버를 파생클래스가 물려받는다.

            public      :  접근레벨 public 이하를, public으로 받는다.

            protected   :  접근레벨 protected 이하를, protected로 받는다.

            private     :  접근레벨 private 이하를, private으로 받는다.



                                                                        Syntax.

                                                                        struct A
                                                                        {
                                                                        };

                                                                        struct B : public A
                                                                        {
                                                                        };


    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    ㅡ 다형성 (polymorphism)

    ㅡ 가상함수 (virtual function)

    ㅡ 순수 가상함수 (pure virtual function)

    ㅡ 다중상속 (multiple inheritance)

    그리고,

    static member variable, singleton에 대해서도 정리하자.
    목적은, 클래스 멤버변수 공유 (상속이든 아니든 다른 클래스에서의 주소값 참조 가능하도록.)
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



<typename 키워드>

        템플릿 정의에서 정해지지 않은 식별자가 TYPE(형식)이라는 것을 컴파일러에게 알려준다.
        템플릿의 타입 매개변수로 정의된 타입을 사용하는 선언문을 정규화하는데 사용된다.
        // TYPE으로 식별되지 않거나 모호할 경우, typename 키워드를 사용하면 된다.
        // 가독성도 신경 써보자.



<템플릿>  (template)

    다형성의 오버로딩 특성에 의해 함수이름이 같을 수 있기 때문에 템플릿이 가능하다.

******************************* C++에서 template을 사용할 때는 일부 에러를 인텔리센스가 알려주지 않는다. 이너클래스일수록 심하며, 당연한 문법오류조차 뜨지 않는다.

    ㅡ 장점.

        1. 코드를 자료형마다 중복으로 작성하지 않아도 된다.

        2. 컴파일러가 미리 코드를 만들어 주기 때문에,
            런타임에서 돌리면 느린 함수들을 컴파일 시에 미리 호출해서
            최종 결과만 상수로 뽑아서 쓸 수가 있다.

    변수, 함수, 연산자 오버로딩, 구조체, 클래스 등 다양한 곳에 사용할 수 있다.



    e.g.
                                ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ <Test_template.h>
                                // 전역변수 템플릿            (선언과 동시에 정의)
                                template<typename T>
                                T a;



                                // 일반함수 템플릿            (선언과 정의)
                                template<typename T>
                                T Add(T a, T b);

                                template<typename T>
                                T Add(T a, T b)
                                {
                                    return a + b;
                                }



                                // 클래스와 멤버함수 템플릿    (선언과 정의)
                                template<typename T>
                                class CMy
                                {
                                public:
                                    T a;
                                    T b;

                                    T Add();
                                    T Sub();
                                };

                                template<typename T>
                                T CMy<T>::Add()
                                {
                                    return a + b;
                                }

                                template<typename T>
                                T CMy<T>::Sub()
                                {
                                    if (a > b)
                                        return a - b;
                                    else
                                        return b - a;
                                }



                                // plural typename
                                template<typename T, typename O, typename P>
                                class A
                                {
                                private:
                                    T m_i1;
                                    O m_i2;
                                    P m_i3;
                                };



                                ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ <main.cpp>
                                #include "Test_template.h"
                                int main()
                                {
                                    // 변수 템플릿
                                    a<int>;
                                    a<int> = 30;



                                    // 일반함수 템플릿
                                    int f = Add<int>(10, 20);           // == Add(10, 20);



                                    // 클래스와 멤버함수 템플릿
                                    CMy<int> Test1;

                                    Test1 = { 10, 20 };
                                    int add = Test1.Add();
                                    int sub = Test1.Sub();



                                    // plural typename
                                    A<int, float, long> Test1;

                                    return 0;
                                }



                                ㅡ T는 치환자료형(generic type)이며, template의 앞글자이다.

                                ㅡ 함수이름 오른쪽에 화살괄호로 해당 템플릿의 모든 치환자료형을 명시해야 한다.
                                   템플릿 정의부분 매개변수에 모든 치환자료형이 명시되어 있다면, 화살괄호를 생략할 수 있다.
                                   현업을 모르니, 가독성을 이유로 명시해주자.

                                ㅡ typename 대신 class를 사용해도 된다.
                                   초창기에 쓰인 class는, 클래스의 선언에 사용되는 키워드와 동일하다는 지적에 의해, typename 키워드가 추가되었다.



************    함수 템플릿과 함수는 다르다.
                일반 함수의 정의처럼, 우리가 직접 사용하게 될 함수(인스턴스)를 템플릿이 생성한다

                타입이 명시된 템플릿이 확인되면, link-time이 아니라 compile-time에 해당 타입의 인스턴스가 생성이 되고,
                해당 타입과 일치하는 인스턴스가 없다면, 일반 함수와 마찬가지로 정의가 없으니 당연히 link-error가 뜬다.



                인스턴스 생성 조건

                    1. 템플릿의 정의와 사용처가, 같은 소스파일에 있어야 한다.
                       (템플릿의 정의와 사용처가 각각 다른 소스파일에 있다면, 인스턴스가 생성되지 않는다.)
                       (이는 일반 함수의 선언만 있고, 정의가 없는 것과 같다.)



                해결 방법

                    1. 사용처의 소스파일에 템플릿의 정의를 둔다.

                    2. 정해진 타입의 명시적 선언을 통해 인스턴스를 생성해두면, link-time에서 확인된다.
                       (이는 추천하지 않는다.)






























<#pragma once, include guard(preprocessor)>

    이들은 [C], [C++]에서 헤더 파일의 중복 포함을 방지하는 데 사용되는 기법이다.


    #pragma once

        전처리기 지시어로, 해당 헤더 파일을 컴파일 세션 동안 단 한 번만 포함하도록 지시한다.

        컴파일러 최적화에 따라 다르지만, 인클루드 가드보다 빠른 컴파일 시간을 제공할 수 있다. (대형 프로젝트에서 몇 시간에 걸친 컴파일 시간이 고작 1초 단축된다 생각하면 된다.)

        비표준 지시어로, 모든 컴파일러에서 지원하지 않는다. (하지만 대부분의 현대 컴파일러에서 지원한다.)

        파일 시스템에 의존적일 수 있어, 매우 드물게 파일 시스템이 유니크 식별자 생성에 문제를 일으킬 수 있다.


    include guard

        모든 C/C++ 표준 컴파일러에서 지원한다.

        파일 시스템에 독립적으로 작동한다.

        매크로 이름 충돌을 고려해야 한다.

                #ifndef MODULE_NAME_H // 일반적으로 매크로 이름은 헤더 파일이름을 모두 대문자로 하여 사용한다. // e.g. add.h -> ADD_H, wifiCommunication.h -> WIFI_COMMUNICATION_H
                #define MODULE_NAME_H

                // code

                #endif // MODULE_NAME_H


!!! 전처리기(preprocessor)도 코드의 일부이다.

        '#include' 지시어는 해당 파일(일반적으로 헤더파일)의 내용을 현재 소스 파일에 직접 삽입한다. (복붙)
        헤더파일은 소스파일 코드의 일부가 되고, 소스파일들은 독립적으로 컴파일된다.
        '#pragma once', 'include guard'는 헤더 파일의 중복 포함을 방지하기 위해 사용한다고 했으나,
        이들은 전처리기이며 코드의 일부이므로 헤더파일에 있는 선언부가 아닌 정의부 코드는 여전히 중복 정의 문제가 발생한다.
        // 그러나 구조체, 클래스, inline 함수, template 등 생각보다 중복 정의에 제외되는 것이 많으므로, separate compilation에서 문제되지 않는다.
        // 단일 소스파일에 대해서는 완벽히 중복 정의 문제를 해결한다.





<inline과 separate compilation(분할 컴파일)>

    inline  :  compile-time에 최적화를 통해 함수 호출을 제거하고 함수 코드를 직접 삽입하여 run-time 성능을 향상시킨다.
                link-time에 함수 본문은 없다.
    ->
        inline 함수는 링크 시점에 함수 본문이 사라지며 분할 컴파일에서 중복 정의가 일어날 수 없다.
        // 일반적으로 선언부만 두는 헤더파일에 inline 함수를 정의할 수 있음을 의미한다.

!!! 분할 컴파일에서, 컴파일 시점에 함수 원형을 헤더파일에서 확인하고 링크 시점에 함수 정의를 소스파일에서 확인해야 하는데,
    inline 함수는 링크 시점에 이미 본문이 없으므로, 모듈화에 있어 소스파일에 정의할 수 없고 반드시 헤더파일에 정의해야 한다.


    function pointer  :  함수의 주소를 필요로 하며, 동시에 inline을 적용할 수 없다.
    ->
        컴파일러는 함수 포인터에게 함수의 주소를 제공하기 위해 별도의 인스턴스(실제 함수 본문, inline 함수의 out-of-line copy(복사본))를 생성한다.


    함수의 inlining은 컴파일러 마음이다. 적용하지 않겠다고 한다면, 일반 함수로 처리하여 중복 정의 등의 문제가 발생할 수 있는가?
    ->
        사용자는 신경쓰지 않아도 된다.

        컴파일러가 함수를 inlining하지 않는다고 해서 함수가 일반 함수처럼 사용되는 것이 아니며,
        언어 규칙과 컴파일러의 링킹 방식에 기반하여 특별한 처리 과정을 거친다.
        ->
            inline 함수는 각 번역 단위(소스파일)에서 로컬 링크(외부 링크가 아닌 내부 링크)로 처리되어 각 번역 단위에서만 존재한다.
            // 각 소스파일은 자신의 로컬 버전의 함수를 사용한다.
            따라서 언어 규칙 - ODR(one definition rule)에 위배되지 않는다.


































<type conversion(형변환)>

    1. explicit type conversion(명시적 형변환) (= type-casting)
    2. implicit type conversion(암시적 형변환) (= type-coercion, automatic type conversion(자동 형변환))



<explicit 키워드>

    class의 constructor(생성자), conversion operator(변환 연산자)를 통한 type-coercion을 금지하며, type-casting만 허용한다.
    ->
            explicit conversion to pnType operator
            ->
            pn = un; (X)
            pn = static_cast<pn>(un); (O)


            explicit pnType constructor
            ->
            un = pn; (X)
            un = static_cast<un>(pn); (O)



<conversion operator(변환 연산자)>

    객체가 다른 타입으로 변환될 때 호출되는 멤버 함수이다.
    변환하고자 하는 타입과 동일한 변환 연산자가 필요하다.

            class IntAndFloat
            {
            private:
                int intField;
                float floatField;

            public:
                IntAndFloat(int _int, float _float)
                    : intField(_int)
                    , floatField(_float)
                {
                }

                // return-type은 함수 이름에 포함되어 있으므로 표기하지 않는다. // const는 필수가 아니다.
                explicit operator int() const { return intField; }
                explicit operator float() const { return floatField; }
            };

            int main()
            {
                int pnInt;                      // primitive number
                float pnFloat;
                IntAndFloat un(10, 3.14f);      // user-defined number

                // 여기서 컴파일러는 un의 변환 연산자 un.operator int(), float()를 찾는다.
                pnInt = static_cast<int>(un);       // = 10
                pnFloat = un;                       // error: explicit으로 인한 자동 변환 금지.

                // explicit이 아닌 기본 타입 변환 연산자가 있다면, 모든 기본 타입이 해당 타입으로의 type-coercion이 일어날 수 있으므로 주의한다.
            }



<conversion constructor(변환 생성자)>

    특정 타입의 값을 받아 새로운 클래스 인스턴스를 생성하는, 매개변수가 하나인 생성자이다.

    // "un = pn;"  이처럼 클래스 인스턴스를 초기화하는 데 사용할 수 있다.
    // 기본 타입 pn이 클래스 타입 un으로 변환된다는 점에서 '변환' 생성자라 한다.



<예시>

    pn = un;
    ->
    우선 un의 변환 연산자 - un.operator pnType()이 필요하다. explicit 선언이면 type-casting하면 된다.

    반대로,
    un = pn;
    ->
    pn은 기본 타입이므로 pn의 변환 연산자 - pn.operator unType()을 만들어 넣을 순 없다. 여기서 컴파일러는 두 가지를 찾는다.

        1. 대입 연산자 un.operator=(pn)
        2. pn을 받아 새로운 un을 생성하여 복사할 수 있는지 확인한다. (매개변수가 pnType인 변환 생성자로 생성, 복사하고 기존의 객체는 소멸된다.)



            1. 대입 연산자 un.operator(pn)
            ->
                    class IntAndFloat
                    {
                    private:
                        int intField = 0;
                        float floatField = 0;

                    public:
                        IntAndFloat(int _int, float _float)
                            : intField(_int)
                            , floatField(_float)
                        {
                        }

                        // 모든 타입의 대입 연산자를 삭제한 후에 원하는 타입을 선택 정의하는 것으로, 매개변수에서의 type-coercion을 방지한다.
                        template<typename T>
                        IntAndFloat& operator=(const T&) = delete;

                        IntAndFloat& operator=(const int _int)
                        {
                            this->intField = _int;
                            return *this;
                        }
                        IntAndFloat& operator=(const float _float)
                        {
                            this->floatField = _float;
                            return *this;
                        }
                    };

                    int main()
                    {
                        IntAndFloat un(0, 0.f);          // user-defined number

                        un = 10;
                        un = 3.14f;
                        un = 3.14;              // error: "삭제된 함수입니다."
                    }



            2. pn을 받아 un을 생성할 수 있는지 확인한다.
            ->
                    // pn을 받아 un을 생성하고 이를 디폴트 대입한다.
                    // 변환 생성자를 통한 type-coercion이므로, 변환 생성자가 explicit이면 "un = static_cast<un>(pn);" 이렇게 명시변환만 가능하다.

                    class IntAndFloat
                    {
                    private:
                        int intField = 0;
                        float floatField = 0;

                    public:
                        explicit IntAndFloat(int _int, float _float)
                            : intField(_int)
                            , floatField(_float)
                        {
                        }

                        template<typename T>
                        IntAndFloat(const T& _T) = delete;

                        explicit IntAndFloat(int _int) : intField(_int) {}
                    };

                    int main()
                    {
                        IntAndFloat un(30, 3.14f);

                        un = static_cast<IntAndFloat>(30);      // 변환 생성자를 사용한다.
                        un = IntAndFloat(30);                   // 변환 생성자로서 이전에 해당 생성자를 직접 사용한다.

                        un = 30;                                // error: explicit constructor
                        un = IntAndFloat(3.14f);                // error: "삭제된 함수입니다."
                        un = 3.14f;                             // error: 둘 다.
                    }

    explicit은 객체의 type-coercion을 방지한다.
    매개변수로의 type-coercion을 막으려면, 다른 타입의 인스턴스들을 모두 삭제하고 선택 정의해야 한다.



































<function pointer(함수 포인터)>

!!! 함수 포인터를 사용할 때 함수의 주소가 필요하기 때문에 inline을 적용할 수 없다.

    일반적으로 하나의 포인터는 하나의 entity만 가리킨다.
    // 데이터와 포인터의 구분이 있고, 데이터 포인터(일반 포인터 변수)와 함수 포인터로의 구분은 없으며 타입캐스팅이 가능하다.

    포인터는 데이터를 가리킬 수 있고, 함수를 가리킬 수 있다.


    data pointer
    ->
    decl  :                         dataType* ptrName;       // 또는 dataType(*ptrName);
    typeCasting  :                  (dataType*)ptrName
    typeCasting and use data  :     *(dataType*)ptrName
    use data  :                     *ptrName


    function pointer
    ->
    decl  :                             returnType (*ptrName)(para1Type, para2Type, ...);
    typeCasting  :                      (returnType (*)(para1Type, para2Type, ...)) ptrName
    typeCasting and use function  :     ((returnType (*)(para1Type, para2Type, ...)) ptrName)(arg1, arg2, ...)
    use function  :                     ptrName(arg1, arg2, ...)



!!! 모든 객체는 배열로 선언할 수 있다. 데이터 변수, 데이터 포인터, 함수 포인터 등

        e.g.
        ->
        데이터 변수의 배열  :  가변 배열(dynamic array) 등

        데이터 포인터의 배열  :  allocTable, garbage collection, ...

        함수 포인터의 배열  :  인터럽트 번호 별로 해당되는 ISR 함수의 주소를 저장하는 interrupt vector table (AVR의 경우 vectorTable은 0번지)



    e.g. 반환 타입이 함수 포인터인 함수
    ->
            int add(int a, int b) { return a + b; }

            int (*getFuncPtr(void))(int, int)
            {
                return add;
            }

            int main(void)
            {
                int (*funcPtr)(int, int) = getFuncPtr();

                int a = funcPtr(10, 20);

                a = getFuncPtr()(20, 30);
            }





<member function pointer(멤버 함수 포인터)>

    decl:  returnType (ClassName::*pointerName)(parameterTypes);
    decl and init:  returnType (ClassName::*pointerName)(parameterTypes) = &ClassName::methodName;
    use:  (instance.*pointerName)(arguments);

    데이터 포인터, 함수 포인터와 다르게 멤버 함수 포인터는 오로지 특정 개체(특정 클래스의 인스턴스)에 바인딩되어야 한다.
    선언에는 특정 클래스에 바인딩해야 하며, 멤버 함수 사용을 위해서 인스턴스에 바인딩해야 한다.
    // 다른 클래스로의 type-casting은 불가능하다.


    멤버 함수는 해당 클래스에 종속이다.  ->  멤버 함수 포인터는 해당 클래스 타입에 엄격하게 결합된다.

    멤버 함수는 클래스의 인스턴스 필드에 접근할 수 있어야 한다.  ->  멤버 함수 포인터는 단순한 주소 이상의 정보를 담기 때문에, 데이터 포인터나 함수 포인터와 호환되지 않는다.

    선언 시 특정 클래스의 메서드만을 가리키고, 사용 시 클래스의 인스턴스와 함께 사용한다.





<inline과 function pointer>

    inline  :  compile-time에 최적화를 통해 함수 호출을 제거하고 함수 코드를 직접 삽입하여 run-time 성능을 향상시킨다.

        함수를 인라인화 하게 되면 매크로화 되어 컴파일 시 코드를 전개한 형태로 바뀌면서 함수 오버헤드가 사라지는데,
        이 인라인화된 함수 코드는 전처리화 되어 본체를 가지지 않기 때문에 주소가 없다, 주소값이 없다.


    function pointer  :  run-time에 특정 함수를 호출하는 간접적인 방법을 제공하여 프로그램의 유연성을 증가시킨다.

        함수 포인터를 사용할 때 함수의 주소가 필요하기 때문에, 동시에 inline을 적용할 수 없다.


    inline을 활용한 최적화와 function pointer의 유연성 사이에는 이러한 trade-off가 존재한다.





<functor(펑터)> (function object(함수 객체))

    // 멤버 함수로 대체할 수 있으며, 둘의 차이는 인터페이스 뿐이다.

    함수처럼 사용하면서 클래스의 특성을 갖는다는 것은 프로그래밍 인터페이스 측면에서 많은 것을 가능하게 한다.
    특히 STL은 generic programming과 functor를 활용하여 유연성과 효율성을 극대화할 수 있도록 설계되었는데,
    STL의 많은 알고리즘들은 다른 함수를 인자로 받기 때문에 functor와 lambda는 STL 설계를 완벽하게 활용할 수 있다.


    operator()가 정의된(overloading) class를 의미한다.

            class Accumulator
            {
            private:
                int total;

            public:
                Accumulator(int _init) : total(_init) {}

                int operator()(int _increment) { return total += _increment; }
            };

            int main(void)
            {
                Accumulator accumulator(0);

                int currTotal = accumulator(10); // 객체를 함수처럼 사용한다.
            }





<mutable>

    mutable로 선언된 필드는 const 메서드 내에서 변경할 수 있도록 허용한다.
    // 캐싱, 참조 카운팅 등 외부 상태가 변하지 않고 내부 데이터를 변경해야 할 필요가 있는데, const/non-const 메서드를 구분하지 않고 필드를 mutable로 함으로써 해결할 수 있다.

            class TestMutable
            {
            private:
                mutable int mutableField;
                int regularField;

            public:
                void myFunc() const
                {
                    mutableField = 1;           // no error
                    regularField = 1;           // error
                }
            };

    mutable은 const 멤버 함수에서만 적용되는 키워드일 뿐이다. [C++]에서 const를 무시할 수 있는 방법은 없다.





<lambda(람다)> (anonymous function(익명 함수))

    람다 표현식은 C++11에서 도입되었으며, 이는 프로그래밍에서 함수형 프로그래밍 요소를 강화하려는 전반적인 추세의 일부였다.
    람다 표현식의 도입 목적은 코드를 더 간결하고 유연하게 만들어, 특히 알고리즘과 같은 라이브러리의 함수에 직접적인 익명 함수를 전달할 수 있도록 하는 것이다.
    이를 통해 STL 같은 라이브러리의 사용성이 크게 향상되었다.

    // capture(캡처): 주로 어떤 정보, 데이터, 상태 또는 컨텍스트를 잡아서 사용하거나 저장하는 행위이다.

            [captures](parameters) mutable exception-specification -> return-type {
                // function body
            }

    captures:       [=] - 값으로 캡처            // [x, &y] - x는 값으로, y는 참조로 캡처
                    [&] - 참조로 캡처            // [] - 캡처 없음

    mutable:        operator() 정의에서 const를 지워, 값 캡처를 변경할 수 있다.

    exception-specification, return-type: 선택적으로 예외 사양과 반환 타입을 명시할 수 있다.
    // C++11에서 auto 키워드와 함께 반환 타입 추론 기능이 추가되면서 lambda의 반환 타입을 생략할 수 있다. 다중 리턴은 모든 리턴 타입이 일치해야 한다.


    e.g.
    ->
            int main(void)
            {
                int total = 0;

                // value capture, auto
                auto accumulator2 = [total](int x) mutable -> int { return total += x; };
                accumulator2(10);

                // reference capture, std::function (from <functional>)
                std::function<int(int)> accumulator3 = [&total](int x) -> int { return total += x; };
                accumulator3(10);

                // anonymity
                [&total](int x) -> int { return total += x; } (10);
            }

    람다 표현식은 compile-time에 anonymous functor로 변환된다. (고유의 클래스 타입)
    // 이는 closure class이며 closure-type이다. 따라서 auto 또는 <functional> 라이브러리의 std::function으로 받는다.


    // std::function: C++11에 도입된 표준 라이브러리의 기능 중 하나로, 가변적인 함수 객체를 보유하고 호출할 수 있는 일반화된 함수 포인터이다.
    // 특정 시그니처를 가진 함수, 람다 표현식, 함수 객체 등을 저장하고 사용할 수 있게 해준다.



    위 코드의 람다들은 아래와 같은 형태의 클래스를 내부적으로 생성한다.

            value capture
            ->
                    class __lambda_unique_class
                    {
                    private:
                        int total;

                    public:
                        explicit __lambda_unique_class(int _total) : total(_total) {}

                        // lambda에서 mutable 키워드는 operator()의 const를 제거한다.
                        int operator()(int x) // const
                        { return total += x; }
                    };


            reference capture
            ->
                    class __lambda_unique_class
                    {
                    private:
                        int& total;

                    public:
                        explicit __lambda_unique_class(int& _total) : total(_total) {}

                        int operator()(int x) const { return total += x; }
                    };































<iostream>

    C++의 표준 라이브러리이며, 입출력 헤더파일이다.

    ㅡ 구조 : C:\Users\bibib\OneDrive\바탕 화면\C언어\[C++] 캡처\iostream 구조

        이처럼 namespace:std 내부에 구현되어있다.



    ㅡ std::cin      std::wcin       etc.
    표준 입력을 담당하는 객체이다.

    ㅡ std::cout     std::wcout      etc.
    표준 출력을 담당하는 객체이다.

        'using namespace std;' 선언함으로써, namespace : 'std::' 을 생략할 수 있다.



*************** ㅡ 해당 namespace에 속한 특정 객체(기능) 소속해제.

                    using std::cin;
                    using std::cout;
                    using std::endl;







ㅡ namespace (네임스페이스, 이름공간)

    서로 다른 scope간에 충돌을 방지하기 위해 추가되었다.

    현재 C++ 표준에서 namespace의 이름은, namespace를 제외하고 이미 선언된 모든 이름과 겹칠 수 없다.

    전역에 정의할 수 있으며, 개체를 구분할 수 있는 범위를 말한다.
    내부공간에 범위 확인 연산자( :: )를 사용하여 접근할 수 있다.
    선언과 정의가 구분되지 않으며, 중괄호를 통한 정의만 가능하다.
    같은 이름의 namespace로 서로 구분되는 객체를 정의할 경우, 합쳐진다.

    (일반적으로 하나의 namespace에서는, 하나의 이름이 단 하나의 개체만을 가리키게 된다.)@@@@@@@@@@@@@@@@@@@@@이해 못함.



    ㅡ using namespace NAME;

        using namespace NAME; 을 선언해줌으로써, (namespace)NAME에 속한 객체를 사용할 때 'NAME::' 을 생략할 수 있다.
        해당 namespace가 정의되어 있는 local이라면, 어디서든 선언이 가능하다.
        using namespace의 scope는, 선언된 local이다.
        여러 개의 namespace를 선언하고 사용하는 것에 제약이 없다.

        basic scope : {}
        중괄호로 scope를 제한하는 방법이 있다.

******* using 선언 취소 불가.



    ㅡ 해당 namespace에 속한 특정 객체(기능) 소속해제.

                    e.g.
                    namespace A
                    {
                        int i = 10;

                        void Test1() {}

                        class A {};
                    };

                    int main()
                    {
                        A::i;               A::Test1();             A::A TestA_1;

                        using A::i;         using A::Test1;         using A::A;

                        i;                  Test1();                A TestA_2;

                        return 0;
                    }




*************************** 장점.

                                namespace의 장점은, 서로 다른 코드 모듈이 충돌에 대한 걱정 없이 변수 . 메서드 등에 대해 원하는 이름을 사용할 수 있다는 것이다.
                                        하지만, C++ 표준 라이브러리를 많이 사용하는 경우, 또는 많이 사용되며 잘 알려진 <iostream>과 같은 경우,
                                        'using namespace std;' 혹은 특정 객체 소속해제 함으로써 사용자의 코드작성이 편리해지며, 코드 가독성이 좋아질 수 있다.



*************************** 단점.

                                코드가 장황해질 수 있으며, 코드 가독성이 나빠질 수 있다.

                                using namespace 사용으로 인해, 충돌 가능성이 재도입될 수 있다.
                                namespace의 목적과 반대되는 것이다. 멤버함수에서 멤버변수를 안쓰는 꼴이다.
                                        가독성을 위해 자주 사용되는 'using namespace std;'의 경우,
                                        std 내부의 객체가, 사용자가 정의한 객체명과 같다면, 충돌이 일어난다.
                                        std는 하나의 예시일 뿐이며, 충돌이 발생하면 compile-time에 'ERROR E0266 : 객체가 모호합니다.' 에러가 뜬다.

                                        실제로 경험 많은 C++ 프로그래머들이 권장하지 않으며, 필요한 경우 std::를 꼭 붙여서 사용하는 데에는 이유가 있다.
                                        사용자가 문제가 없을 것이라는 충분한 판단 하에 'using namespace'를 써야 한다.




                                                namespace, using namespace
                                                e.g. (1)

                                                        using namespace A;      // ERROR : namespace 'A'가 정의되지 않습니다.

                                                        namespace A
                                                        {
                                                            int i = 10;
                                                        }
                                                        namespace A
                                                        {
                                                            float f = 33.3f;
                                                        }                       // namespace A  ::  i(10),  f(33.3f)

                                                        namespace B
                                                        {
                                                            int i = 100;
                                                            float f_2 = 333.33f;
                                                        }

                                                        using namespace A;

                                                                int main()
                                                                {
                                                                    using namespace B;

                                                                    i;(in A)    f;(in A)    B::i;    B::f_2;

                                                                    i;                  // ERROR : 객체가 모호합니다.(충돌)

                                                                    return 0;
                                                                }

                                                        void Test1()
                                                        {
                                                            f_2 = 555.55f;      // ERROR : scope of 'using namespace B'    is in    main()
                                                        }





                                                e.g. (2)

                                                        namespace A
                                                        {
                                                            namespace B
                                                            {
                                                                int i = 10;
                                                            }
                                                        }

                                                                int main()
                                                                {
                                                                    //using namespace B;          // ERROR : 'B' 정보 없음.

                                                                    //using namespace A;
                                                                    //using namespace B;          //  == using namespace A::B;

                                                                    A::B::i;

                                                                    using namespace A;

                                                                    A::B::i;    B::i;

                                                                    using namespace B;

                                                                    A::B::i;    B::i;    i;

                                                                    return 0;
                                                                }



<iterator> (이터레이터)

                                        아래의 글은, 데이터 순회를 목적으로 iterator가 없는 상황에서 설명이 시작된다.

                                        주소값이 연속적인 가변배열 형태의 CArr의 경우, 주소값 연산으로 쉽게 데이터 순회가 가능하다.
                                        주소값이 연속적이지 않은 연결리스트인 CList는 데이터 순회 방식이 다르지만, m_pHeadNode를 시작으로 만들면 그만이다.

                                        내가 사용하기 위해 만든 함수는 이렇다. 인터페이스와 구현 모두 알고 있으니까.
                                        그런데, 남이 만든 코드를 제공받아 사용하면 어떨까??

                                        많이들 사용하는 STL 자료구조의 경우이다.
                                        주소값이 연속적이지 않은 연결리스트인 std::list를 예시로 들어보자.
                                        내부 멤버변수가 비공개되어있는데다가, 공개되어있더라도 구현을 모두 알아야 데이터 순회 코드를 추가할 수 있다.
                                        이러면 편리하게 STL 자료구조를 사용할 이유가 없지 않을까??
                                        STL에서 내부적으로 사용하고 있는 자료형이나 설계 등의 구현을 내가 알고서 사용한다는 건,
                                        내가, 사용자가, 제공받는 기능에 대해서 내부적으로 너무 많이 알아야 한다는 말이다. (또한 그래서도 안된다.)
                                        그래서~
                                        데이터 순회를 목적으로 하는 공통된 인터페이스로 반복자 iterator를 제공한다.

                                        iterator는 inner class이며, 공통된 인터페이스이다.

                                        STL로부터 시작된 공통된 인터페이스로서의 iterator는
                                        Standard뿐만이 아닌 사용자가 직접 정의한 다양한 자료구조에서도 구현된다.
                                        기본적으로 많이 사용되는 STL과 호환이 되어야 하며, 여러 자료구조들간의 데이터 호환이 가능해진다.
                                        //컨테이너의 내부 구조를 몰라도 쉽게 컨테이너를 순회하는 것이 가능해지며, 서로 다른 컨테이너에 통일된 인터페이스로 접근이 가능하기 때문이다.


                                        기본적으로 iterator는 해당 자료구조의 멤버함수 begin()과 end(), 그리고 * ++ != 연산자 인스턴스들을 사용하여 데이터 순회가 가능해야 한다.
                                        이러한 인터페이스만 통일되면 된다. 그렇다면 이를 위한 구현은 어떻게 될까??

                                        잠시 iterator가 없을 때로 돌아가보자.
                                        동적할당을 사용하는 자료구조의 특성상 재할당이 일어났을 때
                                        iterator가 새로이 할당된 컨테이너의 주소를 따라가냐, 이전의 주소에 머무르며 iterator가 사용될 때 오류처리를 할 것이냐로 나눌 수 있다.
                                        나 또한 전자의 방식이 당연하다고 생각했지만, 실제로 STL에서는 이를 문제삼고 있다.

                                        후자의 방식으로 코드를 작성한다면, iterator를 사용하기 전에 항상 begin()을 대입해주어야 한다.

                                        전자의 방식으로 코드를 작성한다면, iterator 객체 생성 시에 딱 한 번만 begin()을 대입해주면 된다.
                                        또한 후자의 방식에 비해 유효성 검사 코드가 많이 줄어든다.


                                        추가로, 여러 가지 연산자 인스턴스를 사용할 수 있는 iterator를
                                        동일한 컨테이너를 가리키는 또 다른 컨테이너로서, 즉 컨테이너를 관리할 수 있는 추가적인 객체로서 사용하고자 한다면,
                                        본인이 선택한 자료구조에 vector 하나가 딸려있는 셈으로, 리소스를 감당해야 할 것이다.
                                        iterator는 애초에 컨테이너의 데이터 순회를 목적으로 만들어졌으며,
                                        애초에 이런 고민은 자료구조를 선택하는 단계에서 나와야 한다.


                                        자신이 작성한 코드의 iterator가 전자의 방식을 벗어나는 것과는 상관없이,
                                        STL 자료구조에서 시작된 공통된 인터페이스라는 정의는 맞추는 것이 좋다.

                                        실제로 myvector를 구현할 때, 가변배열에서 굳이 이렇게 iterator코드를 작성해야 하나 싶은 것들이 있다.
                                        다음으로 myLinkedList 구현하게 된다면, 대부분이 이해가 될 것이다.
                                        아니면 그 전에 앞서 말한 공통된 인터페이스라는 iterator의 정의를 기억한다면 될 것이다.



    반복자(iterator) : 객체 지향적 프로그래밍에서 자료구조 컨테이너의 요소를 순회(traversing)하는 객체이다.

*** iterator 인터페이스는, STL에서는 통일되어 있으나, STL에서도 각 자료구조마다 지원하는 연산자가 다르다.

    iterator가 사용할 연산자로  * [] -> ++ -- += -= == != > >= < <=  등의 일부가 오버로드되어 있기 때문에,
    자료구조가 어떠한 구조든 간에 iterator는 데이터의 순서에 맞게 연속적인 배열형태라고 생각하면 된다.
    단, 자료구조마다 iterator의 구현 방법이 다르기 때문에, iterator가 사용할 연산자의 효율성에 따른 구현 여부 또한 자료구조마다 다르다.



            아래의 코드는, 데이터 순회만을 목적으로 한 iterator의 기본 알고리즘이다.
            해당 자료구조의 멤버함수 begin()과 end(), * ++ != 연산자 인스턴스가 필요하다.

                                        e.g.
                                        #include <iostream>
                                        #include <deque>
                                        using std::deque;
                                        int main()
                                        {
                                            deque<int> dequeINT;
                                            dequeINT.push_back(100);
                                            dequeINT.push_back(200);
                                            dequeINT.push_back(300);

                                            deque<int>::iterator dequeITER;

                                            for (dequeITER = dequeINT.begin(); dequeITER != dequeINT.end(); ++dequeITER)
                                            {
                                                printf("%d\n", *dequeITER);             // 출력 방법.
                                            }

                                            return 0;
                                        }

                                            end()함수는, 컨테이너 데이터의 마지막 다음을 가리키므로,
                                            마지막 데이터까지 순회가 가능하다.

                                            iterator는 포인터가 아닌 객체이지만,
                                            역참조 . 배열첨자 . 증감 등의 연산자 오버로드로 데이터를 가리키며 순회할 수 있는 스마트 포인터이므로,
                                            데이터에 대한 포인터 타입이라 생각하고 연산자를 이해하거나 정의하면 된다.



    ㅡ iterator에 대한 추가, 상세정보

            참고 : https://ansohxxn.github.io/stl/chapter16-2/
            참고 : https://junstar92.tistory.com/323
            참고 : https://euncero.tistory.com/727
            참고 : http://www.tcpschool.com/cpp/cpp_iterator_intro
            참고 : http://www.tcpschool.com/cpp/cpp_iterator_category
            참고 : http://www.tcpschool.com/cpp/cpp_iterator_etc
*/