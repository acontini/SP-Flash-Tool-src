#ifndef ICALLBACK_H
#define ICALLBACK_H

class ICallback
{
public:
    virtual void Execute() = 0;
    virtual ~ICallback(){};
};

template <class Receiver>
class SimpleCallback : public ICallback
{
public:
    typedef void (Receiver::* Action)();

    SimpleCallback(Receiver* r, Action a)
        : _receiver(r), _action(a) { }

    virtual void Execute();

private:
    Receiver* _receiver;
    Action _action;

};

template <class Receiver>
void SimpleCallback<Receiver>::Execute ()
{
    (_receiver->*_action)();
}


#endif // ICALLBACK_H
