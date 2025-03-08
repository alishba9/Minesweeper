#include "gamescene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QApplication>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
      m_game(),
      m_timer(new QTimer(this)),
      m_x(0),
      m_y(0),
      m_resetVal(false)
{
    loadPixmap();
    setSceneRect(0, 0, m_game.RESOLUTION.width(), m_game.RESOLUTION.height());

    // Connect a timer to periodically update the scene
    connect(m_timer, &QTimer::timeout, this, &GameScene::update);
    m_timer->start(static_cast<int>(m_game.ITERATION_VALUE));

    // Connect the mineClicked signal from the Game to the outMessage slot
    connect(&m_game, &Game::mineClicked, this, &GameScene::outMessage);
}

void GameScene::update()
{
    // Clear all existing items before redrawing
    clear();

    // Draw the face (smiley) at the top
    QGraphicsPixmapItem *faceItem =
            new QGraphicsPixmapItem(
                m_facePixmap.copy(
                    int(m_resetVal)*m_facePixmap.width()/2,
                    0,
                    m_facePixmap.width()/2,
                    m_facePixmap.height()));

    int sceneCenter = m_game.RESOLUTION.width() / 2;
    int faceWidth = m_facePixmap.width() / 2;
    faceItem->setPos(sceneCenter - faceWidth / 2, 0);
    addItem(faceItem);

    // Draw the grid
    for (int i = 1; i <= 30; i++) {
        for (int j = 1; j <= 16; j++) {
            // If the player clicked on a bomb at (m_x, m_y),
            // reveal the entire board
            if (m_game.sgrid[m_x][m_y] == 9) {
                m_game.sgrid[i][j] = m_game.grid[i][j];
            }

            // The tile index in sgrid determines which portion of the tileset is displayed
            QGraphicsPixmapItem *gridItem =
                    new QGraphicsPixmapItem(
                        m_tilesPixmap.copy(
                            m_game.sgrid[i][j]*m_game.w,
                            0,
                            m_game.w,
                            m_game.w));
            gridItem->setPos(i*m_game.w, j*m_game.w);
            addItem(gridItem);
        }
    }
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Qt::MouseButton button = event->button();
    QPointF clickedPos = event->scenePos();

    // Convert scene coordinates to grid indices
    m_x = static_cast<int>(clickedPos.x()) / m_game.w;
    m_y = static_cast<int>(clickedPos.y()) / m_game.w;

    qDebug() << "x " << m_x << " y " << m_y;

    // Left-click
    if (button == Qt::LeftButton) {
        // Reveal or reset logic
        m_game.sgrid[m_x][m_y] = m_game.grid[m_x][m_y];
        checkResetClick(m_x, m_y);
    }
    // Right-click
    else if (button == Qt::RightButton) {
        // Mark as flagged (assuming 11 is the 'flag' tile)
        m_game.sgrid[m_x][m_y] = 11;
    }

    QGraphicsScene::mousePressEvent(event);

    // If left-click on a bomb, emit mineClicked (handled in game.cpp via checkMine)
    if (button == Qt::LeftButton) {
        // This checks if (m_x, m_y) is a bomb
        if (m_game.checkMine(m_x, m_y)) {
            // If true, mineClicked was emitted and the game is effectively over
            return;
        } else {
            // Reveal the cell (non-bomb)
            m_game.sgrid[m_x][m_y] = m_game.grid[m_x][m_y];
            checkResetClick(m_x, m_y);
            update(); // Redraw
        }
    }
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // If the user clicked on the 'face' area at top row to reset
    if (m_resetVal) {
        m_resetVal = false;
        m_game.init(); // Re-initialize the board
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void GameScene::loadPixmap()
{
    // Load the tileset
    if (m_tilesPixmap.load(m_game.PATH_TO_TILES)) {
        qDebug() << "TilesPixmap is loaded successfully";
    } else {
        qDebug() << "TilesPixmap is not loaded successfully";
    }

    // Load the face (smiley) image
    if (m_facePixmap.load(m_game.PATH_TO_FACE)) {
        qDebug() << "FacePixmap is loaded successfully";
    } else {
        qDebug() << "FacePixmap is not loaded successfully";
    }
}

// If the user clicks on the face region, prepare for a reset
void GameScene::checkResetClick(int x, int y)
{
    // Arbitrary condition: if the y=0 row and x is 5 or 6 => reset
    if (y == 0 && (x == 5 || x == 6)) {
        m_resetVal = true;
    }
}

// Show a message box if the user hits a mine
void GameScene::outMessage()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Oh No!");
    msgBox.setText("You lose: you stepped on a MINE!");
    msgBox.setInformativeText("Would you like to Restart?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        m_game.init();
        update();
    } else if(ret == QMessageBox::Close){
        qApp->quit();
    }
}

