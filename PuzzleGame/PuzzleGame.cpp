#include <bangtal.h>
#include <string>
#include <time.h>
using namespace std;

SceneID scene;

ObjectID puzzle[9];
ObjectID startButton;

TimerID timer;

int blankN;
int currentPos[9];

//바꾸기
void changeBlankPos(int n) {
    int tmp, movingDegree;
    string str;
    const char* c;
    switch (n)
    {
    case 0:
        if (blankN == 0 || blankN == 1 || blankN == 2) {
            printf("blankN : %d , up impossible\n", blankN);
            return;
        }
        movingDegree = -3;
        break;
    case 1:
        if (blankN == 2 || blankN == 5 || blankN == 8) {
            printf("blankN : %d , right impossible\n", blankN);
            return;
        }
        movingDegree = 1;
        break;
    case 2:
        if (blankN == 6 || blankN == 7 || blankN == 8) {
            printf("blankN : %d , down impossible\n", blankN);
            return;
        }
        movingDegree = 3;
        break;
    case 3:
        if (blankN == 0 || blankN == 3 || blankN == 6) {
            printf("blankN : %d , left impossible\n", blankN);
            return;
        }
        movingDegree = -1;
    }
    printf("switch %d - %d \n", blankN, blankN + movingDegree);
    tmp = currentPos[blankN + movingDegree];
    currentPos[blankN + movingDegree] = currentPos[blankN];
    currentPos[blankN] = tmp;
    str = "Images/";
    str += to_string(currentPos[blankN] + 1);
    str += ".png";
    c = str.c_str();
    setObjectImage(puzzle[blankN], c);
    setObjectImage(puzzle[blankN + movingDegree], "Images/white.png");
    blankN += movingDegree;
    hideObject(puzzle[blankN]);
    hideObject(puzzle[blankN - movingDegree]);
    showObject(puzzle[blankN]);
    showObject(puzzle[blankN - movingDegree]);
    return;

}

void shuffleImage() {
    //빈칸 랜덤으로 하나 설정
    srand((unsigned int)time(NULL));
    blankN = rand() % 9;
    printf("fist blankN : %d\n", blankN);
    setObjectImage(puzzle[blankN], "Images/white.png");

    int n;
    for (int i = 0; i < 1000; i++) {
        n = rand() % 4;
        printf("%d ", n);
        changeBlankPos(n);

    }

}

void gameStart() {
    //초기화
    for (int i = 0; i < 9; i++) {
        string str = "Images/";
        str += to_string(i + 1);
        str += ".png";
        const char* c = str.c_str();
        setObjectImage(puzzle[i], c);

        currentPos[i] = i;
    }
    shuffleImage();
    setTimer(timer, 600.0f);
    startTimer(timer);
}

void checkBlankAround(int n) {
    switch (blankN - n) {
    case 3:
        changeBlankPos(0);
        break;
    case 1:
        changeBlankPos(3);
        break;
    case -3:
        changeBlankPos(2);
        break;
    case -1:
        changeBlankPos(1);
    }
}

void checkGameClear() {
    for (int i = 0; i < 9; i++) {
        if (!(currentPos[i] == i))
            return;
    }
    string str = "Images/";
    str += to_string(blankN + 1);
    str += ".png";
    const char* c = str.c_str();
    setObjectImage(puzzle[blankN], c);

    str = to_string(600 - (int)getTimer(timer));
    str += "초 만에 클리어 성공!";
    c = str.c_str();
    showMessage(c);

    hideTimer();
    stopTimer(timer);

    showObject(startButton);
}


ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true)
{
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);

    if (shown) {
        showObject(object);
    }

    return object;
}


void gameOver() {
    //화면 초기화
    for (int i = 0; i < 9; i++) {
        string str = "Images/";
        str += to_string(i + 1);
        str += ".png";
        const char* c = str.c_str();

        puzzle[i] = createObject(c, scene, 340 + (i % 3) * 200, 475 - 200 * (i / 3));
    }
    stopTimer(timer);
    showObject(startButton);

    showMessage("10분 안에 클리어 실패");

}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
    if (object == startButton) {
        hideObject(startButton);
        gameStart();
    }
    else if (object == puzzle[0]) {
        checkBlankAround(0);
    }
    else if (object == puzzle[1]) {
        checkBlankAround(1);
    }
    else if (object == puzzle[2]) {
        checkBlankAround(2);
    }
    else if (object == puzzle[3]) {
        checkBlankAround(3);
    }
    else if (object == puzzle[4]) {
        checkBlankAround(4);
    }
    else if (object == puzzle[5]) {
        checkBlankAround(5);
    }
    else if (object == puzzle[6]) {
        checkBlankAround(6);
    }
    else if (object == puzzle[7]) {
        checkBlankAround(7);
    }
    else if (object == puzzle[8]) {
        checkBlankAround(8);
    }

    checkGameClear();
}

void timerCallback(TimerID time) {
    if (time == timer) {
        gameOver();
    }
}

int main() {
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);

    scene = createScene("PuzzleGame", "Images/background.png");


    for (int i = 0; i < 9; i++) {
        string str = "Images/";
        str += to_string(i + 1);
        str += ".png";
        const char* c = str.c_str();

        puzzle[i] = createObject(c, scene, 340 + (i % 3) * 200, 475 - 200 * (i / 3));
    }

    startButton = createObject("Images/start.png", scene, 550, 70);
    scaleObject(startButton, 1.5f);

    timer = createTimer(600.f);
    showTimer(timer);

    startGame(scene);
}