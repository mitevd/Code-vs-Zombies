#include <iostream>
#include <vector>
#include <cmath>
#include <map>


constexpr int wide = 16000;
constexpr int high = 9000;


struct Player {
    int x;
    int y;
    int futureX = x;
    int futureY = y;
};

struct Zombie {
    int id;
    int x;
    int y;
    int futureX;
    int futureY;
};

struct Human {
    int id;
    int x;
    int y;
};

// Calculate distance between two objects
int calculateDistance(const int& x1, const int& y1, const int& x2, const int& y2) {
    const int d1 = x2 - x1;
    const int d2 = y2 - y1;
    const int distance = std::floor(sqrt(d1*d1 + d2*d2));

    return distance;
}

void readInput(Player& ash, std::vector<Human>& humans, std::vector<Zombie>& zombies) {
    std::cin >> ash.x >> ash.y; std::cin.ignore();

    int humansCount;
    std::cin >> humansCount; std::cin.ignore();
    for (int i = 0; i < humansCount; i++) {
        Human currentHuman;
        std::cin >> currentHuman.id >> currentHuman.x >> currentHuman.y; std::cin.ignore();
        humans.push_back(currentHuman);
    }

    int zombiesCount;
    std::cin >> zombiesCount; std::cin.ignore();
    for (int i = 0; i < zombiesCount; i++) {
        Zombie currentZombie;
        std::cin >> currentZombie.id >> currentZombie.x >> currentZombie.y >> currentZombie.futureX >> currentZombie.futureY; std::cin.ignore();
        zombies.push_back(currentZombie);
    }

}

Zombie findZombie(const std::vector<Zombie>& zombies, const Human& human) {
    for (auto& zombie : zombies) {
        if (zombie.futureX == human.x && zombie.futureY == human.y) {
            return zombie;
        }
    }
    return Zombie{};
}

bool checkZombie(const std::vector<Zombie>& zombies, const Human& human) {
    for (auto& zombie : zombies) {
        if (zombie.futureX == human.x && zombie.futureY == human.y) {
            return true;
        }
    }
    return false;
}

bool ashBeFirst(const std::pair<int, Human>& distanceToHuman, const std::map<int,int>::iterator& distanceToZombie) {
    int ashTurns = distanceToHuman.first / 1000;
    if (distanceToHuman.first % 1000 != 0) {
        ashTurns += 1;
    }
    int zombieTurns = distanceToZombie->second / 400;
    if (distanceToZombie->second % 400 != 0) {
        zombieTurns += 1;
    }
    if (std::ceil(ashTurns <= zombieTurns)) {
        return true;
    }
    return false;
}

void moveAsh(Player& ash,const std::vector<Human>& humans, const std::vector<Zombie>& zombies) {
    std::map<int, Human> distancesToHumans;
    std::map<int, int> distancesZombiesToHumans;
    Zombie zombieHunter{};

    for (auto& human : humans) {
        if (checkZombie(zombies, human)) {
            zombieHunter = findZombie(zombies, human);
        }
        int distanceToHuman = calculateDistance(ash.x, ash.y, human.x, human.y);
        // increase the destance, because of the gun range
        distanceToHuman -= 2000;
        int distanceZombieToHuman = calculateDistance(zombieHunter.x, zombieHunter.y, human.x, human.y);
       
        distancesToHumans[distanceToHuman] = human;
        distancesZombiesToHumans[human.id] = distanceZombieToHuman;
    }
    for (auto& distance : distancesToHumans) {
        auto it = distancesZombiesToHumans.find(distance.second.id);
        if ( it != distancesZombiesToHumans.end()) {
            if (ashBeFirst(distance, it)) {
                ash.futureX = distance.second.x;
                ash.futureY = distance.second.y;
                break;
            }
        }
    }
}

void mainLoop() {
    while (true){
        Player ash;
        std::vector<Human> humans;
        std::vector<Zombie> zombies;
        readInput(ash, humans, zombies);
        moveAsh(ash, humans, zombies);
        std::cout << ash.futureX << ' ' << ash.futureY << '\n';

    }
    
}

int main() {
    mainLoop();

    return 0;
}