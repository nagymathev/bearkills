#include <iostream>
#include <string>
#include <vector>

class Entity;
void EnemyFight(Entity& you, Entity& enemy);
void Pickup(Entity& player, Entity& sword);

void ClearScreen()
{
    for (int i = 0; i < 20; i++)
    {
        std::cout << std::endl;
    }
}

enum class Entities
{
    Player,
    Enemy,
    Item
};

enum class GameStates
{
    Playing,
    Menu,
    DeathScreen,
    Won
};

class Entity
{
public:
    int x, y, health, damage;
    Entities entityType;
    inline static std::vector<Entity*> entities;
    Entity(int health, int damage, Entities ent, int x = 0, int y = 0)
    {
        this->health = health;
        this->damage = damage;
        this->entityType = ent;
        this->x = x;
        this->y = y;
        entities.push_back(this);
    }

    void HitTarget(Entity& target)
    {
        target.health -= this->damage;
    }
};

struct Grid
{
    bool wasRendered = false;
    void Render(int mapSizeX, int mapSizeY)
    {
        for (int row = -(mapSizeY/2); row <= mapSizeY/2; row++)
        {
            for (int col = -(mapSizeX/2); col <= mapSizeX/2; col++)
            {
                DrawEntity(row, col);
                if (!wasRendered)
                {
                    std::cout << "[ ]";
                }
                wasRendered = false;
            }
            std::cout << std::endl;
        }
    }

    void DrawEntity(int row, int col) {
        for (int e = 0; e < Entity::entities.size(); e++)
        {
            if (Entity::entities[e]->x == col && Entity::entities[e]->y == row && !wasRendered)
            {
                switch (Entity::entities[e]->entityType)
                {
                    case Entities::Player:
                        std::cout << "[P]";
                        break;
                    case Entities::Enemy:
                        std::cout << "[E]";
                        break;
                    case Entities::Item:
                        std::cout << "[?]";
                        break;
                }
                wasRendered = true;
            }
        }
    }
};

void Playing(bool& gameExit, GameStates& gameState)
{
    Entity player(10, 1, Entities::Player);
    Entity enemy(15, 4, Entities::Enemy, 3, 3);
    Entity sword(0, 9, Entities::Item, -3, -4);

    Grid grid;

    while (!gameExit)
    {
        if (player.x == enemy.x && player.y == enemy.y)
        {
            EnemyFight(player, enemy);
            if (player.health <= 0)
            {
                gameState = GameStates::DeathScreen;
                break;
            }
            else if (enemy.health <= 0)
            {
                gameState = GameStates::Won;
                break;
            }
        }

        if (player.x == sword.x && player.y == sword.y)
        {
            Pickup(player, sword);
        }

        ClearScreen();
        grid.Render(10, 10);
        std::cout << "X: " << player.x << " Y: " << player.y << std::endl;

        std::cout << "[W] Go up" << std::endl;
        std::cout << "[S] Go down" << std::endl;
        std::cout << "[A] Go left" << std::endl;
        std::cout << "[D] Go right" << std::endl;
        std::cout << "[Q] Quit" << std::endl;

        std::string direction;
        std::cin >> direction;
        if (direction == "w")
        {
            player.y--;
        }
        else if (direction == "s")
        {
            player.y++;
        }
        else if (direction == "a")
        {
            player.x--;
        }
        else if (direction == "d")
        {
            player.x++;
        }
        else if (direction == "q")
        {
            gameState = GameStates::Menu;
            break;
        }
        else
        {
            continue;
        }
    }

}

void EnemyFight(Entity& you, Entity& enemy)
{
    bool fighting = true;
    while (fighting)
    {
        if (you.health <= 0 || enemy.health <= 0)
        {
            break;
        }

        ClearScreen();

        std::cout << R"(
 .-""-.          ( )-"```"-( )          .-""-.
/ O O  \          /         \          /  O O \
|O .-.  \        /   0 _ 0   \        /  .-. O|
\ (   )  '.    _|     (_)     |     .'  (   ) /
 '.`-'     '-./ |             |`\.-'     '-'.'
   \         |  \   \     /   /  |         /
    \        \   '.  '._.'  .'   /        /
     \        '.   `'-----'`   .'        /
      \   .'    '-._        .-'\   '.   /
       |/`          `'''''')    )    `\|
       /                  (    (      ,\
      ;                    \    '-..-'/ ;
      |                     '.       /  |
      |                       `'---'`   |
      ;                                 ;
       \                               /
        `.                           .'
          '-._                   _.-'
           __/`"  '  - - -  ' "`` \__
         /`            /^\           `\
         \(          .'   '.         )/
          '.(__(__.-'       '.__)__).'
        )" << std::endl;
        std::cout << "Oh NO! You crossed paths with a BIG ANGRY BEAR!" << std::endl;
        std::cout << "What are you gonna do?" << std::endl;
        std::cout << "Your health: " << you.health << std::endl;
        std::cout << "Enemy health: " << enemy.health << std::endl;
        std::cout << "[1] Attack" << std::endl;
        std::cout << "[2] Say hi" << std::endl;

        std::string choice;
        std::cin >> choice;


        if (choice == "1")
        {
            you.HitTarget(enemy);
            enemy.HitTarget(you);
        }
        else if (choice == "2")
        {
            enemy.HitTarget(you);
        }
    }
}

void Pickup(Entity& player, Entity& sword)
{
    while(true)
    {
        ClearScreen();

        std::cout << R"(
              />
 (           //------------------------------------------------------(
(*)OXOXOXOXO(*>                  --------                             \
 (           \\--------------------------------------------------------)
              \>
        )" << std::endl;

        std::cout << "Woah, you found a sword!" << std::endl;
        std::cout << "[1] Pick up" << std::endl;
        std::cout << "[2] Leave it" << std::endl;

        std::string choice;
        std::cin >> choice;

        if (choice == "1")
        {
            player.damage = sword.damage;
            for (int i = 0; i < Entity::entities.size(); i++)
            {
                if (Entity::entities[i] == &sword)
                {
                    Entity::entities.erase(Entity::entities.begin() + i);
                }
            }
            break;
        }
        else
        {
            break;
        }
    }
}

void Menu(bool& gameExit, GameStates& gameState)
{
    while (!gameExit)
    {
        ClearScreen();

        std::cout << R"(
_________                  .__     ________                           ____ ________  ________
\_   ___ \   ____    ____  |  |   /  _____/ _____     _____    ____  /_   |\_____  \ \_____  \
/    \  \/  /  _ \  /  _ \ |  |  /   \  ___ \__  \   /     \ _/ __ \  |   | /  ____/   _(__  <
\     \____(  <_> )(  <_> )|  |__\    \_\  \ / __ \_|  Y Y  \\  ___/  |   |/       \  /       \
 \______  / \____/  \____/ |____/ \______  /(____  /|__|_|  / \___  > |___|\_______ \/______  /
        \/                               \/      \/       \/      \/               \/       \/
        )" << std::endl;

        std::cout << "Select what you want to do: " << std::endl;
        std::cout << "[1] Play Game" << std::endl;
        std::cout << "[2] Quit" << std::endl;

        std::string choice;
        std::cin >> choice;
        if (choice == "1")
        {
            gameState = GameStates::Playing;
            break;
        }
        else if (choice == "2")
        {
            gameExit = true;
        }
        else
        {
            continue;
        }
    }
}

void Won(bool& gameExit, GameStates& gameState)
{
    while (!gameExit)
    {
        ClearScreen();

        std::cout << R"(
                                    o
                                   $""$o
                                  $"  $$
                                   $$$$
                                   o "$o
                                  o"  "$
             oo"$$$"  oo$"$ooo   o$    "$    ooo"$oo  $$$"o
o o o o    oo"  o"      "o    $$o$"     o o$""  o$      "$  "oo   o o o o
"$o   ""$$$"   $$         $      "   o   ""    o"         $   "o$$"    o$$
  ""o       o  $          $"       $$$$$       o          $  ooo     o""
     "o   $$$$o $o       o$        $$$$$"       $o        " $$$$   o"
      ""o $$$$o  oo o  o$"         $$$$$"        "o o o o"  "$$$  $
        "" "$"     """""            ""$"            """      """ "
         "oooooooooooooooooooooooooooooooooooooooooooooooooooooo$
          "$$$$"$$$$" $$$$$$$"$$$$$$ " "$$$$$"$$$$$$"  $$$""$$$$
           $$$oo$$$$   $$$$$$o$$$$$$o" $$$$$$$$$$$$$$ o$$$$o$$$"
           $"""""""""""""""""""""""""""""""""""""""""""""""""""$
           $"                                                 "$
           $"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$"$
        )" << std::endl;

        std::cout << "You WON!" << std::endl;
        std::cout << "[1] Menu" << std::endl;
        std::cout << "[2] Quit" << std::endl;

        std::string choice;
        std::cin >> choice;

        if (choice == "1")
        {
            gameState = GameStates::Menu;
            break;
        }
        else if (choice == "2")
        {
            gameExit = true;
            break;
        }
    }
}

void DeathScreen(bool& gameExit, GameStates& gameState)
{
    while (!gameExit)
    {
        ClearScreen();

        std::cout << R"(
_____.___.                ________   .__             .___
\__  |   |  ____   __ __  \______ \  |__|  ____    __| _/
 /   |   | /  _ \ |  |  \  |    |  \ |  |_/ __ \  / __ |
 \____   |(  <_> )|  |  /  |    `   \|  |\  ___/ / /_/ |
 / ______| \____/ |____/  /_______  /|__| \___  >\____ |
 \/                               \/          \/      \/
        )" << std::endl;

        std::cout << "[1] Menu" << std::endl;
        std::cout << "[2] Quit" << std::endl;

        std::string choice;
        std::cin >> choice;

        if (choice == "1")
        {
            gameState = GameStates::Menu;
            break;
        }
        else if (choice == "2")
        {
            gameExit = true;
            break;
        }
    }
}

int main() {
    bool gameExit = false;
    GameStates gameState = GameStates::Menu;
    while (!gameExit)
    {
        switch (gameState) {
            case GameStates::Playing:
                Playing(gameExit, gameState);
                break;
            case GameStates::Menu:
                Menu(gameExit, gameState);
                break;
            case GameStates::DeathScreen:
                DeathScreen(gameExit, gameState);
                break;
            case GameStates::Won:
                Won(gameExit, gameState);
                break;
        }
    }

    return 0;
}
