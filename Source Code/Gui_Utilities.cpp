
// Inclusions
#include "Animated_Texture.h"
#include "Gui_Utilities.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Texture_Manager.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Gui {

	SDL_Surface * pipeSlice;
	SDL_Surface * pipeUpEndingSurface;
	SDL_Surface * pipeDownEndingSurface;

	std::unique_ptr<Gui::Texture_Manager>  background;
	std::unique_ptr<SDL_Rect>              bottomBar1;
	std::unique_ptr<SDL_Rect>              bottomBar2;
	std::unique_ptr<Gui::Animated_Texture> bottomBarAnimation;
	std::unique_ptr<SDL_Rect>              bottomBar3;
	std::unique_ptr<SDL_Rect>              bottomBar4;
	std::unique_ptr<SDL_Rect>              bottomRectangle;
	std::array<SDL_Surface *, 10>          scoreSurfaces;
	std::array<SDL_Surface *, 10>          scoreMiniSurfaces;
	std::unique_ptr<Gui::Texture_Manager>  gameOverText;
	SDL_Surface *                          finalScorePanelSurface;
	std::unique_ptr<SDL_Rect>              groundHitBox;
	std::unique_ptr<Gui::Texture_Manager>  blackFadingScreen;

	unsigned long BEST_SCORE = 0;
	bool          replay_request = false;
	bool          blackScreenFaded = false;

	class Birb final {

	public:

		// Public constructor definitions
		Birb() : birbAnimation(nullptr), flapSound(nullptr), hitSound(nullptr),
			     birbHitBox({ static_cast<signed short>(Gui_Utilities::MAIN_WINDOW_WIDTH * 0.25 + 7),
			                  static_cast<signed short>(Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.48 + 3), 17 * 3 - 14, 12 * 3 - 6 }), xPos(0), yPos(0), frame(0), isIdle(true), velocity(0.0), force(0.7), angle(0.0),
			     hitSoundPlayed(false), fallingSoundPlayed(false), spaceHolding(false), fallingSoundFrame(0) {
		
		}

		// Public destructor definition
		~Birb() {
			Mix_FreeChunk(flapSound);
			Mix_FreeChunk(hitSound);
			Mix_FreeChunk(fallingSound);
		}

		// Public methods definitions
		void                                     loadBirb(SDL_Renderer * _renderer, const std::initializer_list<std::string> & _paths,
			                                     const unsigned char & _r, const unsigned char & _g, const unsigned char & _b) {
			birbAnimation = std::make_unique<Animated_Texture>(_renderer, _paths, _r, _g, _b);
		}
		void                                     loadFlapSound(const std::string & _path) {
			flapSound = Mix_LoadWAV(_path.c_str());
		}
		void                                     loadHitSound(const std::string & _path) {
			hitSound = Mix_LoadWAV(_path.c_str());
		}
		void                                     loadFallingSound(const std::string & _path) {
			fallingSound = Mix_LoadWAV(_path.c_str());
		}
		std::unique_ptr<Gui::Animated_Texture> & getBirbAnimation() {
			return birbAnimation;
		}
		void                                     userEventsHandler(bool & _gc, bool & _pc) {

			switch (Gui_Utilities::USER_EVENT_HANDLER.type) {

			case SDL_KEYDOWN: {

				switch (Gui_Utilities::USER_EVENT_HANDLER.key.keysym.sym) {
				
				case SDLK_SPACE:

					if (isIdle) 
						isIdle = false;
					
					if (yPos >= 40 && !spaceHolding && !_gc && !_pc) {
						velocity = -10.0;

						Mix_PlayChannel(-1, flapSound, 0);
					}

					spaceHolding = true;
					break;

				default:
					break;
				
				}

				break;
			}

			case SDL_KEYUP: {
				
				switch (Gui_Utilities::USER_EVENT_HANDLER.key.keysym.sym) {

				case SDLK_SPACE:
					spaceHolding = false;
					break;

				default:
					break;

				}

				break;
			}

			default:
				break;

			}

		}
		void                                     update(bool & _gc, bool & _pc) {

			if (isIdle) {
				frame = (Gui_Utilities::FRAME_COUNTER / 7) % 4;
				yPos = Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.45 + 10 * std::sin(Gui_Utilities::FRAME_COUNTER / static_cast<double>(8));

				for (std::size_t i = 0; i != birbAnimation->size(); ++i)
					birbAnimation->setDRY(i, yPos);

				birbHitBox.y = yPos + 3;
			}

			else {
			
				if (!_gc) {
					velocity = std::min(velocity + force, 15.0);
					angle = velocity >= 0.0 ? std::min((velocity + force) * 6, 90.0) : std::max((velocity + force) * 10, -24.0);
					yPos += velocity;
					frame = velocity < 6.0 ? (Gui_Utilities::FRAME_COUNTER / 2) % 4 : 1;

					for (std::size_t i = 0; i != birbAnimation->size(); ++i)
						birbAnimation->setDRY(i, yPos);

					birbHitBox.y = yPos + 3;
				}
			

				if (_pc) {
					++fallingSoundFrame;

					if (!hitSoundPlayed) {
						Mix_PlayChannel(-1, hitSound, 0);
						hitSoundPlayed = true;
					}

					if (!fallingSoundPlayed && fallingSoundFrame == 23) {
						Mix_PlayChannel(-1, fallingSound, 0);
						fallingSoundPlayed = true;
					}

				}

				if (_gc) {

					if (!hitSoundPlayed) {
						Mix_PlayChannel(-1, hitSound, 0);
						hitSoundPlayed = true;
					}

				}

			}

		}
		void                                     checkGroundCollision(const SDL_Rect & _otherHitBox, bool & _gc) {
			_gc = SDL_HasIntersection(&birbHitBox, &_otherHitBox);
		}
		void                                     checkPipesCollision(const SDL_Rect & _otherHitBox, bool & _pc) {
			_pc = SDL_HasIntersection(&birbHitBox, &_otherHitBox);
		}
		void                                     render() {
			birbAnimation->render(Gui_Utilities::MAIN_WINDOW_RENDERER, frame, angle);

			// Birb hitbox
			//SDL_SetRenderDrawColor(Gui_Utilities::MAIN_WINDOW_RENDERER, 255, 0, 0, 255);
			//SDL_RenderDrawRect(Gui_Utilities::MAIN_WINDOW_RENDERER, &birbHitBox);
		}

		// Public field
		std::unique_ptr<Gui::Animated_Texture> birbAnimation;
		Mix_Chunk *   flapSound;
		Mix_Chunk *   hitSound;
		Mix_Chunk *   fallingSound;
		SDL_Rect      birbHitBox;
		signed short  xPos;
		signed short  yPos;
		unsigned char frame;
		bool          isIdle;
		double        velocity;
		double        force;
		double        angle;
		bool          hitSoundPlayed;
		bool          fallingSoundPlayed;
		bool          spaceHolding;
		unsigned char fallingSoundFrame;

	};

	class Pipe_Up final {

	public:

		// Public constructor definitions
		Pipe_Up() : pipe(), pipeEnding(), pipeHitBox(), pipeEndingHitBox(), length(0), xPos(900) {

		}

		// Public destructor definition
		~Pipe_Up() {

		}

		// Public methods definitions
		void generateRandomPipe() {
			length = 100 + (std::rand() % (400 - 100));
			
			pipe.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, pipeSlice);
			pipe.setDRX(xPos);
			pipe.setDRW(78);
			pipe.setDRH(length);

			pipeHitBox.x = xPos;
			pipeHitBox.y = 0;
			pipeHitBox.w = 78;
			pipeHitBox.h = length + 3;

			pipeEnding.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, pipeUpEndingSurface);
			pipeEnding.setDRX(xPos - 3);
			pipeEnding.setDRY(length);
			pipeEnding.setDRW(84);
			pipeEnding.setDRH(39);

			pipeEndingHitBox.x = xPos - 3;
			pipeEndingHitBox.y = length + 3;
			pipeEndingHitBox.w = 84;
			pipeEndingHitBox.h = 39 - 3;
		}
		void update() {
			xPos -= 3;

			pipe.setDRX(xPos);
			pipeHitBox.x = xPos;

			pipeEnding.setDRX(xPos - 3);
			pipeEndingHitBox.x = xPos - 3;
		}
		void render() {
			pipe.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
		    pipeEnding.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
		}

		// Public field
		Texture_Manager pipe;
		Texture_Manager pipeEnding;
		SDL_Rect        pipeHitBox;
		SDL_Rect        pipeEndingHitBox;
		unsigned short  length;
		unsigned short  xPos;

	};

	class Pipe_Down final {

	public:

		// Public constructor definitions
		Pipe_Down() : pipe(), pipeEnding(), pipeHitBox(), pipeEndingHitBox(), xPos(900) {

		}

		// Public destructor definition
		~Pipe_Down() {

		}

		// Public methods definitions
		void generatePipe(const unsigned short & _pipeUpLength) {
			pipeEnding.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, pipeDownEndingSurface);
			pipeEnding.setDRX(xPos - 3);
			pipeEnding.setDRY(_pipeUpLength + 200);
			pipeEnding.setDRW(84);
			pipeEnding.setDRH(39);

			pipeEndingHitBox.x = xPos - 3;
			pipeEndingHitBox.y = _pipeUpLength + 200;
			pipeEndingHitBox.w = 84;
			pipeEndingHitBox.h = 39 - 3;

			pipe.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, pipeSlice);
			pipe.setDRX(xPos);
			pipe.setDRY(pipeEnding.getDRY() + pipeEnding.getDRH());
			pipe.setDRW(78);
			pipe.setDRH(bottomBar1->y - pipeEnding.getDRY() - pipeEnding.getDRH());

			pipeHitBox.x = xPos;
			pipeHitBox.y = pipeEnding.getDRY() + pipeEnding.getDRH() - 3;
			pipeHitBox.w = 78;
			pipeHitBox.h = bottomBar1->y - pipeEnding.getDRY() - pipeEnding.getDRH() + 3;
		}
		void update() {
			xPos -= 3;

			pipe.setDRX(xPos);
			pipeHitBox.x = xPos;

			pipeEnding.setDRX(xPos - 3);
			pipeEndingHitBox.x = xPos - 3;
		}
		void render() {
			pipe.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
			pipeEnding.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
		}

		// Public field
		Texture_Manager pipe;
		Texture_Manager pipeEnding;
		SDL_Rect        pipeHitBox;
		SDL_Rect        pipeEndingHitBox;
		unsigned short  xPos;

	};

	class Pipes_Manager final {

	public:

		// Public constructor definitions
		Pipes_Manager() : pipes(), counter(0), isIdle(true) {

		}

		// Public destructor definition
		~Pipes_Manager() {
		
		}

		// Public methods definitions
		void userEventsHandler() {

			switch (Gui_Utilities::USER_EVENT_HANDLER.type) {

			case SDL_KEYDOWN: {

				switch (Gui_Utilities::USER_EVENT_HANDLER.key.keysym.sym) {

				case SDLK_SPACE:

					if (isIdle)
						isIdle = false;

					break;

				default:
					break;

				}

				break;
			}

			default:
				break;

			}

		}
		void update(bool & _gc, bool & _pc) {

			if (!isIdle && !_gc && !_pc) {

				if (!(counter % 90)) {
					pipes.emplace_back();
					pipes[pipes.size() - 1].first.generateRandomPipe();
					pipes[pipes.size() - 1].second.generatePipe(pipes[pipes.size() - 1].first.length);
				}

				if (pipes[0].first.pipe.getDRX() <= -200)
					pipes.pop_front();

				for (std::pair<Pipe_Up, Pipe_Down> & elem : pipes) {
					elem.first.update();
					elem.second.update();
				}

				++counter;
			}

		}
		void render() {

			if (!isIdle) {
				for (std::pair<Pipe_Up, Pipe_Down> & elem : pipes) {
					elem.first.render();
					elem.second.render();
				}
			}

		}

		// Public fields
		std::deque<std::pair<Pipe_Up, Pipe_Down>> pipes;
		unsigned long long                        counter;
		bool                                      isIdle;

	};

	class Score_Manager final {

	public:

		// Public constructor definitions
		Score_Manager() : score(0), counter(0), scoreTextures(), increaseScoreSound(nullptr), isIdle(true) {

		}

		// Public destructor definition
		~Score_Manager() {
			Mix_FreeChunk(increaseScoreSound);
		}

		// Public methods definitions
		void loadZero() {
			scoreTextures.emplace_back();
			scoreTextures[0].loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, scoreSurfaces[0]);
			scoreTextures[0].setDRW(36);
			scoreTextures[0].setDRH(54);
			scoreTextures[0].setDRX((Gui_Utilities::MAIN_WINDOW_WIDTH - scoreTextures[0].getDRW()) / 2);
			scoreTextures[0].setDRY(130);
		}
		void loadIncreaseScoreSound(const std::string & _path) {
			increaseScoreSound = Mix_LoadWAV(_path.c_str());
		}
		void userEventsHandler() {

			switch (Gui_Utilities::USER_EVENT_HANDLER.type) {

			case SDL_KEYDOWN: {

				switch (Gui_Utilities::USER_EVENT_HANDLER.key.keysym.sym) {

				case SDLK_SPACE:

					if (isIdle)
						isIdle = false;

					break;

				default:
					break;

				}

				break;
			}

			default:
				break;

			}

		}
		void update(bool & _gc, bool & _pc) {

			if (!isIdle && !_gc && !_pc) {

				if (counter > 180 && !(counter % 90 - 65)) {
					++score;
					scoreTextures.clear();

					const std::string scoreToString = std::to_string(score);
					unsigned short totalScoreWidth = 0;

					for (unsigned char i = 0; i != scoreToString.size(); ++i) {
						scoreTextures.emplace_back();

						const unsigned char pos = std::stoi(std::string(1, scoreToString[i]));

						scoreTextures[i].loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, scoreSurfaces[pos]);
						scoreTextures[i].setDRW(pos == 1 ? 24 : 36);
						scoreTextures[i].setDRH(54);
						scoreTextures[i].setDRY(130);

						totalScoreWidth += scoreTextures[i].getDRW();
					}						

					scoreTextures[0].setDRX((Gui_Utilities::MAIN_WINDOW_WIDTH - totalScoreWidth) / 2);

					for (unsigned char i = 1; i != scoreTextures.size(); ++i)
						scoreTextures[i].setDRX(scoreTextures[i - 1].getDRX() + scoreTextures[i - 1].getDRW() - 3);

					Mix_PlayChannel(-1, increaseScoreSound, 0);
				}

				++counter;
			}

		}
		void render() {

			for (Texture_Manager & elem : scoreTextures)
				elem.render(Gui_Utilities::MAIN_WINDOW_RENDERER);

		}

		// Public field
		unsigned long                score;
		unsigned long long           counter;
		std::vector<Texture_Manager> scoreTextures;
		Mix_Chunk *                  increaseScoreSound;
		bool                         isIdle;

	};

	class Final_Stats final {

	public:

		// Public constructor definitions
		Final_Stats() : panelYPos(Gui_Utilities::MAIN_WINDOW_HEIGHT + 100), panel(),
			            currentScoreMiniTextures(), bestScoreMiniTextures(), scoreGenerated(false) {

		}

		// Public destructor definition
		~Final_Stats() {

		}

		// Public methods definitions
		void loadFinalStatsPanel() {
			panel.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, finalScorePanelSurface);
			panel.setDRW(226*2);
			panel.setDRH(114*2);
			panel.setDRX((Gui_Utilities::MAIN_WINDOW_WIDTH - panel.getDRW()) / 2);
			panel.setDRY(panelYPos);
		}
		void generateScores(const unsigned long & _currentScore) {
			const std::string currentScoreToString = std::to_string(_currentScore);
			unsigned short totalScoreWidth = 0;

			for (unsigned char i = 0; i != currentScoreToString.size(); ++i) {
				currentScoreMiniTextures.emplace_back();

				const unsigned char pos = std::stoi(std::string(1, currentScoreToString[i]));

				currentScoreMiniTextures[i].loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, scoreMiniSurfaces[pos]);
				currentScoreMiniTextures[i].setDRW(pos == 1 ? 10 * 1.5 : 14 * 1.5);
				currentScoreMiniTextures[i].setDRH(20 * 1.5);
				currentScoreMiniTextures[i].setDRY(panelYPos + 73);

				totalScoreWidth += currentScoreMiniTextures[i].getDRW();
			}

			currentScoreMiniTextures[0].setDRX(panel.getDRX() + panel.getDRW() - 42 - totalScoreWidth - 3 * currentScoreMiniTextures.size());

			for (unsigned char i = 1; i != currentScoreMiniTextures.size(); ++i)
				currentScoreMiniTextures[i].setDRX(currentScoreMiniTextures[i - 1].getDRX() + currentScoreMiniTextures[i - 1].getDRW() + 3);

			const std::string bestScoreToString = std::to_string(BEST_SCORE);
			totalScoreWidth = 0;

			for (unsigned char i = 0; i != bestScoreToString.size(); ++i) {
				bestScoreMiniTextures.emplace_back();

				const unsigned char pos = std::stoi(std::string(1, bestScoreToString[i]));

				bestScoreMiniTextures[i].loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, scoreMiniSurfaces[pos]);
				bestScoreMiniTextures[i].setDRW(pos == 1 ? 10 * 1.5 : 14 * 1.5);
				bestScoreMiniTextures[i].setDRH(20 * 1.5);
				bestScoreMiniTextures[i].setDRY(panelYPos + 155);

				totalScoreWidth += bestScoreMiniTextures[i].getDRW();
			}

			bestScoreMiniTextures[0].setDRX(panel.getDRX() + panel.getDRW() - 42 - totalScoreWidth - 3 * currentScoreMiniTextures.size());

			for (unsigned char i = 1; i != bestScoreMiniTextures.size(); ++i)
				bestScoreMiniTextures[i].setDRX(bestScoreMiniTextures[i - 1].getDRX() + bestScoreMiniTextures[i - 1].getDRW() + 3);

		}
		void update(const unsigned char & _val, const unsigned long & _currentScore) {

			if (_val == 30) {

				if (!scoreGenerated) {
					generateScores(_currentScore);
					scoreGenerated = true;
				}

				if (panelYPos > Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.34) {
					panelYPos -= 30;
					panel.setDRY(panelYPos);

					for (Texture_Manager & elem : currentScoreMiniTextures)
						elem.setDRY(elem.getDRY() - 30);

					for (Texture_Manager & elem : bestScoreMiniTextures)
						elem.setDRY(elem.getDRY() - 30);

				}

			}

		}
		void render() {
			panel.render(Gui_Utilities::MAIN_WINDOW_RENDERER);

			for (Texture_Manager & elem : currentScoreMiniTextures)
				elem.render(Gui_Utilities::MAIN_WINDOW_RENDERER);

			for (Texture_Manager & elem : bestScoreMiniTextures)
				elem.render(Gui_Utilities::MAIN_WINDOW_RENDERER);

		}

		// Public field
		signed short                 panelYPos;
		Texture_Manager              panel;
		std::vector<Texture_Manager> currentScoreMiniTextures;
		std::vector<Texture_Manager> bestScoreMiniTextures;
		bool                         scoreGenerated;

	};

	class Match_Manager final {

	public:

		// Constructor definition
		Match_Manager() = default;

		// Destructor definition
		~Match_Manager() {
			Mix_FreeChunk(swooshSound);
		}

		// Public methods definitions
		void loadMatch() {
			const unsigned char birbChooser = std::rand() % 3;

			theBirb.loadBirb(Gui_Utilities::MAIN_WINDOW_RENDERER, std::initializer_list<std::string>{
				(!birbChooser ? ".\\Resources\\Birb sprites\\Orange Birb 0.png" : birbChooser == 1 ? ".\\Resources\\Birb sprites\\Red Birb 0.png" : ".\\Resources\\Birb sprites\\Blue Birb 0.png"),
				(!birbChooser ? ".\\Resources\\Birb sprites\\Orange Birb 1.png" : birbChooser == 1 ? ".\\Resources\\Birb sprites\\Red Birb 1.png" : ".\\Resources\\Birb sprites\\Blue Birb 1.png"),
				(!birbChooser ? ".\\Resources\\Birb sprites\\Orange Birb 2.png" : birbChooser == 1 ? ".\\Resources\\Birb sprites\\Red Birb 2.png" : ".\\Resources\\Birb sprites\\Blue Birb 2.png"),
				(!birbChooser ? ".\\Resources\\Birb sprites\\Orange Birb 3.png" : birbChooser == 1 ? ".\\Resources\\Birb sprites\\Red Birb 3.png" : ".\\Resources\\Birb sprites\\Blue Birb 3.png")},
				255, 255, 255);

			for (std::size_t i = 0; i != theBirb.getBirbAnimation()->size(); ++i) {
				theBirb.xPos = Gui_Utilities::MAIN_WINDOW_WIDTH * 0.25;
				theBirb.yPos = Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.48;
				theBirb.getBirbAnimation()->setDRW(i, 17 * 3);
				theBirb.getBirbAnimation()->setDRH(i, 12 * 3);
				theBirb.getBirbAnimation()->setDRX(i, theBirb.xPos);
				theBirb.getBirbAnimation()->setDRY(i, theBirb.yPos);
			}

			theBirb.loadFlapSound(".\\Resources\\Sound Effects\\Flap.wav");
			theBirb.loadHitSound(".\\Resources\\Sound Effects\\Hit.wav");
			theBirb.loadFallingSound(".\\Resources\\Sound Effects\\Falling.wav");

			swooshSound = Mix_LoadWAV(".\\Resources\\Sound Effects\\Swoosh.wav");
			scoreManager.loadZero();
			scoreManager.loadIncreaseScoreSound(".\\Resources\\Sound Effects\\Increase Score.wav");

			getReadyTexture.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, ".\\Resources\\Get Ready.png", 255, 255, 255);
			getReadyTexture.setDRW(92 * 3);
			getReadyTexture.setDRH(86 * 3);
			getReadyTexture.setDRX((Gui_Utilities::MAIN_WINDOW_WIDTH - getReadyTexture.getDRW()) / 2);
			getReadyTexture.setDRY(Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.3);
			getReadyTexture.allowAlphaBlending();

			whiteFadingScreen.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, ".\\Resources\\Fade Out Color.png");
			whiteFadingScreen.setDRW(Gui_Utilities::MAIN_WINDOW_WIDTH);
			whiteFadingScreen.setDRH(Gui_Utilities::MAIN_WINDOW_HEIGHT);
			whiteFadingScreen.allowAlphaBlending();

			finalScorePanel.loadFinalStatsPanel();

			playAgainTexture.loadTexture(Gui_Utilities::MAIN_WINDOW_RENDERER, ".\\Resources\\Play Again.png", 255, 255, 255);
			playAgainTexture.setDRW(104 * 2);
			playAgainTexture.setDRH(58 * 2);
			playAgainTexture.setDRX((Gui_Utilities::MAIN_WINDOW_WIDTH - playAgainTexture.getDRW()) / 2);
			playAgainTexture.setDRY(Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.615);
		}
		void userEventsHandler() {
			pipesManager.userEventsHandler();
			theBirb.userEventsHandler(birbHasGroundCollided, birbHasPipeCollided);
			scoreManager.userEventsHandler();
		}
		void update() {
			pipesManager.update(birbHasGroundCollided, birbHasPipeCollided);
			theBirb.update(birbHasGroundCollided, birbHasPipeCollided);
			scoreManager.update(birbHasGroundCollided, birbHasPipeCollided);
			finalScorePanel.update(gameOverFrameOffset, scoreManager.score);

			if (!theBirb.isIdle) {

				if (getReadyTexture.getAlphaValue() > 6)
					getReadyTexture.setAlphaValue(getReadyTexture.getAlphaValue() - 6);

				else
					getReadyTexture.setAlphaValue(0);

			}

		}
		void render() {
			pipesManager.render();
			theBirb.render();

			if (whiteFadingScreenFrameOffset != 30)
				scoreManager.render();

			getReadyTexture.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
		}
		void renderFadingScreen() {
		
			if (whiteFadingScreen.getAlphaValue() > 10) {
				whiteFadingScreen.setAlphaValue(whiteFadingScreen.getAlphaValue() - 7);

				whiteFadingScreen.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
			}

			else if (whiteFadingScreenFrameOffset < 30)
				++whiteFadingScreenFrameOffset;

		}
		void renderGameOverText() {

			if (gameOverFrame < 25) {
				gameOverText->setDRY(Gui_Utilities::MAIN_WINDOW_HEIGHT * 0.2 + 9 * std::sin(-gameOverFrame / static_cast<double>(5)));

				++gameOverFrame;
			}

			else if (gameOverFrameOffset < 30)
				++gameOverFrameOffset;

			gameOverText->render(Gui_Utilities::MAIN_WINDOW_RENDERER);

			if (!firstSwooshPlayed) {
				Mix_PlayChannel(-1, swooshSound, 0);
				firstSwooshPlayed = true;
			}

		}
		void renderFinalScorePanel() {
			finalScorePanel.render();

			if (!secondSwooshPlayed) {
				Mix_PlayChannel(-1, swooshSound, 0);
				secondSwooshPlayed = true;
			}

			if (finalScorePanelFrameOffset < 60)
				++finalScorePanelFrameOffset;

		}
		void renderPlayAgain() {
			playAgainTexture.render(Gui_Utilities::MAIN_WINDOW_RENDERER);
		}

		// Public fields
		Birb            theBirb;
		Pipes_Manager   pipesManager;
		Score_Manager   scoreManager;
		Texture_Manager getReadyTexture;
		bool            birbHasGroundCollided;
		bool            birbHasPipeCollided;
		Texture_Manager whiteFadingScreen;
		unsigned char   whiteFadingScreenFrameOffset;
		unsigned char   gameOverFrame;
		unsigned char   gameOverFrameOffset;
		unsigned char   bottomBarAnimationFrame;
		Mix_Chunk *     swooshSound;
		bool            firstSwooshPlayed;
		Final_Stats     finalScorePanel;
		unsigned char   finalScorePanelFrameOffset;
		bool            secondSwooshPlayed;
		Texture_Manager playAgainTexture;

	};

	std::unique_ptr<Match_Manager> match;

}

// Public static methods definitions
void Gui::Gui_Utilities::programInitialization() {
	std::srand(std::time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("SDL2 environment failed to initialize!");

	const int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

	if ((IMG_Init(flags) & flags) != flags)
		throw std::runtime_error("SDL_Image environment failed to initialize!");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		throw std::runtime_error("SDL_Mixer environment failed to initialize!");

	if (TTF_Init() < 0)
		throw std::runtime_error("TTF environment failed to initialize!");

	MAIN_WINDOW = SDL_CreateWindow(PROGRAM_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN |
		                                                                                                                                        SDL_WINDOW_OPENGL);

	if (!MAIN_WINDOW)
		throw std::runtime_error("Main Window initialization failed!");

	MAIN_WINDOW_SURFACE = SDL_GetWindowSurface(MAIN_WINDOW);
	MAIN_WINDOW_RENDERER = SDL_CreateRenderer(MAIN_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 255, 250, 240, 255);

	if (SDL_GetCurrentDisplayMode(0, &DISPLAY_MODE))
		throw std::runtime_error("Current display mode initialization failed!");

	MAIN_FONT = TTF_OpenFont("C:\\Users\\Andrea Frasca\\Desktop\\PathFinding tests folder\\Times New Roman.ttf", 15);
}
void Gui::Gui_Utilities::initializeGuiElements() {
	background = std::make_unique<Texture_Manager>();

	const bool backgroundChooser = std::rand() % 2;

	background->loadTexture(MAIN_WINDOW_RENDERER, backgroundChooser ? ".\\Resources\\Background Day.png" : ".\\Resources\\Background Night.png");
	background->setDRW(MAIN_WINDOW_WIDTH);
	background->setDRH(static_cast<unsigned short>(MAIN_WINDOW_HEIGHT * 0.78));

	bottomBar1 = std::make_unique<SDL_Rect>(SDL_Rect{ 0, background->getDRY() + background->getDRH(), MAIN_WINDOW_WIDTH, 3 });
	bottomBar2 = std::make_unique<SDL_Rect>(SDL_Rect{ 0, bottomBar1->y + bottomBar1->h, MAIN_WINDOW_WIDTH, 3 });

	bottomBarAnimation = std::make_unique<Animated_Texture>(MAIN_WINDOW_RENDERER,
		std::initializer_list<std::string>{".\\Resources\\Bottom bar sprites\\Bottom animated layer 0.png",
		".\\Resources\\Bottom bar sprites\\Bottom animated layer 1.png", ".\\Resources\\Bottom bar sprites\\Bottom animated layer 2.png",
		".\\Resources\\Bottom bar sprites\\Bottom animated layer 3.png", ".\\Resources\\Bottom bar sprites\\Bottom animated layer 4.png",
		".\\Resources\\Bottom bar sprites\\Bottom animated layer 5.png", ".\\Resources\\Bottom bar sprites\\Bottom animated layer 6.png",
		".\\Resources\\Bottom bar sprites\\Bottom animated layer 7.png", ".\\Resources\\Bottom bar sprites\\Bottom animated layer 8.png",
		".\\Resources\\Bottom bar sprites\\Bottom animated layer 9.png", ".\\Resources\\Bottom bar sprites\\Bottom animated layer 10.png" });

	for (std::size_t i = 0; i != bottomBarAnimation->size(); ++i) {
		bottomBarAnimation->setDRY(i, bottomBar2->y + bottomBar2->h);
		bottomBarAnimation->setDRW(i, MAIN_WINDOW_WIDTH);
	}

	bottomBar3 = std::make_unique<SDL_Rect>(SDL_Rect{ 0, bottomBarAnimation->getDRY(0) + bottomBarAnimation->getDRH(0), MAIN_WINDOW_WIDTH, 3 });
	bottomBar4 = std::make_unique<SDL_Rect>(SDL_Rect{ 0, bottomBar3->y + bottomBar3->h, MAIN_WINDOW_WIDTH, 3 });
	bottomRectangle = std::make_unique<SDL_Rect>(SDL_Rect{ 0, bottomBar4->y + bottomBar4->h, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT - bottomBar4->y - bottomBar4->h });

	for (unsigned char i = 0; i != 10; ++i) {
		scoreSurfaces[i] = IMG_Load(std::string(".\\Resources\\Scores\\Score " + std::to_string(i) + ".png").c_str());
		SDL_SetColorKey(scoreSurfaces[i], SDL_TRUE, SDL_MapRGB(scoreSurfaces[i]->format, 255, 255, 255));
	}

	for (unsigned char i = 0; i != 10; ++i) {
		scoreMiniSurfaces[i] = IMG_Load(std::string(".\\Resources\\Scores\\Score Mini " + std::to_string(i) + ".png").c_str());
		SDL_SetColorKey(scoreMiniSurfaces[i], SDL_TRUE, SDL_MapRGB(scoreMiniSurfaces[i]->format, 255, 255, 255));
	}

	gameOverText = std::make_unique<Texture_Manager>();
	gameOverText->loadTexture(MAIN_WINDOW_RENDERER, ".\\Resources\\Game Over.png", 255, 255, 255);
	gameOverText->setDRW(96 * 3);
	gameOverText->setDRH(21 * 3);
	gameOverText->setDRX((MAIN_WINDOW_WIDTH - gameOverText->getDRW()) / 2);
	gameOverText->setDRY(static_cast<unsigned short>(MAIN_WINDOW_HEIGHT * 0.2));

	finalScorePanelSurface = IMG_Load(".\\Resources\\Score Panel.png");
	SDL_SetColorKey(finalScorePanelSurface, SDL_TRUE, SDL_MapRGB(finalScorePanelSurface->format, 255, 255, 255));

	match = std::make_unique<Match_Manager>();
	match->loadMatch();

	groundHitBox = std::make_unique<SDL_Rect>(SDL_Rect{ 0, bottomBar1->y, MAIN_WINDOW_WIDTH, bottomBar1->h + bottomBar2->h +
																							 bottomBarAnimation->sprites[0].getDRH() +
																							 bottomBar3->h + bottomBar4->h +
																							 bottomRectangle->h });
	
	pipeSlice = IMG_Load(".\\Resources\\Pipe Slice.png");

	pipeUpEndingSurface = IMG_Load(".\\Resources\\Pipe Up Ending.png");
	SDL_SetColorKey(pipeUpEndingSurface, SDL_TRUE, SDL_MapRGB(pipeUpEndingSurface->format, 255, 255, 255));

	pipeDownEndingSurface = IMG_Load(".\\Resources\\Pipe Down Ending.png");
	SDL_SetColorKey(pipeDownEndingSurface, SDL_TRUE, SDL_MapRGB(pipeDownEndingSurface->format, 255, 255, 255));

	blackFadingScreen = std::make_unique<Texture_Manager>();
	blackFadingScreen->loadTexture(MAIN_WINDOW_RENDERER, ".\\Resources\\Replay Fading Screen Color.png");
	blackFadingScreen->setDRW(Gui_Utilities::MAIN_WINDOW_WIDTH);
	blackFadingScreen->setDRH(Gui_Utilities::MAIN_WINDOW_HEIGHT);
	blackFadingScreen->allowAlphaBlending();
	blackFadingScreen->setAlphaValue(0);
}
void Gui::Gui_Utilities::guiMainLoop() {

	while (!CLOSE_REQUEST) {
		increaseFrameCounter();

		userEventsHandler();

		updateProgramStatus();

		rendering();

		//SDL_Delay(200);
	}

}
void Gui::Gui_Utilities::increaseFrameCounter() {
	++FRAME_COUNTER;
}
void Gui::Gui_Utilities::userEventsHandler() {

	while (SDL_PollEvent(&USER_EVENT_HANDLER)) {
		match->userEventsHandler();

		switch (USER_EVENT_HANDLER.type) {
		
		case SDL_QUIT:
			CLOSE_REQUEST = true;
			break;

		case SDL_MOUSEBUTTONDOWN: {
		
			if (match->finalScorePanelFrameOffset == 60 && !replay_request) {
				signed int x, y;

				SDL_GetMouseState(&x, &y);

				SDL_Point p{ x,y };

				if (SDL_PointInRect(&p, &match->playAgainTexture.destinationRectangle))
					replay_request = true;

			}
		
			break;
		}

		default:
			break;
		
		}

	}

}
void Gui::Gui_Utilities::updateProgramStatus() {
	match->update();
	
	for (const std::pair<Pipe_Up, Pipe_Down> & elem : match->pipesManager.pipes) {

		if (!match->birbHasPipeCollided)
			match->theBirb.checkPipesCollision(elem.first.pipeHitBox, match->birbHasPipeCollided);

		if (!match->birbHasPipeCollided)
			match->theBirb.checkPipesCollision(elem.first.pipeEndingHitBox, match->birbHasPipeCollided);

		if (!match->birbHasPipeCollided)
			match->theBirb.checkPipesCollision(elem.second.pipeEndingHitBox, match->birbHasPipeCollided);

		if (!match->birbHasPipeCollided)
			match->theBirb.checkPipesCollision(elem.second.pipeHitBox, match->birbHasPipeCollided);

	}

	match->theBirb.checkGroundCollision(*groundHitBox, match->birbHasGroundCollided);

	if ((match->birbHasGroundCollided || match->birbHasPipeCollided) && match->scoreManager.score > BEST_SCORE)
		BEST_SCORE = match->scoreManager.score;

}
void Gui::Gui_Utilities::rendering() {
	SDL_RenderClear(MAIN_WINDOW_RENDERER);

	background->render(MAIN_WINDOW_RENDERER);

	match->render();

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 81, 56, 75, 255);
	SDL_RenderFillRect(MAIN_WINDOW_RENDERER, bottomBar1.get());
	
	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 231, 251, 140, 255);
	SDL_RenderFillRect(MAIN_WINDOW_RENDERER, bottomBar2.get());

	if (!match->birbHasGroundCollided && !match->birbHasPipeCollided) {
		match->bottomBarAnimationFrame = FRAME_COUNTER % 11;
		bottomBarAnimation->render(MAIN_WINDOW_RENDERER, match->bottomBarAnimationFrame);
	}

	else
		bottomBarAnimation->render(MAIN_WINDOW_RENDERER, match->bottomBarAnimationFrame);

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 97, 146, 43, 255);
	SDL_RenderFillRect(MAIN_WINDOW_RENDERER, bottomBar3.get());

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 215, 173, 84, 255);
	SDL_RenderFillRect(MAIN_WINDOW_RENDERER, bottomBar4.get());

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 222, 216, 149, 255);
	SDL_RenderFillRect(MAIN_WINDOW_RENDERER, bottomRectangle.get());

	if ((match->birbHasPipeCollided || match->birbHasGroundCollided))
		match->renderFadingScreen();

	if (match->whiteFadingScreenFrameOffset == 30)
		match->renderGameOverText();

	if (match->gameOverFrameOffset == 30)
		match->renderFinalScorePanel();

	if (match->finalScorePanelFrameOffset == 60)
		match->renderPlayAgain();

	if (replay_request) {
	
		if (!blackScreenFaded) {
		
			if (blackFadingScreen->getAlphaValue() < 245)
				blackFadingScreen->setAlphaValue(blackFadingScreen->getAlphaValue() + 7);

			else {
				blackFadingScreen->setAlphaValue(255);

				match = std::make_unique<Match_Manager>();
				match->loadMatch();

				background = std::make_unique<Texture_Manager>();

				const bool backgroundChooser = std::rand() % 2;

				background->loadTexture(MAIN_WINDOW_RENDERER, backgroundChooser ? ".\\Resources\\Background Day.png" : ".\\Resources\\Background Night.png");
				background->setDRW(MAIN_WINDOW_WIDTH);
				background->setDRH(static_cast<unsigned short>(MAIN_WINDOW_HEIGHT * 0.78));

				blackScreenFaded = true;
			}

		}

		else {

			if (blackFadingScreen->getAlphaValue() > 10)
				blackFadingScreen->setAlphaValue(blackFadingScreen->getAlphaValue() - 7);

			else {
				blackFadingScreen->setAlphaValue(0);
				blackScreenFaded = false;
				replay_request = false;
			}

		}

		blackFadingScreen->render(Gui_Utilities::MAIN_WINDOW_RENDERER);
	}

	SDL_SetRenderDrawColor(MAIN_WINDOW_RENDERER, 255, 250, 240, 255);
	SDL_RenderPresent(MAIN_WINDOW_RENDERER);
}
void Gui::Gui_Utilities::programTermination() {

	for (unsigned char i = 0; i != 10; ++i)
		SDL_FreeSurface(scoreSurfaces[i]);

	for (unsigned char i = 0; i != 10; ++i)
		SDL_FreeSurface(scoreMiniSurfaces[i]);

	SDL_FreeSurface(pipeSlice);
	SDL_FreeSurface(pipeUpEndingSurface);
	SDL_FreeSurface(pipeDownEndingSurface);
	SDL_FreeSurface(finalScorePanelSurface);

	TTF_CloseFont(MAIN_FONT);
	SDL_DestroyRenderer(MAIN_WINDOW_RENDERER);
	SDL_DestroyWindow(MAIN_WINDOW);

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Public static fields definitions
std::string        Gui::Gui_Utilities::PROGRAM_NAME("Flappy Birb");
unsigned short     Gui::Gui_Utilities::MAIN_WINDOW_WIDTH(540);
unsigned short     Gui::Gui_Utilities::MAIN_WINDOW_HEIGHT(960);
SDL_Window *       Gui::Gui_Utilities::MAIN_WINDOW;
SDL_Surface *      Gui::Gui_Utilities::MAIN_WINDOW_SURFACE;
SDL_Renderer *     Gui::Gui_Utilities::MAIN_WINDOW_RENDERER;
SDL_Event          Gui::Gui_Utilities::USER_EVENT_HANDLER;
SDL_DisplayMode    Gui::Gui_Utilities::DISPLAY_MODE;
TTF_Font *         Gui::Gui_Utilities::MAIN_FONT;
unsigned long long Gui::Gui_Utilities::FRAME_COUNTER(0);
bool               Gui::Gui_Utilities::CLOSE_REQUEST(false);
