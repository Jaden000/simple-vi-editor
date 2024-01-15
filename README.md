# 개요
vi와 같은 terminal에서 사용하기 위한 개발자용 텍스트 에디터를 개발한다. vi의 일부 기능만을 구현하므로 <u>**simplicity(간이) vi editor**</u>라 칭한다.

</br>

### 주요 개발 조건
1. C언어 이용
2. 자료구조 이용
3. 5MB 이상의 텍스트 파일 열기 가능
4. makefile을 통한 compile
5. 모든 OS에서 동작 가능

### 실행 명령어
makefile을 통해 compile한 후, 다음의 명령어로 실행한다. 
```
./vite
//vi te(xt) (editor)
```

### 기능
vi에 있는 기능 중 다음의 일부 기능만을 구현한다. 

|no.|기능 명| 내용|
|---|---|---|
|1|새로운 파일 열기|`./vite` 명령어를 통해 새로운 파일을 연다.|
|2|기존에 있는 파일 열기|`./vite testfile.txt` 기존에 있는 파일의 이름을 인자로 준다.|
|3|상태바|하단에서 2번째 라인의 상태바는 파일 이름, 라인 수, 현재 라인 위치를 표시한다.|
|4|메세지바|하단에서 1번째 라인에 있는 메세지바는 저장, 탐색, 나가기 방법을 표시하며 상태 변화를 표시한다.|
|5|커서 이동|화살표키, 페이지업, 페이지다운을 통한 커서 이동이 가능하도록 한다.|
|6|저장|ctrl-s 키로 파일의 내용을 저장한다.|
|7|나가기|ctrl-q 키로 나갈 수 있다. 저장과 연달아 이어지는 경우에만 동작하며 저장 없이 나가고자 하는 경우에는 ctrl-q를 연달아 눌러 나간다.|

</br>

---------------------

이용한 자료구조 및 기능 구현에 대한 내용은 report.md에 기재되어있으며 주요 개발 조건 중 ③ (5MB 이상의 텍스트 파일 열기 가능), ⑤ (모든 OS에서 동작 가능)의 사항만 아래에 기재한다.


### 5MB 이상의 텍스트 파일 열기 가능
대용량의 파일을 신속히 읽어오기 위해 feof()와 fread()를 사용한다. 
- main (일부 발췌)
```
if(fo != NULL){
	total_linenum = 1;
	char buffer[1024] = {0, };
	while(feof(fo) == 0){
		int count = fread(buffer, 1, sizeof(buffer), fo);
		if(count > 0){
			for(int i = 0; i < strlen(buffer); i++) {
				head = InsertAtHead(head, buffer[i]);
				if(buffer[i] == '\n') total_linenum++;
			}
			memset(buffer, 0, sizeof(buffer));
		}
	}
}
fclose(fo);
```
다음의 방법으로 구현했을 때에는 대용량 파일 읽기에 많은 시간이 소요됐다.
- feof()를 사용하지 않고 buffer의 마지막을 인식하여 while문 빠져나오기
- buffer를 동적으로 선언 
	- ex)`char* data = (char*)malloc(sizeof(char)*1024);`


### 모든 OS에서 동작 가능
makefile에서 OS별 conditional compile하고 main에서 전처리기(ifdef)를 이용한다.

- makefile
```
CC = gcc
ifeq ($(OS), Windows_NT) //if OS is Windows
    LIBS = -L. -lpdcurses
else //if OS is MacOs or Linux
    LIBS = -lncurses
endif

run: main.o linkedlist.o 
	gcc -o vite main.o linkedlist.o $(LIBS)
main.o: main.c
	gcc -c main.c -o main.o 
linkedlist.o: linkedlist.c
	gcc -c linkedlist.c -o linkedlist.o
clean:
	rm -f *.o
	rm *.o vite
```

curses.h 라이브러리는 Windows에서 `-pdcurses`로, MacOs와 Linux에서 `-lncurses`로 LIBS 매크로를 달리 갖는다.  makefile에서 조건문(ifeq)을 이용하여 운영체제별로 LIBS 매크로를 설정한다. Windows의 경우, pdcurses 라이브러리를 인식하지 못하는 문제를 해결하기 위해 로컬에 pdcurses 라이브러리를 두었고, LIB_DIRS 매크로를 `-L.`(로컬)로 정의했다.


- main (일부 발췌)
```
#ifdef _WIN32
    #include <stdio.h>
    #include "curses.h"   
    #include <string.h>
    #include "linkedlist.h"
#else
    #include <stdio.h>
    #include <curses.h>   
    #include <string.h>
    #include "linkedlist.h"
#endif
```


-----------

### 디렉토리 구조

```
.
├── README.md
├── simple_vi_editor
└── report.md
```

`simple_vi_editor`: 소스 코드 디렉토리



- `simple_vi_editor/`

```
simple_vi_editor/
├── makefile
├── main.c
├── linkedlist.c
├── linkedlist.h
└── libpdcurses.a
```
`main.c` : 메인 코드
`mainpage.ejs`: 자료구조
`libpdcurses.a`: Windows에서의 pdcurses 라이브러리 인식 불가 문제 해결을 위함
