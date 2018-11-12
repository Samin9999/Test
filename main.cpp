#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include"Tilemaps.h"
#include"Variables.h"
#include<iostream>
#define Winwidth 850.0
#define Winheight 350.0
#define gravity  2
#define groundHeight  307
#define jumpheight 80.0



int highscore(float x){
	if (x + maxs>maxdistance) maxdistance = x;
	return maxdistance;
}


void makePathNull(){
	pos.x = 30.0;
	pos.y = 257.0;
	for (int i = 0; i < H; i++)
	for (int j = 0; j < W; j++)
	{
		path[i][j] = 0;
	}
}



void collision(int coy, int cox){
	if (TileMap1[coy][cox] == 'P') {
		onground = true;//   Ground Tiles
		blocked = false;
	}
	if (TileMap1[coy][cox] == 'k')  {
		onground = true;//   Air Bricks
		blocked = true;
	}
	if (TileMap1[coy][cox] == 'w')   {
		onground = false;//   Clouds
		blocked = false;
	}
	if (TileMap1[coy][cox] == 'b') {
		onground = true;//   Air Bricks
		blocked = true;
	}
	if (TileMap1[coy][cox] == 'c'){
		onground = true;//   Question marks
		blocked = true;
	}
	if (TileMap1[coy][cox] == 't')  {
		onground = true;//   Tunnels
		blocked = true;
	}
	if (TileMap1[coy][cox] == 'r')   {
		onground = true;//   Stair Bricks
		blocked = true;
	}
	if (TileMap1[coy][cox] == '0')  {
		onground = true;
		blocked = true;
	}
	if (TileMap1[coy][cox] == ' ')  {
		onground = false;
		blocked = false;
	}

}

void printMap(Sprite& tile, RectangleShape& coin, RenderWindow& window, int coy, int cox)
{
	//  Coins in here

	for (int i = 0; i < H; i++)
	for (int j = 0; j < W; j++)
	{
		//if (TileMap1[i][j] == 'u')  coins.setTextureRect(IntRect(0, 0, 50, 50));  //   Coins
		if (TileMap2[i][j] == ' ' || TileMap2[i][j] == '0' || TileMap2[i][j] == 'r') continue;
		if (TileMap2[i][j] == 'u' && path[i][j] == 0)

			coin.setPosition(j * 16, i * 16);
        if (TileMap2[i][j] == 'u' && path[i][j] == 1)
            maxscore2+=2;
		window.draw(coin);

	}

	//ForeScreen Tiles
	for (int i = 0; i < H; i++)
	for (int j = 0; j < W; j++)
	{
		if (TileMap1[i][j] == 'P')  tile.setTextureRect(IntRect(111, 112, 17, 17));  //   Ground Tiles
		if (TileMap1[i][j] == 'k')  tile.setTextureRect(IntRect(143, 112, 16, 16));  //   Air Bricks
		if (TileMap1[i][j] == 'w')  tile.setTextureRect(IntRect(99, 224, 41, 31));  //  Clouds
		if (TileMap1[i][j] == 'b')  tile.setTextureRect(IntRect(143, 128, 16, 16));  //   Air Bricks
		if (TileMap1[i][j] == 'c')  tile.setTextureRect(IntRect(127, 112, 16, 16));  //    Question marks
		if (TileMap1[i][j] == 't')  tile.setTextureRect(IntRect(0, 47, 32, 48));  //   Tunnels
		if (TileMap1[i][j] == 'r')  tile.setTextureRect(IntRect(111, 112, 16, 16));  //   Stair Bricks
		if (TileMap1[i][j] == ' ' || TileMap1[i][j] == '0' || TileMap1[i][j] == 'u') continue;

		//int k = posX / 2;
		tile.setPosition(j * 16, i * 16);
		window.draw(tile);

	}
}



void enemyMove(positionOfEnemy& enemypos, Sprite& enemy, RenderWindow& window, int k){
	int cox = enemypos.x / 16;
	int coy = enemypos.y / 16;
	collision(coy + 1, cox);
	//enemypos.y += 0.5f;
	if (!onground){
		enemypos.y += 0.5f;
	}
	if (enemypos.y > 300.0f&&enemypos.y<350){
		enemypos.y++;
	}
	//printf("%f ", enemypos.y);

	else{
		if (direction[k] == 1){
			collision(coy, cox + 1);
			enemypos.x += 0.5;
		}
		else if (direction[k] == -1){
			collision(coy, cox);
			enemypos.x -= 0.5;
		}
		if (blocked){
			direction[k] *= -1;
			//	printf("%d %d\n", k, direction[k]);
		}
	}
	//printf("%d\n", direction[k]);
	enemy.setPosition(enemypos.x, enemypos.y - 5);
	window.draw(enemy);
}
//enemy
sf::Sprite enemy[1000];
//enemy.setPosition(120, 282);
sf::Texture sa;

void saveGame(){
	FILE *savePath, *others;
	savePath = fopen("savepath.txt", "w+");
	for (int k = 0; k < H; k++){
		for (int l = 0; l < W; l++){
			fprintf(savePath, "%d", path[k][l]);
		}
		fprintf(savePath, "\n");
	}
	fclose(savePath);

	others = fopen("others.txt", "w+");
	fprintf(others, "%f %f ", pos.x, pos.y);
	fprintf(others, "%d %d %d", lives, level, maxscore2);
	fclose(others);
}

void beforeGameStarting(RenderWindow& window,Font& font,Font& font2,Vector2u& texturesize){
	sf::RectangleShape wlcm(sf::Vector2f(850.0f, 440.0f));
	sf::Texture welcome;
	welcome.loadFromFile("welcomeback.jpg");
	wlcm.setTexture(&welcome);

	sf::Clock clock;

	sf::RectangleShape ratmn(sf::Vector2f(100.0f, 210.0f));
	ratmn.setPosition(0, 30);
	sf::Texture ratmnf;
	ratmnf.loadFromFile("char.png");
	ratmn.setTexture(&ratmnf);

	sf::Text ng;//newGame

	// select the font
	ng.setFont(font); // font is a sf::Font

	ng.setPosition(550.0f, 120.0f);
	// set the string to display
	ng.setString("New Game");

	// set the character size
	ng.setCharacterSize(32); // in pixels, not points!

	// set the color
	ng.setFillColor(sf::Color::White);

	// set the text style
	ng.setStyle(sf::Text::Bold);

	sf::Text cont; //continue
	cont.setFont(font);
	cont.setPosition(550.0f, 80.0f);
	cont.setString("Continue");
	cont.setFillColor(sf::Color::White);
	cont.setStyle(sf::Text::Bold);

	sf::Text hs; //High Score
	hs.setFont(font);
	hs.setPosition(550.0f, 160.0f);
	hs.setString("Highest Score");
	hs.setStyle(sf::Text::Bold);

	sf::Text quit; //Quit
	quit.setFont(font);
	quit.setPosition(550.0f, 200.0f);
	quit.setString("Quit");
	quit.setStyle(sf::Text::Bold);

	//Welcome page content ended
	//Quit window
	sf::Text ques; //confirmation to close?
	ques.setFont(font2);
	ques.setPosition(10.0f, 20.0f);
	ques.setString("Are you sure you want to quit me?");
	ques.setFillColor(sf::Color::White);
	ques.setCharacterSize(18);

	sf::Text yes;
	yes.setFont(font2);
	yes.setPosition(10.0f, 80.0f);
	yes.setString("Yes. See you again");
	yes.setFillColor(sf::Color::White);
	yes.setCharacterSize(14);

	sf::Text no;
	no.setFont(font2);
	no.setPosition(10.0f, 110.0f);
	no.setString("Sorry, it was a mistake");
	no.setFillColor(sf::Color::White);
	no.setCharacterSize(14);
	// set the font size
	ng.setCharacterSize(32); // in pixels, not points!

	// set the color
	ng.setFillColor(sf::Color::White);
	cont.setCharacterSize(32);
	cont.setFillColor(sf::Color::White);
	hs.setCharacterSize(32);
	hs.setFillColor(sf::Color::White);
	quit.setCharacterSize(32);
	quit.setFillColor(sf::Color::White);



	ratmn.setTextureRect(sf::IntRect(texturesize.x * 10, texturesize.y * 19, texturesize.x, texturesize.y));
	ratmnf.setSmooth(true);

	sf::Vector2i localPosition = sf::Mouse::getPosition(window);//getting the position of Mouse;
	if (!gameStarted&&localPosition.y >= 165 && localPosition.y <= 197 && localPosition.x >= 550 && localPosition.x <= 730){
		hs.setCharacterSize(40);
		hs.setFillColor(sf::Color::Red);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			HighScore = true;
			return;
		}
	}
	else if (!gameStarted&&localPosition.y >= 125 && localPosition.y <= 157 && localPosition.x >= 550 && localPosition.x <= 730){
		// set the font size
		ng.setCharacterSize(40); // in pixels, not points!

		// set the color
		ng.setFillColor(sf::Color::Red);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			gameStarted = true;
			createTilemap1();
			level = 1;
			maxscore2 = 0;
			maxs=0;
			maxscore=0;
			lives = 5;
			makePathNull();
			started = true;
			for (int k = 0; k < noOfEnemy; k++){
				//printf("%d ", direction[k]);

				direction[k] = 1;
				enemy[k].setTexture(sa);
				int h = (rand() % ((864 * 16) / noOfEnemy)) + 120;
				enemypos[k].x = h;
				enemypos[k].x *= (k + 1);
				//printf("%d ", enemypos[k].x);
				//printf("%d ", h);
				enemypos[k].y = 240;
			}
		}
	}
	else if (!gameStarted&&localPosition.y >= 85 && localPosition.y <= 117 && localPosition.x >= 550 && localPosition.x <= 730){
		cont.setCharacterSize(40);
		cont.setFillColor(sf::Color::Red);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			gameStarted = true;
		}
		FILE *loadtilemap,*others;
		loadtilemap = fopen("savetilemap.txt", "r+");
		for (int i = 0; i < H; i++){
			for (int j = 0; j < W; j++){
				char c;
				fscanf(loadtilemap, "%c", &c);
				TileMap1[i][j] = c;
			}
		}
		fclose(loadtilemap);
		others = fopen("others.txt", "r+");
		fscanf(others, "%f %f", &pos.x,&pos.y);
		fscanf(others, "%d %d %d", &lives, &level, &maxscore2);
		fclose(others);
		for (int k = 0; k < noOfEnemy; k++){
			//printf("%d ", direction[k]);

			direction[k] = 1;
			enemy[k].setTexture(sa);
			int h = (rand() % ((864 * 16) / noOfEnemy)) + 120;
			enemypos[k].x = h;
			enemypos[k].x *= (k + 1);
			//printf("%d ", enemypos[k].x);
			//printf("%d ", h);
			enemypos[k].y = 240;
		}

	}

	else if (!gameStarted&&localPosition.y >= 205 && localPosition.y <= 237 && localPosition.x >= 550 && localPosition.x <= 730){
		quit.setCharacterSize(40);
		quit.setFillColor(sf::Color::Red);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			sf::RenderWindow quitWindow(sf::VideoMode(400, 200), "Quit");
			while (quitWindow.isOpen()){
				quitWindow.clear(sf::Color::Black);
				sf::Vector2i localPosition2 = sf::Mouse::getPosition(quitWindow);//getting the position of Mouse;

				if (localPosition2.x >= 10 && localPosition2.x <= 239 && localPosition2.y >= 82 && localPosition2.y <= 99){
					yes.setFillColor(sf::Color::Red);
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
						quitWindow.close();
						window.clear();
						no.setPosition(180.0f, 160.0f);
						no.setCharacterSize(64);
						no.setFillColor(sf::Color::Green);
						no.setString("   THANK YOU\n    for Playing");
						window.draw(no);
						window.display();
						clock.restart();
						while (1){
							sf::Time elapsed1 = clock.getElapsedTime();
							if (elapsed1 > t1)
								break;

						}
						window.close();
					}

				}
				else if (localPosition2.x >= 10 && localPosition2.x <= 200 && localPosition2.y >= 113 && localPosition2.y <= 130){
					no.setFillColor(sf::Color::Green);
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
						quitWindow.close();
					}
				}
				else{
					yes.setFillColor(sf::Color::White);
					no.setFillColor(sf::Color::White);
				}

				quitWindow.draw(ques);
				quitWindow.draw(yes);
				quitWindow.draw(no);
				quitWindow.display();
			}
		}
	}
	window.draw(wlcm);
	window.draw(ng);
	//if (canCont)
		window.draw(cont);
	window.draw(hs);
	window.draw(quit);
	window.draw(ratmn);
}

void typeName(RenderWindow& window, Font& font, int k,Text& text){
	window.clear();

	sf::Text name;
	name.setFont(font);
	name.setPosition(100.0f, 200.0f);
	name.setStyle(sf::Text::Bold);
	char n[100];
	std::string playerName;
	window.draw(text);
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == event.Closed){
				window.close();
			}

			if (event.type == sf::Event::TextEntered)
			{

				if (event.text.unicode > 64 && event.text.unicode<91 || event.text.unicode>96 && event.text.unicode < 123)
				{
					playerName += event.text.unicode;
					n[k] = event.text.unicode;
					k++;
				}
				name.setString(playerName);

			}
			window.draw(text);
			window.draw(name);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
				n[k] = '\0';
				FILE *highscore;
				highscore = fopen("highscore.txt", "w+");
				fprintf(highscore, "%s %d", n, maxscore2);
				fclose(highscore);
				return ;
			}

			window.display();
		}
	}
	//window.draw(enterUrName);
	return typeName(window,font,k,text);
}

void quit(RenderWindow& window,Font& font2,Clock& clock){
	sf::Text ques; //confirmation to close?
	ques.setFont(font2);
	ques.setPosition(10.0f, 20.0f);
	ques.setString("Are you sure you want to quit?");
	ques.setFillColor(sf::Color::White);
	ques.setCharacterSize(18);

	sf::Text yes;
	yes.setFont(font2);
	yes.setPosition(10.0f, 80.0f);
	yes.setString("Quit & Save");
	yes.setFillColor(sf::Color::White);
	yes.setCharacterSize(14);

	sf::Text no;
	no.setFont(font2);
	no.setPosition(10.0f, 110.0f);
	no.setString("Don't Save");
	no.setFillColor(sf::Color::White);
	no.setCharacterSize(14);
	sf::RenderWindow quitWindow(sf::VideoMode(400, 200), "Quit");
	while (quitWindow.isOpen()){
            sf::Event event;
            while (quitWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    quitWindow.close();
                }
            }
		quitWindow.clear(sf::Color::Black);
		sf::Vector2i localPosition2 = sf::Mouse::getPosition(quitWindow);//getting the position of Mouse;

		if (localPosition2.x >= 10 && localPosition2.x <= 239 && localPosition2.y >= 82 && localPosition2.y <= 99){
			yes.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				quitWindow.close();
				window.clear();
				no.setPosition(180.0f, 100.0f);
				no.setCharacterSize(64);
				no.setFillColor(sf::Color::Green);
				no.setString("   THANK YOU\n    for Playing");
				window.draw(no);
				saveGame();
				window.display();
				clock.restart();
				while (1){
					sf::Time elapsed1 = clock.getElapsedTime();
					if (elapsed1 > t1)
						break;

				}
				window.close();
			}

		}
		else if (localPosition2.x >= 10 && localPosition2.x <= 200 && localPosition2.y >= 113 && localPosition2.y <= 130){
			no.setFillColor(sf::Color::Green);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				quitWindow.close();
			}
		}
		else{
			yes.setFillColor(sf::Color::White);
			no.setFillColor(sf::Color::White);
		}

		quitWindow.draw(ques);
		quitWindow.draw(yes);
		quitWindow.draw(no);
		quitWindow.display();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(Winwidth, Winheight), "The Game");




	//createTilemap1();

	//Game Background
	sf::RectangleShape player(sf::Vector2f(Winwidth * 20, Winheight));
	player.setPosition(0.0f, 0.0f);
	sf::Texture background;
	background.loadFromFile("marioBack.bmp");
	player.setTexture(&background);
	background.setRepeated(true);

	//  Fore ground Tiles
	Texture tileSet;
	tileSet.loadFromFile("Mario_Tileset.png");
	Sprite tile(tileSet);

	//Coins
	sf::RectangleShape coins(sf::Vector2f(10.0f, 10.0f));
	sf::Texture coinT;
	coinT.loadFromFile("coins.png");
	coins.setTexture(&coinT);

	//Charcter
	sf::RectangleShape ratman(sf::Vector2f(25.0f, 25.0f));
	ratman.setPosition(pos.x, pos.y);
	sf::Texture ratmanf;
	ratmanf.loadFromFile("char.png");
	ratman.setTexture(&ratmanf);




	sa.loadFromFile("enemy.png");
	enemy[0].setTexture(sa);

    sf::RectangleShape bullet(sf::Vector2f(25.0f, 25.0f));          //Second type of enemy
    sf::Texture sa2;
	sa2.loadFromFile("bullet.png");
	bullet.setTexture(&sa2);

	sf::RectangleShape bullet2(sf::Vector2f(25.0f, 25.0f));
    sf::Texture sa3;
	sa3.loadFromFile("bullet.png");
	bullet2.setTexture(&sa3);

	for(int i=0;i<1500;i++){
        by[i]=rand()%250+20;
	}



	//Welcome Page




	tile.setPosition(0, 0);
	tileSet.loadFromFile("Mario_Tileset.png");




	sf::Vector2f velocity(sf::Vector2f(20, 20));

	//character animation of walking
	sf::Vector2u texturesize = ratmanf.getSize();
	texturesize.x /= 13;
	texturesize.y /= 21;
	int i = 0, j = 0;
	// Welcome Page Contents




	sf::Font font;
	font.loadFromFile("films.bradybun.ttf");

	sf::Font font2;
	font2.loadFromFile("ariali.ttf");




	sf::Text go; //Game over text
	go.setFont(font);
	go.setPosition(100, 100);
	go.setString("Game Over");
	go.setStyle(sf::Text::Bold);
	go.setCharacterSize(64);


	sf::Text lvlup; // Level Up text
	lvlup.setFont(font);
	lvlup.setPosition(200, 100);
	lvlup.setString("Level UP");
	lvlup.setStyle(sf::Text::Bold);
	lvlup.setCharacterSize(64);

	sf::Text hiscore3;
	hiscore3.setFont(font);
	hiscore3.setPosition(110.0f, 210.0f);
	hiscore3.setString("CONGRATS!! HIGHSCORE");
	hiscore3.setStyle(sf::Text::Bold);


	FILE *hscore;
	int highest;
	char name[100];
	for (int k = 0; k < 20; k++){
		name[k] = '\0';
	}
	hscore = fopen("highscore.txt", "r");
	//fscanf(hscore, "%s", name);
	fscanf(hscore, "%d", &highest);
	fclose(hscore);
	std::string dispscore = std::to_string(highest);
	sf::Clock clock;
	sf::Clock clock2;
	sf::Clock clock3;
    sf::Clock clock4;
    sf::Clock clock5;

	sf::View view;
	view.reset(sf::FloatRect(0, 0, Winwidth, Winheight));
	view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	sf::Vector2f position(0, 0);


	sf::SoundBuffer buffer;
	buffer.loadFromFile("soundtrack2.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	sound.setLoop(true);

	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("jump.wav");
	sf::Sound sound2;
	sound2.setBuffer(buffer2);

	sf::SoundBuffer buffer3;
	buffer3.loadFromFile("lifelost.wav");
	sf::Sound sound3;
	sound3.setBuffer(buffer3);



	sf::Text hiscore;
	hiscore.setFont(font);
	hiscore.setPosition(410.0f, 150.0f);
	hiscore.setString(dispscore);
	hiscore.setCharacterSize(64);
	hiscore.setStyle(sf::Text::Bold);



	sf::Text life;
	life.setFont(font);
	life.setStyle(sf::Text::Bold);

	sf::Text scores;
	scores.setFont(font);
	scores.setStyle(sf::Text::Bold);
	sf::Text timertext;
	timertext.setFont(font);
	timertext.setStyle(sf::Text::Bold);

	sf::Text levels;
	levels.setFont(font);
	levels.setStyle(sf::Text::Bold);

	sf::Text lifes;
	lifes.setFont(font);
	lifes.setString("LIVES :");
	lifes.setStyle(sf::Text::Bold);

	sf::Text score1;
	score1.setFont(font);
	score1.setString("Score :");
	score1.setStyle(sf::Text::Bold);

	sf::Text timer1;
	timer1.setFont(font);
	timer1.setString("Time :");
	timer1.setStyle(sf::Text::Bold);

	sf::Text level2;
	level2.setFont(font);
	level2.setString("LEVEL :");
	level2.setStyle(sf::Text::Bold);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				quit(window, font2, clock);

				window.close();
			}
		}
		window.clear();
		ratman.setTextureRect(sf::IntRect(texturesize.x * (i / 5), texturesize.y * j, texturesize.x, texturesize.y));
		if (HighScore){
			window.clear();

			sf::Text name;
			name.setFont(font);
			name.setPosition(100.0f, 100.0f);
			name.setStyle(sf::Text::Bold);

			sf::Text text;
			text.setFont(font);
			text.setPosition(200.0f, 100.0f);
			text.setStyle(sf::Text::Bold);

			char n[100];
			int score;
			FILE *highscore;
			highscore = fopen("highscore.txt", "r+");
			fscanf(highscore, "%s %d", n , &score);
			//printf("%d\n",score);
			name.setString(n);
			std::string scrstr = std::to_string(score);
			text.setString(scrstr);
			fclose(highscore);

			window.draw(text);
			window.draw(name);
		}
		else if (!gameStarted&&!HighScore){
			beforeGameStarting(window, font, font2,texturesize);
		}

		else if (gameStarted){

			//after leveling up

			maxscore = highscore(ratman.getPosition().x);

			sf::Time etime2 = clock3.getElapsedTime();
			sf::Time etime = clock2.getElapsedTime();
			sf::Time etime3 = clock4.getElapsedTime();
            sf::Time etime4 = clock4.getElapsedTime();
			int et2 = etime.asMilliseconds();
			int et = etime.asSeconds();
			int et3 = etime2.asMilliseconds();
            int et4 = etime3.asSeconds();
            int et5 = etime4.asSeconds();
            //printf("%d\n",et4);
			std::string lifestr = std::to_string(lives);
			std::string scorestr = std::to_string(maxscore2);
			std::string timerstr = std::to_string(et);
			std::string levelstr = std::to_string(level);

			if (pos.x > 860 * 16){
				level++;
				createTilemap1();
				window.draw(lvlup);
				window.display();
				clock.restart();
				while (1){
					sf::Time elapsed1 = clock.getElapsedTime();
					if (elapsed1 > t1)
						break;

				}
				maxs = maxs + maxscore;
				makePathNull();
				noOfEnemy = level * 2 + 10;
				for (int k = 0; k < noOfEnemy; k++){
					//printf("%d ", direction[k]);
					direction[k] = 1;
					enemy[k].setTexture(sa);
					enemypos[k].x = (rand() % ((864 * 16) / noOfEnemy)) + 120;
					enemypos[k].x *= (k + 1);
					printf("%d ", enemypos[k].x);
					enemypos[k].y = 220;
				}

			}
			maxscore2 = highscore(ratman.getPosition().x) + maxs-30;
			window.draw(player);


			//setview
			position.x = ratman.getPosition().x + 25 - (Winwidth / 2);
			position.y = ratman.getPosition().y + 25 - (Winheight / 2);
			if (position.x < 0) position.x = 0;
			//if (position.y < 0) position.y = 0;
			view.reset(sf::FloatRect(position.x, 0, Winwidth, Winheight));



			life.setString(lifestr);
			scores.setString(scorestr);
			timertext.setString(timerstr);
			levels.setString(levelstr);

			life.setPosition(position.x + 110.0f, 10.0f);
			scores.setPosition(position.x + 130.0f, 40.0f);
			timertext.setPosition(position.x + 490.0f, 10.0f);
			levels.setPosition(position.x + 750.0f, 10.0f);
			lifes.setPosition(position.x + 30.0f, 10.0f);
			score1.setPosition(position.x + 30.0f, 40.0f);
			timer1.setPosition(position.x + 370.0f, 10.0f);
			level2.setPosition(position.x + 670.0f, 10.0f);


			window.setView(view);


			int cox = (pos.x - 8) / 16, coy = pos.y / 16; //For using in the index of Tilemap and path;
			path[coy][cox + 1] = 1;

			if (pos.y > Winheight){
				gover = true;
				makePathNull();
			}



			//   Print all the map here.
			printMap(tile, coins, window, coy, cox);


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
				gameStarted = false;
				FILE *savePath, *others;
				savePath = fopen("savepath.txt", "w+");
				for (int k = 0; k < H; k++){
					for (int l = 0; l < W; l++){
						fprintf(savePath, "%d", path[i][j]);
					}
					fprintf(savePath, "\n");
				}
				fclose(savePath);

				others = fopen("others.txt", "w+");
				fprintf(others,"%f %f ",pos.x,pos.y);
				fprintf(others, "%d %d %d", lives,level,maxscore2);
				fclose(others);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				i++;
				j = 11;
				if (i / 5 > 8) i = 0;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
					pos.x += 10.0f;
				}
				pos.x += 2.0f;
				if (pos.x <= 1) pos.x = 1;
				int cox = pos.x / 16, coy = (pos.y - 10) / 16;
				coy++;
				cox++;
				collision(coy, cox);
				if (blocked){
					pos.x -= 2.0f;
				}

				ratman.setPosition(pos.x, pos.y - 8);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				j = 9;
				i++;
				if (i / 5 > 8) i = 0;
				int cox = (pos.x + 10) / 16, coy = (pos.y - 10) / 16;
				coy++;
				cox;
				collision(coy, cox);
				if (!blocked && ratman.getPosition().x >= 2){
					pos.x -= 2.0f;
				}


				ratman.setPosition(pos.x, pos.y - 8);

			}



			cox = (pos.x - 8) / 16, coy = pos.y / 16;
			coy++;
			cox++;
			collision(coy, cox);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (onground)){
				jump = true;
				sound2.play();
			}

			if (jump){
				pos.y -= 4.0f;
				//enemypos.y -= 0.5f;
				jumped += 2;

			}

			if (jumped >= jumpheight || (onground && jumped > 4)){

				jump = false;
				jumped = 0;
			}

			collision(coy - 1, cox);
			if (blocked){
				jump = false;
				jumped = 0;
			}
			cox = (pos.x - 8) / 16;
			coy = pos.y / 16;
			coy++;
			cox++;
			collision(coy, cox);
			if ((ratman.getPosition().y + ratman.getSize().y < groundHeight || velocity.y<0) && !onground && !jump){
				//posy += gravity;
			}
			else{
				ratman.setPosition(ratman.getPosition().x, groundHeight - ratman.getSize().y);
			}

			if (!onground){
				pos.y += 2.0f;
				//enemypos.y -= 0.5f;
				//enemypos2.y -= 0.5f;
				if (pos.y > Winheight)
					gover = true;
			}

			//printf("%f %f\n", enemypos[0].x, enemypos[1].y);
			bx=(ratman.getPosition().x/1000)*1000+1000-0.1*et3;
            if(bx<ratman.getPosition().x-500){
                clock3.restart();
                ty++;
            }
            bx=(ratman.getPosition().x/1000)*1000+1000-0.9*et3;
            bullet.setPosition(bx,by[ty]);
            bullet2.setPosition(bx-400,by[ty]-100);

			ratman.setPosition(pos.x, pos.y - 8);
			//sf::Mouse::getPosition();
			//enemy move
			if((et4)%100>2) ratman.setFillColor(sf::Color(255, 255, 255, 255));
			if((et4)%100<2) ratman.setFillColor(sf::Color(255, 255, 255, 128));
			for (int k = 0; k < noOfEnemy; k++){
				if (enemypos[k].x - pos.x<500)
					enemyMove(enemypos[k], enemy[k], window, k);
				if (enemypos[k].x == pos.x && enemypos[k].y + 1 == pos.y){
					if((et4)%100>2){
                        lives--;
                        clock4.restart();


					}
                sound3.play();
				}
			}

			if (ratman.getPosition().x < bullet.getPosition().x + 20 &&
            ratman.getPosition().x + 20 > bullet.getPosition().x &&        //Collision detection code
            ratman.getPosition().y < bullet.getPosition().y + 20 &&
            20 + ratman.getPosition().y > bullet.getPosition().y)
            {
                if((et4)%100>2){
                        lives--;
                        clock4.restart();


					}
                sound3.play();
            }
            if (ratman.getPosition().x < bullet2.getPosition().x + 20 &&
            ratman.getPosition().x + 20 > bullet2.getPosition().x &&
            ratman.getPosition().y < bullet2.getPosition().y + 20 &&
            20 + ratman.getPosition().y > bullet2.getPosition().y)
            {
                 if((et4)%100>2){
                        lives--;
                        clock4.restart();


					}
                sound3.play();
            }

			if (lives <= 0) gover = true;

			//enemyMove(enemypos[1],enemy[1], window);
			window.draw(life);
			window.draw(timertext);
			window.draw(scores);
			window.draw(lifes);
			window.draw(timer1);
			window.draw(score1);
			window.draw(levels);
			window.draw(level2);
            window.draw(bullet);
			window.draw(bullet2);
			window.draw(ratman);
			//	window.draw(enemy);
			window.setView(window.getDefaultView());
		}
		if (gover){
			window.clear();

			window.draw(go);

			if (maxscore2>highest){
                typing = true;
				window.draw(hiscore3);
			}
			window.display();
			clock.restart();
			while (1){

				sf::Time elapsed1 = clock.getElapsedTime();
				if (elapsed1 > t1)
					break;

			}
			window.clear();


			gameStarted = false;
			started = false;
			gover = false;
			makePathNull();

		}

		window.display();
		if (typing){
			window.clear();
			sf::Text text;
			text.setFont(font);
			text.setPosition(100.0f, 100.0f);
			text.setStyle(sf::Text::Bold);
			text.setString("Enter your name:");
			window.draw(text);
			window.display();

			typeName(window, font2, 0,text);
			typing = false;
		}
	}
	return 0;
}
