#ifndef SAVE_COMMAND_H
#define SAVE_COMMAND_H
#include"input/Command.h"
#include"common/Map.h"

class SaveCommand : public Command {
public:
    SaveCommand(Map* map);
    void execute() override;

private:
    Map* map;
};
#endif // !SAVE_COMMAND_H