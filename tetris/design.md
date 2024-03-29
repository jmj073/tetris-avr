# 해야할 것



+ tetris.c 가독성 개선
+ 레벨 변경 구현
+ 파일 나누기

























## 16x32 LED Matrix Contol Library

+ 라이브러리가 수행해야할 기능:
  LED에 표시할 데이터를 LED에 보낸다.
  LED에 표시할 데이터를 조작한다.

+ 라이브러리 사용 방법:
  LED 메트릭스 데이터에 출력하고 싶은 색깔 설정.
  출력함수 호출. 주기적으로 리프레시 해줘야 한다. 타이머 인터럽트를 사용할 수 있다.

  

+ LED matrix에 출력할 데이터를 설정하는 것을 '그린다'라고 표현하겠다.

그리는 도중에도 리프레시로 인해 그리기는 계속된다. 
따라서 완성되기 전의 덩어리가 출력되어 보여질 수 있다.
그리기는 보통은 빠르기 때문에 잘 보이지 않는다.













class RGBmatrixPanel: public Adafruit_GFX;


도형그리기는:
	1. startWrite()로 시작
	2. 여러가지 그리기
	3. endWrite()로 끝
난다.

+ drawRect
  + startWrite
  + writeFastHLine
    + drawFastHLine
      + startWrite
      + writeLine
        + 여러가지 전처리
        + writePixel
        + drawPixel
          + 버퍼에 실질적으로 데이터를 기록
      + endWrite
  + endWrite

# LED MATRIX 구현 방법

+ **버퍼 교체 함수**
+ **버퍼 수정 함수**
+ **리프레시 함수**



+ **내부 버퍼 1하나 선언**:
  + **버퍼 교체 함수**: 없음
  + **버퍼 수정 함수**: 내부 버퍼 사용
  + **리프레시 함수**: 내부 버퍼 사용
  + **<span style="color: red">장점</span>**: 간단
  + **<span style="color: skyblue">단점</span>**: 버퍼를 수정하는 동안 출력 불가능.

+ **내부 버퍼 2개 선언**:
  + **버퍼 교체 함수**: 내부 버퍼 사용
  + **버퍼 수정 함수**: (현재 올라가 있지 않은)내부 버퍼 사용
  + **리프레시 함수**: 현재 올라가 있는 내부 버퍼 사용
  + **<span style="color: red">장점</span>**: 비교적 간단.
  + **<span style="color: skyblue">단점</span>**: 버퍼 교체시에 동기화를 위해 버퍼 내용을 복사해야함. 항상 새로 그린다면 상관 없다.

+ **외부적으로 버퍼 선언1**:
  + **버퍼 교체 함수**: 외부 버퍼를 매개변수로 받음
  + **버퍼 수정 함수**: 외부 버퍼를 매개변수로 받음
  + **리프레시 함수**: 내부 버퍼 사용
  + **<span style="color: red">장점</span>**: 좀더 유연하다
  + **<span style="color: skyblue">단점</span>**: 버퍼 수정할 때마다 매개변수로 넘겨줘야 하니 보일러 플레이트가 생긴다.
+ **외부적으로 버퍼 선언2**:
  + **버퍼 교체 함수**: 없음
  + **버퍼 수정 함수**: 외부 버퍼를 매개변수로 받음
  + **리프레시 함수**: 외부 버퍼를 매개변수로 받음
  + **<span style="color: red">장점</span>**: 너, 하는일이 없구나!
  + **<span style="color: skyblue">단점</span>**:

# 입력 메커니즘

## 버튼 입력 방식들

### 연속 방식 

누르고 있으면 효과가 지속된다.

### 반연속 방식

예를들어 설명하자면 키보드로 문자를 입력할때 계속 누르면 한글자 나오고 잠깐의 텀이 있은 뒤에 문자가 연속으로 나온다. 이런 방식이다.

#### 구현

+ **push 순간**: 

  move후에 연속 방식

  + **조건**: 
    + input 1
    + prev 0

+ **누르고 있는 동안**:

  + **조건**:
    + input 1

+ **release 순간**:

  + **조건**:
    + input 0
    + prev 1

### 비연속 방식

효과를 다시 볼려면 버튼을 release했다가 다시 눌러야 한다.

## My Buttons

+ **UP**: rotate piece
  + 비연속 방식
+ **DOWN**: land piece
  + 비연속 방식
+ **LEFT**: move piece to left
  + 반연속 방식
+ **RIGHT**: move piece to right
  + 반연속 방식

