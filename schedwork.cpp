

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
bool scheduleHelper(
    AvailabilityMatrix &avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule &sched,
    Worker_T wId,
    unsigned int totalWorkers,
    int day);
bool isValid(
    AvailabilityMatrix &avail,
    DailySchedule &sched,
    size_t dailyNeed,
    size_t maxShifts,
    long unsigned int maxDay,
    Worker_T maxWorkerId,
    long unsigned int totalWorkers);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix &avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule &sched)
{
    if (avail.size() == 0U)
    {
        return false;
    }
    sched.clear();

    // @summary Populate schedule matrix
    AvailabilityMatrix aMatrix = avail;
    sched.resize(avail.size());
    int cursor = 0;
    for (AvailabilityMatrix::iterator it = aMatrix.begin(); it != aMatrix.end(); ++it)
    {
        sched[cursor].resize(avail[0].size());
        cursor++;
    }

    // Add your code below
    return scheduleHelper(aMatrix, dailyNeed, maxShifts, sched, 0, (unsigned int)avail[0].size(), 0);
}

bool scheduleHelper(
    AvailabilityMatrix &avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule &sched,
    Worker_T wId,
    unsigned int totalWorkers,
    int day)
{

    // @summary Prevent backtracking on second to last day and final worker
    if (day == avail.size() - 1 && wId == totalWorkers)
        return true;

    // @cond If cursor is past the last worker, move onto the next day and reset worker ID
    if (wId == totalWorkers)
    {
        day++;
        wId = 0;
    }

    // @cond If value is already > 0, move onto next worker
    if (sched[day][wId] == 1)
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, wId + 1, totalWorkers, day);

    // @summary Try marking 1 (available) for this worker
    sched[day][wId] = 1;
    if (isValid(avail, sched, dailyNeed, maxShifts, (unsigned long)day, wId, totalWorkers))
    {
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, wId + 1, totalWorkers, day))
        {
            return true;
        }
    }

    // Otherwise, remove assigned number and continue
    sched[day][wId] = 0;
    if (scheduleHelper(avail, dailyNeed, maxShifts, sched, wId + 1, totalWorkers, day))
    {
        return true;
    }

    return false;
}

// @summary Check if this shift is valid
bool isValid(
    AvailabilityMatrix &avail,
    DailySchedule &sched,
    size_t dailyNeed,
    size_t maxShifts,
    long unsigned int currentDay,
    Worker_T currentWorkerId,
    long unsigned int totalWorkers)
{
    // std::cout << "Day: " << currentDay << std::endl;
    // std::cout << "WorkerId: " << currentWorkerId << std::endl;

    if (!avail[currentDay][currentWorkerId])
        return false;

    // @summary Initialize a schedule for each worker
    int shifts[totalWorkers];
    for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
    {
        shifts[workerId] = 0;
    }
    shifts[currentWorkerId] = 1;

    // @summary Make sure daily needs are met
    for (long unsigned int day = 0; day < avail.size(); day++)
    {
        int workersAvailableToday = 0;
        int workersScheduledToday = 0;
        for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
        {
            // if(workerId == currentWorkerId && day == currentDay) continue;
            if (avail[day][workerId] == 1)
            {
                if (shifts[workerId] < maxShifts && sched[day][workerId] == 0)
                {
                    workersAvailableToday++;
                    shifts[workerId]++;
                }
                if (sched[day][workerId] == 1)
                {
                    workersScheduledToday++;
                }
            }
        }
        // std::cout << "Available: " << workersAvailableToday << std::endl;
        // std::cout << "Scheduled: " << workersScheduledToday << std::endl;
        if (dailyNeed > workersAvailableToday + workersScheduledToday)
        {
            // std::cout << "Not enough workers" << std::endl;
            return false;
        }
    }
    // std::cout << "---" << std::endl;

    // @summary Check to make sure to max shifts constraint is satisfied
    for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
    {
        if (shifts[workerId] > maxShifts)
        {
            // std::cout << "Too many shifts" << std::endl;
            return false;
        }
    }

    return true;
}