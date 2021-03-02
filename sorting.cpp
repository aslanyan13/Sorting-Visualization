#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int LINE_WIDTH = 2;

RenderWindow window (VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting visualization");

// Sound frequency play (experimental)
void freqPlay (float freq, float duration) {
    const unsigned SAMPLES = 44100;
    const unsigned SAMPLE_RATE = 44100;
    const unsigned AMPLITUDE = 30000;

    sf::Int16 raw[SAMPLES];

    const double TWO_PI = 6.28318;
    const double increment = freq/44100;
    double x = 0;
    for (unsigned i = 0; i < SAMPLES; i++) {
        raw[i] = AMPLITUDE * sin(x * TWO_PI);
        x += increment;
    }

    sf::SoundBuffer Buffer;
    if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
        std::cerr << "Loading failed!" << std::endl;
        return;
    }

    sf::Sound Sound;
    Sound.setBuffer(Buffer);
    Sound.setLoop(true);
    Sound.play();

    sf::sleep(sf::milliseconds(duration));
    Sound.stop();
}

void drawArray(float * arr, int arrN, vector <int> active) {
    window.clear();

    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
    }

    for (int i = 0; i < arrN; i++)
    {
        RectangleShape line;
        line.setSize(Vector2f(LINE_WIDTH, arr[i]));
        line.setPosition(i * LINE_WIDTH, (WINDOW_HEIGHT - arr[i]) / 2);

        if (count(active.begin(), active.end(), i)) {
            // freqPlay(arr[i] * 20, 1);
            line.setFillColor(Color::White);
        } else {
            int red = arr[i]/(arrN + 20) * 255;
            int green = 255 - arr[i]/(arrN + 20) * 255;
            line.setFillColor(Color(red, green, 0));
        }

        window.draw(line);
    }

    window.display();
}
void drawArray(float * arr, int arrN) {
    window.clear();

    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
    }

    for (int i = 0; i < arrN; i++)
    {
        RectangleShape line;
        line.setSize(Vector2f(LINE_WIDTH, arr[i]));
        line.setPosition(i * LINE_WIDTH, (WINDOW_HEIGHT - arr[i]) / 2);

        int red = arr[i]/(arrN + 20) * 255;
        int green = 255 - arr[i]/(arrN + 20) * 255;
        line.setFillColor(Color(red, green, 0));

        window.draw(line);
    }

    window.display();
}

void arrayShuffle (float * arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        int left = rand() % n;
        int right = rand() % n;

        swap(arr[left], arr[right]);

        vector <int> tmp = {left, right};

        drawArray(arr, n, tmp);
    }

    drawArray(arr, n);
}

void combSorting (float * arr, int n) {
    int index = n - 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - index; j++)
        {
            if (arr[j] > arr[j + index]) {
                swap(arr[j], arr[j + index]);

                vector <int> tmp = {j, j + index};
                drawArray(arr, n, tmp);
                // drawArray(arr, n, j + 1);
            }
        }
        index /= 1.247;
    }
    drawArray(arr, n);
}
void bubbleSorting (float * arr, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);

                vector <int> tmp = {j, j + 1};
                drawArray(arr, n, tmp);
                // drawArray(arr, n, j + 1);
            }
        }
    drawArray(arr, n);
}

int main()
{
    srand(time(NULL));

    int arrN = WINDOW_WIDTH / LINE_WIDTH;
    float * arr = new float[arrN];

    for (int i = 0; i < arrN; i++)
        arr[i] = i + 20;

    arrayShuffle(arr, arrN);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Num1))
            bubbleSorting(arr, arrN);
        if (Keyboard::isKeyPressed(Keyboard::Num2))
            combSorting(arr, arrN);

        if (Keyboard::isKeyPressed(Keyboard::R))
            arrayShuffle(arr, arrN);
    }

    return 0;
}
