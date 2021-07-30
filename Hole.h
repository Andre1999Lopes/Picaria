#ifndef HOLE_H
#define HOLE_H

#include <QObject>
#include <QPushButton>
#include <Picaria.h>

class Hole : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
    enum State {
        EmptyState,
        RedState,
        BlueState,
        SelectableState
    };
    Q_ENUM(State)

    explicit Hole(QWidget *parent = nullptr);
    virtual ~Hole();
    State state() const { return m_state; }
    void setState(State State);
    void setNeighbors(int i, Picaria::Mode mode);
    std::vector<int> getNeighbors() const { return this->neighbors; }
    int getIndex() const { return this->index; }
    void setIndex(int index) { this->index = index; }

public slots:
    void reset();

signals:
    void stateChanged(State State);

private:
    int index;
    State m_state;
    std::vector<int> neighbors;
    static QPixmap stateToPixmap(State state);

private slots:
    void updateHole(State state);

};

#endif // HOLE_H
