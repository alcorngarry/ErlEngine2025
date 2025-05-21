#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
    virtual void execute(float delta) = 0;
    virtual void execute(float delta, float axis) {};
    virtual ~Command() = default;
    bool isContinuous = false;
};
#endif // !COMMAND_H