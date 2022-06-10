/******************************************************************************
 * Author: Yonatan Gross                                                      *
 * Reviewer: Gal                                   		                      *
 * Created: 9.1.2022                                                         *
 *                                                                            *
 * Description:																  *
 * Implementation for logger class.							          *
 *                                                                            *
 * Infinity Labs RD5678														  *
 ******************************************************************************/
#include <cstdio> // puts()
#include <cassert> // assert()
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <boost/bind/bind.hpp> // boost::bind
#include "reactor.hpp"

/************* defines *************************************/
#define UNUSED(x) ((void)(x))
#define TEST(test) (test(), puts("Passed " #test))

using namespace framework;


class Stdin
{
public:
    explicit Stdin(Reactor& reactor) : m_reactor(reactor)
    {

    }

    void ReadFromSTDIN(int fd)
    {
        const size_t BUFFER_SIZE = 1024;
        const char *END_STR = "quit\n";

        char buf[BUFFER_SIZE + 1];
        ssize_t n = read(fd, buf, BUFFER_SIZE);

        buf[n] = '\0';
        if (std::strcmp(buf, END_STR) == 0)
        {
            m_reactor.Remove(fd, Reactor::READ);
            m_reactor.Stop();
        }
        write(STDOUT_FILENO, buf, n + 1);
    }

private:
    Reactor& m_reactor;
};

void STDINTest();
void AnotherTest();

int main()
{
    TEST(STDINTest);
    TEST(AnotherTest);
    return (0);
}


void STDINTest()
{
    Reactor test;

    Stdin stdin_fd(test);

    test.Add(STDIN_FILENO, Reactor::READ, boost::bind(&Stdin::ReadFromSTDIN
        , &stdin_fd, boost::placeholders::_1));

    test.Run();

    test.Remove(STDIN_FILENO, Reactor::READ);
}
/*************************************************/
Reactor MyReactor;
int g_counter = 0;
int g_fdRef = 0;

void ReadFunc1(int fd)
{
	sleep(1);
	std::cout << "ReadFunc1\n";
    UNUSED(fd);
}

void ReadFunc2(int fd)
{
	sleep(1);
	std::cout << "ReadFunc2\n";
    UNUSED(fd);
}

void WriteFunc1(int fd)
{
	sleep(1);
	std::cout << "WriteFunc1\n";
    UNUSED(fd);
}

void RemoveCallback(int fd)
{
	sleep(1);
	MyReactor.Remove(g_fdRef, Reactor::READ);
	g_counter++;
    UNUSED(fd);
}

void StopReactor(int fd)
{
	if (g_counter == 1)
	{
		MyReactor.Stop();
		std::cout << "Stopping\n";
	}
	else
	{
		std::cout << "Not Stopping Yet\n";
	}
    UNUSED(fd);
}

void AnotherTest()
{
	int fd_read1 = open("./read1.txt", O_CREAT | O_EXCL ,0666);
	int fd_read2 = open("./read2.txt", O_CREAT | O_EXCL ,0666);
	int fd_write1 = open("./write1.txt", O_CREAT | O_EXCL ,0666);
	int fd_stopReactor = open("./stopReactor.txt", O_CREAT | O_EXCL ,0666);
	int fd_fileToRemove = open("./fileToRemove.txt", O_CREAT | O_EXCL ,0666);

	MyReactor.Add(fd_read1, Reactor::READ, ReadFunc1);
	MyReactor.Add(fd_read2, Reactor::READ, ReadFunc2);
	MyReactor.Add(fd_write1, Reactor::WRITE, WriteFunc1);
	MyReactor.Add(fd_stopReactor, Reactor::READ, StopReactor);
	MyReactor.Add(fd_fileToRemove, Reactor::READ, RemoveCallback);

	g_fdRef = fd_read1;

	MyReactor.Run();

	close(fd_read1);
    remove("./read1.txt");
	close(fd_read2);
    remove("./read2.txt");
	close(fd_write1);
    remove("./write1.txt");
	close(fd_stopReactor);
    remove("./stopReactor.txt");
	close(fd_fileToRemove);
    remove("./fileToRemove.txt");

	std::cout << "Great success\n";
}
