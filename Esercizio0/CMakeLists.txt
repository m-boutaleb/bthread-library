cmake_minimum_required(VERSION 3.17)
project(Esercizio0)

set(CMAKE_CXX_STANDARD 14)

add_executable(Esercizio0 main.c)

add_executable(TQueueTest test/queue/tqueue_test.c tqueue.c test/Unity-2.5.1/src/unity.c)
add_executable(SchedulerTest test/scheduler/scheduler_test.c bthread.c tqueue.c)
add_executable(BThread bthread.c tqueue.c )
