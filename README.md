# 개요
vi와 같은 terminal에서 사용하기 위한 개발자용 텍스트 에디터를 개발한다. vi의 일부 기능만을 구현하므로 <u>**simplicity(간이) vi editor**</u>라 칭한다.

</br>

### 주요 조건
1. 자료구조 이용
2. 5MB 이상의 텍스트 파일 열기 가능
3. makefile을 통한 compile
4. 모든 OS에서 동작 가능 

</br>

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
</br>

# 실행 및 동작
### 실행 명령어
makefile을 통해 compile한 후, 다음의 명령어로 실행한다. 
```
./vite
```

</br>

### 동작
![2024-01-1516 47 42-ezgif com-video-to-gif-converter](https://github.com/Jaden000/simple-vi-editor/assets/84056591/75ea0b78-a8c2-4d4c-b4af-3b7722d78ce6)

</br>
</br>

# 디렉토리 구조

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
