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

int FindingPlace(Node* head, int row, int column){
    if(head == NULL) return 0;
    // if((row == 0)&(column == 0)) return 0;

    int rowcnt = 0;
    int colcnt = 0;
    int nodecnt = 0;
    Node* p = head;
    while(p != NULL){
        if(rowcnt == row){
            if(column == colcnt) break;
            colcnt ++;
        }
        nodecnt++;
        if(p -> data == '\n') rowcnt ++;
        p = p -> ptr; 
    }
    return nodecnt; 
}

int main(int argc, char* argv[]){
    char filename[50] = "[No Name]";        
    int total_linenum = 0;
    Node* head = NULL;

    if( argc == 2 ){ //argument exists
        filename[0] = '\0';
        strcpy(filename, argv[1]);
        FILE *fo = fopen(filename, "r");
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
    }
    if(head != NULL) head = Reverse(head);
    
    WINDOW *editor, *statusbar, *messagebar;

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    refresh(); 

    editor = newwin(LINES, COLS-2, 0, 0); //from 0 to LINES-3
    statusbar = newwin(1, COLS, LINES-2, 0);
    messagebar = newwin(1, COLS, LINES-1, 0);
    wrefresh(editor);

    //status bar
    char status[100];
    sprintf(status, "%s%s%d%s", filename, " - ", total_linenum, " lines");
    mvwprintw(statusbar, 0, 1, status);
    status[0] = '\0';
    sprintf(status, "%s%d%s%d", "no ft | ", 1, "/", total_linenum);
    mvwprintw(statusbar, 0, COLS - strlen(status) , status); // --------------이거 확인하기 total_len 바뀔 때마다 같이 업데이트. 그리고 커서 up down이동할 때마다 같이 업데이트
    start_color();
    init_pair(1, 0, 7);
    wbkgd(statusbar, COLOR_PAIR(1));
    wrefresh(statusbar);

    //message bar
    mvwprintw(messagebar, 0, 1, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find"); 
    wrefresh(messagebar);
    refresh(); 
//--------------------------------------------------------------------------
    char tmpline[2];
    int maxColumnInaline[100] = {0, }; //한 화면에 뿌릴 약 100줄만 max를 잰다.
    int row = 0;
    int column = 0;

    if( head != NULL ){ //when file existed, spray on the screen for the frist time
        int tmprow = 0;
        int tmpcol = 0;
        Node* p = head;
        while(p != NULL){
            tmpline[0] = '\0';
            sprintf(tmpline, "%c", p -> data);
            mvwprintw(editor, tmprow, tmpcol++, tmpline); 
            if(p -> data == '\n'){
                maxColumnInaline[tmprow] = tmpcol - 1;
                tmpcol = 0;
                wmove(editor, ++tmprow, tmpcol);
                if(tmprow == LINES -2) break; // spray only first page contents
            }
            p = p -> ptr;
        }
        maxColumnInaline[tmprow] = tmpcol;
    }
    wmove(editor,row,column);

    int startlinenum = 0;

    while(1){ 
        wrefresh(editor);
        int ch = getch();
        if( ch == 19 ){ //ctrl - s
            if(strcmp(filename, "[No Name]") == 0) { //case of new file opened 
                filename[0] = '\0';
                werase(messagebar);
                mvwprintw(messagebar, 0, 1, "SAVE MODE, put the filename: "); 
                wrefresh(messagebar);
                char ch_tmp[50];
                tmpline[0] = '\0';
                for(int i =0; i< 49; i++){ //get file name
                    ch_tmp[i] = wgetch(messagebar);
                    sprintf(tmpline, "%c", ch_tmp[i]);
                    mvwprintw(messagebar, 0, 30+i, tmpline);
                    wrefresh(messagebar);
                    if(ch_tmp[i] == 17){
                        strncat(filename, ch_tmp, i); 
                        FILE *fo = fopen(filename, "w");
                        Node* p = head;
                        while( p != NULL){
                            fprintf(fo,"%c", p -> data);
                            p = p -> ptr;
                        }
                        fclose(fo);
                        break;
                    }
                }
                break;
            }
            else{ //case of existed file opened
                werase(messagebar);
                mvwprintw(messagebar, 0, 1, "SAVED"); 
                wrefresh(messagebar);
                FILE *fo = fopen(filename, "w");
                Node* p = head;
                while( p != NULL){
                    fprintf(fo,"%c", p -> data);
                    p = p -> ptr;
                }
                fclose(fo);
                char sch = getch();
                if (sch == 17) break;
            }
        }
        if( ch == 17 ) { //ctrl - q x2
            char quitch = getch();
            if (quitch == 17) {
                break;
            }
        }
        else if( ch == 6 ){ //ctrl - f
            werase(messagebar);
            mvwprintw(messagebar, 0, 1, "FIND MODE: "); 
            char ch_tmp[50];
            tmpline[0] = '\0';
            for(int i = 0; i < 49; i++){
                ch_tmp[i] = wgetch(messagebar);
                sprintf(tmpline, "%c", ch_tmp[i]);
                mvwprintw(messagebar, 0, 11+i, tmpline);
                wrefresh(messagebar);
                if(ch_tmp[i] == '\n') {
                    wmove(editor, row, column);
                    break;
                }
            }
        }
        //---------------------------arrow KEY--------------------------
        else if (ch == KEY_LEFT){
            if(column != 0) wmove(editor, row, --column);
        } 
        else if (ch == KEY_RIGHT) {
            if(maxColumnInaline[row] > column) wmove(editor, row, ++column); 
        }
        else if (ch == KEY_UP) { //updown을 startline기준으로 바꿔야 하려나?
            if(row > 0){ // 전체 내용이 한 페이지 이내
                --row;
                if(maxColumnInaline[row] < column) column = maxColumnInaline[row];
                wmove(editor, row ,column);
            }
            else if(row == 0){ 
                if(startlinenum == 0) continue;
                else{
                    --startlinenum;
                    Node* p = head;
                    int cnt = 0;
                    int tmprow = 0;
                    int tmpcol = 0;
                    while( p != NULL ){
                        if(cnt < startlinenum){
                            if(p -> data == '\n') cnt++; 
                            p = p -> ptr; 
                            continue;
                        }
                        if((tmprow == LINES - 3)&(p -> data == '\n')) break; 
                        tmpline[0] = '\0';
                        sprintf(tmpline, "%c", p -> data);
                        mvwprintw(editor, tmprow, tmpcol++, tmpline);
                        if(p -> data == '\n'){
                            maxColumnInaline[tmprow] = tmpcol - 1;
                            tmpcol = 0;
                            wmove(editor, ++tmprow, tmpcol);
                            tmpline[0] = '\n';
                            mvwprintw(editor, tmprow, tmpcol, tmpline);
                        }
                        p = p -> ptr;
                    } 
                    maxColumnInaline[tmprow] = tmpcol;
                }
            }
            status[0] = '\0';
            sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row + 1, "/", total_linenum);
            mvwprintw(statusbar, 0, COLS - strlen(status) , status); 
            wrefresh(statusbar);
            wmove(editor, row , column);

        }
        else if (ch == KEY_DOWN){ //key up down의 status 바가 조금 문제 있음
            if(total_linenum -1 > row) {
                if(LINES - 3 == row){ //마지막 line-3위치에서 다운
                    if(startlinenum == 0) row ++;
                    startlinenum++;
                    Node* p = head;
                    int cnt = 0;
                    int tmprow = 0;
                    int tmpcol = 0;
                    while( p != NULL ){
                        if(cnt < startlinenum){
                            if(p -> data == '\n') cnt++; 
                            p = p -> ptr; 
                            continue;
                        }
                        // ln 다음의 char부터 시작
                        if((tmprow == LINES - 3)&(p -> data == '\n')) break;  //\n없이 끝날 땐 p->data null로 끝나지는 건가?
                        tmpline[0] = '\0';
                        sprintf(tmpline, "%c", p -> data);
                        mvwprintw(editor, tmprow, tmpcol++, tmpline);
                        if(p -> data == '\n'){
                            maxColumnInaline[tmprow] = tmpcol - 1; // adding -1 because of \n
                            tmpcol = 0;
                            wmove(editor, ++tmprow, tmpcol);
                            tmpline[0] = '\n';
                            mvwprintw(editor, tmprow, tmpcol, tmpline);
                        }
                        p = p -> ptr;
                    }
                    maxColumnInaline[tmprow] = tmpcol;
                    wmove(editor, tmprow, column);
                    // status[0] = '\0';
                    // sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row , "/", total_linenum);
                }
                else if(maxColumnInaline[++row] >= column) {
                    wmove(editor, row, column);
                }
                else{
                    column = maxColumnInaline[++row];
                    // status[0] = '\0';
                    // sprintf(status, "%s%d%s%d", "no ft | ", startlinenum+row+1, "/", total_linenum);
                    wmove(editor, row, column);
                } 
            }
            status[0] = '\0';
            if(startlinenum == 0) sprintf(status, "%s%d%s%d", "no ft | ", startlinenum+row+1, "/", total_linenum);
            else sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row, "/", total_linenum);
            mvwprintw(statusbar, 0, COLS - strlen(status) , status); 
            wrefresh(statusbar);
        }
        else if(ch == KEY_PPAGE){ // page up
            if(startlinenum != 0){//0이 되기 전까지 할 수 있는 거니까...
                startlinenum -= LINES-2;
                if(startlinenum < 0) startlinenum = 0;
                for(int i = 0; i<= LINES - 3; i++) {
                    tmpline[0] = '\n';
                    mvwprintw(editor, i, 0, tmpline);
                }
                //spray
                Node* p = head;
                int cnt = 0;
                int tmprow = 0;
                int tmpcol = 0;
                while( p != NULL ){
                    if(cnt < startlinenum){
                        if(p -> data == '\n') cnt++; 
                        p = p -> ptr; 
                        continue;
                    }
                    if((tmprow == LINES - 3)&(p -> data == '\n')) break; 
                    tmpline[0] = '\0';
                    sprintf(tmpline, "%c", p -> data);
                    mvwprintw(editor, tmprow, tmpcol++, tmpline);
                    if(p -> data == '\n'){
                        maxColumnInaline[tmprow] = tmpcol - 1;
                        tmpcol = 0;
                        wmove(editor, ++tmprow, tmpcol);
                        tmpline[0] = '\n';
                        mvwprintw(editor, tmprow, tmpcol, tmpline);
                    }
                    p = p -> ptr;
                }
                maxColumnInaline[tmprow] = tmpcol;
                wmove(editor, row, column);

                status[0] = '\0';
                sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row + 1, "/", total_linenum);
                mvwprintw(statusbar, 0, COLS - strlen(status) , status); 
                wrefresh(statusbar);
            }
        }
        else if(ch == KEY_NPAGE){ // page down
            if(total_linenum > LINES - 2){ 
                startlinenum += (LINES - 2);
                if(startlinenum > total_linenum) continue;
                for(int i = 0; i<= LINES - 3; i++) {
                    tmpline[0] = '\n';
                    mvwprintw(editor, i, 0, tmpline);
                }
                //spray
                Node* p = head;
                int cnt = 0;
                int tmprow = 0;
                int tmpcol = 0;
                while( p != NULL ){
                    if(cnt < startlinenum){
                        if(p -> data == '\n') cnt++; 
                        p = p -> ptr; 
                        continue;
                    }
                    if((tmprow == LINES - 3)&(p -> data == '\n')) break; 
                    tmpline[0] = '\0';
                    sprintf(tmpline, "%c", p -> data);
                    mvwprintw(editor, tmprow, tmpcol++, tmpline);
                    if(p -> data == '\n'){
                        maxColumnInaline[tmprow] = tmpcol - 1;
                        tmpcol = 0;
                        wmove(editor, ++tmprow, tmpcol);
                        tmpline[0] = '\n';
                        mvwprintw(editor, tmprow, tmpcol, tmpline);
                    }
                    p = p -> ptr;
                }
                maxColumnInaline[tmprow] = tmpcol;
                wmove(editor, row, column);

                status[0] = '\0';
                sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row + 1, "/", total_linenum);
                mvwprintw(statusbar, 0, COLS - strlen(status) , status); 
                wrefresh(statusbar);
            }
        }
        //--------------------------backspace-------------------------
        else if(ch == 127) {//backspace
            if((row == 0)&(column == 0)&(startlinenum==0)) continue;
            int nodenum = 0;
            nodenum = FindingPlace(head, startlinenum + row , column); 
            head = Delete(head, nodenum);

            //curser
            if(column == 0){
                --row;
                column = maxColumnInaline[row];
                total_linenum --;
            }
            else --column;

            //spray and counting maxcolumn 
            Node* p = head;
            int cnt = 0;
            int tmprow = 0;
            int tmpcol = 0;
            for(int i = 0; i<= LINES - 3; i++) {
                tmpline[0] = '\n';
                mvwprintw(editor, i, 0, tmpline);
            }
            while( p != NULL ){
                if(cnt < startlinenum){
                    if(p -> data == '\n') cnt++; 
                    p = p -> ptr; 
                    continue;
                }
                if((tmprow == LINES - 3)&(p -> data == '\n')) break;
                tmpline[0] = '\0';
                sprintf(tmpline, "%c", p -> data);
                mvwprintw(editor, tmprow, tmpcol++, tmpline);
                if(p -> data == '\n'){
                    maxColumnInaline[tmprow] = tmpcol - 1;
                    tmpcol = 0;
                    if(tmprow + 1 == LINES - 2) break; 
                    wmove(editor, ++tmprow, tmpcol);
                }
                p = p -> ptr;
            }
            maxColumnInaline[tmprow] = tmpcol;
            
            wmove(editor, row, column);
            
            ///changing line number in status bar
            if(head == NULL) total_linenum = 0;
            status[0] = '\0';
            sprintf(status, "%s%s%d%s", filename, " - ", total_linenum, " lines");
            mvwprintw(statusbar, 0, 1, status);
            wrefresh(statusbar);
        }
        //----------------------------new char-------------------------
        else{
            int nodenum = FindingPlace(head, startlinenum + row, column);
            if(nodenum == 0) total_linenum++;
            head = InsertNode(head, nodenum, ch); 

            if(ch == '\n') {
                row++;
                column = 0;
                total_linenum++;
            }
            else column ++;

            //spray
            Node* p = head;
            int cnt = 0;
            int tmprow = 0;
            int tmpcol = 0;
            while( p != NULL ){
                if(cnt < startlinenum){
                    if(p -> data == '\n') cnt++; 
                    p = p -> ptr; 
                    continue;
                }
                if((tmprow == LINES - 3)&(p -> data == '\n')) break; 
                tmpline[0] = '\0';
                sprintf(tmpline, "%c", p -> data);
                mvwprintw(editor, tmprow, tmpcol++, tmpline);
                if(p -> data == '\n'){
                    maxColumnInaline[tmprow] = tmpcol - 1;
                    tmpcol = 0;
                    wmove(editor, ++tmprow, tmpcol);
                }
                p = p -> ptr;
            }
            maxColumnInaline[tmprow] = tmpcol;
            wmove(editor, row, column);

            status[0] = '\0'; ///changing line number in status bar
            sprintf(status, "%s%s%d%s", filename, " - ", total_linenum, " lines");
            mvwprintw(statusbar, 0, 1, status);
            status[0] = '\0';
            sprintf(status, "%s%d%s%d", "no ft | ", startlinenum + row + 1, "/", total_linenum);
            mvwprintw(statusbar, 0, COLS - strlen(status) , status); 
            wrefresh(statusbar);

            wmove(editor, row, column);
        }
    }
    DeleteAll(head);
    endwin();
}

