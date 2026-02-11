#ifndef TURTLE_OS_TIME_H
#define TURTLE_OS_TIME_H

class Time {
public:
    Time();
    void tick();

    float delta_seconds() const;

private:
    double _last;
    float _delta;
};

#endif // TURTLE_OS_TIME_H
