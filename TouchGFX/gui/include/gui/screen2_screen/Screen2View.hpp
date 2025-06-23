#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void tickEvent();
    void newGame();
    void newGameOver();
    void exitGame();

protected:
    int gameBoard[4][4];
    int score;
    int bestScore;

    void addNewTile();
    void updateBoard();
    void updateScore();
    void updateBestScore();
    bool slideLeft();
    bool slideRight();
    bool slideUp();
    bool slideDown();
    bool isGameOver();
    void playSound(uint32_t freq, uint32_t durationMs);
    void playGameOverSound();

    void saveBestScoreToFlash();            // Hàm lưu bestScore vào Flash
    void loadBestScoreFromFlash();          // Hàm đọc bestScore từ Flash

};

#endif // SCREEN2VIEW_HPP
