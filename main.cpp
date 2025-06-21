#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

/**********************************************************
* Title          : Final Project
* Author         : Nicholas Sweet
* Date           : 04/29/2021
* Course/Section : CSC-222-301
* Program Description:  This program spawns a playable
*   character and all him to traverse the maze. This
*   character has to kill all three of the zombies at each dead end.
*   When all three are dead then the player can exit.
* Data Requirements:
*
* Input:
* 1.)the arrow keys move the pig in a new direction
*    which include left, right, up, and down.
*
* Graphics needed:
* eastFarmer.bmp
* eastZombie.bmp
* walkEast.bmp
* walkWest.bmp
* westFarmer.bmp
* westSouth.bmp
* walkNorth.bmp
* northFarmer.bmp
* northZombie.bmp
* walkSouth.bmp
* southFarmer.bmp
* southZombie.bmp
* explosion.bmp
* map.map
*
* Formulas:
* No new formulas that I created.
*
* Methods Modified / Where Refined Algorithms Are :
* CreateGameImages - all that was added was the explosion
*                    texture.
* DrawGame - new if statement where added to make the
*            explosion happen. Added explosion sprite sheet.
* updateExplosion - update or runs animation.
* player/enemy collision - initial sprite sheet
*                          rect with enemy rect.
* all destroy zombie methods - New if statments were added
*                              to keep track of the kill
*                              counter or to add one to it
*                              when destroyed once.
***********************************************************/
//Create a window
SDL_Window *gameWindow;

//Textures
//Create a texture for the tile sheet
SDL_Texture* textureMap = NULL;

SDL_Texture* textureFarmerEastSheet = NULL;     //sheet of east farmer texture
SDL_Texture* textureFarmerWestSheet = NULL;     //sheet of west farmer texture
SDL_Texture* textureFarmerSheet = NULL;         //generic sheet of farmer texture
SDL_Texture* textureFarmerNorthSheet = NULL;    //sheet of north farmer texture
SDL_Texture* textureFarmerSouthSheet = NULL;    //sheet of south farmer texture
SDL_Texture* textureFarmerSheetNS = NULL;       //generic sheet of farmer texture

//Zombie 1
SDL_Texture* textureZombieEastSheet = NULL;     //sheet of east zombie texture
SDL_Texture* textureZombieWestSheet = NULL;     //sheet of west zombie texture
SDL_Texture* textureZombieSheetEW = NULL;       //generic sheet of zombie texture

//Zombie 2
SDL_Texture* textureZombie2EastSheet = NULL;     //sheet of east zombie 2 texture
SDL_Texture* textureZombie2WestSheet = NULL;     //sheet of west zombie 2 texture
SDL_Texture* textureZombie2SheetEW = NULL;       //generic sheet of zombie 2 texture

//Zombie 3
SDL_Texture* textureZombie3EastSheet = NULL;     //sheet of east zombie 3 texture
SDL_Texture* textureZombie3WestSheet = NULL;     //sheet of west zombie 3 texture
SDL_Texture* textureZombie3SheetEW = NULL;       //generic sheet of zombie 3 texture

SDL_Texture* textureExplosionSheet = NULL;
SDL_Texture* textureExplosion = NULL;

//Create a temporary surface because we always need one at hand
SDL_Surface* tempSurface = NULL;
//Create a Renderer 'cause we always need one of these as well
SDL_Renderer* renderer;

//Rectangles
//Source and destination rectangles for tile sheet and placement of one tile
SDL_Rect dstRect;
SDL_Rect srcRect;
//SDL_Rect rectBackground;
SDL_Rect solidTileRect;

//Explosion
SDL_Rect rectExplosion;         //rectangle for explosion sheet
SDL_Rect rectExplosionPlacement;//rectangle for placement of explosion

//All the farmer rectangles
SDL_Rect rectFarmerEastSheet;   //Rectangle for sheet of east walking farmer
SDL_Rect rectFarmerEast;        //One east walking farmer rectangle
SDL_Rect rectFarmerWestSheet;   //Rectangle for sheet of west walking farmer
SDL_Rect rectFarmerWest;        //One west walking farmer rectangle
SDL_Rect rectFarmerNorthSheet;  //Rectangle for sheet of north walking farmer
SDL_Rect rectFarmerNorth;       //One north walking farmer rectangle
SDL_Rect rectFarmerSouthSheet;  //Rectangle for sheet of south walking farmer
SDL_Rect rectFarmerSouth;       //One south walking farmer rectangle

SDL_Rect rectFarmerPlacement;   //Where to place the e/w farmer on the screen
SDL_Rect rectFarmerSheet;       //Sheet containing e/w farmer for currect direction
SDL_Rect rectFarmer;            //Rectangle containing e/w farmer placed on screen

//All the zombies rectangles
//Zombie 1
SDL_Rect rectZombieEastSheet;   //Rectangle for sheet of east walking zombie
SDL_Rect rectZombieEast;        //One east walking zombie rectangle
SDL_Rect rectZombieWestSheet;   //Rectangle for sheet of west walking zombie
SDL_Rect rectZombieWest;        //One west walking zombie rectangle

SDL_Rect rectZombiePlacementEW;   //Where to place the e/w zombie on the screen
SDL_Rect rectZombieSheetEW;       //Sheet containing e/w zombie for currect direction
SDL_Rect rectZombieEW;            //Rectangle containing e/w zombie placed on screen

//Zombie 2
SDL_Rect rectZombie2EastSheet;   //Rectangle for sheet of east walking zombie 2
SDL_Rect rectZombie2East;        //One east walking zombie 2 rectangle
SDL_Rect rectZombie2WestSheet;   //Rectangle for sheet of west walking zombie 2
SDL_Rect rectZombie2West;        //One west walking zombie 2 rectangle

SDL_Rect rectZombie2PlacementEW;   //Where to place the e/w zombie 2 on the screen
SDL_Rect rectZombie2SheetEW;       //Sheet containing e/w zombie 2 for currect direction
SDL_Rect rectZombie2EW;            //Rectangle containing e/w zombie 2 placed on screen

//Zombie 3
SDL_Rect rectZombie3EastSheet;   //Rectangle for sheet of east walking zombie 3
SDL_Rect rectZombie3East;        //One east walking zombie 3 rectangle
SDL_Rect rectZombie3WestSheet;   //Rectangle for sheet of west walking zombie 3
SDL_Rect rectZombie3West;        //One west walking zombie 3 rectangle

SDL_Rect rectZombie3PlacementEW;   //Where to place the e/w zombie 3 on the screen
SDL_Rect rectZombie3SheetEW;       //Sheet containing e/w zombie 3 for currect direction
SDL_Rect rectZombie3EW;            //Rectangle containing e/w zombie 3 placed on screen

//Game constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

const int EXPLOSION_WIDTH = 30;
const int EXPLOSION_HEIGHT = 30;

const int FARMER_WIDTH = 32;
const int FARMER_HEIGHT = 32;

const int ZOMBIE_WIDTH = 32;
const int ZOMBIE_HEIGHT = 30;

const int ZOMBIE2_WIDTH = 32;
const int ZOMBIE2_HEIGHT = 30;

const int ZOMBIE3_WIDTH = 32;
const int ZOMBIE3_HEIGHT = 30;

const int FPS = 30;
const int FRAME_DELAY = 100/FPS;

const int EXPLOSION_SPEED = 1;
const int MAX_EXPLOSION_FRAME = 9;  //number of explosion per sheet

const int FARMER_SPEED = 1;     //the farmer moves
const int MAX_FARMER_FRAME = 8; //number of farmers per sheet

const int ZOMBIE_SPEED = 1;     //the zombie moves
const int MAX_ZOMBIE_FRAME = 8; //number of zombies per sheet

const int ZOMBIE2_SPEED = 1;     //the zombie 2 moves
const int MAX_ZOMBIE2_FRAME = 8; //number of zombies 2 per sheet

const int ZOMBIE3_SPEED = 1;     //the zombie 3 moves
const int MAX_ZOMBIE3_FRAME = 8; //number of zombies 3 per sheet

//Global Variables so everyone can share
//Counters for explosions
int z = 0;                      //Zombie's explosion counter

//Background variables
int displayRow = 0;             //number of rows you want displayed
int displayCol = 0;             //number of columns you want displayed
int mapRow = 0;                 //number of rows in map
int mapCol = 0;                 //number of columns in map
int tileHeight = 0;             //height of tiles in tile sheet
int tileWidth = 0;              //width of tiles in tile sheet

//Explosion variables
int explosionWidth;                //width of one farmer frame
int explosionHeight;               //height of one farmer frame
int SpriteFrameExplosion = 0;
int FrameCounterExplosion = 0;
int xExplosionPos;          //x position of e/w farmer on screen
int yExplosionPos;         //y position of e/w farmer on screen
bool explosionDestroyed = false;

//Farmer variables
int farmerWidth;                //width of one farmer frame
int farmerHeight;               //height of one farmer frame
int SpriteFrame = 0;
int FrameCounterFarmerEW = 0;
int FrameCounter = 0;
int xFarmerPos = 0;          //x position of e/w farmer on screen
int yFarmerPos = 64;         //y position of e/w farmer on screen
bool farmerDestroyed = false;

//Zombie variables
//Zombie 1
int zombieWidth;                //width of one zombie frame
int zombieHeight;               //height of one zombie frame
int SpriteFrameZombieEW = 0;
int FrameCounterZombieEW = 0;
int xZombieEWpos = 585;          //x position of e/w zombie on screen
int yZombieEWpos = 400;         //y position of e/w zombie on screen
bool zombieDestroyed = false;

//Zombie 2
int zombie2Width;                //width of one zombie 2 frame
int zombie2Height;               //height of one zombie 2 frame
int SpriteFrameZombie2EW = 0;
int FrameCounterZombie2EW = 0;
int xZombie2EWpos = 585;          //x position of e/w zombie 2 on screen
int yZombie2EWpos = 110;         //y position of e/w zombie 2 on screen
bool zombie2Destroyed = false;

//Zombie 3
int zombie3Width;                //width of one zombie 3 frame
int zombie3Height;               //height of one zombie 3 frame
int SpriteFrameZombie3EW = 0;
int FrameCounterZombie3EW = 0;
int xZombie3EWpos = 265;          //x position of e/w zombie 3 on screen
int yZombie3EWpos = 550;         //y position of e/w zombie 3 on screen
bool zombie3Destroyed = false;
int z1Dead = 0;
int z2Dead = 0;
int z3Dead = 0;

//Note:  west = 0, east = 1, south = 2, north = 3
int farmerDirection;
int zombieDirectionEW = 1;
int zombie2DirectionEW = 1;
int zombie3DirectionEW = 1;

//Timer/Counters
int zombieKillCounter = 0;
float begTime;
float timeLapse = 0;
float endTime = 100000;

//place holders for array of frame ID numbers
int* layer1, *layer2;

int sheetWidth = 0;             //width of destination game screen
int sheetHeight = 0;            //height of destination game screen

vector<int> solidTiles;         //array of solid tile frameIDs
struct SolidTileStruct          //structure to hold position of solid tile data
{
    int x;          //x position of solid tile
    int y;          //y position of solid tile
    bool solid;     //we know the tile is solid but put a 1 here anyway
    int frameID;    //frame ID of solid tile
};
SolidTileStruct tiles[1000];
int numSolidTiles = 0;

//Generic Function prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool InitGame();
bool InitSDL();
void CreateGameImages();
void RunGame();
void DrawGame();

//void setUpEachSheet(const char* path , int r, int g, int b, int maxFrame, SDL_Texture textureSheet, SDL_Rect rectSheet, SDL_rectDirSheet)

//Tile function prototypes
void getMapData();
void generateSolidLayer();
void drawLayer(int layer[]);
bool isTileSolid(int tile);

//Piggy function prototypes//Switch with farmer
void UpdateFarmer();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();

//Zombie/Explosion function prototypes
void updateExplosion();
void updateEastWestZombie();
void updateEastWestZombie2();
void updateEastWestZombie3();

//Collision function prototypes
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos );
void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction);
void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos);
bool EnemyTileCollision(SDL_Rect movingRect, int &direction);
void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect);
bool CheckCollision(SDL_Rect r1,SDL_Rect r2);

//Destruction function prototypes
void destroyFarmer();
void destroyZombie();
void destroyZombie2();
void destroyZombie3();

/**********************************************************
* Title          : main
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description:  This method calls method to create
*   the maze.
*
* Refined Algorithm:
*   BEGIN main
*       Init game and keys
*       Print instructions in debug window
*       Init begTime
*       Create all the game images
*       WHILE( program is running)
*           Compute current time
*           CHECK INPUT
*           Update & Draw
*       END WHILE
*       IF( time lapse is greater than end time )
*           Print lose message
*       ELSE( if won the game before time ran out )
*           Print win message
*       END IF
*       Close shop
*   END main
***********************************************************/
int main(int argc, char *argv[])
{
    //Init game and keys
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(!InitGame())
    {
        printf("Unable to initialize from main.\n");
        CloseShop();   //If InitGame failed, stop the presses
    }

    //Print instructions in debug window
    printf("Maze | How to : Traverse the maze and kill 3 enemies before the timer runs out.");

    //Init begTime
    begTime = SDL_GetTicks();

    //Create all the game images
    CreateGameImages();

    //WHILE( program is running)
    while(ProgramIsRunning())
    {
        //Compute current time
        timeLapse = SDL_GetTicks() - begTime;

        //CHECK INPUT
        //Move the piggy when the up/down/left right key is pressed
        if(keys[SDL_SCANCODE_UP])
        {
            //move the shape north
            moveUp();
            yFarmerPos = yFarmerPos - FARMER_SPEED; //decrement y
        }

        if(keys[SDL_SCANCODE_DOWN])
        {
            //move shape south
            moveDown();
            yFarmerPos = yFarmerPos + FARMER_SPEED; //increment y
        }

        if(keys[SDL_SCANCODE_LEFT])
        {
            //move shape east
            moveLeft();
            xFarmerPos = xFarmerPos - FARMER_SPEED; //decrement x
        }

        if(keys[SDL_SCANCODE_RIGHT])
        {
            //move shape west
            moveRight();
            xFarmerPos = xFarmerPos + FARMER_SPEED;  //increment x
        }
        RunGame();          //Update & Draw
    }//end while not quit

    //IF( time lapse is greater than end time )
    if(timeLapse > endTime)
    {
        //Print lose message
        printf("Time has ran out and now you have been eaten by zombies");

    }
    //ELSE( if won the game before time ran out )
    else
    {
        //Print win message
        printf("Congratulations you have killed all the zombie and got to the exit in time!!");

    }//END IF

    //Close shop
    CloseShop();
    return 0;
}

//Functions
void getMapData()
{
    //Get the tile data
    //Initialize the layers
    layer1 = layer2 = NULL;

    //Load map file
     std::ifstream in("graphics/map.map");

    if(!in.good())
        printf("The map did not load.");

    std::string buffer;

    //1. Get the line that says 'destWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    sheetWidth = atoi(buffer.c_str());

    //2. Get the line that says 'destHeight'
    getline(in, buffer, ' ');
    //Get the height value
    getline(in, buffer, '\n');
    sheetHeight = atoi(buffer.c_str());

    //3. Get the line that says 'mapWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapCol = atoi(buffer.c_str());

    //4. Get the line that says 'mapHeight'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapRow = atoi(buffer.c_str());

    //5. Get the line that says 'tile_width'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileWidth = atoi(buffer.c_str());

    //6. Get the line that says 'tile_height'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileHeight = atoi(buffer.c_str());

    //7. Get the line that says 'solid_tiles'
    getline(in, buffer, '\n');

    //8. Get the row containing solid tiles
    getline(in, buffer, '\n');

    char* string = new char[buffer.size() + 1];
    strcpy(string, buffer.c_str());

    char* token = strtok(string, " ,");

    while(token != NULL)
    {
        solidTiles.push_back(atoi(token));
        token = strtok(NULL, " ,");
    }

    delete[] string;

    //Set up an array for each layer before getting data for each layer
    layer1 = new int[mapCol * mapRow];
    layer2 = new int[mapCol * mapRow];

    //9. Get the row containing 'layer1' of background (not solid tiles)
    getline(in, buffer, '\n');

    int i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer1[i] = data;                    //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    //10. Get the row containing 'layer2' of solid tile placement
    getline(in, buffer, '\n');

    i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer2[i] = data;                   //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    in.close();
}//end load map

SDL_Surface* loadImage(const char* path)
{
    //Load image at specified path
	tempSurface = SDL_LoadBMP(path);

	if( tempSurface == NULL )
	{
		printf("Unable to load image at path: %s\n", path);
	}
    return tempSurface;
}//end loadImage

SDL_Texture* loadTexture(SDL_Surface* tempSurface)
{
	//texture
	SDL_Texture* newTexture = NULL;
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if( newTexture == NULL )
    {
        printf("Unable to create texture\n");
    }
    //Get rid of the surface
    SDL_FreeSurface( tempSurface );
	return newTexture;
}//end create a texture

void FillRect(SDL_Rect &rect, int x, int y, int width, int height)
{
    //Initialize the rectangle
    rect.x = x;         //initial x position of upper left corner
    rect.y = y;         //initial y position of upper left corner
    rect.w = width;     //width of rectangle
    rect.h = height;    //height of rectangle
}//end rectangle initializing

void CloseShop()
{
    //Destroy all objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);

    //SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureMap);

    if (farmerDestroyed == false)
    {
        destroyFarmer();
    }

    if (zombieDestroyed == false)
    {
        destroyZombie();
    }

    if (zombie2Destroyed == false)
    {
        destroyZombie2();
    }

    if (zombie3Destroyed == false)
    {
        destroyZombie3();
    }

    //destroy pointers to arrays
    if (layer1 != NULL)
    {
        delete[] layer1;
        layer1 = NULL;
    }
    if (layer2 != NULL)
    {
        delete[] layer2;
        layer2 = NULL;
    }
    //release vector memory
    solidTiles.clear();

    SDL_Quit(); //Quit the program
}//End Close Shop

void destroyFarmer()
{
    SDL_RenderPresent(renderer);
    SDL_Delay(5);  //delay a bit
    SDL_DestroyTexture(textureFarmerSouthSheet);
    SDL_DestroyTexture(textureFarmerNorthSheet);
    SDL_DestroyTexture(textureFarmerEastSheet);
    SDL_DestroyTexture(textureFarmerWestSheet);
    SDL_DestroyTexture(textureFarmerSheet);
}

/**********************************************************
* Title          : destroyZombie
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description: This method add to the kill counter
*   once and destroys zombie 1.
*
* Refined Algorithm:
*   BEGIN destroyZombie
*       IF( zombie was destroyed zero times )
*           Add 1 to the zombie 1 is dead counter
*           Add 1 to kill counter
*       END IF
*       Destroy zombie 1
*   END destroyZombie
***********************************************************/
void destroyZombie()
{
    //IF( zombie 1 was destroyed zero times )
    if( z1Dead == 0 )
    {
        //Add 1 to the zombie is dead counter
        z1Dead++;

        //Add 1 to kill counter
        zombieKillCounter++;

    }//END IF

    //Destroy zombie 1
    SDL_DestroyTexture(textureZombieEastSheet);
    SDL_DestroyTexture(textureZombieWestSheet);
    SDL_DestroyTexture(textureZombieSheetEW);
}

/**********************************************************
* Title          : destroyZombie2
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description: This method add to the kill counter
*   once and destroys zombie 2.
*
* Refined Algorithm:
*   BEGIN destroyZombie2
*       IF( zombie 2 was destroyed zero times )
*           Add 1 to the zombie 2 is dead counter
*           Add 1 to kill counter
*       END IF
*       Destroy zombie 2
*   END destroyZombie2
***********************************************************/
void destroyZombie2()
{
    //IF( zombie was destoryed zero times)
    if( z2Dead == 0 )
    {
        //Add 1 to the zombie 2 is dead counter
        z2Dead++;

        //Add 1 to kill counter
        zombieKillCounter++;

    }//END IF

    //Destroy zombie 2
    SDL_DestroyTexture(textureZombie2EastSheet);
    SDL_DestroyTexture(textureZombie2WestSheet);
    SDL_DestroyTexture(textureZombie2SheetEW);
}

/**********************************************************
* Title          : destroyZombie3
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description: This method add to the kill counter
*   once and destroys zombie 3.
*
* Refined Algorithm:
*   BEGIN destroyZombie3
*       IF( zombie 3 was destroyed zero times )
*           Add 1 to the zombie 3 is dead counter
*           Add 1 to kill counter
*       END IF
*       Destroy zombie 3
*   END destroyZombie3
***********************************************************/
void destroyZombie3()
{
    //IF( zombie was destoryed zero times)
    if( z3Dead == 0 )
    {
        //Add 1 to the zombie 3 is dead counter
        z3Dead++;

        //Add 1 to kill counter
        zombieKillCounter++;

    }//END IF

    //Destroy zombie 3
    SDL_DestroyTexture(textureZombie3EastSheet);
    SDL_DestroyTexture(textureZombie3WestSheet);
    SDL_DestroyTexture(textureZombie3SheetEW);
}

SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b)
{
    //create the transparent color (pink in this case) and put in colorKey
    Uint32 colorKey;
    colorKey = SDL_MapRGB(surface->format, r, g, b);
    //set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    return surface;
}//End setTransparent Color

/**********************************************************
* Title          : ProgramIsRunning
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description: This method check if the program
*   needs to be closed.
*
* Refined Algorithm:
*   BEGIN ProgramIsRunning
*       Get event
*       Init running is true
*       WHILE( events are still going on )
*           IF( window is x out )
*               Set running equal to false
*           END IF
*           IF( time ran out )
*               Set running equal to false
*           END IF
*           IF( player kill all three zombie and makes it to the exit )
*               Set running equal to false
*           END IF
*       END WHILE
*       Return running
*   END ProgramIsRunning
***********************************************************/
bool ProgramIsRunning()
{
    //Get event
    SDL_Event event;

    //Init running is true
    bool running = true;

    //WHILE( events are still going on )
    while(SDL_PollEvent(&event))
    {
        //IF( window is x out )
        if(event.type == SDL_QUIT)
            //Set running equal to false
            running = false;
        //END IF

        //IF( time ran out )
        if(timeLapse > endTime)
            //Set running equal to false
            running = false;
        //END IF

        //IF( player kill all three zombie and makes it to the exit )
        if(xFarmerPos >= SCREEN_WIDTH - FARMER_WIDTH && zombieKillCounter == 3)
            //Set running equal to false
            running = false;
        //END IF

    }//END WHILE

    //Return running
    return running;
}

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    return true;
}

bool InitSDL()
{
     //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!\n");
        SDL_Quit();
    }
    //Init audio subsystem
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        printf("Open Audio failed to initialize!\n");
        return false;
    }
    return true;
}

/**********************************************************
* Title          : CreateGameImages
* Author         : Nicholas Sweet
* Date           : 4/19/2021
* Course/Section : CSC-222-301
* Program Description:  This method creates all of the
*   images needed to created the game.
*
* Refined Algorithm:
*   BEGIN CreateGameImages
*       Create a window
*       Create a renderer for the window
*       Load the tile sheet
*       Get the map data and create the solid layer
*       Load all farmer stuff
*       Load all zombie 1 stuff
*       Load all zombie 2 stuff
*       Load all zombie 3 stuff
*       Create the surface - load explosion
*       Set white as transparency color
*       Create the texture for the whole sheet
*   END CreateGameImages
***********************************************************/
void CreateGameImages()
{
    //SDL_Init( SDL_INIT_EVERYTHING );
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Load the tile sheet
    tempSurface = loadImage("graphics/tiles1.bmp");
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //no need to set transparent color for the tiles
    textureMap = loadTexture(tempSurface);
    //Get the map data and create the solid layer
    getMapData();
    generateSolidLayer();

    //Load all farmer stuff
    //Set up East walking farmer
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerEastSheet, NULL, NULL, &rectFarmerEastSheet.w, &rectFarmerEastSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerEastSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerEastSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectFarmerEast, 0, 0, farmerWidth, farmerHeight);

    //Set up West walking farmer
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerWestSheet, NULL, NULL, &rectFarmerWestSheet.w, &rectFarmerWestSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerWestSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerWestSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer west
    FillRect(rectFarmerWest, 0, 0, farmerWidth, farmerHeight);

    //Set up North walking farmer
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/WalkNorth/northFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerNorthSheet, NULL, NULL, &rectFarmerNorthSheet.w, &rectFarmerNorthSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerNorthSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerNorthSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectFarmerNorth, 0, 0, farmerWidth, farmerHeight);

    //Set up South walking farmer
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/WalkSouth/southFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerSouthSheet, NULL, NULL, &rectFarmerSouthSheet.w, &rectFarmerSouthSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerSouthSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerSouthSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer west
    FillRect(rectFarmerSouth, 0, 0, farmerWidth, farmerHeight);

    //load all zombie stuff
    //Zombie 1
    //Set up East walking zombie
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieEastSheet, NULL, NULL, &rectZombieEastSheet.w, &rectZombieEastSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieEastSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieEastSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectZombieEast, 0, 0, zombieWidth, zombieHeight);

    //Set up West walking zombie
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieWestSheet, NULL, NULL, &rectZombieWestSheet.w, &rectZombieWestSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieWestSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieWestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectZombieWest, 0, 0, zombieWidth, zombieHeight);

    //Zombie 2
    //Set up East walking zombie
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombie2EastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombie2EastSheet, NULL, NULL, &rectZombie2EastSheet.w, &rectZombie2EastSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombie2Width = rectZombie2EastSheet.w/MAX_ZOMBIE2_FRAME;
    //get the height of one frame
    zombie2Height = rectZombie2EastSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectZombie2East, 0, 0, zombie2Width, zombie2Height);

    //Set up West walking zombie
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombie2WestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombie2WestSheet, NULL, NULL, &rectZombie2WestSheet.w, &rectZombie2WestSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombie2Width = rectZombie2WestSheet.w/MAX_ZOMBIE2_FRAME;
    //get the height of one frame
    zombie2Height = rectZombie2WestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectZombie2West, 0, 0, zombie2Width, zombie2Height);

    //Zombie 3
    //Set up East walking zombie
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombie3EastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombie3EastSheet, NULL, NULL, &rectZombie3EastSheet.w, &rectZombie3EastSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombie3Width = rectZombie3EastSheet.w/MAX_ZOMBIE3_FRAME;
    //get the height of one frame
    zombie3Height = rectZombie3EastSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectZombie3East, 0, 0, zombie3Width, zombie3Height);

    //Set up West walking zombie
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombie3WestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombie3WestSheet, NULL, NULL, &rectZombie3WestSheet.w, &rectZombie3WestSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombie3Width = rectZombie3WestSheet.w/MAX_ZOMBIE3_FRAME;
    //get the height of one frame
    zombie3Height = rectZombie3WestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectZombie3West, 0, 0, zombie3Width, zombie3Height);

    //Explosion
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/explosion1.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureExplosionSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureExplosionSheet, NULL, NULL, &rectExplosion.w, &rectExplosion.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    explosionWidth = rectExplosion.w/MAX_EXPLOSION_FRAME;
    //get the height of one frame
    explosionHeight = rectZombie3WestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectExplosion, 0, 0, explosionWidth, explosionHeight);
}

void generateSolidLayer()   //create a structure to hold solid tile data
{
    int begIndex = 0;   //position of solid tile in tiles structure
    for (int row = 0; row < mapRow; ++row)
    {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer2[row*mapCol+col];//get frame ID of solid tile from layer 2
                if (frame > 0)  //if the frame is not zero
                {
                    if (isTileSolid(layer2[row*mapCol+col])) //if tile is solid
                    {
                        tiles[begIndex].solid = 1;              //then solid field is 1
                        tiles[begIndex].frameID = frame;        //save the frame ID
                        tiles[begIndex].x = col*tileWidth;      //save THIS x position of the solid tile
                        tiles[begIndex].y = row*tileHeight;     //save THIS y position of the solid tile
                        begIndex = begIndex + 1;                //move to the next tile
                    }//end if tile is solid
                }
            }
    }
    numSolidTiles = begIndex - 1;
}

bool isTileSolid(int tile)
{
    for(unsigned int i = 0; i < solidTiles.size(); i++)
    {
        if(solidTiles[i] == tile)
        {
            return true;
        }
    }
    return false;
}

void drawLayer(int layer[])
{
    //Draw a Layer
        for (int row = 0; row < mapRow; ++row)
        {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer[row*mapCol+col];//get frame ID number to draw
                //if frameID is not a zero then grab it from sheet
                if(frame > 0)
                {
                    //Compute the x and y destination tile position
                    int xRawBytes = ((frame - 1) * tileWidth);
                    int yRawBytes = ((frame - 1) * tileHeight);
                    int xSource = xRawBytes % sheetWidth;
                    int ySource = (yRawBytes / sheetWidth) * tileHeight;
                    //Initialize the source rectangle with position of tile you want to draw
                    FillRect(srcRect, xSource, ySource, tileWidth, tileHeight);

                    // Compute the x and y location on game screen
                    int xDest = col*tileWidth;
                    int yDest = row*tileHeight;

                    //Initialize the destination rectangle with x and y position on game screen
                    FillRect(dstRect, xDest, yDest, tileWidth, tileHeight);

                    //Grab tile from sheet and place on game screen
                    SDL_RenderCopy(renderer, textureMap, &srcRect, &dstRect);

                }//end if frame > 0
            }//end for col
        }//end for row
}

void UpdateFarmer()
{
    //Increment time to delay each frame
    FrameCounter++;
    //If delay time is greater than max amount
    if(FrameCounter > FRAME_DELAY)
    {
        FrameCounter = 0;   //start new delay time
        SpriteFrame++;      //advance to next frame
    }
    if(SpriteFrame > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrame = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (farmerDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectFarmerEast.x = SpriteFrame * rectFarmerEast.w;
        if ( xFarmerPos > SCREEN_WIDTH - FARMER_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            xFarmerPos = SCREEN_WIDTH - FARMER_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectFarmer, rectFarmerEast.x, rectFarmerEast.y, rectFarmerEast.w, rectFarmerEast.h);
        textureFarmerSheet = textureFarmerEastSheet;
    }// end walking east

    if (farmerDirection == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectFarmerWest.x = SpriteFrame * rectFarmerWest.w;
       // xPiggyPos = xPiggyPos - PIGGY_SPEED;              //decrement x
        if ( xFarmerPos < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            xFarmerPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectFarmer, rectFarmerWest.x, rectFarmerWest.y, rectFarmerWest.w, rectFarmerWest.h);
        textureFarmerSheet = textureFarmerWestSheet;
    }// end walking West

    if (farmerDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectFarmerSouth.x = SpriteFrame * rectFarmerSouth.w;
        //yPiggyPos = yPiggyPos + PIGGY_SPEED;                       //increment y
        if ( yFarmerPos > SCREEN_HEIGHT - FARMER_HEIGHT )    //check bottom
        {
            yFarmerPos = SCREEN_HEIGHT - FARMER_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectFarmer, rectFarmerSouth.x, rectFarmerSouth.y, rectFarmerSouth.w, rectFarmerSouth.h);
        //copy the south facing piggys to the generic sheet
        textureFarmerSheet = textureFarmerSouthSheet;
    }// end walking south

    if (farmerDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectFarmerNorth.x = SpriteFrame * rectFarmerNorth.w;
        //yPiggyPos = yPiggyPos - PIGGY_SPEED;                       //decrement y
        if ( yFarmerPos < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            yFarmerPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectFarmer, rectFarmerNorth.x, rectFarmerNorth.y, rectFarmerNorth.w, rectFarmerNorth.h);
        //copy the north facing piggy to the generic piggy sheet
        textureFarmerSheet = textureFarmerNorthSheet;
    }// end walking north

    //Initialize the placement rectangle to the position on the screen the piggy will be drawn.
    FillRect(rectFarmerPlacement, xFarmerPos, yFarmerPos, farmerWidth, farmerHeight);

    //Now check to see if the piggy has collided with a solid tile
    PlayerTileCollision(rectFarmerPlacement, xFarmerPos, yFarmerPos);

    //Now check to see if the piggy has collided with an enemy
    //playerEnemyCollide(rectPiggyPlacement, rectFarmerPlacementEW);
    //Now check to see if the piggy has collided with an enemy
    playerEnemyCollide(rectFarmerPlacement, rectZombiePlacementEW);
    playerEnemyCollide(rectFarmerPlacement, rectZombie2PlacementEW);
    playerEnemyCollide(rectFarmerPlacement, rectZombie3PlacementEW);
}

//modified or added update explosion
void updateExplosion()
{
    //Increment time to delay each frame
    FrameCounterExplosion++;
    //If delay time is greater than max amount
    if(FrameCounterExplosion > FRAME_DELAY)
    {
        FrameCounterExplosion = 0;   //start new delay time
        SpriteFrameExplosion++;      //advance to next frame
    }
    if(SpriteFrameExplosion > MAX_EXPLOSION_FRAME - 1) //if last frame is reached
        SpriteFrameExplosion = 0;                   //start at frame zero again

    //Advance to next frame on sheet
    rectExplosion.x = SpriteFrameExplosion * rectExplosion.w;
    xExplosionPos = xExplosionPos - EXPLOSION_SPEED;              //decrement x
    //create the rectangle on screen for explosion placement
    FillRect(rectExplosion, rectExplosion.x, rectExplosion.y, rectExplosion.w, rectExplosion.h);
    textureExplosionSheet = textureExplosionSheet;

    //Initialize the placement rectangle to the position on the screen the explosion will be drawn.
    FillRect(rectExplosionPlacement, xExplosionPos, yExplosionPos, explosionWidth, explosionHeight);
}

void updateEastWestZombie()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterZombieEW++;
    //If delay time is greater than max amount
    if(FrameCounterZombieEW > FRAME_DELAY)
    {
        FrameCounterZombieEW = 0;   //start new delay time
        SpriteFrameZombieEW++;      //advance to next frame
    }
    if(SpriteFrameZombieEW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameZombieEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (zombieDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectZombieEast.x = SpriteFrameZombieEW * rectZombieEast.w;
        xZombieEWpos = xZombieEWpos + ZOMBIE_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieEast.x, rectZombieEast.y, rectZombieEast.w, rectZombieEast.h);
        textureZombieSheetEW = textureZombieEastSheet;
    }// end walking east

    if (zombieDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectZombieWest.x = SpriteFrameZombieEW * rectZombieWest.w;
        xZombieEWpos = xZombieEWpos - FARMER_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieWest.x, rectZombieWest.y, rectZombieWest.w, rectZombieWest.h);
        textureZombieSheetEW = textureZombieWestSheet;
    }// end walking West

    //Now check to see if the zombie has collided with a solid tile
    collision = EnemyTileCollision(rectZombiePlacementEW, zombieDirectionEW);

    if (collision == true)
    {
        FrameCounterZombieEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the zombie will be drawn.
    FillRect(rectZombiePlacementEW, xZombieEWpos, yZombieEWpos, zombieWidth, zombieHeight);
}

void updateEastWestZombie2()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterZombie2EW++;
    //If delay time is greater than max amount
    if(FrameCounterZombie2EW > FRAME_DELAY)
    {
        FrameCounterZombie2EW = 0;   //start new delay time
        SpriteFrameZombie2EW++;      //advance to next frame
    }
    if(SpriteFrameZombie2EW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameZombie2EW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (zombie2DirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectZombie2East.x = SpriteFrameZombie2EW * rectZombie2East.w;
        xZombie2EWpos = xZombie2EWpos + ZOMBIE2_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombie2EW, rectZombie2East.x, rectZombie2East.y, rectZombie2East.w, rectZombie2East.h);
        textureZombie2SheetEW = textureZombie2EastSheet;
    }// end walking east

    if (zombie2DirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectZombie2West.x = SpriteFrameZombie2EW * rectZombie2West.w;
        xZombie2EWpos = xZombie2EWpos - FARMER_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombie2EW, rectZombie2West.x, rectZombie2West.y, rectZombie2West.w, rectZombie2West.h);
        textureZombie2SheetEW = textureZombie2WestSheet;
    }// end walking West

    //Now check to see if the zombie has collided with a solid tile
    collision = EnemyTileCollision(rectZombie2PlacementEW, zombie2DirectionEW);

    if (collision == true)
    {
        FrameCounterZombie2EW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the zombie will be drawn.
    FillRect(rectZombie2PlacementEW, xZombie2EWpos, yZombie2EWpos, zombie2Width, zombie2Height);
}

void updateEastWestZombie3()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterZombie3EW++;
    //If delay time is greater than max amount
    if(FrameCounterZombie3EW > FRAME_DELAY)
    {
        FrameCounterZombie3EW = 0;   //start new delay time
        SpriteFrameZombie3EW++;      //advance to next frame
    }
    if(SpriteFrameZombie3EW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameZombie3EW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (zombie3DirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectZombie3East.x = SpriteFrameZombie3EW * rectZombie3East.w;
        xZombie3EWpos = xZombie3EWpos + ZOMBIE3_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombie3EW, rectZombie3East.x, rectZombie3East.y, rectZombie3East.w, rectZombie3East.h);
        textureZombie3SheetEW = textureZombie3EastSheet;
    }// end walking east

    if (zombie3DirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectZombie3West.x = SpriteFrameZombie3EW * rectZombie3West.w;
        xZombie3EWpos = xZombie3EWpos - FARMER_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombie3EW, rectZombie3West.x, rectZombie3West.y, rectZombie3West.w, rectZombie3West.h);
        textureZombie3SheetEW = textureZombie3WestSheet;
    }// end walking West

    //Now check to see if the zombie has collided with a solid tile
    collision = EnemyTileCollision(rectZombie3PlacementEW, zombie3DirectionEW);

    if (collision == true)
    {
        FrameCounterZombie3EW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the zombie will be drawn.
    FillRect(rectZombie3PlacementEW, xZombie3EWpos, yZombie3EWpos, zombie3Width, zombie3Height);
}

void RunGame()
{
    UpdateFarmer();              //Update position of piggy
    updateEastWestZombie();      //Update position of east/west farmer
    updateEastWestZombie2();     //Update position of east/west farmer
    updateEastWestZombie3();     //Update position of east/west farmer
    DrawGame();
}

/**********************************************************
* Title          : DrawGame
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description:  This method draws to game screen.
*
* Refined Algorithm:
*   BEGIN DrawGame
*       Clear the window
*       Add menu bar
*       Draw each layer
*       Render the farmer
*       Zombie 1 render
*       Zombie 2 render
*       Zombie 3 render
*   END DrawGame
***********************************************************/
void DrawGame()
{
    //Clear the window
    SDL_RenderClear(renderer);

    //Add menu bar
    char buffer[64];    sprintf(buffer, "Countdown:  %0.f ", endTime - timeLapse);
    SDL_SetWindowTitle(gameWindow, buffer);

    //Draw each layer
    drawLayer (layer1);
    drawLayer (layer2);

    //Render the farmer
    SDL_RenderCopy(renderer, textureFarmerSheet, &rectFarmer, &rectFarmerPlacement);

    //Render the zombie
    SDL_RenderCopy(renderer, textureZombieSheetEW, &rectZombieEW, &rectZombiePlacementEW);

    //Render the zombie
    SDL_RenderCopy(renderer, textureZombie2SheetEW, &rectZombie2EW, &rectZombie2PlacementEW);

    //Render the zombie
    SDL_RenderCopy(renderer, textureZombie3SheetEW, &rectZombie3EW, &rectZombie3PlacementEW);

    //Display the game screen with updated position of Piggy
    SDL_RenderPresent(renderer);

    //Delay a bit
    SDL_Delay(5);
}

    //Note:  west = 0, east = 1, south = 2, north = 3
    void moveUp()
    {
        //y's decrease to move north
        //if we are not facing north
        if (farmerDirection != 3)
        {
            farmerDirection = 3;    //then face north
            FrameCounter = 0;       //start new position
        }
    }

    void moveDown()
    {
        //y's increase to move south
        //if we are not facing south
        if (farmerDirection != 2)
        {
            farmerDirection = 2;     //then face south
            FrameCounter = 0;        //start new position
        }
    }

    void moveLeft()
    {
        //x's increase to move west
        //if we are not facing west
        if (farmerDirection != 0)
        {
            farmerDirection = 0;    //then face west
            FrameCounter = 0;       //start new position
        }
    }

    void moveRight()
    {
        //x's decrease to move east
        //if we are not facing east
        if (farmerDirection != 1)
        {
            farmerDirection = 1;    //then face east
            FrameCounter = 0;       //start new position
        }
    }

//not mod
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos )
{
        //we're here because we've had a collision with a solid tile
        //now determine what side of the solid tile did the movingRect collide with
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        //now determine which direction the sprite needs to back off from
        if(moveInXDirection)            //if moving left or right
        {
            if(intersectsRight)         //if colliding on the right
            {
                xPos = xPos + 1;    //stop by increasing x (move back right)
            }
            else                        //else colliding on the left
            {
                xPos = xPos - 1;    //stop by decreasing x (move back left)
            }
        }
        else                            //else moving up or down
        {
            if(intersectsTop)           //if colliding with top
            {
                yPos = yPos - 1;    //stop by decreasing y (move back up)
            }
            else                        //else colliding with bottom
            {
                yPos = yPos + 1;    //stop by increasing y (move back down)
            }
        }//end: back off, bub.
}//end Collide

//not mod
void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos)
{
    //loop indexes to cycle through solid tile vector array
    int begIndex = 0;
    //int endIndex = solidTiles.size();
    int endIndex = numSolidTiles;

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;

    for (int i = begIndex; i <= endIndex; i++)
    {
        //Get the x and y location of solid tile
        int xDest = tiles[i].x;
        int yDest = tiles[i].y;

        //make the solid tile a rectangle
        FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

        //make the solid rectangle a constant
        const SDL_Rect& solid = solidTileRect;

        //now check to see if the moving rectangle (sprite) has intersected a solid tile
        if( SDL_HasIntersection(&moving, &solid))
        {
            //the moving sprite has collided with the solid tile so stop that sprite in its tracks
            Collide(movingRect, solidTileRect, xPos, yPos);
        }//end if intersected
    }//end for
}//end tile collision

//not mod
bool EnemyTileCollision(SDL_Rect movingRect, int &direction)
{
    //loop indexes to cycle through solid tile vector array
    int i = 0;
    //int endIndex = solidTiles.size();

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;
    bool done = false;

        while (i <= numSolidTiles && done == false)
        {
            //Get the x and y location of solid tile
            int xDest = tiles[i].x;
            int yDest = tiles[i].y;

            //make the solid tile a rectangle
            FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

            //make the solid rectangle a constant
            const SDL_Rect& solid = solidTileRect;

            //now check to see if the moving rectangle (sprite) has intersected a solid tile
            if( SDL_HasIntersection(&moving, &solid))
            {
                //the moving sprite has collided with the solid tile so stop that sprite in its tracks
                enemyCollide(movingRect, solidTileRect, direction);
                done = true;       //done checking solid tiles as soon as 1 collision occurs
            }//end if intersected
            i++;
        }//end while
        if (done == true)
            return true;
        else
            return false;
}//end tile collision

//not mod
void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction)
{
    {
        //now determine what side of the obstacle did the movingRect collide with?
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        // adjust moving object's position accordingly
        if(moveInXDirection == true)            //if moving left or right
        {
            if(intersectsRight == true)         //if colliding on the right
            {
                direction = 1;    //change direction to left
            }
            //else
            if(intersectsRight == false)
                //else colliding on the left
            {
                direction = 0;    //change direction to right
            }
        }
        //else                            //else moving up or down
        if (moveInXDirection == false)
        {
            if(intersectsTop == true)           //if colliding with top
            {
                direction = 3;    //then change direction to up
            }
            //else                        //else colliding with bottom
            if(intersectsTop == false)
            {
                direction = 2;    //then change direction to down
            }
        }//end adjustment of direction
     }//end check if there is a collision
}//end Collide

/**********************************************************
* Title          : playerEnemyCollide
* Author         : Nicholas Sweet
* Date           : 05/11/2021
* Course/Section : CSC-222-301
* Program Description:  This method check if enemy and
*   player has collided. If the player and enemy did
*   collide with one another then the zombie is destroyed
*   and an explosion will occur.
*
* Refined Algorithm:
*   BEGIN playerEnemyCollide
*       Make constants
*       IF(collision with player is true)
*           IF(collision with zombie 1 is true)
*               Init x & y of explosion to zombie 1's x and y
*               Draw explosion
*               Destroy zombie
*           END IF
*           IF(collision with zombie 2 is true)
*               Init x & y of explosion to zombie 2's x and y
*               Draw explosion
*               Destroy zombie 2
*           END IF
*           IF(collision with zombie 3 is true)
*               Init x & y of explosion to zombie 3's x and y
*               Draw explosion
*               Destroy zombie 3
*           END IF
*       END IF
*   END playerEnemyCollide
***********************************************************/
void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect)
{
    //Make constants
    //make the moving rectangle a constant
    const SDL_Rect& PLAYER_RECT = playerRect;
    //make the solid rectangle a constant
    const SDL_Rect& ENEMY_RECT = enemyRect;
    //create a constant rectangle with current zombie position for comparing
    const SDL_Rect& CONST_ZOMBIE_RECT = rectZombiePlacementEW;
    const SDL_Rect& CONST_ZOMBIE2_RECT = rectZombie2PlacementEW;
    const SDL_Rect& CONST_ZOMBIE3_RECT = rectZombie3PlacementEW;
    //create a constant rectangle with current farmer position for comparing
    //const SDL_Rect& CONST_FARMER_RECT = rectFarmerPlacementEW;

   //IF(collision with player is true)
   if (CheckCollision(playerRect, enemyRect))
    {
        //IF(collision with zombie 1 is true)
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_ZOMBIE_RECT))
        {
            //Init x & y of explosion to zombie 1's x and y
            xExplosionPos = xZombieEWpos;
            yExplosionPos = yZombieEWpos;

            //Draw explosion
            SDL_RenderCopy(renderer, textureExplosionSheet, &rectExplosion, &rectExplosionPlacement);

            //Destroy zombie
            zombieDestroyed = true;
            destroyZombie();

        }//END IF

        //IF(collision with zombie 2 is true)
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_ZOMBIE2_RECT))
        {
            //init x & y of explosion to zombie 2's x and y
            xExplosionPos = xZombie2EWpos;
            yExplosionPos = yZombie2EWpos;

            //Draw explosion
            SDL_RenderCopy(renderer, textureExplosionSheet, &rectExplosion, &rectExplosionPlacement);

            //Destroy zombie 2
            zombie2Destroyed = true;
            destroyZombie2();
        }//END IF

        //IF(collision with zombie 3 is true)
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_ZOMBIE3_RECT))
        {
            //init x & y of explosion to zombie 3's x and y
            xExplosionPos = xZombie3EWpos;
            yExplosionPos = yZombie3EWpos;

            //Draw explosion
            SDL_RenderCopy(renderer, textureExplosionSheet, &rectExplosion, &rectExplosionPlacement);

            ////Destroy zombie 3
            zombie3Destroyed = true;
            destroyZombie3();

        }//END IF

    }//END IF
}

//not mod
bool CheckCollision(SDL_Rect r1,SDL_Rect r2)
{
    //check to see if rectangles overlap
    //if not, return false
    if(r1.x > r2.x+r2.w)
    {
      return false;
    }
    else if(r1.y > r2.y+r2.h)
    {
      return false;
    }
    else if(r1.x+r1.w < r2.x)
    {
      return false;
    }
    else if(r1.y+r1.h < r2.y)
    {
      return false;
    }
    else
    {
      return true;
    }
}

