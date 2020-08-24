#include <utils/threads.h>
#include "../include/MtkHdmiService.h"

using namespace android;
// ---------------------------------------------------------------------------

class HdmiUEventThread : public Thread
{
public:
    HdmiUEventThread();
    virtual ~HdmiUEventThread();

    void init();

protected:
    mutable Mutex m_lock;

private:
    virtual bool threadLoop();

    void handleHdmiUEvents(const char *buff, int len);

    int m_socket;

};

