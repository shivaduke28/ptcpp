#include <iostream>
#include <cmath>
#include <random>

inline double f(double x, double y)
{
    return x * x + y * y;
}

int main()
{
    const int N = 10000;
    double sum = 0;

    // 非決定論的な乱数生成エンジン
    std::random_device rnd_dev;
    std::mt19937 mt(rnd_dev());
    std::uniform_real_distribution<> dist(0, 1);

    auto rnd = [&dist, &mt]() -> double
    {
        return dist(mt);
    };

    for (int i = 0; i < N; i++)
    {
        double x = rnd();
        double y = rnd();
        sum += f(x, y);
    }

    sum /= N;
    std::cout << sum << std::endl;
    return 0;
}