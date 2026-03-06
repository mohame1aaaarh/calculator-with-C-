#include <graphics.h>
#include <string>
#include <cstdio>

// ── State ──────────────────────────────────────
std::string display = "0";
double num1 = 0;
char   op   = 0;
bool   fresh = true;

const char* btns[4][4] = {
    {"7","8","9","/"},
    {"4","5","6","*"},
    {"1","2","3","-"},
    {"C","0","=","+"}
};

// ── Logic ──────────────────────────────────────
void press(const char* b) {
    if (b[0] == 'C') {
        display = "0"; num1 = 0; op = 0; fresh = true;
    }
    else if (b[0]=='+'||b[0]=='-'||b[0]=='*'||b[0]=='/') {
        num1 = std::stod(display); op = b[0]; fresh = true;
    }
    else if (b[0] == '=') {
        if (!op) return;
        double num2 = std::stod(display), res = 0;
        if (op=='+') res = num1 + num2;
        if (op=='-') res = num1 - num2;
        if (op=='*') res = num1 * num2;
        if (op=='/') res = num2 ? num1/num2 : 0;
        char buf[32];
        snprintf(buf, sizeof(buf), "%.8g", res);
        display = buf;
        op = 0; fresh = true;
    }
    else {
        if (fresh) { display = b; fresh = false; }
        else if (display == "0") display = b;
        else display += b;
    }
}

// ── Draw ───────────────────────────────────────
void drawCalc() {
    cleardevice();

    // شاشة العرض
    setfillstyle(SOLID_FILL, BLACK);
    bar(10, 10, 310, 70);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    int tw = textwidth((char*)display.c_str());
    outtextxy(300 - tw, 30, (char*)display.c_str());

    // الأزرار
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int x = 10 + c * 76;
            int y = 80 + r * 76;
            const char* lbl = btns[r][c];

            if (lbl[0]=='+'||lbl[0]=='-'||lbl[0]=='*'||lbl[0]=='/')
                setfillstyle(SOLID_FILL, YELLOW);
            else if (lbl[0] == '=')
                setfillstyle(SOLID_FILL, GREEN);
            else if (lbl[0] == 'C')
                setfillstyle(SOLID_FILL, RED);
            else
                setfillstyle(SOLID_FILL, DARKGRAY);

            bar(x, y, x+68, y+68);
            setcolor(WHITE);
            rectangle(x, y, x+68, y+68);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            int lw = textwidth((char*)lbl);
            int lh = textheight((char*)lbl);
            outtextxy(x+(68-lw)/2, y+(68-lh)/2, (char*)lbl);
        }
    }
    refresh();
}

// ── Main ───────────────────────────────────────
int main() {
    initwindow(320, 390, (char*)"Calculator");
    setbkcolor(DARKGRAY);
    drawCalc();

    bool mouseWasDown = false;  // ← الحل

    while (true) {
        bool mouseNowDown = ismouseclick(WM_LBUTTONDOWN);

        // شغّل الضغطة بس لما الزرار ينزل من جديد
        if (mouseNowDown && !mouseWasDown) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            for (int r = 0; r < 4; r++)
                for (int c = 0; c < 4; c++) {
                    int x = 10 + c * 76;
                    int y = 80 + r * 76;
                    if (mx>=x && mx<=x+68 && my>=y && my<=y+68) {
                        press(btns[r][c]);
                        drawCalc();
                    }
                }
        }

        mouseWasDown = mouseNowDown;

        // ESC للخروج
        if (kbhit()) {
            char k = getch();
            if (k == 27) break;
        }

        delay(50);
    }

    closegraph();
    return 0;
}