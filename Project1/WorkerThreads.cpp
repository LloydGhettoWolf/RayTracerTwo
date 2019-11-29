#include "WorkerThreads.h"

std::mutex queueMutex;
std::queue<workItem> workQueue;
std::condition_variable cond;

bool notFinished = true;

void WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lk(queueMutex);
		cond.wait(lk, [] { return !workQueue.empty(); });

		if (notFinished)
		{
			if (!workQueue.empty())
			{
				workItem item = workQueue.front();
				workQueue.pop();
				lk.unlock();
				item.func((void*)&item.args);
			}
		}
		else
		{
			break;
		}

	}
}