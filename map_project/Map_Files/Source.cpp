#include <SFML/Graphics.hpp> 
#include <iostream> 
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#define gravity 35
using namespace std;
using namespace sf;

const int screen_bound1 = 1280;
const int screen_bound2 = 720;
RenderWindow window(VideoMode(screen_bound1, screen_bound2), "Space map");

RectangleShape player(Vector2f(30.f, 60.f));
RectangleShape player2(Vector2f(30.f, 60.f));
RectangleShape weapon1(Vector2f(20.f, 5.f));
RectangleShape weapon2(Vector2f(20.f, 5.f));
RectangleShape weapon3(Vector2f(20.f, 5.f));
RectangleShape weapon4(Vector2f(20.f, 5.f));

Sprite blocks3; Sprite obs;

const int MAX_BLOCKS = 50;
Sprite finalground[MAX_BLOCKS];
Sprite finalblock[MAX_BLOCKS];
Sprite finalobs[MAX_BLOCKS];
int groundCount = 0;
int blockCount = 0;
int obsCount = 0;

Sprite backgroundmap3;
Texture backgroundtexture;

Texture mapblock_Texture;
Texture obstacels_texture;

RectangleShape playercollider1(Vector2f(28, 46));
RectangleShape playercollider2(Vector2f(28, 46));
Music backgroundmusic;

void start();
void update();
void draw();
void map_groundd();
void block_position(int x, int y);
void obstacels_position(int x, int y);
void map_positions();
void handleCollision(RectangleShape& playerCOLLIDER, Sprite& obj, RectangleShape& duck);
void collision(RectangleShape& playercollider, RectangleShape& duck);
void handle_BulletCollision3(Sprite& obj);
void weapon_handleCollision3(RectangleShape& weapon, Sprite& obj);
void collision_Weapon(RectangleShape& weapon);
void windowcollison(RectangleShape& player);
void weapon_position();

float deltaTime;
float speed = 150.f;


int main()
{
    window.setFramerateLimit(60);
    start();
    Clock clock; // measures time between frames for smooth motion.
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        update();
        draw();
    }
    return 0;
}

void start()
{

    backgroundtexture.loadFromFile("Background/background.jpg");
    mapblock_Texture.loadFromFile("blocks & platform/cccc.png");
    obstacels_texture.loadFromFile("blocks & platform/Block 2.png");


    backgroundmap3.setTexture(backgroundtexture);
    backgroundmap3.setScale(0.5, 0.5);

    blocks3.setTexture(mapblock_Texture);
    blocks3.setScale(0.45, 0.17);

    obs.setTexture(obstacels_texture);
    obs.setScale(0.035, 0.035);

    player.setPosition(180, 250);
    player.setPosition(530, 660);

    backgroundmusic.openFromFile("backgroundmusic.mp3");
    backgroundmusic.setLoop(true);
    backgroundmusic.setVolume(60);
    backgroundmusic.play();



    map_positions();
    weapon_position();


}

void update()
{
    player.move(0, gravity * deltaTime);
    player2.move(0, gravity * deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.move(-speed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.move(speed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.move(0.f, -speed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.move(0.f, speed * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player2.move(-speed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player2.move(speed * deltaTime, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player2.move(0.f, -speed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player2.move(0.f, speed * deltaTime);
    }

    windowcollison(player);
    windowcollison(player2);
    collision(playercollider1, player);
    collision(playercollider2, player2);

    // BulletCollision();

    collision_Weapon(weapon1);
    collision_Weapon(weapon2);
    collision_Weapon(weapon3);
    collision_Weapon(weapon4);

}

void draw()
{
    window.clear();

    window.draw(backgroundmap3);

    for (int i = 0; i < groundCount; i++)
        window.draw(finalground[i]);

    for (int i = 0; i < blockCount; i++)
        window.draw(finalblock[i]);

    for (int i = 0; i < obsCount; i++)
        window.draw(finalobs[i]);


    window.display();
}
void map_groundd()
{
    for (int i = 0; i < 10; i++)
    {

        blocks3.setPosition(i * 150, 697);
        finalground[groundCount++] = blocks3;
    }
}
void block_position(int x, int y)
{

    blocks3.setPosition(x, y);
    finalblock[blockCount++] = blocks3;

}
void obstacels_position(int x, int y)
{
    obs.setPosition(x, y);
    finalobs[obsCount++] = obs;

}
void map_positions()
{
    map_groundd();

    block_position(295, 545);
    block_position(0, 595);
    block_position(1060, 465);
    block_position(0, 470);
    block_position(190, 370);
    block_position(400, 370);
    block_position(610, 370);
    block_position(820, 370);
    block_position(1058, 620);
    block_position(800, 540);
    block_position(0, 270);
    block_position(1050, 270);
    block_position(290, 200);
    block_position(550, 130);
    block_position(800, 180);

    obstacels_position(265, 332);
    obstacels_position(300, 332);
    obstacels_position(700, 619);
    obstacels_position(661, 658);
    obstacels_position(700, 658);
    obstacels_position(98, 431);
    obstacels_position(1100, 390);
    obstacels_position(1100, 427);
    obstacels_position(950, 331);
    obstacels_position(460, 161);
}

void handleCollision(RectangleShape& playerCOLLIDER, Sprite& obj, RectangleShape& duck)
{
    FloatRect duckBounds = playerCOLLIDER.getGlobalBounds();
    FloatRect objBounds = obj.getGlobalBounds();

    if (duckBounds.intersects(objBounds))
    {
        FloatRect intersection;
        duckBounds.intersects(objBounds, intersection);

        if (intersection.width < intersection.height)
        {
            if (duckBounds.left < objBounds.left)
                duck.setPosition(duck.getPosition().x - intersection.width, duck.getPosition().y);
            else
                duck.setPosition(duck.getPosition().x + intersection.width, duck.getPosition().y);
        }
        else
        {
            if (duckBounds.top < objBounds.top)
            {
                /*if (duck.velocityY < 0) {
                    return;
                }*/
                duck.setPosition(duck.getPosition().x, obj.getPosition().y + 1.f);
                /*duck.onGround = 1;
                duck.isJumping = 0;
                duck.velocityY = 0;*/
            }
            else {
                /*if (duck.velocityY > 0) {
                    return;
                }*/
                //duck.velocityY = 0;
                duck.setPosition(duck.getPosition().x, duck.getPosition().y + intersection.height);
            }
        }
    }
}
void collision(RectangleShape& playercollider, RectangleShape& duck)
{
    for (int i = 0; i < blockCount; i++)
        handleCollision(playercollider, finalblock[i], duck);

    for (int i = 0; i < groundCount; i++)
        handleCollision(playercollider, finalground[i], duck);

    for (int i = 0; i < obsCount; i++)
        handleCollision(playercollider, finalobs[i], duck);
}
//void handle_BulletCollision3(Sprite& obj)
//{
//    for (int i = bulls.size() - 1;i >= 0; i--)
//    {
//        if (bulls[i].bullet.getGlobalBounds().intersects(obj.getGlobalBounds()))
//        {
//
//            bulls.erase(i);
//            break;
//
//        }
//
//    }
//}
void weapon_handleCollision3(RectangleShape& weapon, Sprite& obj)
{
    //if (weapon.type == "grenade" && weapon.boom) return;
    FloatRect weapBounds = weapon.getGlobalBounds();
    FloatRect objBounds = obj.getGlobalBounds();

    if (weapBounds.intersects(objBounds))
    {
        FloatRect intersection;
        weapBounds.intersects(objBounds, intersection);

        if (intersection.width < intersection.height)
        {
            if (weapBounds.left < objBounds.left)
                weapon.setPosition(weapon.getPosition().x - intersection.width, weapon.getPosition().y);
            else
                weapon.setPosition(weapon.getPosition().x + intersection.width, weapon.getPosition().y);
            // weapon.velocityX = 0;
        }

        else {
            if (weapBounds.top < objBounds.top)
            {
                weapon.setPosition(weapon.getPosition().x, obj.getPosition().y + 1.f);
                /*weapon.velocityY = 0;
                weapon.velocityX = 0;
                weapon.onGround = true;*/

            }
            else
            {
                weapon.setPosition(weapon.getPosition().x, weapon.getPosition().y + intersection.height);
                //weapon.velocityY = 0;
            }

        }
    }
}
void collision_Weapon(RectangleShape& weapon)
{
    for (int i = 0; i < blockCount; i++)
        weapon_handleCollision3(weapon, finalblock[i]);
    for (int i = 0; i < groundCount; i++)
        weapon_handleCollision3(weapon, finalground[i]);
    for (int i = 0; i < obsCount; i++)
        weapon_handleCollision3(weapon, finalobs[i]);
}
void windowcollison(RectangleShape& player)
{

    FloatRect playerBounds = player.getGlobalBounds();

    if (playerBounds.left < 0) // Left
        player.setPosition(playerBounds.width / 2, player.getPosition().y);

    if (playerBounds.left + playerBounds.width > screen_bound1) // Right
        player.setPosition(screen_bound1 - playerBounds.width / 2, player.getPosition().y);

    if (playerBounds.top < 0)// Top
        player.setPosition(player.getPosition().x, playerBounds.height / 2);

}
void weapon_position()
{

    weapon1.setFillColor(Color::Cyan);
    weapon1.setPosition(60, 466);

    weapon2.setFillColor(Color::Red);
    weapon2.setPosition(1240, 460);

    weapon3.setFillColor(Color::Yellow);
    weapon3.setPosition(750, 326);

    weapon4.setFillColor(Color::Green);
    weapon4.setPosition(875, 535);

}