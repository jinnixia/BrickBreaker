//Jinni Xia

#include "myLib.h"
#include "title.h"
#include "gameover.h"
#include "ball.h"
#include "won.h"
#include "text.h"
#include <stdlib.h>
#include <stdio.h>


enum GBAState{
	START, 
	PLAY,
	OVER,
	WON
};

int brickNum;
int lives;
int state;
int level;
int frame;

int main(void) {
	REG_DISPCNT = MODE_3 | BG2_EN;
	brickNum = 0;
	lives = 2;

	state = START;

	while(1) {
		while (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = START;
		}
		switch (state) {
			case START:
				start();
				break;
			case PLAY:
				play();
				break;
			case OVER:
				lostGame();
				break;
			case WON:
				wonGame();
				break;
		}
	}
	return 0;
}

void start() {
	frame = 0;
	while (state == START) {
		drawImage3(0, 0, 240, 160, title_frames[frame % TITLE_FRAMES]);
		waitForVblank();
		frame++;
		if (KEY_DOWN_NOW(BUTTON_A)) {
			//makes it so that the game doesn't move on until A is released and so that doesnt not keep redrawing itself
			while (KEY_DOWN_NOW(BUTTON_A));
			state = PLAY;
			initialize();
			makeBricks();
		}
	}
}

void play() {    
	while (state == PLAY) {
		fillScreen3(BLACK);
		movePaddle();
		moveBall();
		if (brickNum <= 0) {
			//if there are no more bricks left, you have won!
			state = WON;
		}
		updateBricks();

		//life counter at bottom of screen
		if (lives == 2) {
			char *buffer = "2 lives remaining";
			drawString(150, 5, buffer, YELLOW);
		} else if (lives == 1) {
			char *buffer = "1 life remaining";
			drawString(150, 5, buffer, YELLOW);
		} else {
			//no more lives, transitions to game over state
			state = OVER;
		}
		while (KEY_DOWN_NOW(BUTTON_SELECT)){
			state = START;
			brickNum = 0;
			lives = 2;
		}
		waitForVblank();
	}
}

void movePaddle() {
	paddle.prevX = paddle.x;
	paddle.prevY = paddle.y;
	if (KEY_DOWN_NOW(BUTTON_LEFT)){
		//only moves left if paddle is not at edge of left screen bound
		if (paddle.x - 2 >= 0) {
			paddle.x = paddle.x - 2;
		}
	}
	if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
		//only moves right if paddle is not at right of right screen bound 
		if (paddle.x + paddle.width + 2 <= 240) {
			paddle.x = paddle.x + 2;
		}
	}
	drawRect(paddle.y, paddle.x, paddle.height, paddle.width, BLUE);
}

void updateBricks() {
	for (int i = 0; i < brickNum; i++) {
		//checks ball and brick collision
		if ((bricks[i].x <= gBall.x + gBall.width &&
				bricks[i].x >= gBall.x &&
				bricks[i].y <= gBall.y + gBall.height &&
				bricks[i].y >= gBall.y) || (gBall.x <= bricks[i].x + bricks[i].width &&
				gBall.x >= bricks[i].x &&
				gBall.y <= bricks[i].y + bricks[i].height &&
				gBall.y >= bricks[i].y)) {
			gBall.yDir *= -1;

		//changes position of brick in array
			for (int k = i; k < brickNum - 1; k++) {
				bricks[k] = bricks[k + 1];
			}
			brickNum--;
			i--;
		}
		drawRect(bricks[i].y, bricks[i].x, bricks[i].height, bricks[i].width, bricks[i].color);
	}
}

void moveBall() {
	//if at the top of the screen, bounce back down
	if (gBall.y + gBall.yDir >= 0) {
		gBall.y += gBall.yDir;
	} else {
		gBall.yDir *= -1;
	}

	//change direction if ball is at left of screen
	if (gBall.x + gBall.xDir >= 0) {
			gBall.x += gBall.xDir;
	} else {
		gBall.xDir *= -1;
	}

	//change direction if ball is at right of screen
	if (gBall.x + gBall.xDir + gBall.width <= 240) {
			gBall.x += gBall.xDir;
	} else {
		gBall.xDir *= -1;
	}

	//checks if ball hits the paddle, change direction
	if (gBall.y + gBall.height > paddle.y) {
		if (paddle.x < gBall.x && gBall.x < paddle.x + paddle.width) {
			gBall.y = paddle.y - paddle.height - gBall.width;
			gBall.yDir *= -1;
		} else {
			//if ball doesn't hit the paddle, lose a life but continue the game
			gBall.y = paddle.y - gBall.width;
			gBall.yDir *= -1;
			lives--;
			if (lives == 0) {
				state = OVER;
			}
		}
	}
	gBall.prevY = gBall.y;
	gBall.prevX = gBall.x;
	drawImage3(gBall.y, gBall.x, gBall.height, gBall.width, ball);
}

void lostGame() {
	while (state == OVER) {
		drawImage3(0, 0, 240, 160, gameover);
		waitForVblank();
		//pressing a or select will allow you to restart
		if (KEY_DOWN_NOW(BUTTON_A)) {
			while (KEY_DOWN_NOW(BUTTON_A));
			state = START;
			brickNum = 0;
			lives = 2;
		}
		while (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = START;
			brickNum = 0;
			lives = 2;
		}
	}
}

void wonGame() {
	while (state == WON) {
		drawImage3(0, 0, 240, 160, won);
		waitForVblank();
		if (KEY_DOWN_NOW(BUTTON_A)) {
			while (KEY_DOWN_NOW(BUTTON_A));
			state = START;
			brickNum = 0;
			lives = 2;
		}
		while (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = START;
			brickNum = 0;
			lives = 2;
		}
	}
}


void makeBricks() {
	for (int i = 20; i < 220; i += 20) {
		Brick newBrick;
		newBrick.x = i;
		newBrick.y = 20;
		newBrick.width = 15;
		newBrick.height = 5;
		newBrick.color = RED;
		bricks[brickNum++] = newBrick;
	}
}

void initialize() {
	paddle.x = 75;
	paddle.y = 140;
	paddle.prevX = paddle.x;
	paddle.prevY = paddle.y;
	paddle.width = 35;
	paddle.height = 5;
	paddle.color = BLUE;

	gBall.width = 5;
	gBall.height = 5;
	gBall.color = BLUE;
	gBall.x =  80;
	gBall.y = 140 - gBall.height;
	gBall.prevX = gBall.x;
	gBall.prevY = gBall.y;
	gBall.xDir = -1;
	gBall.yDir = -1;
}
