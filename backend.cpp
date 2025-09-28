#include "backend.h"

using dist = std::uniform_int_distribution<int>;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

EnemiesDragon::~EnemiesDragon() {}
std::string EnemiesDragon::set_answer() {
    throw std::runtime_error("set_answer() must be overridden");
}

std::string EnemiesDragon::question() {
    return quest_;
}

bool EnemiesDragon::check_answer(int user_answer) {
    return user_answer == answer_;
}

std::string GreenDragon::set_answer() {
    int a = dist(0, 99)(rng);
    int b = dist(0, 99)(rng);
    answer_ = a + b;
    quest_ = std::to_string(a) + " + " + std::to_string(b) + " = ?";
    return quest_;
}

std::string RedDragon::set_answer() {
    int a = dist(0, 99)(rng);
    int b = dist(0, a)(rng);
    answer_ = a - b;
    quest_ = std::to_string(a) + " - " + std::to_string(b) + " = ?";
    return quest_;
}

std::string BlueDragon::set_answer() {
    int a = dist(1, 10)(rng);
    int b = dist(1, 10)(rng);
    answer_ = a * b;
    quest_ = std::to_string(a) + " * " + std::to_string(b) + " = ?";
    return quest_;
}
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

std::vector<int> prime_factors(int n) {
    std::vector<int> factors;
    int divisor = 2;
    while (divisor * divisor <= n) {
        if (n % divisor == 0) {
            factors.push_back(divisor);
            while(n % divisor == 0){
                n /= divisor;
            }
        } else {
            divisor++;
        }
    }
    if(n != 1){
        factors.push_back(n);
    }
    return factors;
}
Troll::~Troll() {}

std::string MountainTroll::set_answer() {
    int k = dist(2,10)(rng);
    answer_ = dist(1, k)(rng);
    quest_ = "Guess a number between 1 and " + std::to_string(k) + ": ";
    return quest_;
}

std::string IceTroll::set_answer() {
    int num = dist(2, 100)(rng);
    answer_ = is_prime(num) ? 1 : 0;
    quest_ = "Is " + std::to_string(num) + " prime? (1 for yes, 0 for no): ";
    return quest_;
}

std::string DesertTroll::set_answer() {
    int num = dist(10, 100)(rng);
    correct_factors_ = prime_factors(num);

    std::string ss = "Factorize " ;
    ss += std::to_string(num);
    ss +=  " into prime factors (enter product mull of unique prime): ";
    quest_ = ss;
    answer_ = 1;
    for (int factor : correct_factors_) {
        answer_ *= factor;
    }

    return quest_;
}

bool DesertTroll::check_answer(int user_answer) {
    return user_answer == answer_;
}

Enemies::~Enemies() {}
