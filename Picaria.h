#ifndef PICARIA_H
#define PICARIA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Picaria;
}
QT_END_NAMESPACE

class Hole;

class Picaria : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(Picaria::Mode mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    enum Mode {
        NineHoles,
        ThirteenHoles
    };
    Q_ENUM(Mode)

    enum Player {
        RedPlayer,
        BluePlayer
    };
    Q_ENUM(Player)

    enum Phase {
        DropPhase,
        MovePhase
    };
    Q_ENUM(Phase)

    Picaria(QWidget *parent = nullptr);
    virtual ~Picaria();

    Picaria::Mode mode() const { return m_mode; }
    void setMode(Picaria::Mode mode);

signals:
    void modeChanged(Picaria::Mode mode);
    void gameOver(Player player);

private:
    Ui::Picaria *ui;
    Hole* m_holes[13];
    Mode m_mode;
    Player m_player;
    int m_dropCount;
    Hole* m_selected;
    Phase m_phase;

    bool isSelectable(Hole* hole);
    bool isGameOver(Hole* hole);
    void drop(Hole* hole);
    void move(Hole* hole);
    void switchPlayer();
    void clearSelectable();
    std::vector<Hole*> findSelectable(Hole* hole);

private slots:
    void play(int id);
    void reset();

    void showAbout();

    void updateMode(QAction* action);
    void updateStatusBar();
    void showGameOver(Player player);

};

#endif // PICARIA_H
