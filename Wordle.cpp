#include <algorithm>
#include "Color.h"
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <windows.h>

#define str std::string
#define elif else if
#define CTRL(c) ((c) & 037)

std::ifstream inAlphabet("Alphabet.txt");

int ap[25], currIndex;
str givenWord;

class Alphabet
{
    public :
        int len;
        str word[11500];

        str randomWord()
        {
            srand(time(NULL));
            bool pick = (bool)(rand() % 2); 
            std::mt19937 mt(time(nullptr));
            int x = mt() % len, y = len - 1 - (mt() % len);
            return pick ? word[x] : word[y]; 
        }

    private :
        friend std::ifstream & operator >> (std::ifstream &in, Alphabet &input)
        {
            input.len = 0;
            while (in >> input.word[input.len])
                ++input.len;
            return in;
        }

} input;

class Character
{
    public :
        char ch[5];
        int color[5];
        bool usedColor;

        void defaultValue()
        {
            for (int i = 0; i < 5; ++i)
            {
                ch[i] = ' ';
                color[i] = 3;
            }
            usedColor = false;
        }

        void output(int i)
        {
            if (color[i] == 1)
                std::cout << dye::on_green(ch[i]).invert();
            elif (color[i] == 2)
                std::cout << dye::on_yellow(ch[i]).invert();
            else
                std::cout << ch[i];
        }

} currLane[11500];

class Read
{
    private :
        char input[5];

        void read(int index, bool color)
        {
            if (!index)
                std::cout << " _____ _____ _____ _____ _____ \n";
            std::cout << "|     |     |     |     |     |\n|  " ;
            for (int i = 0; i < 5; ++i)
            {
                if (color)
                    currLane[index].output(i);
                else
                    std::cout << currLane[index].ch[i];
                if (i != 4)
                    std::cout << "  |  ";
            }
            std::cout << "  |\n|_____|_____|_____|_____|_____|\n";
        }

    public :
        void clearScreen()
        {
            HANDLE hOut;
            COORD Position;
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            Position.X = 0;
            Position.Y = 0;
            SetConsoleCursorPosition(hOut, Position);
        }

        void noColor(int index)
        {
            read(index, 0);
        }

        void color(int index)
        {
            for (int i = 0; i < 5 && !currLane[index].usedColor; ++i)
                currLane[index].usedColor = (currLane[index].color[i] != 3);
            read(index, 1);
        }

} readInput;

class Table
{   
    public :
        void noColor()
        {
            for (int i = 0; i <= currIndex; ++i)
                if (currLane[i].usedColor)
                    readInput.color(i);
                else
                    readInput.noColor(i);
        }

        void color()
        {
            for (int i = 0; i <= currIndex; ++i)
                readInput.color(i);
        }

} wordle;

void checkStatus()
{
    for (int i = 0; i < 25; ++i)
        ap[i] = 0;
    for (int i = 0; i < 5; ++i)
        ++ap[(int)(givenWord[i] - 65)];
    for (int i = 0; i < 5; ++i)
        if (currLane[currIndex].ch[i] == givenWord[i])
        {
            currLane[currIndex].color[i] = 1;
            --ap[(int)(currLane[currIndex].ch[i] - 65)];
        }
    for (int i = 0; i < 5; ++i)
    {
        int j = givenWord.find(currLane[currIndex].ch[i]);
        if (j != std::string::npos && j != i && ap[(int)(currLane[currIndex].ch[i] - 65)] && currLane[currIndex].color[i] == 3)
        {
            currLane[currIndex].color[i] = 2;
            --ap[(int)(currLane[currIndex].ch[i] - 65)];
        }
    }
}

char goodInput(char ch, bool &end)
{
    if (ch >= 'a' && ch <= 'z')
        return ch - 32;
    if (ch == CTRL('C'))
    {
        std::cout << "^C";
        end = true;
        return '0';
    }
    while (ch <= 'A' || ch >= 'Z' || ch == '\r' || ch == 127)
        ch = getch();
    return ch;
}

int main()
{
    int step;
    char c;
    bool end;
    inAlphabet >> input;
    givenWord = input.randomWord();
    str currWord = "";
    system("cls");
    readInput.clearScreen();
    currLane[0].defaultValue();
    wordle.noColor();
    while (currWord != givenWord && !end)
    {
        currLane[currIndex].defaultValue();
        for (int i = 0; i < 5; ++i)
        {
            end = false;
            c = goodInput(getch(), end);
            if (end)
                break;
            currLane[currIndex].ch[i] = c;
            readInput.clearScreen();
            if (i == 4)
            {
                checkStatus();
                wordle.color();
            }
            else
                wordle.noColor();
            currWord.push_back(c);
        }
        step = (binary_search(input.word, input.word + input.len, currWord));
        if (givenWord != currWord)
        {
            currWord = "";
            currLane[currIndex + step].defaultValue();
            readInput.clearScreen();
            if (step)
                ++currIndex;
            wordle.color();
        }
    }
    return 0;
}
