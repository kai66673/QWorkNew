#ifndef ICHANGEPROJECTFILEWATCHER_H
#define ICHANGEPROJECTFILEWATCHER_H


class IChangeProjectFileWatcher
{
public:
    virtual void handleChanges() = 0;
};

#endif // ICHANGEPROJECTFILEWATCHER_H
