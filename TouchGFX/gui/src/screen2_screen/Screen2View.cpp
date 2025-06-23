#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/Color.hpp>
#include <cstdlib>
#include <ctime> // Sử dụng std::clock()
#ifndef SIMULATOR
#include "stm32f4xx_hal.h" // Bao gồm HAL cho phần cứng
#include "stm32f4xx_hal_flash.h" // Bao gồm HAL Flash
#include "main.h"          // Bao gồm main.h để truy cập hrng
#include "cmsis_os.h"
#endif

#define FLASH_ADDR 0x080F0000 // sector 11

extern osMessageQueueId_t myQueue01Handle;
extern int bestScore;

Screen2View::Screen2View() : Screen2ViewBase()
{
#ifdef SIMULATOR
    // Trong môi trường giả lập, sử dụng std::clock() để sinh số ngẫu nhiên (để test)
    std::srand(static_cast<unsigned>(std::clock()));
#else
    // Trên phần cứng, không cần gieo hạt vì RNG phần cứng không cần
#endif
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gameBoard[i][j] = 0;
        }
    }

    score = 0;
#ifndef SIMULATOR
    loadBestScoreFromFlash(); // Đọc bestScore từ Flash trên phần cứng
#else
    bestScore = 9999; // Giá trị mặc định trong Simulator (để test)
#endif

    addNewTile();
    addNewTile();

    updateBoard();
    updateScore();
    updateBestScore();
}


void Screen2View::playSound()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  // Bật buzzer
    buzzerOn = true;
    buzzerCountdown = 10;  // ~160ms nếu tick 60Hz (tick mỗi 16ms)
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::addNewTile()
{
    int emptyCount = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == 0)
            {
                emptyCount++;
            }
        }
    }

    if (emptyCount == 0) return;

    // Sử dụng RNG phần cứng để chọn vị trí ngẫu nhiên
#ifdef SIMULATOR
    int newTilePos = std::rand() % emptyCount; // Không cần randomValue trong simulator
#else
    uint32_t randomValue;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomValue) != HAL_OK)
    {
        randomValue = 0; // Xử lý lỗi
    }
    int newTilePos = randomValue % emptyCount;
#endif

    int currentEmpty = -1;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == 0)
            {
                currentEmpty++;
                if (currentEmpty == newTilePos)
                {
#ifdef SIMULATOR
                    int r = std::rand() % 10;
#else
                    uint32_t randomValue;
                    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomValue) != HAL_OK)
                    {
                        randomValue = 0; // Xử lý lỗi
                    }
                    int r = randomValue % 10;
#endif
                    gameBoard[i][j] = (r < 9) ? 2 : 4;
                    return;
                }
            }
        }
    }
}

void Screen2View::updateBoard()
{
    TextAreaWithOneWildcard* textAreas[4][4] = {
        {&text_1_1, &text_1_2, &text_1_3, &text_1_4},
        {&text_2_1, &text_2_2, &text_2_3, &text_2_4},
        {&text_3_1, &text_3_2, &text_3_3, &text_3_4},
        {&text_4_1, &text_4_2, &text_4_3, &text_4_4}
    };
    Box* boxes[4][4] = {
        {&box_1_1, &box_1_2, &box_1_3, &box_1_4},
        {&box_2_1, &box_2_2, &box_2_3, &box_2_4},
        {&box_3_1, &box_3_2, &box_3_3, &box_3_4},
        {&box_4_1, &box_4_2, &box_4_3, &box_4_4}
    };
    Unicode::UnicodeChar* textBuffers[4][4] = {
        {text_1_1Buffer, text_1_2Buffer, text_1_3Buffer, text_1_4Buffer},
        {text_2_1Buffer, text_2_2Buffer, text_2_3Buffer, text_2_4Buffer},
        {text_3_1Buffer, text_3_2Buffer, text_3_3Buffer, text_3_4Buffer},
        {text_4_1Buffer, text_4_2Buffer, text_4_3Buffer, text_4_4Buffer}
    };
    const uint16_t textSizes[4][4] = {
        {TEXT_1_1_SIZE, TEXT_1_2_SIZE, TEXT_1_3_SIZE, TEXT_1_4_SIZE},
        {TEXT_2_1_SIZE, TEXT_2_2_SIZE, TEXT_2_3_SIZE, TEXT_2_4_SIZE},
        {TEXT_3_1_SIZE, TEXT_3_2_SIZE, TEXT_3_3_SIZE, TEXT_3_4_SIZE},
        {TEXT_4_1_SIZE, TEXT_4_2_SIZE, TEXT_4_3_SIZE, TEXT_4_4_SIZE}
    };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TextAreaWithOneWildcard& textArea = *textAreas[i][j];
            Box* box = boxes[i][j];
            Unicode::snprintf(textBuffers[i][j], textSizes[i][j], gameBoard[i][j] == 0 ? "" : "%d", gameBoard[i][j]);

            switch (gameBoard[i][j])
            {
                case 0: box->setColor(touchgfx::Color::getColorFromRGB(205, 193, 181)); break;
                case 2: box->setColor(touchgfx::Color::getColorFromRGB(238, 228, 218)); break;
                case 4: box->setColor(touchgfx::Color::getColorFromRGB(237, 224, 200)); break;
                case 8: box->setColor(touchgfx::Color::getColorFromRGB(242, 177, 121)); break;
                case 16: box->setColor(touchgfx::Color::getColorFromRGB(245, 149, 99)); break;
                case 32: box->setColor(touchgfx::Color::getColorFromRGB(246, 124, 95)); break;
                case 64: box->setColor(touchgfx::Color::getColorFromRGB(246, 94, 59)); break;
                case 128: box->setColor(touchgfx::Color::getColorFromRGB(237, 207, 114)); break;
                case 256: box->setColor(touchgfx::Color::getColorFromRGB(237, 204, 97)); break;
                case 512: box->setColor(touchgfx::Color::getColorFromRGB(237, 200, 80)); break;
                case 1024: box->setColor(touchgfx::Color::getColorFromRGB(237, 197, 63)); break;
                case 2048: box->setColor(touchgfx::Color::getColorFromRGB(237, 194, 46)); break;
                default: box->setColor(touchgfx::Color::getColorFromRGB(205, 193, 181)); break;
            }

            textArea.invalidate();
            box->invalidate();
        }
    }
}

void Screen2View::updateScore()
{
	Unicode::snprintf(text_scoreBuffer, TEXT_SCORE_SIZE, "%d", score);
	text_score.setWildcard(text_scoreBuffer);
	text_score.invalidate();
}

void Screen2View::updateBestScore()
{
    if (score > bestScore)
    {
    	bestScore = score;
#ifndef SIMULATOR
    saveBestScoreToFlash(); // Lưu vào Flash trên phần cứng
#endif
    }
    Unicode::snprintf(text_bestBuffer, TEXT_BEST_SIZE, "%d", bestScore);
    text_best.setWildcard(text_bestBuffer);
    text_best.invalidate();
}

void Screen2View::tickEvent()
{
    // Luôn xử lý buzzer nếu đang phát âm thanh (chỉ dành cho âm thanh ngắn khi di chuyển)
    if (buzzerOn)
    {
        buzzerCountdown--;
        if (buzzerCountdown <= 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Tắt buzzer
            buzzerOn = false;
        }
    }

    uint8_t res;
    if (osMessageQueueGetCount(myQueue01Handle) > 0)
    {
        osMessageQueueGet(myQueue01Handle, &res, NULL, osWaitForever);

        bool boardChanged = false;

        switch (res)
        {
            case 'W': boardChanged = slideUp();  break;
            case 'S': boardChanged = slideDown(); break;
            case 'A': boardChanged = slideLeft(); break;
            case 'D': boardChanged = slideRight(); break;
        }

        if (boardChanged)
        {
            playSound();  // chỉ âm di chuyển
            addNewTile();
            updateBoard();
            updateScore();

            if (isGameOver())
            {
                container_game_over.setVisible(true);
                container_game_over.invalidate();
                updateBestScore();
            }
        }
    }
}



bool Screen2View::slideLeft()
{
    bool moved = false;
    for (int i = 0; i < 4; i++)
    {
        int newPos = 0;
        int lastMerged = -1;
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == 0) continue;

            if (newPos > 0 && gameBoard[i][newPos - 1] == gameBoard[i][j] && lastMerged != newPos - 1)
            {
                gameBoard[i][newPos - 1] *= 2;
                score += gameBoard[i][newPos - 1];
                gameBoard[i][j] = 0;
                lastMerged = newPos - 1;
                moved = true;
            }
            else
            {
                if (newPos != j)
                {
                    gameBoard[i][newPos] = gameBoard[i][j];
                    gameBoard[i][j] = 0;
                    moved = true;
                }
                newPos++;
            }
        }
    }
    return moved;
}

bool Screen2View::slideRight()
{
    bool moved = false;
    for (int i = 0; i < 4; i++)
    {
        int newPos = 3;
        int lastMerged = 4;
        for (int j = 3; j >= 0; j--)
        {
            if (gameBoard[i][j] == 0) continue;

            if (newPos < 3 && gameBoard[i][newPos + 1] == gameBoard[i][j] && lastMerged != newPos + 1)
            {
                gameBoard[i][newPos + 1] *= 2;
                score += gameBoard[i][newPos + 1];
                gameBoard[i][j] = 0;
                lastMerged = newPos + 1;
                moved = true;
            }
            else
            {
                if (newPos != j)
                {
                    gameBoard[i][newPos] = gameBoard[i][j];
                    gameBoard[i][j] = 0;
                    moved = true;
                }
                newPos--;
            }
        }
    }
    return moved;
}

bool Screen2View::slideUp()
{
    bool moved = false;
    for (int j = 0; j < 4; j++)
    {
        int newPos = 0;
        int lastMerged = -1;
        for (int i = 0; i < 4; i++)
        {
            if (gameBoard[i][j] == 0) continue;

            if (newPos > 0 && gameBoard[newPos - 1][j] == gameBoard[i][j] && lastMerged != newPos - 1)
            {
                gameBoard[newPos - 1][j] *= 2;
                score += gameBoard[newPos - 1][j];
                gameBoard[i][j] = 0;
                lastMerged = newPos - 1;
                moved = true;
            }
            else
            {
                if (newPos != i)
                {
                    gameBoard[newPos][j] = gameBoard[i][j];
                    gameBoard[i][j] = 0;
                    moved = true;
                }
                newPos++;
            }
        }
    }
    return moved;
}

bool Screen2View::slideDown()
{
    bool moved = false;
    for (int j = 0; j < 4; j++)
    {
        int newPos = 3;
        int lastMerged = 4;
        for (int i = 3; i >= 0; i--)
        {
            if (gameBoard[i][j] == 0) continue;

            if (newPos < 3 && gameBoard[newPos + 1][j] == gameBoard[i][j] && lastMerged != newPos + 1)
            {
                gameBoard[newPos + 1][j] *= 2;
                score += gameBoard[newPos + 1][j];
                gameBoard[i][j] = 0;
                lastMerged = newPos + 1;
                moved = true;
            }
            else
            {
                if (newPos != i)
                {
                    gameBoard[newPos][j] = gameBoard[i][j];
                    gameBoard[i][j] = 0;
                    moved = true;
                }
                newPos--;
            }
        }
    }
    return moved;
}

bool Screen2View::isGameOver()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == 0) return false;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameBoard[i][j] == gameBoard[i][j + 1]) return false;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (gameBoard[i][j] == gameBoard[i + 1][j]) return false;
        }
    }

    return true;
}

void Screen2View::newGame()
{
    setupScreen();
}

void Screen2View::newGameOver()
{
	container_game_over.setVisible(false);
	container_game_over.invalidate();
    setupScreen();
}
void Screen2View::exitGame()
{
    application().gotoScreen1ScreenNoTransition();
}

// Hàm lưu bestScore vào Flash
#ifndef SIMULATOR
void Screen2View::saveBestScoreToFlash()
{
    HAL_FLASH_Unlock(); // Mở khóa Flash
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError;

    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.Sector = FLASH_SECTOR_11; // Sector 11 (0x080F0000)
    eraseInit.NbSectors = 1;
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&eraseInit, &sectorError) != HAL_OK)
    {
        Error_Handler();
        return;
    }

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_ADDR, bestScore) != HAL_OK)
    {
        Error_Handler();
        return;
    }

    HAL_FLASH_Lock(); // Khóa Flash lại
}
#endif

// Hàm đọc bestScore từ Flash
#ifndef SIMULATOR
void Screen2View::loadBestScoreFromFlash()
{
	uint32_t value = (*(__IO uint32_t*)FLASH_ADDR);

	    if (value != 0xFFFFFFFF)
	    {
	    	bestScore = value;
	    }
	    else
	    {
	    	bestScore = 0;
	    }
}
#endif


