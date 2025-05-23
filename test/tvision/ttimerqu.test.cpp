#define Uses_TTimerQueue
#include <tvision/tv.h>

#include <test/test.h>

static TTimePoint currentTime;
static int currentTimeRequests;
static TTimePoint mockCurrentTime()
{
    ++currentTimeRequests;
    return currentTime;
}

static int timeouts;
static void handleTimeout(TTimerId, void *)
{
    ++timeouts;
}

TEST(TTimerQueue, EmptyQueueShouldReturnNoExpiredTimers)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, -1);

    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 0);
}

TEST(TTimerQueue, ShouldCollectExpiredTimers)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1000);

    currentTime = 1000;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 1);
}

TEST(TTimerQueue, ShouldMeasureNextTimeoutProperly)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1000);

    currentTime = 1;
    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 999);

    currentTime = 1001;
    nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 0);
}

TEST(TTimerQueue, ShouldUnqueueSingleShotTimer)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1000);

    currentTime = 1000;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);

    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, -1);
}

TEST(TTimerQueue, ShouldRequeuePeriodicTimer)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1000, 500);

    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 1000);

    currentTime = 1000;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 1);

    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 0);

    nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 500);

    currentTime = 1500;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 1);
}

TEST(TTimerQueue, ShouldCollectOnlyExpiredTimeouts)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1500);
    timerQueue.setTimer(1000);

    currentTime = 1000;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 1);

    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 500);
}

TEST(TTimerQueue, ShouldMeasureNextTimeoutProperlyWithSeveralTimers)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1500);
    timerQueue.setTimer(1000);

    currentTime = 1;
    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 999);
}

TEST(TTimerQueue, ShouldCollectSeveralExpiredTimers)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1500);
    timerQueue.setTimer(1000);

    currentTime = 1500;
    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 2);
}

TEST(TTimerQueue, ShouldRemoveTimer)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1500);
    TTimerId id2 = timerQueue.setTimer(1000);

    timerQueue.killTimer(id2);

    currentTime = 1;
    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 1499);
}

TEST(TTimerQueue, RemovingInvalidTimersShouldNotProduceErrors)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(1500);
    TTimerId id2 = timerQueue.setTimer(1000);

    timerQueue.killTimer(id2);
    timerQueue.killTimer(id2);
    timerQueue.killTimer(nullptr);
    timerQueue.killTimer((TTimerId) 3);

    currentTime = 1;
    int32_t nextTimeout = timerQueue.timeUntilNextTimeout();
    EXPECT_EQ(nextTimeout, 1499);
}

TEST(TTimerQueue, ShouldHandleZeroTimedTimersProperly)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(0, 0);
    timerQueue.setTimer(0, 0);
    timerQueue.setTimer(0, 0);

    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 3);

    timeouts = 0;
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);
    EXPECT_EQ(timeouts, 3);
}

static void nestedHandleTimeout(TTimerId, void *args)
{
    ++timeouts;
    currentTime = 1500;
    (*(TTimerQueue *) args).collectExpiredTimers(handleTimeout, nullptr);
}

TEST(TTimerQueue, ShouldNotCollectTimeoutsWithNestedInvocation)
{
    currentTime = 0;
    TTimerQueue timerQueue(mockCurrentTime);

    timerQueue.setTimer(0, 0);
    timerQueue.setTimer(1000, 500);

    currentTime = 1000;
    timeouts = 0;
    timerQueue.collectExpiredTimers(nestedHandleTimeout, &timerQueue);
    EXPECT_EQ(timeouts, 2);
}

TEST(TTimerQueue, ShouldNotRequestCurrentTimeIfThereAreNoTimers)
{
    currentTimeRequests = 0;

    TTimerQueue timerQueue(mockCurrentTime);
    timerQueue.collectExpiredTimers(handleTimeout, nullptr);

    EXPECT_EQ(currentTimeRequests, 0);
}
