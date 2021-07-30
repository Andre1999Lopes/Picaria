#include "Picaria.h"
#include "ui_Picaria.h"

#include <vector>
#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>

Picaria::Player state2player(Hole::State state) {
    switch (state) {
        case Hole::RedState:
            return Picaria::RedPlayer;
        case Hole::BlueState:
            return Picaria::BluePlayer;
        default:
            Q_UNREACHABLE();
    }
}

Hole::State player2state(Picaria::Player player) {
    return player == Picaria::RedPlayer ? Hole::RedState : Hole::BlueState;
}

Picaria::Picaria(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Picaria),
      m_mode(Picaria::NineHoles),
      m_player(Picaria::RedPlayer),
      m_dropCount(0),
      m_selected(nullptr),
      m_phase(Picaria::DropPhase) {

    ui->setupUi(this);

    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    modeGroup->addAction(ui->action9holes);
    modeGroup->addAction(ui->action13holes);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(modeGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateMode(QAction*)));
    QObject::connect(this, SIGNAL(modeChanged(Picaria::Mode)), this, SLOT(reset()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(showGameOver(Player)));
    QObject::connect(this, SIGNAL(gameOver(Player)), this, SLOT(reset()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int id = 0; id < 13; ++id) {
        QString holeName = QString("hole%1").arg(id+1, 2, 10, QChar('0'));
        Hole* hole = this->findChild<Hole*>(holeName);
        Q_ASSERT(hole != nullptr);

        m_holes[id] = hole;
        hole->setIndex(id);
        map->setMapping(hole, id);
        QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));
        hole->setNeighbors(id, this->m_mode);
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

Picaria::~Picaria() {
    delete ui;
}

void Picaria::setMode(Picaria::Mode mode) {
    if (m_mode != mode) {
        m_mode = mode;
        emit modeChanged(mode);
    }
}

void Picaria::switchPlayer() {
    m_player = m_player == Picaria::RedPlayer ?
                    Picaria::BluePlayer : Picaria::RedPlayer;
    this->updateStatusBar();
}

void Picaria::play(int id) {
    Hole* hole = m_holes[id];

    switch(m_phase) {
        case Picaria::DropPhase:
            drop(hole);
        break;

        case Picaria::MovePhase:
            move(hole);
        break;

        default:
            Q_UNREACHABLE();
    }
}

void Picaria::drop(Hole* hole) {
    if (hole->state() == Hole::EmptyState) {
        hole->setState(player2state(m_player));

        if (isGameOver(hole))
            emit gameOver(m_player);
        else {
            ++m_dropCount;
            if (m_dropCount == 6)
                m_phase = Picaria::MovePhase;
            this->switchPlayer();
        }
    }
}

void Picaria::move(Hole* hole) {
    QPair<Hole*,Hole*>* movement = nullptr;

        if (hole->state() == Hole::SelectableState) {
            Q_ASSERT(m_selected != 0);
            movement = new QPair<Hole*,Hole*>(m_selected, hole);
        }
        else {
            if (hole->state() == player2state(m_player)) {
                std::vector<Hole*> selectable = findSelectable(hole);
                this->clearSelectable();
                foreach (Hole* tmp, selectable)
                    tmp->setState(Hole::SelectableState);

                m_selected = hole;
            }
        }

        if (movement != nullptr) {
            this->clearSelectable();
            m_selected = 0;

            qDebug() << "movement 1: " << movement->first->state();
            qDebug() << "movement 2: " << movement->second->state();

            Q_ASSERT(movement->first->state() == player2state(m_player));
            Q_ASSERT(movement->second->state() == Hole::EmptyState);

            movement->first->setState(Hole::EmptyState);
            movement->second->setState(player2state(m_player));

            if (isGameOver(movement->second))
                emit gameOver(m_player);
            else
                this->switchPlayer();

            delete movement;
        }
}

void Picaria::clearSelectable() {
    for (int i=0; i< 13; i++) {
        Hole* hole = m_holes[i];
        if (hole->state() == Hole::SelectableState)
            hole->setState(Hole::EmptyState);
    }
}

std::vector<Hole*> Picaria::findSelectable(Hole* hole) {
    std::vector<Hole*> selectableNeighbors;
    std::vector<int> neighbors = hole->getNeighbors();

    foreach(int tmp, neighbors) {
        if (isSelectable(m_holes[tmp])) {
            selectableNeighbors.push_back(m_holes[tmp]);
        }
    }

    return selectableNeighbors;
}

bool Picaria::isSelectable(Hole* hole) {
    qDebug() << "state: " << hole->state();
    return hole != nullptr &&
            (hole->state() == Hole::EmptyState ||
             hole->state() == Hole::SelectableState);
}

bool Picaria::isGameOver(Hole* hole) {
    Hole::State state = hole->state();
    switch(m_mode) {
        case NineHoles:
            switch(hole->getIndex()) {
                case 0:
                    return (m_holes[1]->state() == state && m_holes[2]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[5]->state() == state && m_holes[10]->state() == state);
                break;

                case 1:
                    return (m_holes[0]->state() == state && m_holes[2]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[11]->state() == state);
                break;

                case 2:
                    return (m_holes[0]->state() == state && m_holes[1]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[10]->state() == state) ||
                            (m_holes[7]->state() == state && m_holes[12]->state() == state);
                break;

                case 5:
                    return (m_holes[0]->state() == state && m_holes[10]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[7]->state() == state);
                break;

                case 6:
                   return (m_holes[0]->state() == state && m_holes[12]->state() == state) ||
                           (m_holes[1]->state() == state && m_holes[11]->state() == state) ||
                           (m_holes[2]->state() == state && m_holes[10]->state() == state) ||
                           (m_holes[5]->state() == state && m_holes[7]->state() == state);
                break;

                case 7:
                    return (m_holes[2]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[5]->state() == state && m_holes[6]->state() == state);
                break;

                case 10:
                    return (m_holes[0]->state() == state && m_holes[5]->state() == state) ||
                            (m_holes[2]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[11]->state() == state && m_holes[12]->state() == state);
                break;

                case 11:
                    return (m_holes[10]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[1]->state() == state && m_holes[6]->state() == state);
                break;

                case 12:
                    return (m_holes[2]->state() == state && m_holes[7]->state() == state) ||
                            (m_holes[0]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[10]->state() == state && m_holes[11]->state() == state);
                break;

                default:
                    return false;
                break;
            }
        break;

        case ThirteenHoles:
            switch(hole->getIndex()) {
                case 0:
                    return (m_holes[1]->state() == state && m_holes[2]->state() == state) ||
                            (m_holes[3]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[5]->state() == state && m_holes[10]->state() == state);
                break;

                case 1:
                    return (m_holes[0]->state() == state && m_holes[2]->state() == state) ||
                            (m_holes[3]->state() == state && m_holes[5]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[11]->state() == state) ||
                            (m_holes[4]->state() == state && m_holes[7]->state() == state);
                break;

                case 2:
                    return (m_holes[0]->state() == state && m_holes[1]->state() == state) ||
                            (m_holes[4]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[7]->state() == state && m_holes[12]->state() == state);
                break;

                case 3:
                    return (m_holes[0]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[1]->state() == state && m_holes[5]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[9]->state() == state);
                break;

                case 4:
                    return (m_holes[2]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[1]->state() == state && m_holes[7]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[8]->state() == state);
                break;

                case 5:
                    return (m_holes[0]->state() == state && m_holes[10]->state() == state) ||
                            (m_holes[1]->state() == state && m_holes[3]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[7]->state() == state) ||
                            (m_holes[8]->state() == state && m_holes[11]->state() == state);
                break;

                case 6:
                   return (m_holes[0]->state() == state && m_holes[3]->state() == state) ||
                           (m_holes[1]->state() == state && m_holes[11]->state() == state) ||
                           (m_holes[2]->state() == state && m_holes[4]->state() == state) ||
                           (m_holes[5]->state() == state && m_holes[7]->state() == state) ||
                           (m_holes[9]->state() == state && m_holes[12]->state() == state) ||
                           (m_holes[8]->state() == state && m_holes[10]->state() == state) ||
                           (m_holes[3]->state() == state && m_holes[9]->state() == state) ||
                           (m_holes[4]->state() == state && m_holes[8]->state() == state);
                break;

                case 7:
                    return (m_holes[1]->state() == state && m_holes[4]->state() == state) ||
                            (m_holes[2]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[9]->state() == state && m_holes[11]->state() == state) ||
                            (m_holes[5]->state() == state && m_holes[6]->state() == state);
                break;

                case 8:
                    return (m_holes[5]->state() == state && m_holes[11]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[10]->state() == state) ||
                            (m_holes[4]->state() == state && m_holes[6]->state() == state);
                break;

                case 9:
                    return (m_holes[6]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[7]->state() == state && m_holes[11]->state() == state) ||
                            (m_holes[3]->state() == state && m_holes[6]->state() == state);
                break;

                case 10:
                    return (m_holes[0]->state() == state && m_holes[5]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[8]->state() == state) ||
                            (m_holes[11]->state() == state && m_holes[12]->state() == state);
                break;

                case 11:
                    return (m_holes[10]->state() == state && m_holes[12]->state() == state) ||
                            (m_holes[5]->state() == state && m_holes[8]->state() == state) ||
                            (m_holes[1]->state() == state && m_holes[6]->state() == state) ||
                            (m_holes[7]->state() == state && m_holes[9]->state() == state);
                break;

                case 12:
                    return (m_holes[2]->state() == state && m_holes[7]->state() == state) ||
                            (m_holes[6]->state() == state && m_holes[9]->state() == state) ||
                            (m_holes[10]->state() == state && m_holes[11]->state() == state);
                break;

                default:
                    return false;
                break;
            }
        break;

        default:
            return false;
        break;
    }


}

void Picaria::reset() {
    // Reset each hole.
    for (int id = 0; id < 13; ++id) {
        Hole* hole = m_holes[id];
        hole->reset();
        hole->setNeighbors(id, this->m_mode);
        // Set the hole visibility according to the board mode.
        switch (id) {
            case 3:
            case 4:
            case 8:
            case 9:
                hole->setVisible(m_mode == Picaria::ThirteenHoles);
                break;
            default:
                break;
        }
    }

    // Reset the player and phase.
    m_player = Picaria::RedPlayer;
    m_phase = Picaria::DropPhase;
    this->m_dropCount = 0;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Picaria::showAbout() {
    QMessageBox::information(this, tr("About"), tr("Picaria\n\nAndré Júnior - andrejuniorlopes@gmail.com\n"
    "Bruna Gomes - brunagomescamilo@gmail.com"));
}

void Picaria::updateMode(QAction* action) {
    if (action == ui->action9holes)
        this->setMode(Picaria::NineHoles);
    else if (action == ui->action13holes)
        this->setMode(Picaria::ThirteenHoles);
    else
        Q_UNREACHABLE();
}

void Picaria::updateStatusBar() {
    QString player(m_player == Picaria::RedPlayer ? "vermelho" : "azul");
    QString phase(m_phase == Picaria::DropPhase ? "colocar" : "mover");

    ui->statusbar->showMessage(tr("Fase de %1: vez do jogador %2").arg(phase).arg(player));
}

void Picaria::showGameOver(Picaria::Player player) {
    switch (player) {
        case Picaria::RedPlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador vermelho venceu."));
        break;

        case Picaria::BluePlayer:
            QMessageBox::information(this, tr("Vencedor"), tr("Parabéns, o jogador azul venceu."));
        break;

        default:
            Q_UNREACHABLE();
        }
}
