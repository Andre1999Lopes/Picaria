#include "Hole.h"

Hole::Hole(QWidget *parent)
        : QPushButton(parent),
          m_state(Hole::EmptyState) {
    this->updateHole(m_state);

    QObject::connect(this, SIGNAL(stateChanged(State)), this, SLOT(updateHole(State)));
}

Hole::~Hole() {
}

void Hole::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

void Hole::setNeighbors(int i, Picaria::Mode mode) {
    this->neighbors.clear();
    switch(mode) {
        case Picaria::NineHoles:
            switch(i) {
                case 0:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                break;

                case 1:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                break;

                case 2:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                break;

                case 5:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(10);
                    this->neighbors.push_back(11);
                break;

                case 6:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(10);
                    this->neighbors.push_back(11);
                    this->neighbors.push_back(12);
                break;

                case 7:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(11);
                    this->neighbors.push_back(12);
                break;

                case 10:
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(11);
                break;

                case 11:
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(10);
                    this->neighbors.push_back(12);
                break;

                case 12:
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(11);
                break;

            }

        break;

        case Picaria::ThirteenHoles:
            switch(i) {
                case 0:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(3);
                    this->neighbors.push_back(5);
                break;

                case 1:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(3);
                    this->neighbors.push_back(4);
                    this->neighbors.push_back(6);
                break;

                case 2:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(4);
                    this->neighbors.push_back(7);
                break;

                case 3:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                break;

                case 4:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                break;

                case 5:
                    this->neighbors.push_back(0);
                    this->neighbors.push_back(3);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(8);
                    this->neighbors.push_back(10);
                break;

                case 6:
                    this->neighbors.push_back(1);
                    this->neighbors.push_back(3);
                    this->neighbors.push_back(4);
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(8);
                    this->neighbors.push_back(9);
                    this->neighbors.push_back(11);
                break;

                case 7:
                    this->neighbors.push_back(2);
                    this->neighbors.push_back(4);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(9);
                    this->neighbors.push_back(12);
                break;

                case 8:
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(10);
                    this->neighbors.push_back(11);
                break;

                case 9:
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(11);
                    this->neighbors.push_back(12);
                break;

                case 10:
                    this->neighbors.push_back(5);
                    this->neighbors.push_back(8);
                    this->neighbors.push_back(11);
                break;

                case 11:
                    this->neighbors.push_back(6);
                    this->neighbors.push_back(8);
                    this->neighbors.push_back(9);
                    this->neighbors.push_back(10);
                    this->neighbors.push_back(12);
                break;

                case 12:
                    this->neighbors.push_back(7);
                    this->neighbors.push_back(9);
                    this->neighbors.push_back(11);
                break;
            }

        break;

    }
}

void Hole::reset() {
    m_state = Hole::EmptyState;
    this->updateHole(m_state);
}

QPixmap Hole::stateToPixmap(State state) {
    switch (state) {
        case Hole::EmptyState:
            return QPixmap(":empty");
        case Hole::RedState:
            return QPixmap(":red");
        case Hole::BlueState:
            return QPixmap(":blue");
        case Hole::SelectableState:
            return QPixmap(":selectable");
        default:
            return QPixmap();
    }
}

void Hole::updateHole(State state) {
    this->setIcon(Hole::stateToPixmap(state));
}
