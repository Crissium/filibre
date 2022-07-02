#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard
{
public:
	RunGuard(const QString & key);
	RunGuard(const RunGuard &) = delete;
	RunGuard operator=(const RunGuard &) = delete;
	~RunGuard();

	bool isAnotherRunning();
	bool tryToRun();
	void release();

	static const QString RandomKey();

private:
	const QString key;
	const QString memLockKey;
	const QString sharedmemKey;

	QSharedMemory sharedMem;
	QSystemSemaphore memLock;
};

#endif // RUNGUARD_H
