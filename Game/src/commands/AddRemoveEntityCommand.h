#ifndef ADD_REMOVE_ENTITY_COMMAND_H
#define ADD_REMOVE_ENTITY_COMMAND_H
#include"input/Command.h"
#include"common/Map.h"

class AddRemoveEntityCommand : public Command {
public:
    AddRemoveEntityCommand(Map* map, bool isAdd);
    void execute() override;

private:
    Map* map;
    bool isAdd;
};
#endif // !ADD_REMOVE_ENTITY_COMMAND_H