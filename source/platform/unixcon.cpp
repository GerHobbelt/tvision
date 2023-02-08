#include <tvision/internal/unixcon.h>
#include <tvision/internal/scrlife.h>

#ifdef _TV_UNIX

namespace tvision
{

UnixConsoleStrategy::~UnixConsoleStrategy()
{
    delete sigwinch;
    delete &input;
    delete &display;
    delete &scrl;
}

void UnixConsoleStrategy::forEachSource(void *args, void (&action)(void *, EventSource &)) noexcept
{
    action(args, input);
    forEachPrivateSource(args, action);
}

} // namespace tvision

#endif // _TV_UNIX
