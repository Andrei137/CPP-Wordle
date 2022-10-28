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

#define ip input
#define str std::string
#define elif else if
#define CTRL(c) ((c) & 037)

std::ifstream inAlphabet("Alphabet.txt");

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
            while (in >> ip.word[ip.len])
                ++ip.len;
            return in;
        }
};

class Character
{
    public :
        char ch0, ch1, ch2, ch3, ch4;
        bool usedColor;
        void defaultValue()
        {
            ch0 = ch1 = ch2 = ch3 = ch4 = ' ';
            usedColor = false;
        }
        void giveCh(int i, char c)
        {
            if (!i)
                ch0 = c;
            elif (i == 1)
                ch1 = c;
            elif (i == 2)
                ch2 = c;
            elif (i == 3)
                ch3 = c;
            else
                ch4 = c;
        }
};

void cls()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

bool foundSolution(str myWord, str givenWord)
{
    return myWord == givenWord;
}

char status(char ch, str givenWord, int index)
{
    if (ch == givenWord[index])
        return '1';
    for (int i = 0; i < givenWord.length(); ++i)
        if (ch == givenWord[i])
            return '2';
    return '3';
}

void statusOutput(char status, char ch)
{
    if (status == '1')
        std::cout << dye::on_green(ch).invert();
    elif (status == '2')
        std::cout << dye::on_yellow(ch).invert();
    else
        std::cout << ch;
}

void readInputNoColor(int i, str givenWord, Character ch[])
{
    if (!i)
        std::cout << " _____ _____ _____ _____ _____ \n";
    std::cout << "|     |     |     |     |     |\n";
    std::cout << "|  " << ch[i].ch0 << "  |  " << ch[i].ch1 << "  |  " << ch[i].ch2 << "  |  " << ch[i].ch3 << "  |  " << ch[i].ch4 << "  |\n";
    std::cout << "|_____|_____|_____|_____|_____|\n";
}

void readInputWithColor(int i, str givenWord, Character ch[])
{
    Character o;
    o.ch0 = status(ch[i].ch0, givenWord, 0);
    o.ch1 = status(ch[i].ch1, givenWord, 1);
    o.ch2 = status(ch[i].ch2, givenWord, 2);
    o.ch3 = status(ch[i].ch3, givenWord, 3);
    o.ch4 = status(ch[i].ch4, givenWord, 4);
    if (o.ch0 != '3' || o.ch1 != '3' || o.ch2 != '3' || o.ch3 != '3' || o.ch4 != '3')
        ch[i].usedColor = true;
    if (!i)
        std::cout << " _____ _____ _____ _____ _____ \n";
    std::cout << "|     |     |     |     |     |\n";
    std::cout << "|  " ;
    statusOutput(o.ch0, ch[i].ch0);
    std::cout << "  |  ";
    statusOutput(o.ch1, ch[i].ch1); 
    std::cout << "  |  ";
    statusOutput(o.ch2, ch[i].ch2);
    std::cout << "  |  ";
    statusOutput(o.ch3, ch[i].ch3);
    std::cout << "  |  ";
    statusOutput(o.ch4, ch[i].ch4);
    std::cout << "  |\n";
    std::cout << "|_____|_____|_____|_____|_____|\n";
}

void wordleTableNoColor(int index, str currWord, Character ch[])
{
    for (int i = 0; i <= index; ++i)
        if (ch[i].usedColor)
            readInputWithColor(i, currWord, ch);
        else
            readInputNoColor(i, currWord, ch);
}

void wordleTableColor(int index, str currWord, Character ch[])
{
    for (int i = 0; i <= index; ++i)
        readInputWithColor(i, currWord, ch);
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
    while (ch <= 'A' || ch >= 'Z' || ch == '\r')
        ch = getch();
    return ch;
}

int binarySearch(Alphabet input, str currWord)
{
    int left = 0, right = input.len + 1, middle;
    while (left < right)
    {
        middle = left + (right - left) / 2;
        std::cout << input.word[middle] << '\n';
        if (!currWord.compare(input.word[middle]))
            return middle;
        elif (currWord.compare(input.word[middle]) > 0)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return -1;
}

int main()
{
    int index = 0, step;
    char c;
    bool end;
    Character currLane[11500];
    Alphabet input;
    inAlphabet >> input;
    str givenWord = ip.randomWord(), currWord = "";
    system("cls");
    cls();
    currLane[0].defaultValue();
    wordleTableNoColor(0, givenWord, currLane);
    while (!foundSolution(givenWord, currWord))
    {
        currLane[index].defaultValue();
        for (int i = 0; i < 5; ++i)
        {
            end = false;
            c = goodInput(getch(), end);
            if (end)
                return 0;
            currLane[index].giveCh(i, c);
            cls();
    
            if (i == 4)
                wordleTableColor(index, givenWord, currLane);
            else
                wordleTableNoColor(index, givenWord, currLane);
            currWord.push_back(c);
        }
        step = (binary_search(input.word, input.word + input.len, currWord));
        if (givenWord != currWord)
        {
            currWord = "";
            currLane[index + step].defaultValue();
            cls();
    
            wordleTableColor(index + step, givenWord, currLane);
            if (step)
                ++index;
        }
    }
    return 0;
}
