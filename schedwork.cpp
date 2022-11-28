

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
            std::cout << "Confirming Worker " << wId << " for Day " << day << " schedule" << std::endl;
            return true;
        }
    }

    // Otherwise, remove assigned number and continue
    std::cout << "Removing Worker " << wId << " from Day " << day << " schedule" << std::endl;
    sched[day][wId] = 0;
    if (scheduleHelper(avail, dailyNeed, maxShifts, sched, wId + 1, totalWorkers, day))
    {
        std::cout << "Confirming Worker " << wId << " as absent for Day " << day << " schedule" << std::endl;
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
    std::cout << "CURRENT DAY: " << currentDay << std::endl;
    std::cout << "CURRENT WORKER ID: " << currentWorkerId << std::endl;

    //  @cond Make sure worker is available if scheduled
    if (!avail[currentDay][currentWorkerId] && sched[currentDay][currentWorkerId])
    {
        std::cout << "Worker not available" << std::endl;
        std::cout << "---" << std::endl;
        return false;
    }

    // @summary Initialize a schedule for each worker
    int shifts[totalWorkers];
    int potentialShifts[totalWorkers];
    for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
    {
        shifts[workerId] = 0;
        potentialShifts[workerId] = 0;
    }

    //if(sched[currentDay][currentWorkerId]) shifts[currentWorkerId]++;

    // @summary Make sure daily needs are met
    for (long unsigned int day = 0; day < avail.size(); day++)
    {

        std::cout << "Parsing Day: " << day << std::endl;
        int workersAvailableToday = 0;
        int workersScheduledToday = 0;
        for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
        {
            //if(workerId == currentWorkerId && day == currentDay) continue;
            std::cout << "Parsing Worker with ID: " << workerId << std::endl;
            if (avail[day][workerId] == 1)
            {
                if (potentialShifts[workerId] + shifts[workerId] < maxShifts && sched[day][workerId] == 0)
                {
                    std::cout << workerId << " is available on day " << day << std::endl;
                    workersAvailableToday++;
                    potentialShifts[workerId]++;
                }
                if (sched[day][workerId] == 1)
                {
                    std::cout << workerId << " scheduled on day " << day << std::endl;
                    workersScheduledToday++;
                    shifts[workerId]++;
                }  

                std::cout << "Scheduled Shifts: " << shifts[workerId] << std::endl;
                std::cout << "Potential Shifts: " << potentialShifts[workerId] << std::endl;

                if (shifts[workerId] + potentialShifts[workerId] > maxShifts)
                {
                    std::cout << "Too many shifts" << std::endl;
                    std::cout << "-------" << std::endl;
                    return false;
                }

                std::cout << "NEXT\n" << std::endl;
            }
            std::cout << "---" << std::endl;
        }
        std::cout << "Available: " << workersAvailableToday << std::endl;
        std::cout << "Scheduled: " << workersScheduledToday << std::endl;
        if (dailyNeed > workersAvailableToday + workersScheduledToday)
        {
            std::cout << "Not enough workers" << std::endl;
            std::cout << "-------" << std::endl;
            return false;
        }
    }
    std::cout << "-------" << std::endl;

    // @summary Check to make sure to max shifts constraint is satisfied
    for (Worker_T workerId = 0; workerId < totalWorkers; workerId++)
    {
        std::cout << "Scheduled shifts for worker " << workerId << ": " << (shifts[workerId] + potentialShifts[workerId]) << std::endl;
        if (shifts[workerId] + potentialShifts[workerId] > maxShifts)
        {
            std::cout << "Too many shifts" << std::endl;
            return false;
        }
    }

    return true;
}