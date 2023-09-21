
#include <iostream>
#include <cstdlib>
#include <ctime>

// Класс Игрок
class Player 
{
protected:
    int hp;
    int attackMin;
    int attackMax;
    int def;
    std::string skillName;
    int skillDamage;

public:
    Player(int hp, int attackMin, int attackMax, int def, std::string skillName, int skillDamage)
        : hp(hp), attackMin(attackMin), attackMax(attackMax), def(def), skillName(skillName), skillDamage(skillDamage) {}

    virtual ~Player() {}

    int getHP() const 
    {
        return hp;
    }

    void setHP(int newHP) 
    {
        hp = newHP;
    }

    int getAttack() const 
    {
        return rand() % (attackMax - attackMin + 1) + attackMin;
    }

    int getDef() const 
    {
        return def;
    }

    std::string getSkillName() const 
    {
        return skillName;
    }

    int getSkillDamage() const 
    {
        return skillDamage;
    }

    virtual void useSkill() const = 0;
};

// Персонаж Человек
class Human : public Player 
{
public:
    Human() : Player(120, 8, 12, 3, "Sword Dance", 8) {}

    void useSkill() const override
    {
        std::cout << "Activating Special Human Skill: [" << skillName << "]. You caused " << skillDamage << " extra damage." << std::endl;
    }
};

// Персонаж Эльф
class Elf : public Player
{
public:
    Elf() : Player(100, 7, 10, 5, "Arrow of Saggetarius", 10) {}

    void useSkill() const override
    {
        std::cout << "Activating Special Elf Skill: [" << skillName << "]. You caused " << skillDamage << " extra damage." << std::endl;
    }
};

// Персонаж Орк
class Orc : public Player
{
public:
    Orc() : Player(140, 10, 14, 0, "Battle Axe", 7) {}

    void useSkill() const override
    {
        std::cout << "Activating Special Orc Skill: [" << skillName << "]. You caused " << skillDamage << " extra damage." << std::endl;
    }

};

// Класс Игра
class Game 
{
private:
    Player* player1;
    Player* player2;
    int round;
    int currentPlayer;

    Game() : round(1), currentPlayer(1)
    {
        // Инициализация игроков
        player1 = nullptr;
        player2 = nullptr;
    }

    ~Game()
    {
        delete player1;
        delete player2;
    }

public:
    static Game& getInstance() 
    {
        static Game instance;
        return instance;
    }

    void choosePlayers()
    {
        std::cout << "Choose character for Player1 (1 - Human, 2 - Elf, 3 - Orc): ";
        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
            player1 = new Human();
        }
        else if (choice == 2)
        {
            player1 = new Elf();
        }
        else if (choice == 3)
        {
            player1 = new Orc();
        }
        else
        {
            std::cout << "Incorrect. Try again." << std::endl;
            choosePlayers();
            return;
        }
        std::cout << "Choose character for Player2 (1 - Human, 2 - Elf, 3 - Orc): ";
        std::cin >> choice;

        if (choice == 1)
        {
            player2 = new Human();
        }
        else if (choice == 2)
        {
            player2 = new Elf();
        }
        else if (choice == 3)
        {
            player2 = new Orc();
        }
        else
        {
            std::cout << "Incorrect. Try again." << std::endl;
            choosePlayers();
            return;
        }
    }
    //Информация о текущем раунде
    void ShowRoundInfo(int roundN, int curP)
    {
        std::cout << "===========================" << std::endl;
        std::cout << "Round " << roundN << std::endl;
        std::cout << "Turn of Player " << curP << std::endl;
    }
    //Текущее ХП игроков
    void ShowHP(int p1, int p2)
    {
        std::cout << "HP Player 1: " << p1 << ", HP Player 2: " << p2 << std::endl;
    }
    void DiceInfo(int currP, int dValue)
    {
        std::cout << "Player " << currP << "  rolled the dice and got  " << dValue << std::endl;
    }
    void CoinInfo(int pTurn)
    {
        std::cout << "===========================" << std::endl;
        std::cout << "Player " << pTurn << " goes first." << std::endl;
    }
    void AttackState(int dmg)
    {
        if (dmg != 0) std::cout << "The attack was successful! You caused " << dmg << " damage." << std::endl;
        else std::cout << "The attack failed. You caused " << dmg << " damage" << std::endl;
    }
    void flipCoin()
    {
        srand(time(0));
        currentPlayer = rand() % 2 + 1;
        CoinInfo(currentPlayer);
    }

    void playRound()
    {

        ShowRoundInfo(round, currentPlayer);
        ShowHP(player1->getHP(), player2->getHP());
        std::cout << "Input $ to roll the dice: ";
        char input;
        std::cin >> input;

        if (input == '$')
        {
            int diceValue = rand() % 6 + 1;
            DiceInfo(currentPlayer, diceValue);
            Player* attackingPlayer = (currentPlayer == 1) ? player1 : player2;
            Player* defendingPlayer = (currentPlayer == 1) ? player2 : player1;

            if (diceValue >= 3)
            {
                int damage = attackingPlayer->getAttack() - defendingPlayer->getDef();
                defendingPlayer->setHP(defendingPlayer->getHP() - damage);
                AttackState(damage);

                if (diceValue >= 5)
                {
                    attackingPlayer->useSkill();
                    defendingPlayer->setHP(defendingPlayer->getHP() - attackingPlayer->getSkillDamage());
                }
            }
            else
            {
                int fdamage = 0;
                AttackState(fdamage);
            }

            if (defendingPlayer->getHP() <= 0)
            {
                std::cout << "Player " << currentPlayer << " won!" << std::endl;
                return;
            }

            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            round++;
            playRound();
        }
        else
        {
            std::cout << "Incorrect! Try again" << std::endl;
            playRound();
        }
    }

    void startGame()
    {
        std::cout << "Game started!" << std::endl;
        choosePlayers();
        flipCoin();
        playRound();
    }
};

int main() 
{
    Game& game = Game::getInstance();
    game.startGame();

    return 0;
}

