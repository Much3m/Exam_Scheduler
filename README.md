# Midterm Scheduling Project


## Getting Started

### Makefile 사용시
```
make all                                        // compile & link
make clean                                      // delete object file
make fclean                                     // delete objfile & executive file
make re                                         // recompile & link
./scheduler                                     // execute
```
### Makefile 사용하지 않은 경우
```
gcc -o scheduler -I ./scheduler.h scheduler.c   // compile & link
./scheduler                                     // execute
```
-----
## 입력 파일 형식

>"(이름):과목명1,과목명2,과목명3..." 와 개행문자로 구성된 txt파일을 입력으로 한다.
### 예시
```
Albert Einstein:A,B,C,D
Isaac Newton:A,B,E,F
```

