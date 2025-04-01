#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
    bool isContinuous = false;
};
#endif // !COMMAND_H